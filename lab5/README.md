
<!DOCTYPE html>
<html>
<body>
  <nav class="top">
  </nav>
  <header>
    <h1 class="heading">Week 05 Lab Exercise</h1>
    <p class="subheading lead">Functions and Matrices in MIPS</p>
  </header>
<h2>Objectives</h2>
<ul>
<li>to give you practice with programming in MIPS assembly</li>
<li>to learn how to write functions in MIPS assembly</li>
<li>to learn how to manipulate arrays in MIPS assembly</li>
</ul>
<h2>Admin</h2>
<dl class="dl-horizontal">
<dt>Marks</dt><dd>A+=outstanding, A=very good, B=adequate, C=sub-standard, D=hopeless</dd>
<dt>Demo</dt><dd>in this week's lab, or next week's lab</dd>
<dt>Submit</dt><dd><code>give cs1521 lab05 fac3.s isi.s</code> or via WebCMS</dd>
<dt>Deadline</dt><dd>submit by Mon 8 Jul 2019, 08:59:59<br>
<span class="badge badge-info">Monday labs</span> Thu 11 Jul 2019, 08:59:59</dd></dl>
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
Recursion is an effective way of
concisely describing various kinds of computations.
Seeing how recursion is implemented in assembler
gives you more insight into
the operation of the run-time stack.
The first exercise in this lab looks at this
in the context of converting last week's
factorial function into a recursive function.

<p>
Arrays and matrices occur in many contexts in computing
(not least in the first assignment).
It is often essential to squeeze maximum performance
from array and matrix manipulation operations,
so knowing how to manipulate them in assembly code
is potentially very useful.
The second exercise in this lab
aims to give you practice in scanning a matrix,
to determine whether it is an identity matrix.

<p>
This video gives a brief overview of
working with arrays and matrices in MIPS,
and on the <var>.data</var> segment,
where these data structures are typically stored:

<figure class="text-center">
<iframe class="embed-responsive"
    title="soln" width="600" height="400"
    src="https://www.soln.io/e/BJ4YoK8LX?course_id=SkpA4IVHX&amp;series_id=rJ-ufzvUQ">
    Browser not compatible.
</iframe>
</figure>

<h2>Setting Up</h2>
<p>
Make a new directory for this lab,
change into that directory,
and run the command:

<pre is="tty">
<kbd is="sh">unzip <a href="https://www.cse.unsw.edu.au/~cs1521/19T2/labs/week05/lab.zip">/web/cs1521/19T2/labs/week05/lab.zip</a></kbd>
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
<dt><code><a href="./files/fac3.c">fac3.c</a></code> </dt>
<dd>a simple factorial function, in C</dd>
<dt><code><a href="./files/fac3.s">fac3.s</a></code> </dt>
<dd>a template for a factorial function, in MIPS</dd>
<dt><code><a href="./files/isi.c">isi.c</a></code> </dt>
<dd>checks whether a NxN matrix is an identity matrix, in C</dd>
<dt><code><a href="./files/isi.s">isi.s</a></code> </dt>
<dd>a template for an identity matrix checker, in MIPS</dd>
<dt><code><a href="./files/matrix.h">matrix.h</a></code> </dt>
<dd>a 3x3 identity matrix, in C</dd>
<dt><code><a href="./files/matrix.s">matrix.s</a></code> </dt>
<dd>a 3x3 identity matrix, in MIPS</dd>
<dt><code><a href="./files/tests/">tests/</a></code> </dt>
<dd>a directory containing simple test cases</dd>
</dl>
<p>
As with last week, you can simply invoke <code>check</code>
in the lab directory to run the tests.

<pre is="tty">
<kbd is="sh">1521 check</kbd>
</pre>

<p>
You can run tests for one of the exercises
by running a command like

<pre is="tty">
<kbd is="sh">1521 check <var>fac3.s</var></kbd>
</pre>

<p>
You can run individual tests
by running a command like

<pre is="tty">
<kbd is="sh">sh tests/fac3.01.sh</kbd>
<span class="comment">... output from fac3 test 01 appears here ...</span>
</pre>

