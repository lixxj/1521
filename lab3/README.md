<!DOCTYPE html>
<html>
<head>
  <meta charset="utf-8" />
  <meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no" />
  <!-- base href="https://www.cse.unsw.edu.au/~cs1521/19T2/" -->
  <link rel="stylesheet" href="https://www.cse.unsw.edu.au/~cs1521/19T2/course.css" type="text/css" /></head>
<body>
  <nav class="top">
</div>
  </nav>
  <header>
    <h1 class="heading">Week 03 Lab Exercise</h1>
    <p class="subheading lead">Bit-fields, Unions, and Floats</p>
  </header>
<h2>Objectives</h2>
<ul>
<li>to gain experience with C union types</li>
<li>to investigate how floating-point numbers are constructed</li>
</ul>
<h2>Admin</h2>
<dl class="dl-horizontal">
<dt>Marks</dt><dd>A+=outstanding, A=very good, B=adequate, C=sub-standard, D=hopeless</dd>
<dt>Demo</dt><dd>in this week's lab, or next week's lab</dd>
<dt>Submit</dt><dd><code>give cs1521 lab03 watb.c maf.c</code> or via WebCMS</dd>
<dt>Deadline</dt><dd>submit by Mon 24 Jun 2019, 08:59:59</dd></dl>
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

<aside class="tip">
<p>
Jake Fitzgerald and Hugh Chan have made
some <a href="videos.php"><strong>helpful videos</strong></a>
that explain the important concepts
behind this lab exercise.
Try to watch them before attempting the lab!
</aside>


<h4>Floating-point Representation</h4>
<p>
Single-precision floating-point numbers
that follow the IEEE 754 standard
have an internal structure that looks like this:

<center><img src="Pics/float.png"></center>

<p>
The value is determined as:
\[
    {-1}^{\text{sign}} \times \left(1 + \text{frac}\right) \times 2^{\text{exp} - 127}
\]
The 32 bits in each <code>float</code> are used as follows:

<dl class="row">
<dt class="col-3 text-right">
<code>sign</code>
<dd class="col-9"><p>
is a single bit that indicates the number's sign.
If set to 0, the number is positive;
if set to 1, the number is negative.

<dt class="col-3 text-right">
<code>exp</code>
<dd class="col-9"><p>
is an unsigned 8-bit value (giving a range of \([0\cdots255]\))
which is interpreted as a value in the range \([-127\cdots128]\)
by subtracting 127 (the <q>bias</q>)
from the stored 8-bit value.
It gives a multiplier
for the fraction part
(<abbr>i.e.</abbr>, \( 2^{\text{exp}-127} \)).

<dt class="col-3 text-right">
<code>frac</code>
<dd class="col-9"><p>
is a value in the range \( [0\cdots1] \),
determined using positional notation:
\[
    \frac{\text{bit}_{22}}{2^{1}}
  + \frac{\text{bit}_{21}}{2^{2}}
  + \frac{\text{bit}_{20}}{2^{3}}
  + \cdots
  + \frac{\text{bit}_{2}}{2^{21}}
  + \frac{\text{bit}_{1}}{2^{22}}
  + \frac{\text{bit}_{0}}{2^{23}}
\]
The overall value of the floating-point value
is determined by adding 1 to the fraction:
we assume that the "fraction" part
is actually a value in the range \( [1\cdots2] \),
but save bits by not explicitly storing
the leading 1 bit.
</dl>

<p>
For example:

<pre>
raw 32 bits:  01000000001000000000000000000000
partitioned:  0 10000000 01000000000000000000000
sign:         0, so positive
exp:          10000000 = 128, so multiplier is 2<sup>128-127</sup> = 2<sup>1</sup>
frac:         0&times;2<sup>-1</sup> + 1&times;2<sup>-2</sup> + ... = 0.25, but we need to add 1

final value:  1.25 &times; 2<sup>1</sup> = 2.5
</pre>


<h4>Bit-field Representation</h4>
<p>
In C,
it is possible to define <code>struct</code>s
where the components comprise
a sequence of bits (a <dfn>bit-field</dfn>),
typically within a larger object
(such as an <code>unsigned int</code>).
An example of defining such a structure is:

<pre class="c" style="font-family:monospace;"><span style="color: #993333;">struct</span> bitfields <span style="color: #009900;">&#123;</span>
	<span style="color: #993333;">unsigned</span> <span style="color: #993333;">int</span> a <span style="color: #339933;">:</span> <span style="color: #0000dd;">4</span><span style="color: #339933;">;</span>
	<span style="color: #993333;">unsigned</span> <span style="color: #993333;">int</span> b <span style="color: #339933;">:</span> <span style="color: #0000dd;">8</span><span style="color: #339933;">;</span>
	<span style="color: #993333;">unsigned</span> <span style="color: #993333;">int</span> b <span style="color: #339933;">:</span> <span style="color: #0000dd;">20</span><span style="color: #339933;">;</span>
