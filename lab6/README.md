
<!DOCTYPE html>
<html>
<body>

  <header>
    <h1 class="heading">Week 06 Lab Exercise</h1>
    <p class="subheading lead">Manipulating File System Objects</p>
  </header>

<h2>Objectives</h2>
<ul>
<li>to learn how to manipulate objects in the Unix file system</li>
<li>to practice extracting information from the Linux Programmer's Manual</li>
<li>    to explore the functions <cite>opendir(3)</cite>, <cite>readdir(3)</cite>,    <cite>closedir(3)</cite>, <cite>stat(2)</cite> and <cite>lstat(2)</cite>    </li>
</ul>
<h2>Admin</h2>
<dl class="dl-horizontal">
<dt>Marks</dt><dd>A+=outstanding, A=very good, B=adequate, C=sub-standard, D=hopeless</dd>
<dt>Demo</dt><dd>in this week's lab, or next week's lab</dd>
<dt>Submit</dt><dd><code>give cs1521 lab06 myls.c</code> or via WebCMS</dd>
<dt>Deadline</dt><dd>submit by Mon 15 Jul 2019, 08:59:59<br>
<span class="badge badge-info">Monday labs</span> Thu 18 Jul 2019, 08:59:59</dd></dl>
<aside class="tip">
<p>
Doing the exercise well and correctly
as specified will get you an A grade;
an A grade gets you full marks.
You need to do something <em>truly</em> outstanding,
above and beyond the <q>call of duty</q> to get A+;
an A+ grade also gets a small bonus.
</aside>


<h2>Background</h2>
<p>
On Unix/Linux,
there are a rich collection of
libraries and system calls
to manipulate objects in the file system.
In particular,
if you want to examine the meta-data of an object,
you can use either the <cite>stat(2)</cite> or <cite>lstat(2)</cite> functions:

<pre class="c" style="font-family:monospace;"><span style="color: #993333;">int</span> stat <span style="color: #009900;">&#40;</span><span style="color: #993333;">const</span> <span style="color: #993333;">char</span> <span style="color: #339933;">*</span>pathname<span style="color: #339933;">,</span> <span style="color: #993333;">struct</span> stat <span style="color: #339933;">*</span>statbuf<span style="color: #009900;">&#41;</span><span style="color: #339933;">;</span>
<span style="color: #993333;">int</span> fstat <span style="color: #009900;">&#40;</span><span style="color: #993333;">int</span> fd<span style="color: #339933;">,</span> <span style="color: #993333;">struct</span> stat <span style="color: #339933;">*</span>statbuf<span style="color: #009900;">&#41;</span><span style="color: #339933;">;</span>
<span style="color: #993333;">int</span> lstat <span style="color: #009900;">&#40;</span><span style="color: #993333;">const</span> <span style="color: #993333;">char</span> <span style="color: #339933;">*</span>pathname<span style="color: #339933;">,</span> <span style="color: #993333;">struct</span> stat <span style="color: #339933;">*</span>statbuf<span style="color: #009900;">&#41;</span><span style="color: #339933;">;</span></pre>
<p>
Both functions take, as an argument,
a Unix path name of an object,
and a pointer to a <code>struct stat</code> object
that they will fill with the meta-data of
the named file system object.
The only difference between the two functions
is in the way they handle symbolic links (<dfn>symlinks</dfn>).

<p>
You can find details on these functions
and the <code>struct stat</code> type,
and an example of how to manipulate a <code>struct stat</code>
(the same code that is in the supplied <code>stat.c</code> program)
by consulting the (usually linked) manual entries
for <cite>stat(2)</cite> or <cite>lstat(2)</cite>:

<pre is="tty"><kbd is="sh">man 2 stat</kbd></pre>

<p>
Or, try this video,
which explains the structures that
<code>stat(2)</code> and <code>lstat(2)</code> return:

<figure class="text-center">
<iframe class="embed-responsive"
    title="soln" width="600" height="400"
    src="https://www.soln.io/e/B1A5boxD7?course_id=SkpA4IVHX&amp;series_id=BJ_n2ilPQ">
    Browser not compatible.
</iframe>
</figure>
<p>
If you don't know what a symbolic link is,
try <a href="https://en.wikipedia.org/wiki/Symbolic_link">Wikipedia</a>.


<p>
Another important set of library functions
are those for manipulating file system directories:

