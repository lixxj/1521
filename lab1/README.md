

<!DOCTYPE html>
<html>
<head>
  <meta charset="utf-8" />
  <meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no" />
  <title>COMP1521 19T2 - Week 01 Lab Exercise</title>
  <!-- base href="https://www.cse.unsw.edu.au/~cs1521/19T2/" -->
  <link rel="stylesheet" href="https://www.cse.unsw.edu.au/~cs1521/19T2/course.css" type="text/css" /></head>
<body>
  <nav class="top">
    <div class="left"><a href="https://www.cse.unsw.edu.au/~cs1521/19T2/index.php">COMP1521 19T2</a></div>
    <div class="right"><a href="https://www.cse.unsw.edu.au/~cs1521/19T2/webcms3.html">Computer Systems Fundamentals</a></div>
  </nav>
  <header>
    <h1 class="heading">Week 01 Lab Exercise</h1>
    <p class="subheading lead">Input/Output Functions</p>
  </header>
<h2>Objectives</h2>
<ul>
<li>to explore input/output functions from the stdio library</li>
<li>to explore the Unix Programmer's Manual</li>
<li>to use the <code>make</code>, <code>check</code> and <code>time</code> commands</li>
<li>to perform simple performance analysis</li>
</ul>
<h2>Admin</h2>
<dl class="dl-horizontal">
<dt>Marks</dt><dd>A+=outstanding, A=very good, B=adequate, C=sub-standard, D=hopeless</dd>
<dt>Demo</dt><dd>in this week's lab, or next week's lab</dd>
<dt>Submit</dt><dd><code>give cs1521 lab01 cat1.c cat2.c cat3.c cat4.c</code> or via WebCMS</dd>
<dt>Deadline</dt><dd>submit by Mon 10 Jun 2019, 08:59:59</dd></dl>
<aside class="tip">
<p>
Doing the exercise well and correctly
as specified will get you an A grade;
an A grade gets you full marks.
You need to do something <em>truly</em> outstanding,
above and beyond the <q>call of duty</q> to get A+;
an A+ grade also gets a small bonus.
</aside>

<aside class="note">
<p>
In the example interactions,
we assume that you are typing at
a terminal emulator running a shell,
and the shell is giving you a <code>$</code> prompt.
All the text that you type
is in <code><strong>monospace bold</strong></code>;
and all the text that is printed to you
is in <code>monospace</code>.
</aside>

<h2>Background</h2>
<p>
Unix/Linux has a large number of useful commands
which act as filters and read an input byte- or character-stream,
and copy it to an output byte- or character-stream,
generally with some changes.
The simplest filter is the <cite>cat(1)</cite> command,
which is effectively the identity filter:
it copies standard input to standard output,
without making any changes at all.
If used as, <abbr>e.g.</abbr>

<pre is="tty">
<kbd is="sh">cat &lt; /etc/passwd</kbd>
</pre>

<p>
it will copy the contents of
the <code>/etc/passwd</code> file
onto the screen.

<p>
You can also use the <cite>cat(1)</cite> command
by giving one or more file names, <abbr>e.g.</abbr>

<pre is="tty">
<kbd is="sh">cat f1 f2 f3</kbd>
</pre>

<p>
This will (attempt to) open
each of the named files in turn
and copy their contents to the screen.
If a file does not exist or is not readable,
an error message will be written to the screen instead.

<p>
Unix/Linux has a large library of
functions related to input/output
(the <q>standard i/o</q> or <code>stdio.h</code> library).
Many functions provide similar kinds of behaviour
(<abbr>e.g.</abbr>, reading a character).
The aim in this lab is
to write several different versions of
a simple <cite>cat(1)</cite>-like command,
using different i/o functions
and determining which is the most efficient.


<h2>Setting Up</h2>
<p>
To keep your files manageable,
you should do each lab exercise
in a separate directory (folder).
In your home directory,
we would suggest creating
a subdirectory called <var>cs1521</var>,
and then creating a subdirectory under that
called <var>labs</var>,
and then subdirectories <var>week01</var>, <var>week02</var>, etc.
— something like:

<pre>
└── cs1521/
    └── labs/
        ├── week01/
        ├── week02/
        ├── week03/
        ├── week04/
        ├── week05/
        ├── week06/
        ├── week07/
        ├── week08/
        ├── week09/
        └── week10/
</pre>

<p>
We'll assume that the directory
you set up for this lab is <var>lab01dir</var>.

<p>
Change into your <var>lab01dir</var> directory,
and run the following command:

