

<!DOCTYPE html>
<html>
<head>
  <meta charset="utf-8" />
  <meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no" />
  <title>COMP1521 19T2 - Week 02 Lab Exercise</title>
  <!-- base href="https://www.cse.unsw.edu.au/~cs1521/19T2/" -->
  <link rel="stylesheet" href="https://www.cse.unsw.edu.au/~cs1521/19T2/course.css" type="text/css" /></head>
<body>
  <nav class="top">
    <div class="left"><a href="https://www.cse.unsw.edu.au/~cs1521/19T2/index.php">COMP1521 19T2</a></div>
    <div class="right"><a href="https://www.cse.unsw.edu.au/~cs1521/19T2/webcms3.html">Computer Systems Fundamentals</a></div>
  </nav>
  <header>
    <h1 class="heading">Week 02 Lab Exercise</h1>
    <p class="subheading lead">Big Numbers</p>
  </header>
<h2>Objectives</h2>
<ul>
<li>to practice manipulating dynamic arrays</li>
<li>to implement the addition operator for very long numbers</li>
</ul>
<h2>Admin</h2>
<dl class="dl-horizontal">
<dt>Marks</dt><dd>A+=outstanding, A=very good, B=adequate, C=sub-standard, D=hopeless</dd>
<dt>Demo</dt><dd>in this week's lab, or next week's lab</dd>
<dt>Submit</dt><dd><code>give cs1521 lab02 BigNum.c</code> or via WebCMS</dd>
<dt>Deadline</dt><dd>submit by Mon 17 Jun 2019, 08:59:59</dd></dl>
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
C's builtin types have a limited range of values.
A typical <code>int</code> can hold values
in the range \({-2}^{31} \cdots 2^{31}-1 \).
It would be useful to be able to
manipulate much larger numbers.
The aim for this lab is to start
implementing a <code>BigNum</code> data type,
which can deal with such numbers.

<p>
To make it feasible to implement in a lab class,
the <code>BigNum</code> data type has
a very simple (and inefficient) structure.
Each <code>BigNum</code> has
an array of <code>char</code> values,
where each char should be a decimal digit
(<code>'0'</code>..<code>'9'</code>).
The following diagram shows
how numbers are represented in <code>BigNum</code>s:

<center><img src="Pics/BigNum.png"></center>

<p>
The diagram shows
two <code>BigNum</code>s being added together,
and the result sum <code>BigNum</code>.
Note that <code>BigNum</code>s
do not all have to be the same size.
One goal of this lab exercise is
to be able to deal with <code>BigNum</code>s being
too small to hold some number,
and thus be able to grow
to accommodate all of the required digits.


<h2>Setting Up</h2>
<p>
Make a new directory for this lab,
change into that directory,
and run the command:

<pre is="tty">
<kbd is="sh">unzip <a href="https://www.cse.unsw.edu.au/~cs1521/19T2/labs/week02/lab.zip">/web/cs1521/19T2/labs/week02/lab.zip</a></kbd>
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
<dt><code><a href="./files/bits.c">bits.c</a></code> </dt>
<dd>a small skeleton program for the warm-up exercise</dd>
<dt><code><a href="./files/BigNum.h">BigNum.h</a></code> </dt>
<dd>interface definition of a data type for very large numbers</dd>
<dt><code><a href="./files/BigNum.c">BigNum.c</a></code> </dt>
<dd>implementation of a data type for very large numbers</dd>
<dt><code><a href="./files/add.c">add.c</a></code> </dt>
<dd>a small program to sum two very large numbers</dd>
<dt><code><a href="./files/tests/">tests/</a></code> </dt>
<dd>a directory containing simple test cases</dd>
</dl>
<p>
The main task for this lab is
to implement the <code>BigNum</code> data type
by completing the functions in <code>BigNum.c</code>.
You should not change <code>add.c</code> or <code>BigNum.h</code>
(unless you implement the challenge exercise).


<h2>Warm-up Exercise</h2>
<p>
In order to find out the range of values
that can be stored in C's builtin (integral) data types,
you should complete the <code>bits.c</code> program
to find out the largest value
that can be represented by various data types.
You might find some of C's bit-manipulation operators
to be useful here.