<pre class="c" style="font-family:monospace;">DIR <span style="color: #339933;">*</span>opendir <span style="color: #009900;">&#40;</span><span style="color: #993333;">const</span> <span style="color: #993333;">char</span> <span style="color: #339933;">*</span>name<span style="color: #009900;">&#41;</span><span style="color: #339933;">;</span>
<span style="color: #993333;">struct</span> dirent <span style="color: #339933;">*</span>readdir <span style="color: #009900;">&#40;</span>DIR <span style="color: #339933;">*</span>dirp<span style="color: #009900;">&#41;</span><span style="color: #339933;">;</span>
<span style="color: #993333;">int</span> closedir <span style="color: #009900;">&#40;</span>DIR <span style="color: #339933;">*</span>dirp<span style="color: #009900;">&#41;</span><span style="color: #339933;">;</span></pre>
<p>
These functions manipulate a <code>DIR *</code> object,
which is analogous to a <code>FILE *</code>
for scanning through files,
except that is specifically for scanning through directories.
The <cite>readdir(3)</cite> function
returns a pointer to a <code>struct dirent</code>,
which contains data for a single directory entry.
The most useful field (for our purposes)
is the <code>d_name</code> field,
which gives the name of the next object
(file, subdirectory, ...)
in the open directory.

<p>
You can find out more about these functions
and the <code>struct dirent</code> type
by consulting the manual entries for
<cite>opendir(3)</cite>,
<cite>readdir(3)</cite>, and
<cite>closedir(3)</cite>:

<pre is="tty">
<kbd is="sh">man 3 opendir</kbd>
<span class="comment">... lots of text describing opendir() ...</span>
<kbd is="sh">man 3 readdir</kbd>
<span class="comment">... lots of text describing readdir() and struct dirent ...</span>
<kbd is="sh">man 3 closedir</kbd>
<span class="comment">... lots of text describing closedir() ...</span>
</pre>


<h2>Setting Up</h2>
<p>
Make a new directory for this lab,
change into that directory,
and run the command:

<pre is="tty">
<kbd is="sh">unzip <a href="https://www.cse.unsw.edu.au/~cs1521/19T2/labs/week06/lab.zip">/web/cs1521/19T2/labs/week06/lab.zip</a></kbd>
</pre>

<p>
If you're working at home,
download <code>lab.zip</code>
from the link above,
and extract it using unzip.

<p>
If you've done the above correctly,
you should now find
the following files in the directory:

<dl class="dl-horizontal">
<dt><code><a href="./files/Makefile">Makefile</a></code> </dt>
<dd>a set of dependencies used to control compilation</dd>
<dt><code><a href="./files/stat.c">stat.c</a></code> </dt>
<dd>an example program to show how <cite>stat(3)</cite> is used</dd>
<dt><code><a href="./files/myls.c">myls.c</a></code> </dt>
<dd>an incomplete template for the lab exercise</dd>
</dl>
<p>
If you run the <cite>make(1)</cite> command,
it will produce two executables:

<pre is="tty">
<kbd is="sh">make</kbd>
gcc -Wall -Werror -std=gnu11 -g   -c -o stat.o stat.c
gcc   stat.o   -o stat
gcc -Wall -Werror -std=gnu11 -g   -c -o myls.o myls.c
gcc   myls.o  -lbsd -o myls
<kbd is="sh">./myls .</kbd>
<span class="comment">... produces no output; program is incomplete ...</span>
<kbd is="sh">./stat stat.c</kbd>
               File type: regular file
            Inode number: <var>1087253</var>
               File mode: 0100640 (octal)
              Link count: 1
               Ownership: <var>uid=28159   gid=36820</var>
Preferred I/O block size: 2560 bytes
               File size: 2130 bytes
        Blocks allocated: 4 blocks
      Last status change: <var>Sun Jul  7 16:20:24 2019</var>
        Last file access: <var>Mon Jul  8 00:20:36 2019</var>
  Last file modification: <var>Sun Jul  7 16:20:24 2019</var>
