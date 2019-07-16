# COMP1521 19T2 Lab1 Reflection

## In this lab, we were ask to build the directory for the labs in this course, and get familiar with the Debian Linux system again.

This video by CSeLearning shows you how to set up the vlab and cse lab computers:

<a href="https://www.youtube.com/watch?v=2OITiyXqKfg&list=PLbSaCpDlfd6r3PgpUQVcTZezdhEVjVzab
" target="_blank"><img src="https://i.ytimg.com/vi/2OITiyXqKfg/hqdefault.jpg?sqp=-oaymwEYCKgBEF5IVfKriqkDCwgBFQAAiEIYAXAB&rs=AOn4CLDWv3e6AKOiKNIl66qnnaAbulKdrg" 
alt="IMAGE ALT TEXT HERE" width="240" height="180" border="10" /></a>


## Then we were showed HOW to use make and Makefile:
<pre is="tty">
<kbd is="sh">make
 </pre>
In this course we don't have to write our own Makefile, however, we should understand why and how it works. And at least, know how to use it.
Personally, I think it's really useful, especially for assignments where we need to compile multiple(many) files.

### Here are some great quotes by [Jashank Jeremy](https://webcms3.cse.unsw.edu.au/users/z5017851)
When I specify that I want to make a thing, make thinks about how it needs to build it by looking at the dependencies stated. For example, in this week's lab, add needs add.o and BigNum.o; add.o needs add.c and BigNum.h; BigNum.o needs BigNum.c and BigNum.h. If any of the things in the hierarchy are older than the things they need, that thing would be rebuilt — if I modify BigNum.c, BigNum.o and add need to be rebuilt; if I modify BigNum.h, then add.o, BigNum.o, and add would need to be rebuilt.

Make establishes this "tree" of dependencies (strictly, it's a directed acyclic graph — directed because dependencies are one-way, and acyclic because things can't depend on themselves, and graph because there may be multiple paths to a particular thing), then prunes it to minimise the work it needs to do (by skipping things that don't need to be rebuilt), then works out an order to do things (using a topological sort which attempts to fulfil things before the things they depend on), then "makes" those things up-to-date.

Make only knows about the dependencies you state, so if you touch something Make doesn't know about or which is irrelevant to what you've asked to re-make, it won't care. But the rules to fulfil the "make the things" step may be arbitrarily complex, and may touch things beyond those you state dependencies on, so it's really important to be careful about what you state dependencies on.

Most of our labs won't specify "how" to make things: Make already knows how to compile C (and a few other languages, often including C++, Pascal, Fortran, and assembly) so we can rely on the "implicit rules". See the documentation for details.

Make manages two things: a set of targets and their dependencies, and a set of directives of how particular targets should be made.

Once the dependency graph is built, it is then pruned by inspecting timestamps to determine whether a target is "up-to-date" compared with its dependencies. It can only consider the dependencies stated in the Makefile — it does not (and cannot) 'guess' whether other files are relevant.

So, for add, only add.o and BigNum.o would be considered to determine whether it is up-to-date. Similarly, for add.o, only add.c and BigNum.h would be considered. Other files would be ignored, because Make knows nothing about them.

What I meant by the 'arbitrary complex' part is the second half: the actual 'making' of things. It's very easy to construct rules that introduce dependencies that you have not considered or stated — for instance, if you didn't state that add.o depended on BigNum.h, but add.c still depended on it, changes to that file such as, for example, changing prototypes of functions, wouldn't cause add.o to respect that, and the result is a program that either won't link, or will link and do illegal things.

(Incidentally, stating all dependencies explicitly is rather unusual. The usual approach here is to state a minimal subset of the dependencies, then to ask the compiler to output dependency information as it does its work, and then using that for any incremental re-compilation. Both Clang and GCC support this.)

## The main exercise of this lab is collecting input from stdin and send output to stdout, using different C functions

### the descriptions and tutorials of following C functions are provided by [tutorialspoint](https://tutorialspoint.com/about/index.htm)

### fscanf and fprintf
[fscanf](https://www.tutorialspoint.com/c_standard_library/c_function_fscanf.htm)

[fprintf](https://www.tutorialspoint.com/c_standard_library/c_function_fprintf.htm)

### fgetc and fputc
[fgetc](https://www.tutorialspoint.com/c_standard_library/c_function_fgetc.htm)

[fputc](https://www.tutorialspoint.com/c_standard_library/c_function_fputc.htm)

### fgets and fputs
[fgets](https://www.tutorialspoint.com/c_standard_library/c_function_fgets.htm)

[fputs](https://www.tutorialspoint.com/c_standard_library/c_function_fputs.htm)

## After these programs are working, we were required to use the time(1) command to test them on a large input to find out which is the most efficient
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
  
## Conclusions
### fgets and fputs are the most efficient functions
fgets is used to get a non-terminated string at one time, until it hits EOF.
fgetc is used to get one single character at one time, which is more low level.


## Some other useful resources:
[Linux Man Page Online](http://man.he.net/)

[1521 19T2 lab1 manual](https://cgi.cse.unsw.edu.au/~cs1521/19T2/labs/week01/index.php)
