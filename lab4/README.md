
<!DOCTYPE html>
<html>
<head>
  <meta charset="utf-8" />
  <meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no" />
  <!-- base href="https://www.cse.unsw.edu.au/~cs1521/19T2/" -->
  <link rel="stylesheet" href="https://www.cse.unsw.edu.au/~cs1521/19T2/course.css" type="text/css" /></head>
<body>
  <nav class="top">
  </nav>
  <header>
    <h1 class="heading">Week 04 Lab Exercise</h1>
    <p class="subheading lead">Introduction to MIPS</p>
  </header>
<h2>Objectives</h2>
<ul>
<li>to give you practice with programming in MIPS assembly</li>
<li>to see how functions are implemented in MIPS assembly</li>
</ul>
<h2>Admin</h2>
<dl class="dl-horizontal">
<dt>Marks</dt><dd>A+=outstanding, A=very good, B=adequate, C=sub-standard, D=hopeless</dd>
<dt>Demo</dt><dd>in this week's lab, or next week's lab</dd>
<dt>Submit</dt><dd><code>give cs1521 lab04 fac1.s fac2.s</code> or via WebCMS</dd>
<dt>Deadline</dt><dd>submit by Mon 1 Jul 2019, 08:59:59<br>
<span class="badge badge-info">Monday labs</span> Thu 4 Jul 2019, 08:59:59</dd></dl>
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
Most instruction sets provide
a small number of simple instructions
which lend themselves to
an efficient implementation in hardware,
but require more instructions
to render typical constructs
from high-level programming languages.
MIPS is no exception.

<p>
We'll be running our MIPS programs on the SPIM emulator,
using either

<ul>
<li> <code>1521 spim</code>, the command-line interface,
<li> <code>qtspim</code>, the graphical interface, or
<li> <code>1521 xspim</code>, the <em>old</em> graphical interface
</ul>

<p>
These all use the same backend,
so programs that work on one
should work without change on the other.
The <code>qtspim</code> interface
is the best one to use for debugging,
but requires you to be running in
an environment that supports windowing
(<abbr>e.g.</abbr>, CSE Labs,
most modern operating systems).

<p>
This video gives you a quick overview of MIPS,
SPIM, and assembly language:

<figure class="text-center">
<iframe class="embed-responsive"
    title="soln" width="600" height="400"
    src="https://www.soln.io/e/SJfQFTar7?course_id=SkpA4IVHX&amp;series_id=BJC_sNprm">
    Browser not compatible.
</iframe>
</figure>
<h2>Setting Up</h2>
<p>
Make a new directory for this lab,
change into that directory,
and run the command:

<pre is="tty">
<kbd is="sh">unzip <a href="https://www.cse.unsw.edu.au/~cs1521/19T2/labs/week04/lab.zip">/web/cs1521/19T2/labs/week04/lab.zip</a></kbd>
</pre>


<p>
If you're working at home,
download <code>lab.zip</code>
from the link above,
and extract it using unzip.

<aside class="note">
<p>
If you want to work at home,
you'll need to set up SPIM.
Executables for <code>qtspim</code> are available
for most operating systems from <a href="http://spimsimulator.sourceforge.net/">spimsimulator.sourceforge.net</a>.
If you want to use
the <code>spim</code> command-line interface
on your own machine,
you'll need to download the source code
from SourceForge and compile it yourself,
after modifying the <code>Makefile</code>
in the <code>spim/</code> subdirectory
of the source code to tell <code>make</code>
where to install the executable
and the exception handling code.
After doing that, installation is
as simple as running <code>make &amp;&amp; make install</code>
This is true on MacOS (with <code>gcc</code>installed) and Linux.
I haven't tried to install <code>spim</code> on Windows.
</aside>

<p>
If you've done the above correctly,
you should now find
the following files in the directory:

<dl class="dl-horizontal">
<dt><code><a href="./files/Makefile">Makefile</a></code> </dt>
<dd>a set of dependencies used to control compilation</dd>
<dt><code><a href="./files/fac1.c">fac1.c</a></code> </dt>
<dd>a simple factorial program, in C</dd>
<dt><code><a href="./files/fac1.s">fac1.s</a></code> </dt>
<dd>a template for a factorial program, in MIPS</dd>
<dt><code><a href="./files/fac2.c">fac2.c</a></code> </dt>
<dd>a simple factorial function, in C</dd>
<dt><code><a href="./files/fac2.s">fac2.s</a></code> </dt>
<dd>a template for a factorial function, in MIPS</dd>
<dt><code><a href="./files/tests/">tests/</a></code> </dt>
<dd>a directory containing simple test cases</dd>
</dl>
<p>
Note that you run these tests slightly differently to others,
because there's no binary executable involved.
You can just run the <code>check</code> script
in your lab directory:

<pre is="tty">
<kbd is="sh">1521 check</kbd>
</pre>

<p>
In order to run these programs manually,
you can use <code>spim</code> via e.g.

<pre is="tty">
<kbd is="sh">1521 spim -file fac1.s</kbd>
Loaded: /home/cs1521/share/spim/exceptions.s
n  = <kbd>5</kbd>
n! = 120
<kbd is="sh">1521 spim -file fac2.s</kbd>
Loaded: /home/cs1521/share/spim/exceptions.s
n  = <kbd>10</kbd>
n! = 3628800
</pre>

<p>
This is a quick and easy way to run working SPIM programs,
but is a poor environment to try and debug them.

<p>
For debugging in this lab,
you should use <code>qtspim</code>.

<pre is="tty">
<kbd is="sh">qtspim</kbd>
</pre>

<p>
This will start the GUI interface
from within which you can
load and execute SPIM programs,
and monitor the execution
(<abbr>e.g.,</abbr> step by step).

<p>
A very quick tutorial on using <code>qtspim</code> is available on
<a target="_new" href="https://youtu.be/j1TCuxRXuUQ">YouTube</a>.

<p>
If you look at the <code>fac1.s</code> and <code>fac2.s</code> files,
you will see quite a lot of MIPS code there already.
The only code that is missing is
the 10-15 lines of code required
to compute the value of the factorial.
However, you should not ignore the other code...

<p>
One aim of this lab is for you to see
how functions can be implemented in MIPS.
If you look at the start of the <code>main()</code> function,
you will see code that manipulates the <dfn>stack</dfn>.
When a function commences execution in MIPS,
it needs to create a small environment for itself,
so that it can execute independently of the rest of the program.
At the end of the <code>main()</code> function,
there is more code that manipulates the stack;
this code cleans up the small environment created
at the start of the function.
The following diagram shows how this happens:

<figure>
<img class="img w-100" src="Pics/stack-frame.png">
</figure>

<p>
Note that we have implemented
all of the function stack-frame
code for you in this lab.
In future labs, and in the assignment,
you will need to do it yourself.

<p>
If the above explanation is too terse, the following video might help:

<figure class="text-center">
<iframe class="embed-responsive"
    title="soln" width="600" height="400"
    src="https://www.soln.io/e/BJ7hQr6r7?course_id=SkpA4IVHX&amp;series_id=BJC_sNprm">
    Browser not compatible.
</iframe>
</figure>

<p>
The main requirement for this lab is
to implement a loop to compute the factorial.
There is a standard mapping from
a C <code>while</code> loop to MIPS,
which is illustrated by the following
two versions of a <code>while</code> loop in C.
The second version is very close to
what you need to do in MIPS.
What you need to work out is
how to implement the test <code>goto</code> in MIPS
... and, of course, how to implement the initialisation,
processing and update steps in MIPS.

<pre>
Standard C version                    C version closer to MIPS

<var>initialise</var>                            <var>MIPS code for initialise</var>
while (<var>Condition</var>) {                   TopOfLoop:
   <var>do some processing</var>                    if (!<var>Condition</var>) goto BottomOfLoop;
   <var>update loop variables</var>                 <var>do some processing</var>
}                                        <var>update loop variables</var>
                                         goto TopOfLoop;
                                      BottomOfLoop:
</pre>

<p>
The loops are expressed as <code>for</code> loops in the C code,
but could easily be mapped to <code>while</code>
before translating to MIPS.

<p>
The following video gives a more extensive description
of how loops are implemented in MIPS:

<figure class="text-center">
<iframe class="embed-responsive"
    title="soln" width="600" height="400"
    src="https://www.soln.io/e/ByWWna6r7?course_id=SkpA4IVHX&amp;series_id=BJC_sNprm">
    Browser not compatible.
</iframe>
</figure>

<h2>Exercise</h2>
<p>
In this lab, the goal is
to implement two MIPS programs
to compute factorials.

<p>
The first program, <code>fac1.s</code>,
should be implemented like <code>fac1.c</code>,
which performs all of its work in the main program
(with no functions); <abbr>i.e.</abbr>