<span style="color: #009900;">&#125;</span><span style="color: #339933;">;</span></pre>
<p>
… which partitions up an <code>unsigned int</code>
(a 32-bit integer) into three components:
<code>a</code> is 4-bits long,
<code>b</code> is 8-bits long, and
<code>c</code> is 20-bits long.

<p>
One thing which is not
immediately clear from this definition
is how the fields will be placed
within the <code>unsigned int</code>.
This is determined by the compiler,
and could be one of these:

<center><img src="Pics/bit-fields.png"></center>

<p>
One of your tasks is to work out
which layout is actually used.
This is important,
because it leads in to
your second task of
building a floating point value
<q>from the ground up</q>.
</p>

<h2>Setting Up</h2>
<p>
Make a new directory for this lab,
change into that directory,
and run the command:

<pre is="tty">
<kbd is="sh">unzip <a href="https://www.cse.unsw.edu.au/~cs1521/19T2/labs/week03/lab.zip">/web/cs1521/19T2/labs/week03/lab.zip</a></kbd>
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
<dt><code><a href="./files/watb.c">watb.c</a></code> </dt>
<dd>a program to reveal internal layout of bit-fields</dd>
<dt><code><a href="./files/maf.c">maf.c</a></code> </dt>
<dd>a program to construct floats using bit-fields and unions</dd>
<dt><code><a href="./files/tests/">tests/</a></code> </dt>
<dd>a directory containing simple test cases</dd>
</dl>

<h2>Exercise 1: Investigate Bit-field Layout</h2>
<p>
Your first task is
to determine how the C compiler
lays out the bit-fields
within a 32-bit word.

<p>
We have provided a skeleton for a C program to do this:
<code>watb.c</code>.
This program simply defines a <code>struct</code>
like the one above,
defines a variable of this <code>struct</code> type,
and then prints its size
(which you should be able to determine
before running the program).

<p>
Add whatever code you want to this program
that will allow you to determine conclusively
which of the above layouts is used.
Once you've done this,
show your tutor how you did it
and check that you've come to the right conclusion.

<p>
You can assume that the C compiler uses
the same layout strategy for
all bit-fields within a single word
for all other <code>struct</code>s
defined like the above.


<h2>Exercise 2: Build Float Values using Bit-fields and Unions</h2>
<p>
Your second task is
to implement a way of
building <code>float</code> values
by inputting an appropriate bit-pattern
representing the number
according to the IEEE 754 standard.

<p>
We have supplied a skeleton program
that defines a <code>union</code> type
consisting of a 32-bit quantity
that can be interpreted as either
an <code>unsigned int</code>,
an <code>unsigned int</code>
that has been partitioned
into three bit-fields,
or as a <code>float</code> value.
The program reads three bit-strings
from the command-line,
corresponding to the (sign,exp,frac) components
of a floating-point number,
stores these values in the <code>union</code>,
then displays the bit-strings from the union (for debugging),
and finally displays
the contents of the <code>union</code>
as a <code>float</code> value.

<p>
What you need to do is

<ul>
<li><p>
devise an appropriate bit-field representation for floats
that will allow you to easily access to components

<li><p>
complete the <code>getBits()</code> function, which takes
bit-strings from the command-line and stores them in
the appropriate components in a <code>Union32</code> object

<li><p>
complete the <code>showBits()</code> function, which
converts the bit-strings for the three components of
a floating point value in a <code>Union32</code> object
into a single C string, formatted as in the examples below
</ul>

<p>
Examples of how <code>maf</code> should eventually work:

<pre is="tty">
<kbd is="sh">make</kbd>
gcc -Wall -Werror -std=gnu11 -g   -c -o maf.o maf.c
gcc   maf.o   -o maf
<kbd is="sh">./maf</kbd>
Usage: ./maf Sign Exp Frac
<kbd is="sh">./maf 0 1 1</kbd>
./maf: invalid Exp: 1
<kbd is="sh">./maf 0 10000000 01000000000000000000000</kbd>
bits : 0 10000000 01000000000000000000000
float: 2.5000000000
<kbd is="sh">./maf 1 10000000 01000000000000000000000</kbd>
bits : 1 10000000 01000000000000000000000
float: -2.5000000000
<kbd is="sh">./maf 0 00000000 11111111111111111111111</kbd>
bits : 0 00000000 11111111111111111111111
float: 0.0000000000  <span class="comment">// not really 0, but very close</span>
<kbd is="sh">./maf 0 01111111 11000000000000000000000</kbd>
bits : 0 01111111 11000000000000000000000
float: 1.7500000000
<kbd is="sh">./maf 0 10000010 01000000000000000000000</kbd>
bits : 0 10000010 01000000000000000000000
float: 10.0000000000
<kbd is="sh">./maf 0 01111100 10000000000000000000000</kbd>
bits : 0 01111100 10000000000000000000000
float: 0.1875000000
<kbd is="sh">./maf 0 10000011 10101000000000000000000</kbd>
bits : 0 10000011 10101000000000000000000
float: 26.5000000000
<kbd is="sh">./maf 0 10001111 11111000000000000000000</kbd>
bits : 0 10001111 11111000000000000000000
float: 129024.0000000000
<kbd is="sh">./maf 0 01101111 10101010101010101010101</kbd>
bits : 0 01101111 10101010101010101010101
float: 0.0000254313
</pre>