<p>
For each data type,
you should print out
the most negative,
and most positive values
it can store.
You will need to work out a way to
generate both of these values for each type,
and work out appropriate <cite>printf(3)</cite>
formatting options to display it.

<p>
Once your program is working,
it should print results like:

<pre is="tty">
<kbd is="sh">make bits</kbd>
<kbd is="sh">./bits</kbd>
                  char (1 bytes): -128 ... 127
         unsigned char (1 bytes): 0 ... 255
             short int (2 bytes): -32768 ... 32767
    unsigned short int (2 bytes): 0 ... 65535
                   int (4 bytes): -2147483648 ... 2147483647
          unsigned int (4 bytes): 0 ... 4294967295
              long int (4 bytes): -2147483648 ... 2147483647
     unsigned long int (4 bytes): 0 ... 4294967295
         long long int (8 bytes): -9223372036854775808 ... 9223372036854775807
unsigned long long int (8 bytes): 0 ... 18446744073709551615
</pre>

<p>
Note that you <em>will</em> get
different results on different machines:
on my laptop,
it shows that <code>long int</code>
has the same maximum value
as <code>long long int</code>;
on my phone,
<code>char</code> is unsigned by default.

<p>
Don't <q>solve</q> the problem
simply by printing the above strings,
or by getting its value from <code>&lt;limits.h&gt;</code>.
The aim is for you to think about
how to generate the maximum value,
and learn more about <cite>printf(3)</cite>'s
wide range of formatting options.


<h2>Exercise</h2>
<p>
You are required to complete
the implementation of the <code>BigNum</code>
so that the <code>add</code> program
works as expected.
Specifically,
you need to provide implementations
for the following functions:

<ul class="list-unstyled">
<li>
<pre class="c" style="font-family:monospace;"><span style="color: #993333;">void</span> initBigNum <span style="color: #009900;">&#40;</span>BigNum <span style="color: #339933;">*</span>bn<span style="color: #339933;">,</span> <span style="color: #993333;">int</span> Nbytes<span style="color: #009900;">&#41;</span><span style="color: #339933;">;</span></pre><p class="ml-5">
Initialise a <code>BigNum</code> object
by setting the byte counter and
by creating (<cite>malloc(3)</cite>'ing)
an array of length <code>Nbytes</code>.
Use an <cite>assert(3)</cite> to check
whether the array was successfully created.
No return value.

<li>
<pre class="c" style="font-family:monospace;"><span style="color: #993333;">void</span> addBigNums <span style="color: #009900;">&#40;</span>BigNum bnA<span style="color: #339933;">,</span> BigNum bnB<span style="color: #339933;">,</span> BigNum <span style="color: #339933;">*</span>res<span style="color: #009900;">&#41;</span><span style="color: #339933;">;</span></pre><p class="ml-5">
Add the values in <code>bnA</code> and <code>bnB</code>,
and store the result in <code>res</code>.
If <code>res</code> is not initially
large enough to hold the result,
increase its size.
No return value.

<p class="ml-5">
Note that this function (and all the others as well)
use the convention that,
if the function only reads
a <code>struct</code> parameter,
then it's passed by value;
however, if the intention of the function is
to change the value of the <code>struct</code>,
then it is passed by reference
(<abbr>i.e.</abbr>, using a pointer).

<li>
<pre class="c" style="font-family:monospace;"><span style="color: #993333;">int</span> scanBigNum <span style="color: #009900;">&#40;</span><span style="color: #993333;">char</span> <span style="color: #339933;">*</span>s<span style="color: #339933;">,</span> BigNum <span style="color: #339933;">*</span>bn<span style="color: #009900;">&#41;</span></pre>
<p class="ml-5">
Read a <code>BigNum</code> from a string.
Ignore any leading or trailing
space characters in the string;
if the first non-space is a digit,
then scan to a non-digit (or end of string)
and note where the last digit occurs.
You may find the <code>&lt;ctype.h&gt;</code> functions
<cite>isspace(3)</cite> and <cite>isdigit(3)</cite> useful.
Then, scan the digit-rich region of the string
(<abbr>i.e.</abbr>, a contiguous sequence of digits)
and place them in the appropriate location
in the <code>BigNum</code> array,
which you may need to resize if
the digit-rich region is larger than
the array can store.
If the string does not contain
a sequence of digits after the spaces,
then return 0; otherwise, return 1.

<li>
<pre class="c" style="font-family:monospace;"><span style="color: #993333;">void</span> showBigNum <span style="color: #009900;">&#40;</span>BigNum bn<span style="color: #009900;">&#41;</span></pre><p class="ml-5">
Display the contents of a <code>BigNum</code> on standard output.
Ignore leading '0' digits and then display all digits to the
(lower) end of the array. If there are no non-'0'. digits in
the array, then print a single <code>0</code>. Do not print any
other characters except the digits.
No return value.
</ul>

<p>
We recommend that you implement the functions in the following
order: <code>initBigNum()</code>,
<code>showBigNum()</code> (possibly inserting some data manually into the digits array),
<code>scanBigNum()</code>, <code>addBigNums()</code>.
Altogether, it will require around 50 lines of C code.
</p>
<p>
Once the program is working, you should be able to do things like:
</p>
<pre is="tty">
<kbd is="sh">./add 1000000000 42</kbd>
Sum of 1000000000
and 42
is 1000000042
<kbd is="sh">./add 987654321987654321987654321 98765987659876598765</kbd>
Sum of 987654321987654321987654321
and 98765987659876598765
is 987654420753641981864253086
</pre>

<p>
You can try other numbers and
check them using, for example, <cite>bc(1)</cite>,
an arbitrary-precision calculator — <abbr>e.g.</abbr>,

<pre is="tty">
<kbd is="sh">bc</kbd>
bc 1.06.95
Copyright 1991-1994, 1997, 1998, 2000, 2004, 2006 Free Software Foundation, Inc.
This is free software with ABSOLUTELY NO WARRANTY.
For details type `warranty'. 
<kbd>987654321987654321987654321 + 98765987659876598765</kbd>
987654420753641981864253086
<kbd class="font-italic">^D</kbd>  <span class="comment">control-D to exit bc</span>
</pre>

<p>
You can use the <code>check</code> script
to run some simple tests via the command:

<pre is="tty">
<kbd is="sh">1521 check add</kbd>
</pre>

<p>
while you are in the directory for this lab.

<p>
Note that the <code>add.c</code> main program
sets all of the <code>BigNum</code>s
to initially hold 20 digits.
Most of the tests will work
without needing to increase the number of digits.
However, your <code>BigNum</code> functions
should be able to work
even if the initial size was just 4 digits.

<aside class="challenge">
<h4>Challenge exercises</h4>
<p class="text-center text-muted font-italic"><small>
Achieve great distinction and honour
by completing large amounts of
meaningless busywork for little gain!
</small>

<ul class="list-unstyled">
<li><p>
<span class="badge badge-warning">!</span>
Implement <code>subtractBigNums()</code> and <code>multiplyBigNums()</code>.
They have the same parameters as <code>addBigNums()</code>.

<li><p>
<span class="badge badge-warning">!!</span>
The current data structure can only represent positive integers.
Come up with a different representation
that can represent negative integers,
and extend your functions to support it.

<li><p>
<span class="badge badge-warning">!!!</span>
The current data structure could be much more compact.
Come up with a different representation
that uses less space for a given number of digits.

<li><p>
<span class="badge badge-danger">!!!!</span>
For extra kudos,
come up with a different representation
that can represent arbitrary-precision decimals
(in, for example, a fixed-point format),
and extend your functions to support it.
Try implementing division, modulo, exponentiation.

<li><p>
<span class="badge badge-danger">!!!!!</span>
Implement an interactive, arbitrary-precision calculator,
like <cite>bc(1)</cite> or <cite>dc(1)</cite>,
using your <code>BigNum</code> code.

</ul>
</aside>

<h2>Submission</h2>
<p>You need to submit
<code id="0">BigNum.c</code>.

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
