
<!DOCTYPE html>
<html>
<head>
  <meta charset="utf-8" />
  <meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no" />
<body>
  <nav class="top">
  </nav>
  <header>
    <h1 class="heading">Week 07 Lab Exercise</h1>
    <p class="subheading lead">Virtual Memory Simulation</p>
  </header>

<h2>Objectives</h2>
<ul>
<li>to implement a simulation in C</li>
<li>to investigate virtual memory page replacement</li>
</ul>
<h2>Admin</h2>
<dl class="dl-horizontal">
<dt>Marks</dt><dd>A+=outstanding, A=very good, B=adequate, C=sub-standard, D=hopeless</dd>
<dt>Demo</dt><dd>in this week's lab, or next week's lab</dd>
<dt>Submit</dt><dd><code>give cs1521 lab07 vmsim.c</code> or via WebCMS</dd>
<dt>Deadline</dt><dd>submit by Mon 22 Jul 2019, 08:59:59<br>
<span class="badge badge-info">Monday labs</span> Thu 25 Jul 2019, 08:59:59</dd></dl>
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
<dfn>Virtual memory</dfn>
is a technique for managing
process memory and physical memory by
(a) partitioning process memory
and physical memory into fixed-size <dfn>pages</dfn>,
(b) loading process pages into memory on demand, and
(c) recording where they are loaded
in a <dfn>page table</dfn> for each process.
Pages in virtual memory are backed by <dfn>frames</dfn> of physical memory.
A page table is an array of <dfn>page table entries</dfn>,
or <abbr>PTE</abbr>s: each entry provides information on
(a) where the process page occurs in the physical memory,
(b) whether it has been modified since it was loaded, and
(c) when it was last accessed (either read or written).

<p>
All very nice, but why do we need virtual memory?
This video gives some reasons:

<figure class="text-center">
<iframe class="embed-responsive" src="https://www.soln.io/e/HJlf9j9wm?course_id=SkpA4IVHX"
        title="soln" width="600" height="400">
Browser not compatible.
</iframe>
</figure>
<p>
Consider the following
simple virtual memory setup,
where the process has 6 pages
and the physical memory has 4 frames.
When the process starts,
all PTEs are empty as
no process pages are loaded into memory.
The following diagram shows
the state at the start of the process:

<figure class="text-center">
<img class="img img-fluid p-2" src="Pics/vmstat0.png" />
</figure>

<p>
The first column in the page table
gives the status of each page,
which, here, is one of:

<ul>
<li> <code>'-'</code> if the page is not currently loaded
<li> <code>'L'</code> if the page is loaded into memory but not modified
<li> <code>'M'</code> if the page is loaded and has been modified since loading
</ul>

<p>
The second column in the page table
is either <code>'-'</code> (if the page is not loaded), or
is the number of the frame where the page is loaded.
The third column is a timestamp
indicating when the page was
last read or written.
Timestamps are simply integers
starting from 0 and increasing by one
each time an instruction is executed.