<p>
Note that the bit-strings printed
are <strong>not</strong> generated by
echoing the command-line arguments.
Rather, they are generated by
passing the <code>union</code> as
an argument to the <code>showBits()</code> function.

<p>
You can use the check script to run some simple tests via the command:

<pre is="tty">
<kbd is="sh">1521 check maf</kbd>
</pre>

<p>
while you are in the directory for this lab.

<p>
You should make up more test cases of your own.
Think of a floating-point number,
convert it into an IEEE-format bit-string,
split the bit-string into its three components,
and use these as arguments to <code>maf</code>.

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
Implement a version of <code>maf</code> that works
for the double-precision 64-bit format,
the extended-precision 80-bit format,
the quad-precision 128-bit format,
and/or the half-precision 16-bit format.
Make <code>maf</code> choose the right length
depending on the bit-strings passed to it.
Don't forget to consider endianness!

<li><p>
<span class="badge badge-warning">!!</span>
Read David Goldberg's excellent
<a href="http://docs.oracle.com/cd/E19957-01/806-3568/ncg_goldberg.html">
<q>What Every Computer Scientist Should Know About Floating-Point Numbers</q>
</a>
from ACM Computing Surveys, March 1991,
and write up a short <q>executive summary</q>
(no more than a few hundred words)
of why floating-point numbers are inaccurate,
and why this is acceptable.

<li><p>
<span class="badge badge-warning">!!!</span>
Read William D. Clinger's excellent
<a href="http://www.cesura17.net/~will/Professional/Research/Papers/howtoread.pdf">
<q>How to Read Floating-Point Numbers Accurately</q>
</a>
from the proceedings of the 1990 ACM Conference on
Programming Language Design and Implementation.
Try implementing the algorithm to
scan in floating-point numbers, with prototype
<pre class="c" style="font-family:monospace;"><span style="color: #993333;">float</span> read_float <span style="color: #009900;">&#40;</span><span style="color: #993333;">char</span> <span style="color: #339933;">*</span>str<span style="color: #009900;">&#41;</span><span style="color: #339933;">;</span>       <span style="color: #666666; font-style: italic;">// looks a bit like atof(3), doesn't it?</span></pre>
<li><p>
<span class="badge badge-danger">!!!!</span>
Read Guy L. Steele and Jon L. White's excellent
<a href="https://lists.nongnu.org/archive/html/gcl-devel/2012-10/pdfkieTlklRzN.pdf">
<q>How to Print Floating-Point Numbers Accurately</q></a>,
also from the proceedings of the 1990 ACM Conference on
Programming Language Design and Implementation.
Try implementing the algorithm to
write out floating-point numbers, with prototype
<pre class="c" style="font-family:monospace;"><span style="color: #993333;">void</span> show_float <span style="color: #009900;">&#40;</span><span style="color: #993333;">float</span> f<span style="color: #339933;">,</span> <span style="color: #993333;">char</span> <span style="color: #339933;">*</span>str<span style="color: #009900;">&#41;</span><span style="color: #339933;">;</span></pre>
<li><p>
<span class="badge badge-danger">!!!!</span>
With your newfound knowledge of
the layout of a floating-point value,
and how to manipulate the bits directly,
implement some floating-point arithmetic operations in C.
For example, you may like to try
implementing addition or multiplication,
These functions should be of the form
<pre class="c" style="font-family:monospace;">float32 float32_add <span style="color: #009900;">&#40;</span>float32 a<span style="color: #339933;">,</span> float32 b<span style="color: #009900;">&#41;</span><span style="color: #339933;">;</span></pre><p>
With the arithmetic operations you've implemented,
benchmark their performance by
timing them doing an operation —
for example, adding pi to itself —
and compare the result to
its <q>native</q> implementation.
How could you make your implementation faster?
See if you can implement some of these methods.

</ul>
</aside>

<h2>Submission</h2>
<p>You need to submit
<code id="0">watb.c</code>, and 
<code id="1">maf.c</code>.

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