<pre class="c" style="font-family:monospace;"><ol><li style="font-weight: normal; vertical-align:top;"><div style="font: normal normal 1em/1.2em monospace; margin:0; padding:0; background:none; vertical-align:top;"><span style="color: #666666; font-style: italic;">// Simple factorial calculator</span></div></li><li style="font-weight: normal; vertical-align:top;"><div style="font: normal normal 1em/1.2em monospace; margin:0; padding:0; background:none; vertical-align:top;"><span style="color: #666666; font-style: italic;">//</span></div></li><li style="font-weight: normal; vertical-align:top;"><div style="font: normal normal 1em/1.2em monospace; margin:0; padding:0; background:none; vertical-align:top;"><span style="color: #666666; font-style: italic;">// Doesn't do error checking</span></div></li><li style="font-weight: normal; vertical-align:top;"><div style="font: normal normal 1em/1.2em monospace; margin:0; padding:0; background:none; vertical-align:top;"><span style="color: #666666; font-style: italic;">// n &lt; 1 or not a number produce n! = 1</span></div></li><li style="font-weight: normal; vertical-align:top;"><div style="font: normal normal 1em/1.2em monospace; margin:0; padding:0; background:none; vertical-align:top;">&nbsp;</div></li><li style="font-weight: normal; vertical-align:top;"><div style="font: normal normal 1em/1.2em monospace; margin:0; padding:0; background:none; vertical-align:top;"><span style="color: #339933;">#include &lt;stdio.h&gt;</span></div></li><li style="font-weight: normal; vertical-align:top;"><div style="font: normal normal 1em/1.2em monospace; margin:0; padding:0; background:none; vertical-align:top;">&nbsp;</div></li><li style="font-weight: normal; vertical-align:top;"><div style="font: normal normal 1em/1.2em monospace; margin:0; padding:0; background:none; vertical-align:top;"><span style="color: #993333;">int</span> main <span style="color: #009900;">&#40;</span><span style="color: #993333;">void</span><span style="color: #009900;">&#41;</span></div></li><li style="font-weight: normal; vertical-align:top;"><div style="font: normal normal 1em/1.2em monospace; margin:0; padding:0; background:none; vertical-align:top;"><span style="color: #009900;">&#123;</span></div></li><li style="font-weight: normal; vertical-align:top;"><div style="font: normal normal 1em/1.2em monospace; margin:0; padding:0; background:none; vertical-align:top;">	<span style="color: #993333;">int</span> n <span style="color: #339933;">=</span> <span style="color: #0000dd;">0</span><span style="color: #339933;">;</span></div></li><li style="font-weight: normal; vertical-align:top;"><div style="font: normal normal 1em/1.2em monospace; margin:0; padding:0; background:none; vertical-align:top;">	<a href="http://www.opengroup.org/onlinepubs/009695399/functions/printf.html"><span style="color: #000066;">printf</span></a> <span style="color: #009900;">&#40;</span><span style="color: #ff0000;">&quot;n  = &quot;</span><span style="color: #009900;">&#41;</span><span style="color: #339933;">;</span></div></li><li style="font-weight: normal; vertical-align:top;"><div style="font: normal normal 1em/1.2em monospace; margin:0; padding:0; background:none; vertical-align:top;">	<a href="http://www.opengroup.org/onlinepubs/009695399/functions/scanf.html"><span style="color: #000066;">scanf</span></a> <span style="color: #009900;">&#40;</span><span style="color: #ff0000;">&quot;%d&quot;</span><span style="color: #339933;">,</span> <span style="color: #339933;">&amp;</span>n<span style="color: #009900;">&#41;</span><span style="color: #339933;">;</span></div></li><li style="font-weight: normal; vertical-align:top;"><div style="font: normal normal 1em/1.2em monospace; margin:0; padding:0; background:none; vertical-align:top;">	<span style="color: #993333;">int</span> fac <span style="color: #339933;">=</span> <span style="color: #0000dd;">1</span><span style="color: #339933;">;</span></div></li><li style="font-weight: normal; vertical-align:top;"><div style="font: normal normal 1em/1.2em monospace; margin:0; padding:0; background:none; vertical-align:top;">	<span style="color: #b1b100;">for</span> <span style="color: #009900;">&#40;</span><span style="color: #993333;">int</span> i <span style="color: #339933;">=</span> <span style="color: #0000dd;">1</span><span style="color: #339933;">;</span> i <span style="color: #339933;">&lt;=</span> n<span style="color: #339933;">;</span> i<span style="color: #339933;">++</span><span style="color: #009900;">&#41;</span></div></li><li style="font-weight: normal; vertical-align:top;"><div style="font: normal normal 1em/1.2em monospace; margin:0; padding:0; background:none; vertical-align:top;">		fac <span style="color: #339933;">*=</span> i<span style="color: #339933;">;</span></div></li><li style="font-weight: normal; vertical-align:top;"><div style="font: normal normal 1em/1.2em monospace; margin:0; padding:0; background:none; vertical-align:top;">	<a href="http://www.opengroup.org/onlinepubs/009695399/functions/printf.html"><span style="color: #000066;">printf</span></a> <span style="color: #009900;">&#40;</span><span style="color: #ff0000;">&quot;n! = %d<span style="color: #000099; font-weight: bold;">\n</span>&quot;</span><span style="color: #339933;">,</span> fac<span style="color: #009900;">&#41;</span><span style="color: #339933;">;</span></div></li><li style="font-weight: normal; vertical-align:top;"><div style="font: normal normal 1em/1.2em monospace; margin:0; padding:0; background:none; vertical-align:top;">	<span style="color: #b1b100;">return</span> <span style="color: #0000dd;">0</span><span style="color: #339933;">;</span></div></li><li style="font-weight: normal; vertical-align:top;"><div style="font: normal normal 1em/1.2em monospace; margin:0; padding:0; background:none; vertical-align:top;"><span style="color: #009900;">&#125;</span></div></li><li style="font-weight: normal; vertical-align:top;"><div style="font: normal normal 1em/1.2em monospace; margin:0; padding:0; background:none; vertical-align:top;">&nbsp;</div></li></ol></pre>
<p>
The second program, <code>fac2.s</code>,
should be implemented like <code>fac2.c</code>,
which uses a function to compute the factorial; <abbr>i.e.</abbr>