<p>
You should also look at the <code>tests/*.sh</code> scripts
to see what they're actually doing:
it's usually a short shell script,
maybe one or two lines long,
which should be easy enough to understand.
Looking at the expected output files
(<abbr>e.g.</abbr>, <code>tests/*.exp</code>)
would also be helpful,
as would looking at any data files
mentioned in the <code>tests/*.sh</code> script.

<p>
Note that the matrix exercise requires that
a data file and the program file be merged
before it can run.
The way we achieve this is simply
to concatenate the data file and the program file
(<code>isi.s</code>, the one that you edit) as follows:

<pre is="tty">
<kbd is="sh">cat isi.s <var>DataFile</var>.s &gt; exe.s</kbd>
<kbd is="sh">1521 spim -file exe.s</kbd>
<span class="comment">... output from execution appears here ...</span>
</pre>

<p>
The <code>cat</code> command puts the assembly files together
in a single file called <code>exe.s</code>.
This file is a complete MIPS assembly program,
and can be executed by <code>spim</code>.

<p>
Just in case it's not obvious from the above,
you <em>cannot</em> run the <code>isi.s</code> file by itself:
it does not contain any data.
If you try, you'll get something like:

<pre is="tty">
<kbd is="sh">1521 spim -file isi.s</kbd>
Loaded: <var>a/path/to/</var>exceptions.s
The following symbols are undefined:
N
m

The matrix
Instruction references undefined symbol at 0x00400044
  [0x00400044]	0x3c010000  lui $1, 0 [m]                   ; 50: la	$a0, m		#            + from &amp;m[0][0]
</pre>

<p>
The testing scripts for exercise 2
(those testing <code>isi.s</code>)
do the concatenation step for you,
and remove the concatenated file when they're done.


<h2>Exercise 1</h2>

<p>
In this exercise,
your goal is to implement
a recursive function to compute factorial.
We have supplied a skeleton program
which is missing the entire contents of
the <code>fac</code> function.
You need to implement
the prologue, epilogue, <em>and</em> the body of the function.
The assembly code should reflect
the C code for this function in <code>fac3.c</code>.
Note that submitting an iterative version of factorial
based on the Week 04 lab is worth zero marks,
even though it will pass all of the <code>check</code> tests.

<p>
Test your code initially by running it manually — <abbr>e.g.</abbr>

<pre is="tty">
<kbd is="sh">1521 spim -file fac.s</kbd>
Loaded: <var>we_re/up/all/night/to/</var>exceptions.s
n  = <kbd>3</kbd>
n! = 6
$
</pre>

<p>
Once you think it's working,
test it out on our test data
by running the command:
</p>

<pre is="tty">
<kbd is="sh">1521 check fac3.s</kbd>
<span class="comment">... which will hopefully show ...</span>
Running 5 tests on fac3 ...
Passed test fac3.01
Passed test fac3.02
Passed test fac3.03
Passed test fac3.04
Passed test fac3.05
5 test(s):	5 passed; 0 failed.
<kbd is="sh"></kbd>
</pre>

<h2>Exercise 2</h2>
<p>
In this exercise your goal is
to implement a function
that checks whether
a supplied \( N \times N \) matrix
is an identity matrix.
Recall that an identity matrix has
1 in each entry on the leading diagonal,
and 0 in all other entries.
The function is passed two parameters:

<ul>
<li><p>
a pointer to the first element in the matrix
(effectively <code>&amp;m[0][0]</code>)
in the argument register <code>$a0</code>
<li><p>
the number of rows and columns in the matrix
(the parameter <code>n</code> in the C code)
in register <code>$a1</code>
</ul>

<p>
The function should behave in the same way as
the <code>isIdent()</code> function
in the <code>isi.c</code> file.

<p>
Once you think your function is working,
test it out using the check tests;
hopefully you will see:

<pre is="tty">
<kbd is="sh">1521 check isi.s</kbd>
<span class="comment">... which will hopefully show ...</span>
Running 9 tests on isi ...
Passed test isi.01
Passed test isi.02
Passed test isi.03
Passed test isi.04
Passed test isi.05
Passed test isi.06
Passed test isi.07
Passed test isi.08
Passed test isi.09
9 test(s):	9 passed; 0 failed.
<kbd is="sh"></kbd>
</pre>

<p>
Note that this function returns either 0 or 1,
so even if your function does nothing,
it will probably still pass some tests.
Don't get excited until it passes all of them.

<p>
You may find this video,
on iterating over matrices in MIPS,
useful background for this exercise.

<figure class="text-center">
<iframe class="embed-responsive"
    title="soln" width="600" height="400"
    src="https://www.soln.io/e/Hk1b3YUUQ?course_id=SkpA4IVHX&amp;series_id=rJ-ufzvUQ">
    Browser not compatible.
</iframe>
</figure>

<h4>Challenge exercises</h4>
<p class="text-center text-muted font-italic"><small>
Achieve great distinction and honour
by completing large amounts of
meaningless busywork for little gain!
</small>

<ul class="list-unstyled">
<li><p>
<span class="badge badge-warning">!!</span>
Get factorial to work for values of \( n \) larger than 12,
or even for arbitrarily large values
(which would require something like <code>BigNum</code>
from the Week 02 lab ... except in MIPS).

</ul>
</aside>

<h2>Submission</h2>
<p>You need to submit
<code id="0">fac3.s</code>, and 
<code id="1">isi.s</code>.

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