<pre is="tty">
<kbd is="sh">unzip <a href="https://www.cse.unsw.edu.au/~cs1521/19T2/labs/week01/lab.zip">/web/cs1521/19T2/labs/week01/lab.zip</a></kbd>
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
<dt><code><a href="./files/cat1.c">cat1.c</a></code> </dt>
<dd>a program for copying input to output</dd>
<dt><code><a href="./files/cat2.c">cat2.c</a></code> </dt>
<dd>a program for copying input to output</dd>
<dt><code><a href="./files/cat3.c">cat3.c</a></code> </dt>
<dd>a program for copying input to output</dd>
<dt><code><a href="./files/cat4.c">cat4.c</a></code> </dt>
<dd>a program for copying input to output</dd>
<dt><code><a href="./files/tests/">tests/</a></code> </dt>
<dd>a directory containing simple test cases</dd>
</dl>
<p>
Have a quick look at the <code>cat?.c</code> programs.
They're all the same:
they provide some <q>boiler plate</q>
into which you can embed your code,
consisting of a <code>main()</code> function
which simply invokes a <code>copy()</code> function
to read from an input source
and write to an output destination.
The first three programs will be used
simply to copy the standard input stream
to the standard output stream.

<p>
Once you've looked at the programs,
the next thing to do is
to run the <cite>make(1)</cite> command,
which will compile all the code.
<cite>make(1)</cite> prints out the commands it runs,
so you should see something like:

<pre is="tty">
<kbd is="sh">make</kbd>
gcc -Wall -Werror -std=c99   -c -o cat1.o cat1.c
gcc   cat1.o   -o cat1
gcc -Wall -Werror -std=c99   -c -o cat2.o cat2.c
gcc   cat2.o   -o cat2
gcc -Wall -Werror -std=c99   -c -o cat3.o cat3.c
gcc   cat3.o   -o cat3
gcc -Wall -Werror -std=c99   -c -o cat4.o cat4.c
gcc   cat4.o   -o cat4
</pre>

<p>
Since none of the programs do anything yet,
you may as well clean them up by running