<pre class="c" style="font-family:monospace;"><ol><li style="font-weight: normal; vertical-align:top;"><div style="font: normal normal 1em/1.2em monospace; margin:0; padding:0; background:none; vertical-align:top;"><span style="color: #666666; font-style: italic;">// Iterative factorial function</span></div></li><li style="font-weight: normal; vertical-align:top;"><div style="font: normal normal 1em/1.2em monospace; margin:0; padding:0; background:none; vertical-align:top;"><span style="color: #666666; font-style: italic;">//</span></div></li><li style="font-weight: normal; vertical-align:top;"><div style="font: normal normal 1em/1.2em monospace; margin:0; padding:0; background:none; vertical-align:top;"><span style="color: #666666; font-style: italic;">// Doesn't do error checking</span></div></li><li style="font-weight: normal; vertical-align:top;"><div style="font: normal normal 1em/1.2em monospace; margin:0; padding:0; background:none; vertical-align:top;"><span style="color: #666666; font-style: italic;">// n &lt; 1 or not a number produce n! = 1</span></div></li><li style="font-weight: normal; vertical-align:top;"><div style="font: normal normal 1em/1.2em monospace; margin:0; padding:0; background:none; vertical-align:top;">&nbsp;</div></li><li style="font-weight: normal; vertical-align:top;"><div style="font: normal normal 1em/1.2em monospace; margin:0; padding:0; background:none; vertical-align:top;"><span style="color: #339933;">#include &lt;stdio.h&gt;</span></div></li><li style="font-weight: normal; vertical-align:top;"><div style="font: normal normal 1em/1.2em monospace; margin:0; padding:0; background:none; vertical-align:top;">&nbsp;</div></li><li style="font-weight: normal; vertical-align:top;"><div style="font: normal normal 1em/1.2em monospace; margin:0; padding:0; background:none; vertical-align:top;"><span style="color: #993333;">int</span> fac <span style="color: #009900;">&#40;</span><span style="color: #993333;">int</span><span style="color: #009900;">&#41;</span><span style="color: #339933;">;</span> <span style="color: #666666; font-style: italic;">// fac :: int -&gt; int</span></div></li><li style="font-weight: normal; vertical-align:top;"><div style="font: normal normal 1em/1.2em monospace; margin:0; padding:0; background:none; vertical-align:top;">&nbsp;</div></li><li style="font-weight: normal; vertical-align:top;"><div style="font: normal normal 1em/1.2em monospace; margin:0; padding:0; background:none; vertical-align:top;"><span style="color: #993333;">int</span> main <span style="color: #009900;">&#40;</span><span style="color: #993333;">void</span><span style="color: #009900;">&#41;</span></div></li><li style="font-weight: normal; vertical-align:top;"><div style="font: normal normal 1em/1.2em monospace; margin:0; padding:0; background:none; vertical-align:top;"><span style="color: #009900;">&#123;</span></div></li><li style="font-weight: normal; vertical-align:top;"><div style="font: normal normal 1em/1.2em monospace; margin:0; padding:0; background:none; vertical-align:top;">	<span style="color: #993333;">int</span> n <span style="color: #339933;">=</span> <span style="color: #0000dd;">0</span><span style="color: #339933;">;</span></div></li><li style="font-weight: normal; vertical-align:top;"><div style="font: normal normal 1em/1.2em monospace; margin:0; padding:0; background:none; vertical-align:top;">	<a href="http://www.opengroup.org/onlinepubs/009695399/functions/printf.html"><span style="color: #000066;">printf</span></a> <span style="color: #009900;">&#40;</span><span style="color: #ff0000;">&quot;n  = &quot;</span><span style="color: #009900;">&#41;</span><span style="color: #339933;">;</span></div></li><li style="font-weight: normal; vertical-align:top;"><div style="font: normal normal 1em/1.2em monospace; margin:0; padding:0; background:none; vertical-align:top;">	<a href="http://www.opengroup.org/onlinepubs/009695399/functions/scanf.html"><span style="color: #000066;">scanf</span></a> <span style="color: #009900;">&#40;</span><span style="color: #ff0000;">&quot;%d&quot;</span><span style="color: #339933;">,</span> <span style="color: #339933;">&amp;</span>n<span style="color: #009900;">&#41;</span><span style="color: #339933;">;</span></div></li><li style="font-weight: normal; vertical-align:top;"><div style="font: normal normal 1em/1.2em monospace; margin:0; padding:0; background:none; vertical-align:top;">	<a href="http://www.opengroup.org/onlinepubs/009695399/functions/printf.html"><span style="color: #000066;">printf</span></a> <span style="color: #009900;">&#40;</span><span style="color: #ff0000;">&quot;n! = %d<span style="color: #000099; font-weight: bold;">\n</span>&quot;</span><span style="color: #339933;">,</span> fac <span style="color: #009900;">&#40;</span>n<span style="color: #009900;">&#41;</span><span style="color: #009900;">&#41;</span><span style="color: #339933;">;</span></div></li><li style="font-weight: normal; vertical-align:top;"><div style="font: normal normal 1em/1.2em monospace; margin:0; padding:0; background:none; vertical-align:top;">	<span style="color: #b1b100;">return</span> <span style="color: #0000dd;">0</span><span style="color: #339933;">;</span></div></li><li style="font-weight: normal; vertical-align:top;"><div style="font: normal normal 1em/1.2em monospace; margin:0; padding:0; background:none; vertical-align:top;"><span style="color: #009900;">&#125;</span></div></li><li style="font-weight: normal; vertical-align:top;"><div style="font: normal normal 1em/1.2em monospace; margin:0; padding:0; background:none; vertical-align:top;">&nbsp;</div></li><li style="font-weight: normal; vertical-align:top;"><div style="font: normal normal 1em/1.2em monospace; margin:0; padding:0; background:none; vertical-align:top;"><span style="color: #993333;">int</span> fac <span style="color: #009900;">&#40;</span><span style="color: #993333;">int</span> n<span style="color: #009900;">&#41;</span></div></li><li style="font-weight: normal; vertical-align:top;"><div style="font: normal normal 1em/1.2em monospace; margin:0; padding:0; background:none; vertical-align:top;"><span style="color: #009900;">&#123;</span></div></li><li style="font-weight: normal; vertical-align:top;"><div style="font: normal normal 1em/1.2em monospace; margin:0; padding:0; background:none; vertical-align:top;">	<span style="color: #993333;">int</span> prod <span style="color: #339933;">=</span> <span style="color: #0000dd;">1</span><span style="color: #339933;">;</span></div></li><li style="font-weight: normal; vertical-align:top;"><div style="font: normal normal 1em/1.2em monospace; margin:0; padding:0; background:none; vertical-align:top;">	<span style="color: #b1b100;">for</span> <span style="color: #009900;">&#40;</span><span style="color: #993333;">int</span> i <span style="color: #339933;">=</span> <span style="color: #0000dd;">1</span><span style="color: #339933;">;</span> i <span style="color: #339933;">&lt;=</span> n<span style="color: #339933;">;</span> i<span style="color: #339933;">++</span><span style="color: #009900;">&#41;</span></div></li><li style="font-weight: normal; vertical-align:top;"><div style="font: normal normal 1em/1.2em monospace; margin:0; padding:0; background:none; vertical-align:top;">		prod <span style="color: #339933;">*=</span> i<span style="color: #339933;">;</span></div></li><li style="font-weight: normal; vertical-align:top;"><div style="font: normal normal 1em/1.2em monospace; margin:0; padding:0; background:none; vertical-align:top;">	<span style="color: #b1b100;">return</span> prod<span style="color: #339933;">;</span></div></li><li style="font-weight: normal; vertical-align:top;"><div style="font: normal normal 1em/1.2em monospace; margin:0; padding:0; background:none; vertical-align:top;"><span style="color: #009900;">&#125;</span></div></li><li style="font-weight: normal; vertical-align:top;"><div style="font: normal normal 1em/1.2em monospace; margin:0; padding:0; background:none; vertical-align:top;">&nbsp;</div></li></ol></pre>
<p>
<code>fac1.s</code> and <code>fac2.s</code> contain
partial implementations of the code.
To make things easier,
they set up and clean up
the function stack frames for you.