<p>
Consider the scenario where
the first thing that the process does
is to access its stack.
It needs to have the startup code loaded
(assume it's in page 0),
and the stack data loaded
(assume it's in page 5).
The next diagram shows the state
after this has happened.
We assume that the code is loaded first,
because it contains the instructions to access the stack.

<figure class="text-center">
<img class="img img-fluid p-2" src="Pics/vmstat1.png" />
</figure>

<p>
Note that the first page loaded
is placed in the first free memory frame;
the second page loaded
is placed in the second memory frame;
and so on.
If the memory was large enough
to hold all of the process's pages,
they would be loaded one-by-one as required,
and then stay in the memory
until the process was finished.

<p>
Now consider the state later on in the computation,
where the process has accessed a number of its pages
and they are loaded in memory as follows:

<figure class="text-center">
<img class="img img-fluid p-2" src="Pics/vmstat2.png" />
</figure>

<p>
If the next instruction
references an address from page 3,
then we have a problem.
All of the memory frames are in use,
so we need to replace the contents of
one of the frames
to handle the new address reference
(<abbr>i.e.</abbr>,
we need to evict a page from a frame,
and replace the contents of the frame
by the page which has just been referenced).
If the page being evicted has been modified,
it will need to be written to disk
before being replaced in memory.

<p>
The following diagram shows
the scenario after page 3 was loaded.
It replaced page 1 in frame 3.

<figure class="text-center">
<img class="img img-fluid p-2" src="Pics/vmstat3.png" />
</figure>

<p>
How did we decide to replace page 1 rather than, say, page 5?
Here, we've used the <dfn>Least Recently Used</dfn> strategy:
this involves scanning the page table
and finding the loaded page
with the oldest access timestamp.
In the above example,
this was page 1, last accessed at time t=10.
All of the other pages were
more recently accessed than this.


<p>
Here's a more extensive (and animated) example:

<figure class="text-center">
<iframe class="embed-responsive" src="https://www.soln.io/e/rk0Xoftw7?course_id=SkpA4IVHX"
        title="soln" width="600" height="400">
Browser not compatible.
</iframe>
</figure>

<h2>Setting Up</h2>
<p>
Make a new directory for this lab,
change into that directory,
and run the command:

<pre is="tty">
<kbd is="sh">unzip <a href="https://www.cse.unsw.edu.au/~cs1521/19T2/labs/week07/lab.zip">/web/cs1521/19T2/labs/week07/lab.zip</a></kbd>
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
<dt><code><a href="./files/vmsim.c">vmsim.c</a></code> </dt>
<dd>a partially-complete virtual memory simulator</dd>
<dt><code><a href="./files/tests/">tests/</a></code> </dt>
<dd>a directory containing simple test cases</dd>
</dl>
<p>
Note that, as supplied,
the program will compile,
but will <em>not</em> behave as we want.
It will simply complain that
any address you supply is invalid.


<h2>Exercise</h2>
<p>
The simulation program involves two core data structures:

<ul>
<li><p>
<code>PageTable</code>,
an array of <code>nPages</code> page table entries, and
<li><p>
<code>MemFrames</code>,
an array of <code>nFrames</code> integers,
where each entry is -1 (nothing loaded) or a page number
</ul>

<p>
The simulation assumes that there is a single process
whose page table entries are maintained in <code>PageTable</code>,
and a memory with <code>nFrames</code> frames.

<p>
What the <code>main()</code> function does is roughly as follows:

<pre>
main:
    set nPages from argv[1]
    set nFrames from argv[2]
    initialise the clock, PageTable and MemFrames
    for each line of input in stdin:
        extract the operation and virtual address
        convert the virtual address to a physical address
        display the new state of all of the data structures
        tick the clock
</pre>

<p>
It performs the above until it either runs out of input
or until it encounters an invalid address
(outside the range of addresses \( \left[ 0 \ldots \mathtt{MAX} \right) \),
where the maximum address \( \mathtt{MAX} = \mathtt{nPages} \times \mathtt{PAGESIZE} \)).

<p>
You should read the definitions at the start of <code>vmsim.c</code>,
then the <code>main()</code> function (whose operation is described above).
After that, look at the helper functions
<code>initPageTable()</code>,
<code>initMemFrames</code>, and
<code>showState()</code>,
which should give you a clearer idea
of how the data structures work.
Once you've done that,
your goal for this lab is
to complete the <code>physicalAddress()</code> function,
which should behave as follows:

<pre>
physicalAddress (vAddr, action):
    extract page# and offset from vAddr
    if the page# is not valid, return -1

    if the page is already loaded:
        set the Modified flag if action is a write,
        update the access time to the current clock tick, and
        use the frame number and offset to compute a physical address

    otherwise:
        look for an unused frame;
        if we find one, use that,
        otherwise:
            // we need to replace a currently loaded frame, so
            find the Least Recently Used loaded page,
            set its PageTable entry to indicate "no longer loaded",
            increment the nReplaces counter,
            increment the nSaves counter if modified, and
            use the frame that backed that page.

        // we should now have a frame# to use, so:
        increment the nLoads counter,
        set PageTable entry for the new page
            (flags, frame#, accesstime=current clock tick), and
        use the frame number and offset to compute a physical address

    return the physical address
</pre>

<p>
The input file for the simulator is
a trace of memory accesses,
one access per line,
where each line contains an action
(<code>'R'</code> or <code>'W'</code>),
and a virtual (process) address (in decimal).
For example:

<pre>
R 1000
W 2000
R 1004
W 2004
...
</pre>

<p>
which indicates a program that,
first, reads a value from virtual address 1000,
then writes a value to virtual address 2000,
then reads a value from virtual address 1004,
then writes a value to virtual address 2004.

<p>
The output of the simulator is a sequence of states,
where each state is preceded by
the address that was last processed.
For example:

<pre>
@ t=7, read from pA=2 (vA=12290)

PageTable (Stat,Acc,Frame)
[ 0] --, -1, -1
[ 1] LM,  3,  1 @ 4096
[ 2] L-,  5,  2 @ 8192
[ 3] L-,  7,  0 @ 0
[ 4] --, -1, -1
MemFrames
[ 0]  3 @ 0
[ 1]  1 @ 4096
[ 2]  2 @ 8192
</pre>

<p>
... shows that, at time t=7,
the virtual address 12290 was accessed.
After this, the page table shows
that pages 1, 2, 3 are loaded in memory.
Page 1 has been modified since it was loaded,
while pages 2 and 3 are unmodified.
The second column in the page table
shows the last accessed time
of each loaded page (or -1 if it is not loaded).
The third column shows
which frame the page is contained in;
while the value for
the corresponding frame shows
which page is loaded in that frame.

<p>
The final line of the output
shows the values of the three counters
after the program has finished accessing addresses.


<p>
You can see several examples of
the expected output from the program
in the <code>tests/</code> directory.
Tests are in pairs, <abbr>e.g.</abbr>,
the file <code>05.sh</code> shows
the command line parameters
that were used to run the test,
while <code>05.exp</code>
shows the expected output.
You can use these tests in <q>the usual way</q>:

<pre is="tty">
<kbd is="sh">1521 check vmsim.c</kbd>
</pre>

<p>
Notes:

<ul>
<li> pages are 4 KiB = 2<sup>12</sup> bytes
<li> all address values are in decimal (not hexadecimal)
<li> no replacements ever occur if nFrames &geq; nPages
<li> if the Modified bit is set, the Loaded bit will also always be set
<li> if the Loaded bit is set, the Modified bit not necessarily be set
<li> you can force different behaviours by changing the command-line parameters
</ul>


<aside class="challenge">
<h4>Challenge exercises</h4>
<p class="text-center text-muted font-italic"><small>
Achieve great distinction and honour
by completing large amounts of
meaningless busywork for little gain!
</small>

<ul class="list-unstyled"><li><p>
<span class="badge badge-warning">!</span>
Improve your least-recently-used strategy
by tracking whether a page has been modified,
and try to evict the least-recently-used
unmodified frame instead,
so modified pages are not replaced
if there are any unmodified pages
that can be swapped out,
otherwise fall back to
the least-recently-used modified page.

<li><p>
<span class="badge badge-warning">!!</span>
So far, when we unmap pages from frames,
we have ignored the problem of keeping the data.
Most applications don't take kindly
to their data simply disappearing,
so when we unmap pages from frames,
we need to store that data somewhere;
this is usually referred to
as a <q>swap file</q> or <q>page file</q>.
Modify your simulator to keep track of the swap file,
which you could implement like <code>MemFrames</code>;
add a command-line argument for
the number of 'swap' pages you can store,
and when pages are unmapped from memory frames,
store in your (<abbr>e.g.</abbr>,) <code>SwapFrames</code>
(information about) the page being stored there;
when pages need to return to memory,
move them back to <code>MemFrames</code>.
Avoid copying pages that aren't modified.

<li><p>
<span class="badge badge-warning">!!!</span>
Implement other page replacement strategies:
try implementing one or more of
first-in-first-out,
second-chance,
clock-sweep,
least-frequently-used, or
adaptive-replacement-cache strategies.
Add a command-line argument
to specify which strategy you use.
Try to make your changes as general as possible,
so choices can be added without having to
radically change the way your implementation works.

<li><p>
<span class="badge badge-danger">!!!!</span>
Once you have multiple strategies implemented,
measure the performance of each strategy —
you may need to think about what you need to measure —
and see if you can concoct traces to make each perform badly.

<li><p>
<span class="badge badge-danger">!!!!!</span>
Extend your simulator to manage multiple simulated processes
competing for a small amount of physical memory;
add a process ID to the request line
(<abbr>e.g.</abbr>, <code>R/W <var>address</var> <var>process-id</var></code>).
What changes do you need to make to support this?
How many processes can you support?
Add support for <em>locked</em> pages,
which cannot be paged out —
it's usually very embarrassing
when a memory management subsystem pages itself out.
</ul>
</aside>

<h2>Submission</h2>
<p>You need to submit
<code id="0">vmsim.c</code>.

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