<pre is="tty">
<kbd is="sh">make clean</kbd>
rm -f cat1 cat1.o
rm -f cat2 cat2.o
rm -f cat3 cat3.o
rm -f cat4 cat4.o
rm -f tests/*.out
</pre>

<p>
This removes all of the files
created by the compiler and our test system.
Look at the manual entry for <cite>rm(1)</cite>
to find out what the <code>-f</code> option does.

<aside class="note">
<p>
Text of the form <i>somename(N)</i>
refers to the <var>somename</var> entry
in section <var>N</var> of the Unix manual.
Unix manual entries are
sometimes referred to as <q>man pages</q>,
because the command to view them is <cite>man(1)</cite>.

<p>
The Unix manual has several sections;
common ones we'll see are
<em>1</em>, for 'normal' programs like <cite>rm(1)</cite>;
<em>2</em>, for system calls; and
<em>3</em>, for library functions like <cite>fopen(3)</cite>.
There are several other sections —
see if you can find out more about them.

<p>
On the course website,
manual entries link to
an online man-page viewer.
At the command line,
you can use the <code>man</code> command:

<pre is="tty">
<kbd is="sh">man rm</kbd>
</pre>

<p>
Most of the time,
this will pick the right section for you.
Sometimes, it won't,
and you'll need to choose a specific section:

<pre is="tty">
<kbd is="sh">man 3 printf</kbd>
</pre>

<p>
We'll be using the manual <em>a lot</em> during this course,
and it will likely be available to you during exams.
You should definitely become familiar with reading manual entries!
</aside>

<p>
It's worth taking a look at the <code>Makefile</code>
to see if you can work out what it's doing.
Don't worry if you don't understand it all;
you'll see lots of examples of <code>Makefile</code>s
in lectures and labs.
You will need to run <code>make</code>
to recompile the system each time
you make changes to the source code files
and are ready to test the program again.


<h2>Exercise</h2>
<p>
There are five tasks in this lab.
For most of them,
you'll need to consult the Unix manual
to find out how to use
the required functions or commands.

<ol>
<li>
<p>
In <code>cat1.c</code>,
implement a version of the <code>copy()</code> function
that uses <cite>fscanf(3)</cite>
to read one character at a time from the input,
and <cite>fprintf(3)</cite>
to print one character at a time to the output.

<p>
Hint:

<pre is="tty">
<kbd is="sh">man 3 fscanf</kbd>
<kbd is="sh">man 3 fprintf</kbd>
</pre>

<p>
You can check whether your program is behaving correctly
by using the <code>check</code> script as follows:

<pre is="tty">
<kbd is="sh">1521 check cat1</kbd>
</pre>


<li>
<p>
In <code>cat2.c</code>,
implement a version of the <code>copy()</code> function
that uses <cite>fgetc(3)</cite>
to read one character at a time from the input,
and <cite>fputc(3)</cite>
to print one character at a time to the output.

<p>
Hint:

<pre is="tty">
<kbd is="sh">man 3 fgetc</kbd>
<kbd is="sh">man 3 fputc</kbd>
</pre>

<p>
You can check whether your program is behaving correctly
by using the <code>check</code> script as follows:

<pre is="tty">
<kbd is="sh">1521 check cat2</kbd>
</pre>


<li>
<p>
In <code>cat3.c</code>,
implement a version of the <code>copy()</code> function
that uses <cite>fgets(3)</cite>
to read one line at a time from the input,
and <cite>fputs(3)</cite>
to print one line at a time to the output.
Since <cite>fgets(3)</cite> requires a buffer,
make sure it's a large one
(<abbr>e.g.</abbr>,
using the constant <code>BUFSIZ</code>
defined in <code>stdio.h</code>).

<p>
Hint:

<pre is="tty">
<kbd is="sh">man 3 fgets</kbd>
<kbd is="sh">man 3 fputs</kbd>
</pre>

<p>
You can check whether your program is behaving correctly
by using the <code>check</code> script as follows:

<pre is="tty">
<kbd is="sh">1521 check cat3</kbd>
</pre>


<li>
<p>
Once your <code>cat1</code>, <code>cat2</code>,
and <code>cat3</code> commands are working,
use the <cite>time(1)</cite> command
to test them on a large input
to find out which is the most efficient.
A large input file is available in

<pre>/web/cs1521/19T2/labs/week01/WarAndPeace.txt</pre>

<p>
If you want to check that
your programs can handle this file,
try the following commands:

<pre is="tty">
<kbd is="sh">./cat1 &lt; /web/cs1521/19T2/labs/week01/WarAndPeace.txt &gt; /tmp/WP.out</kbd>
<span class="comment"># there should be no output here</span>
<kbd is="sh">diff /web/cs1521/19T2/labs/week01/WarAndPeace.txt /tmp/WP.out</kbd>
<span class="comment"># if your program is correct, there should be no output here</span>
<kbd is="sh">rm /tmp/WP.out</kbd>
</pre>

<p>
Since you know that your programs are working (don't you?),
you don't need to see the large output
that will come from the large input;
one way to avoid the output
is to redirect it to the data sink <code>/dev/null</code>.
Run your timing tests via

<pre is="tty">
<kbd is="sh">time ./cat1 &lt; /web/cs1521/19T2/labs/week01/WarAndPeace.txt &gt; /dev/null</kbd>
</pre>

<p>
which will produce output that looks something like

<pre is="tty">
real   0m0.920s
user   0m0.908s
sys    0m0.008s
</pre>

<p>
Note that you will almost certainly not
get numbers anything like these.

<p>
The <cite>time(1)</cite> output reports:

<ul>
<li>
<code>real</code> time,
the total elapsed time the command ran for,
but this can be affected by
the load on the machine from other processes,
and can vary quite a lot from one run to the next;

<li>
<code>user</code> time,
the overall time spent in the program code,
and will vary less over several runs of the program,
but will still show variations; and

<li>
<code>sys</code> time,
the time that the operating system spent
doing work on behalf of your code,
which will also vary a bit
over multiple runs of the code.
</ul>

<p>
Also, the times will vary significantly
depending on the machine you run the program on.
You should do all of the comparative testing
on a single machine.

<p>
To get a good sense of the relative efficiency
of the various versions of <code>cat?</code>,
you should run the timing test several times
and take an average.
Once you have reached a conclusion
about which approach is most efficient,
use that approach to implement <code>cat4.c</code>.


<li>
<p>
Copy the most efficient version of <code>copy()</code>
into <code>cat4.c</code>,
and then modify the <code>main()</code> function
to behave as follows:

<pre>
if there are no command-line arguments,
    call copy with stdin and stdout;
otherwise,
    for each command-line argument,
        attempt to open the named file for reading;
        if it fails to open,
            print "Can't read <var>name-of-file</var>",
        otherwise,
            call copy with the open file and stdout,
            close the file
</pre>

<p>
Use the <cite>fopen(3)</cite> and <cite>fclose(3)</cite> functions
to access the file given by the command-line argument.
</ol>

<h2>Submission</h2>
<p>You need to submit
<code id="0">cat1.c</code>, 
<code id="1">cat2.c</code>, 
<code id="2">cat3.c</code>, and 
<code id="3">cat4.c</code>.

You can submit them
from the command line using <code>give</code>,
or you can submit from within WebCMS.
After submitting the code
(either in this lab class
or before the deadline)
show your tutor,
who'll give you feedback on your work
and award a grade.<footer><p>
  <strong>COMP1521 19T2: Computer Systems Fundamentals</strong>
  is brought to you by <br />
  the <a href="https://www.cse.unsw.edu.au/">School of Computer Science and Engineering</a>
  at the <a href="https://www.unsw.edu.au/">University of New South Wales</a>, Sydney.<br />
  For all enquiries, please email the class account at
  <a href="mailto:cs1521@cse.unsw.edu.au">cs1521@cse.unsw.edu.au</a><br />
  <small>CRICOS Provider 00098G</small>
</p></footer>

<script src="https://www.cse.unsw.edu.au/~cs1521/19T2/course.js"></script><script src="https://cdnjs.cloudflare.com/ajax/libs/mathjax/2.7.5/latest.js?config=TeX-MML-AM_CHTML" async></script></body>
</html>