<p>
For <code>fac1.s</code>,
you need to add code into
the body of <code>main()</code> to...

<ul>
<li><p>
get the value which has been read into register <code>$v0</code>, and
<li><p>
use this value to compute the factorial,
leaving the result in register <code>$a0</code>.
</ul>

<p>
For <code>fac2.s</code>, you need to...

<ul>
<li><p>
add the call to the <code>fac()</code> function
in the body of the <code>main()</code> function,
<li><p>
put the factorial calculation code in
the body of the <code>fac()</code> function, and
<li><p>
ensure that the final value of \( n! \)
is placed in the <code>$v0</code> register
before the function returns.
</ul>

<p>
In both cases the code sets things up
so that you can use <code>$s?</code> registers
as local variables,
and tells you which register to use for which variable.

<p>
Do all of your calculations using 32 bits,
which means that you should use <code>mul</code> instruction
(<em>not</em> <code>mult</code>),
and you can ignore the <code>LO</code> and <code>HI</code> registers.

<p>
Some examples of the expected output you should get,
regardless of which of the <code>fac?.s</code> files you use:

<pre is="tty">
<kbd is="sh">1521 spim -file fac1.s</kbd>
Loaded: /home/cs1521/share/spim/exceptions.s
n  = <kbd>5</kbd>
n! = 120
<kbd is="sh">1521 spim -file fac1.s</kbd>
Loaded: /home/cs1521/share/spim/exceptions.s
n  = <kbd>10</kbd>
n! = 3628800
<kbd is="sh">1521 spim -file fac1.s</kbd>
Loaded: /home/cs1521/share/spim/exceptions.s
n  = <kbd>20</kbd>
n! = -2102132736
<kbd is="sh">1521 spim -file fac1.s</kbd>
Loaded: /home/cs1521/share/spim/exceptions.s
n  = <kbd>-1</kbd>
n! = 1
<kbd is="sh">1521 spim -file fac1.s</kbd>
Loaded: /home/cs1521/share/spim/exceptions.s
n  = <kbd>hello</kbd>
n! = 1
</pre>

<p>
Note that the <code>check</code> tests
filter out the first line of the above outputs.
The test outputs in <code>tests/*.out</code>
will look like <code>n  = n! = <var>N</var></code>


<aside class="challenge">
<h4>Challenge exercises</h4>
<p class="text-center text-muted font-italic"><small>
Achieve great distinction and honour
by completing large amounts of
meaningless busywork for little gain!
</small>

<ul class="list-unstyled">
<li><p>
<span class="badge badge-warning">!!</span>
The current programs assume that
the factorial value fits in 32 bits.
Modify the program so that it makes use of
the <code>LO</code> and <code>HI</code> registers
to compute a 64-bit factorial result,
and then print this result.
Note that you can't just use
<code>syscall</code> 1 for this.
</ul>
</aside>

<h2>Submission</h2>
<p>You need to submit
<code id="0">fac1.s</code>, and 
<code id="1">fac2.s</code>.

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