<kbd is="sh">make clean</kbd>
rm -f stat stat.o
rm -f myls myls.o
rm -f tests/*.out
<kbd is="sh"></kbd>
</pre>

<p>
The <code>stat</code> program
already gives you quite a lot of information
about file system objects.
You can use it for reference in this lab.
Note that the values it prints
(<abbr>e.g.</abbr>, inode, dates, ownership)
will be different for you,
since they'll refer to
the specific instance of the <code>stat.c</code> file
on your workstation/laptop.


<h2>Exercise</h2>
<p>
Your task is to write
a simple version of the <cite>ls(1)</cite> command
that prints a list of objects in a specified directory
and gives some information about each object:

<ul>
<li><p>
a single character giving the object type
  <ul>
  <li> <code>'d'</code> if the object is a directory
  <li> <code>'-'</code> if the object is a regular file
  <li> <code>'l'</code> if the object is a symbolic link
  <li> <code>'?'</code> for all other object types
  </ul>

<li><p>
a 9-character string
giving the permissions for owner, group and others
in the same <code>-rwxr-x--x</code>-style format
as <cite>ls(1)</cite> gives:
you should complete the <code>rwxmode()</code> function
to produce this string;
you may not use <cite>strmode(3)</cite>.

<li><p>
the name of the object's owner;
use the <code>username()</code> function
to produce this

<li><p>
the name of the object's group;
use the <code>groupname()</code> function
to produce this

<li><p>
the object's size in bytes

<li><p>
the object's name
</ul>

<p>
The program should ignore any objects
whose name starts with the character <code>'.'</code> (dot).
The program is not required
to print the objects in any particular order
(<abbr>i.e.</abbr>, it doesn't need to
follow <code>ls</code>'s alphabetical order).

<p>
There are some string buffers
defined at the top of <code>main()</code>
that you can use to hold the various string values.
You <em>must</em> use <cite>lstat(2)</cite>
to collect the object's information,
otherwise you won't be able to detect symbolic links.
Print the output using
the following <cite>printf(3)</cite> format string:

<pre>
printf (
	"%s  %-8.8s %-8.8s %8lld  %s\n",
	rwxmode (<var>ModeInfo</var>, mode),
	username (<var>OwnerUID</var>, uname),
	groupname(<var>GroupGID</var>, gname),
	(long long) <var>Size</var>,
	<var>ObjectName</var>
);
</pre>
<p>
where obviously you need to
replace things like <code><var>Size</var></code>
by values from the <code>struct stat</code>
for the current object.

<p>
The main part of your program
should be structured something like this:

<pre>
open the directory
while there are more entries {
	ignore the object if its name starts with '.'
	get the struct stat info for the object (using its path)
	print the details using the object name and struct stat info
}
close the directory
</pre>

<p>
Once your program is working correctly,
it should produce output like:

<pre is="tty">
<kbd is="sh">./myls .</kbd>
lrwxr-xr-x  jas      cs1521         17  Makefile
lrwxr-xr-x  jas      cs1521         15  stat.c
-rw-r-----  jas      cs1521       2934  myls.c
-rwxr-xr-x  jas      cs1521      28440  myls
-rwxr-xr-x  jas      cs1521      26616  stat
-rw-r--r--  jas      cs1521      10040  stat.o
-rw-r--r--  jas      cs1521      12504  myls.o
<b>./myls ~cs1521</b>
drwx------  cs1521   cs1521       4096  18s1
lrwxrwxrwx  cs1521   cs1521          9  work
drwxr-x---  cs1521   cs1521       4096  17s2.work
drwxr-xr-x  cs1521   cs1521       4096  share
drwxr-x---  cs1521   cs1521       4096  sbin
drwxr-x---  cs1521   cs1521       4096  19T2.work
drwxr-x---  cs1521   cs1521       4096  18s2.work
drwx------  cs1521   cs1521       4096  18s2
lrwxrwxrwx  cs1521   cs1521         11  web
-rw-r-----  cs1521   cs1521         60  give.spec
drwxr-x---  cs1521   cs1521       4096  19T2
drwxr-xr-x  cs1521   cs1521       4096  bin
drwxr-xr-x  cs1521   cs1521       4096  lib
drwx------  cs1521   cs1521       4096  17s2
drwxr-xr-x  cs1521   cs1521       4096  spim
drwxr-x---  cs1521   cs1521       4096  18s1.work
lrwxrwxrwx  cs1521   cs1521          4  tmp
drwxr-xr-x  cs1521   cs1521       4096  public_html
<kbd is="sh"></kbd>
</pre>

<p>
Although, of course, in the first case,
it will be your username,
rather than <code>jas</code>,
and the file ordering, sizes, and types
may be different.

<p>
There is an executable version of the solution in the file

<pre>/web/cs1521/19T2/labs/week06/myls</pre>

<p>
which you can use to help with testing your solution.


<h4>Challenge exercises</h4>
<p class="text-center text-muted font-italic"><small>
Achieve great distinction and honour
by completing large amounts of
meaningless busywork for little gain!
</small>

<ul class="list-unstyled"><p>
Extend <code>myls</code> to produce something more like
the real <cite>ls(1)</cite> command,
by implementing some of these extensions.

<li><p>
<span class="badge badge-warning">!</span>
Display files in alphabetical order.
Show the modification time of a file.
Show the path name referenced by symbolic links.

<li><p>
<span class="badge badge-warning">!!</span>
Use command-line arguments to switch between modes;
start with, say, <code>-l</code>
to switch between <q>short</q> and <q>long</q> listings,
or <code>-t</code> to list files in order of modification,
or <code>-r</code> to reverse the order.

<li><p>
<span class="badge badge-warning">!!!</span>
Colour your output!
Try raw ANSI escape sequences
to choose colours and boldness,
then try using Curses to choose colours.

<li><p>
<span class="badge badge-danger">!!!!</span>
The <code>DIR *</code>-manipulating functions
are limited in that they can only work with
one directory at a time.
The <code>FTS *</code> family of functions
(<cite>fts_open(3)</cite>, <cite>fts_read(3)</cite>, <abbr>etc.</abbr>)
let us traverse hierarchies of directories.
Implement a <code>-R</code> option
to recursively traverse and list directories you encounter.
</ul>
</aside>

<h2>Submission</h2>
<p>You need to submit
<code id="0">myls.c</code>.

You can submit them
from the command line using <code>give</code>,
or you can submit from within WebCMS.
After submitting the code
(either in this lab class
or before the deadline)
show your tutor,
who'll give you feedback on your work
and award a grade.

</html>
