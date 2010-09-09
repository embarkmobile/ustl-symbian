// uSTL Documentation - Doxygen format
// This file is part of the ustl library, an STL implementation.
//
// Portions copyright (c) 2005 by Mike Sharov <msharov@users.sourceforge.net>, 
//
// Portions by Penrillian, Smartphone Software Innovators <http://www.penrillian.com>
//
// This file is free software, distributed under the MIT License.

/**
@mainpage

\section intro Introduction

uSTL is a partial implementation of the C++ Standard Template Library (STL),
which reduces the code size of programs using it. 

The following sections describe the implementation:

 - @ref AuthorIntro - Mike Sharov's introduction to uSTL (recommended reading).
 - @ref SymbianIntro - Introduction to the Symbian OS Port and extensions.
 - @ref Installation - How to install and test on Linux.
 - @ref SymbianInstallation - How to install and use on Symbian OS.
 - @ref SymbianTest - How to test the Symbian OS version against the orginal.
 - @ref Savings - How uSTL saves code size.
 - @ref ContainersIntro - Introduction to STL Containers.
 - @ref Strings - Introduction to uSTL Strings.
 - @ref AlgorithmsIntro - Introduction to STL Algorithms.
 - @ref StreamsIntro - Introduction to uSTL Streams.
 - @ref Tuples - Introduction to STL Tuples.
 - @ref Memblocks - The underlying uSTL memory block implementation.
 - @ref Gotchas - Summarises the main differences between uSTL and STL.
 - @ref Contact.

@section MainClasses Main Functionality

uSTL provides the following main STL classes (and their associated typedefs and iterators): 
 - ustl::vector, ustl::list, ustl::set, ustl::multiset, ustl::map, ustl::multimap, 
 - ustl::queue, ustl::stack
 - ustl::string, ustl::bitset,
 - ustl::ostringstream, ustl::istringstream
 - ustl::exception, ustl::auto_ptr, ustl::pair

It has a pretty complete set of the useful vector operations, including:
 - ustl::copy, ustl::copy_if, ustl::count, ustl::count_if, ustl::fill, ustl::find, ustl::find_if, 
 - ustl::for_each, ustl::remove, ustl::remove_if, ustl::reverse, ustl::sort, ustl::stable_sort, 
 - ustl::transform, ustl::unique, ustl::binary_search, ustl::make_heap 

It has various combination and functor functions including:
 - ustl::bind1st, ustl::bind2nd, ustl::ptr_fun, ustl::make_pair.

*/



/**
@page AuthorIntro Author's Introduction
Mike Sharov

 So what's this STL thing? Simply put, it is a collection of common
containers and algorithms in template form. It is the Holy Pill after
which you never have to worry about memory management again. It is
all-powerful and all-purpose, it can save the world and make the fishes
breed. It is the best thing since C++ and if you are not using it,
you're not really using C++. Unfortunately, it can be large and bloating,
as its standard incarnation shipped with gcc demonstrates. Not only is
the library itself rather large, with the current version approaching a
megabyte in size, but with all the code you instantiate by using a vector
for each of your containers, it is easy to become fearful and opt for
using static arrays instead. This is especially painful to former DOS
assembly programmers like myself, who fret endlessly when the size of
the executable crosses the magic 64k boundary, forgetting that nobody
cares about memory anymore.

Of course, these days everyone has gigabytes of RAM (I have 3G myself) and
has no compunction about loading up OpenOffice, whose source tree is over
a gigabyte in size. Why then bother with saving a kilobyte of code here
and there? I can't really say. Maybe it's that warm fuzzy knowledge that
you are making maximum possible use of your computer's resources. Maybe
it's that thrill you get after expressing your program's functionality in
the fewest possible instructions and the minimum imaginable overhead. Or
maybe it really is of no importance and any code bloat will be easily
overcome by faster processors in some near future. I just know what I
like, and it's the sight of clean, concise, and fast code. Therefore
this library.

@section HowItWorks How It Works

  In most STL implementations, the STL containers
manage their own storage with new[] and delete[] operators, which create
strongly typed storage. That is the standard way of allocating C++ object
vectors, allowing appropriate constructors and destructors to be called
on the allocated storage and ensuring that objects are copied via their
copy operators. Although type safety is a good thing, placing memory
management code into a template necessitates its reinstantiation for
every template instance used by the derivative program. This produces
substantial code bloat, that is frequently derided by C developers
and used by them as an argument that C is better than C++. 

The uSTL
implementation attempts to work around this problem by factoring
memory management code into a non-template base class, ustl::memblock,
which performs unstructured memory allocation. STL containers are then
implemented as template wrappers for memblock to provide a measure of
type safety. The result is that each template instantiation contains
less code, and although it does not completely "disappear", due to the
requirement for calling placement constructors on the allocated memory,
most of it does, being replaced by calls to memblock methods. A vector<T>
template instance, for instance, overrides resize and deallocate, the
former being a call to placement new[] and the latter iterates over all
elements to manually call the destructor, and two algorithms copy and
fill (which most STL programs use anyway); everything else melts away at
compile time as if you were using only unstructured storage in the first
place. 

ustl::string is implemented as a static class, not a template,
so it is shared among all users of the library. The base classes for
unstructured storage management (cmemlink - link to constant memory,
memlink - link to mutable memory, and memblock - owner of mutable memory)
are, of course, also available for use as data buffers wherever those are
needed, and streams that efficiently read and write binary data into them
are also available.

@b Next: @ref Savings
*/

/**
@page Installation Linux Installation

<p>
To start with you'll need a decent compiler. Although uSTL will compile
under gcc 2.95, some features require at least gcc 3.4 and are simply
turned off with an older version. C++ is vastly improved in the recent
compiler versions, and I strongly recommend gcc 4.0.0 or later for
best possible code.
</p><pre>
./configure
make install
</pre><p>
configure is not autoconf, but rather a custom program with similar
functionality. <kbd>./configure --help</kbd> lists available options.
You might want to specify a different installation prefix with
<kbd>--prefix=/usr</kbd>; the default destination is /usr/local.
Developers will want to build with <kbd>--with-debug</kbd> to get a lot
of assert error checking, which I highly recommend. If you are the type
to edit configuration manually, it's in %Common.mk and %config.h. When
it's built, you can run the included tests with <kbd>cd bvt; make run</kbd>
Finally, here's a simple hello world application:
</p><pre>
 \#include &lt;%ustl.h&gt;	// Yes, just one header file. The better to precompile it.
using namespace ustl;

int main (void)
{
    cout &lt;&lt; "Hello world!" &lt;&lt; endl;
    return (EXIT_SUCCESS);
}
</pre><p>
If you have at least gcc 3.4, uSTL is built as a standalone library, without
linking to libstdc++. Because g++ links to it by default, you'll need to link
with <kbd>-nodefaultlibs -lc</kbd> if you want to use uSTL to completely
replace libstdc++. This is where the actual space savings happen.
</p>
<p>
Note for versions 0.8, 0.9Symbian:  The UNIX build appears broken - you'll need to edit the
makefile to compile cxx files, and do dos2unix on the <pre>configure</pre> script.
</p>
*/
/**
@page Savings Save! Save! Save!
<p>
So how much space are you going to save and where? Allow me to demonstrate with
the following small program. I'm basically creating a vector and exercise the
most common operations. Those are resize, push_back, insert, and erase, which
you use pretty much every time you have a vector.
</p><pre>
 \#if USING_USTL
    \#include &lt;%ustl.h&gt;
    using namespace ustl;
 \#else
    \#include &lt;vector&gt;
    using namespace std;
 \#endif

int main (void)
{
    vector&lt;int&gt; v;
    v.resize (30);
    for (size_t i = 0; i &lt; v.size(); ++ i)
	v[i] = i;
    v.push_back (57);
    v.insert (v.begin() + 20, 555);
    v.erase (v.begin() + 3);
    return (EXIT_SUCCESS);
}
</pre><p>
Feel free to compile it and see for yourself. I'm compiling with gcc 4.0.1
and <kbd>-O3 -march=athlon-mp -DNDEBUG=1</kbd>. The libstdc++ version is
linked implicitly with it, and uSTL version is linked with
<kbd>-nodefaultlibs -lustl -lc</kbd>. Both executables are stripped.
The libstdc++ version looks like this:
</p><pre>
% ls -l std/tes
5316 tes*
% size std/tes
text    data     bss     dec     hex filename
3204     304       4    3512     db8 tes
% size -A std/tes.o
tes.o:
section                                                                                           size   addr
.text                                                                                              307      0
.data                                                                                                0      0
.bss                                                                                                 0      0
.gnu.linkonce.t._ZNSt6vectorIiSaIiEE5eraseEN9__gnu_cxx17__normal_iteratorIPiS1_EE                   78      0
.rodata.str1.1                                                                                      45      0
.gnu.linkonce.t._ZNSt6vectorIiSaIiEE13_M_insert_auxEN9__gnu_cxx17__normal_iteratorIPiS1_EERKi      303      0
.gnu.linkonce.t._ZNSt6vectorIiSaIiEE6insertEN9__gnu_cxx17__normal_iteratorIPiS1_EERKi              123      0
.gnu.linkonce.t._ZNSt6vectorIiSaIiEE14_M_fill_insertEN9__gnu_cxx17__normal_iteratorIPiS1_EEjRKi    541      0
.gnu.linkonce.t._ZNSt6vectorIiSaIiEE5eraseEN9__gnu_cxx17__normal_iteratorIPiS1_EES5_               118      0
.gcc_except_table                                                                                   15      0
.eh_frame                                                                                          180      0
.comment                                                                                            18      0
.note.GNU-stack                                                                                      0      0
_ZNSt6vectorIiSaIiEE5eraseEN9__gnu_cxx17__normal_iteratorIPiS1_EE                                    8      0
_ZNSt6vectorIiSaIiEE13_M_insert_auxEN9__gnu_cxx17__normal_iteratorIPiS1_EERKi                        8      0
_ZNSt6vectorIiSaIiEE6insertEN9__gnu_cxx17__normal_iteratorIPiS1_EERKi                                8      0
_ZNSt6vectorIiSaIiEE14_M_fill_insertEN9__gnu_cxx17__normal_iteratorIPiS1_EEjRKi                      8      0
_ZNSt6vectorIiSaIiEE5eraseEN9__gnu_cxx17__normal_iteratorIPiS1_EES5_                                 8      0
Total                                                                                             1768
</pre><p>
The uSTL version looks like this:
</p><pre>
% ls -l ustl/tes
4380 tes*
% size ustl/tes
text    data     bss     dec     hex filename
2281     304       4    2589     a1d tes
% size -A ustl/tes.o
tes.o:
section                                            size   addr
.text                                               308      0
.data                                                 0      0
.bss                                                  0      0
.gnu.linkonce.t._ZN4ustl6vectorIiE10deallocateEv      9      0
.gcc_except_table                                    49      0
.eh_frame                                            64      0
.comment                                             18      0
.note.GNU-stack                                       0      0
_ZN4ustl6vectorIiE10deallocateEv                      8      0
Total                                               456
</pre><p>
Let's see what's going on here. The .text size is the same, but look at the
other things instantiated: libstdc++ instantiates all those vector functions.
Two versions of erase are 78 and 118 bytes, insert_aux (used by insert and
push_back) is 303 bytes, insert is 123 bytes, and _fill_insert is 541 bytes.
That's 1163 bytes total for just one vector type. These functions will become
larger for containers with objects, but about 1k in savings is a good measure.
The uSTL version only instantiates a 9 byte deallocate function, which calls
the nonexistent destructors of the int vector (it isn't marked inline because
when there are destructors, it can get larger). The rest is basically inlined
casts and calls to memblock functions.
</p><p>
1k doesn't seem like much, but consider that you get it for <em>every type
of container you instantiate</em>! An int vector here, a float vector
here, a bunch of object containers there, and before you know it you
are using half your executable just for container overhead.
</p><p>
But wait, there is more! Let's look at the total memory footprint:
</p><pre>
% ldd std/tes
linux-gate.so.1 =&gt;  (0xffffe000)
libstdc++.so.6 =&gt; /usr/lib/libstdc++.so.6 (0xb7e7f000)
libm.so.6 =&gt; /lib/libm.so.6 (0xb7e5b000)
libgcc_s.so.1 =&gt; /usr/lib/libgcc_s.so.1 (0xb7e50000)
libc.so.6 =&gt; /lib/libc.so.6 (0xb7d33000)
/lib/ld-linux.so.2 (0xb7f36000)

% ldd ustl/tes
linux-gate.so.1 =&gt;  (0xffffe000)
libc.so.6 =&gt; /lib/libc.so.6 (0xb7e23000)
libustl.so.0.8 =&gt; /home/msharov/lib/libustl.so.0.8 (0xb7df3000)
libgcc_s.so.1 =&gt; /usr/lib/libgcc_s.so.1 (0xb7de8000)
/lib/ld-linux.so.2 (0xb7f40000)

% size of each lib
   text    data     bss     dec     hex filename
1136553   11344   10932 1158829  11aead /lib/libc-2.3.4.so
 131871     376      64  132311   204d7 /lib/libm-2.3.4.so
  38910     412     296   39618    9ac2 /usr/lib/libgcc_s.so.1
 703989   18816   23768  746573   b644d /usr/lib/libstdc++.so.6.0.5
 136826    3680   16660  157166   265ee libustl.so.0.8.0
</pre><p>
After adding up the numbers (from the dec column) std/tes loads
2077331 bytes of shared libraries and ustl/tes loads 1355613 bytes,
saving 721718 bytes or 35%. If you don't count libc, measuring only
the C++-specific overhead, libstdc++ loads 918502 while libustl only
196784, <em>five times less</em>! But enough about space; let's talk
about the library itself.

@b Next: @ref ContainersIntro
*/
/**
@page ContainersIntro Containers and Iterators
<p>
Are you still programming with malloc'ed arrays and iterating with
pointers?  You poor shmuck! The pains you must be going through
every time you need some memory... You must dread the very thought
of variable-sized arrays and live in constant fear of memory leaks
and dangling pointers. You wake up in cold sweat every night wondering
whether you freed that pointer you allocated and keep wondering why in the
world you're still using C++ instead of some comfy language with garbage
collection, like Java or Python. In STL, you will find your salvation and
rebirth! Free those mallocs and discover containers! STL containers are,
um... well, they are basically, uh... malloc'ed blocks of memory. And
iterators are, um... basically pointers into those blocks. But don't
run away yet! It's all different, I promise. Honest!
</p><p>
The first rule you should learn before switching to STL is: "never use
new".  That's right. Just forget about ever manually allocating memory
again. The one and only exception to it is too advanced for you now;
I'll mention it later. From now on you should be thinking in resizable
memory blocks instead.  These blocks know how take care of themselves,
allocating memory when it is needed and freeing it when it is now. They
do all the hard work so you don't have to. The first one you should know
about is the vector object.
</p><pre>
vector&lt;int&gt; v;
v.push_back (1);
v.push_back (2);
v[1] = 0;
v.erase (v.begin() + 1);
v.pop_back();
v.insert (v.begin(), 4);
v.resize (15);
</pre><p>
As you can see, a vector is basically the same thing as the arrays
you use now, except that it is resizable. The function names
ought to be self-explanatory with the exception of the addressing
arguments. You can do index addressing and get free bounds checking
with asserts. Incidentally, I highly recommend you work with a debug
build when writing code; uSTL is chock full of various asserts checking
for error conditions. In the optimized build, most such errors will be
silently ignored where possible and will cause crashes where not. That
is so because they are programmer errors, existing because you have a
bug in your code, not because the user did something wrong, or because
of some system failure. Programmer errors assert. User or system errors
throw exceptions.
</p><p>
Vectors are addressed with iterators, which are just like pointers (and
usually are). Calling begin() gives you the pointer to the first element,
calling end() gives you the pointer to the end of the last element. No,
not the last element, the end of it, or, more accurately, the end of the
array. It's that way so you can keep incrementing an iterator until it
is equal to the end() value, at which point you know you have processed
all the elements in the list. This brings me to demonstrate how you
ought to do that:
</p><pre>
foreach (vector&lt;int&gt;::iterator, i, v)
    if (*i &lt; 5 || *i &gt; 10)
	*i = 99;
</pre><p>
Although the foreach macro is a uSTL-only extension, it is a one-liner
you can easily copy out of uutility.h if you ever want to switch back to
regular STL.  It is a great way to ensure you don't forget to increment
the counter or run past the end of the vector. The only catch to be aware
of, when inside an iterator loop, is that if you modify the container,
by adding or removing entries, you have to update the iterator, since the
container memory storage may have moved when resized. So, for example,
if you wish to remove certain types of elements, you'd need to do use
an index loop or something like:
</p><pre>
foreach (vector&lt;CEmployee&gt;::iterator, i, employees)
    if (i-&gt;m_Salary &gt; 50000 || i-&gt;m_Performance &lt; 100)
	--(i = employees.erase (i));
</pre><p>
This is pretty much all there is to say about containers. Create them,
use them, resize them, that's what they are for. There are other
container types, but you will probably not use them much. There's
<tt>set</tt>, which is a perpetually sorted vector, useful when you
want to binary_search a large collection. There's <tt>map</tt> which
is an associative container where you can look up entries by key. Its
utility goes down drastically when you have complex objects that need to
be searched with more than one parameter, in which cast you are better
off with vector and foreach. The rest I have never needed, which explains
their perfunctory (though functional) implementations.

@b Next: @ref Strings
*/
/**
@page Strings Strings

Every program uses strings, and STL was kind enough to provide
a specification.  Unfortunately, uSTL deviates a bit and does not
implement it fully. There is only one <tt>string</tt> class, and the
wide-character version is not supported.  Instead the string class
assumes all your strings will be UTF8-encoded, and provides some
additional functionality to make working with those easier. I did that
for the same reason I dropped the locale classes; bloat. It is simply
too expensive to implement the standard locale classes. If you need them,
you can still include them from libstdc++.
</p><p>
Anyway, back to strings. You can think of the string object as a
char vector with some additional operations built-in, like searching,
concatenation, etc.
</p><pre>
string s ("Hello");
s += ' ';
s += "world?";
%s.replace (%s.find ('?'), 1, "!");
s[3] = s[s.find_first_of("lxy")];
s[s.rfind('w')] = 'W';
cout &lt;&lt; s &lt;&lt; endl;
</pre><p>
A nonstandard behaviour you may encounter is from linked strings created
by the string constructor when given a null-terminated const string. In
the above example, the constructor links when given a const string and
stays as a const link until the space is added. If you try to write to it,
you'll get an assert telling you to use copy_link first to convert the
link into a copy. Resizing the linked object automatically does that for
you, so most of the time it is transparent. 
</p><p>
You may also encounter another
instance of this if you try getting iterators from such an object. The
compiler uses the non-const accessors by default for local objects,
so you may need to declare it as a const string if you don't wish to
copy_link. Why does uSTL string link instead of copying?  To save space
and time. All those strings are already in memory, so why waste heap
space and processor time to copy them if you just want to read them? I
thought it a good tradeoff, considering that it is trasparent for the
most common uses.
</p><p>
Other nonstandard extensions include a <tt>format</tt> function to
give you the functionality of sprintf for string objects. 
</p><p>
  Another extension is
the UTF8 stuff. Following the standard <tt>length</tt>
returns the string length in bytes; however there is also <tt>utf_length</tt> 
answering the length in characters.
You can iterate by characters instead of bytes with a special utf8
iterator:
</p><pre>
for (string::utf8_iterator i = s.utf8_begin(); i &lt; s.utf8_end(); ++ i)
    DrawChar (*i);
</pre><p>
or just copy all the chars into an array and iterate over that:
</p><pre>
vector&lt;wchar_t&gt; result (s.length());
copy (s.utf8_begin(), s.utf8_end(), result.begin());
</pre><p>
To write wide characters to the string, wchar_t values can be directly
given to push_back, insert, append, or assign, in the same way as the
char ones. 
</p><p>
@section SymbianStrings Symbian Strings
<a href="http://www.penrillian.com">Penrillian</a> have provided further translation functions to simplify interworking with Symbian descriptors:

 - ustl::string::string( const TDesC& ): constructs a UTF-8 string from a Symbian (UTF-16) descriptor.
 - ustl::string::DesL() answers a (temporary, stack-based) descriptor.
 - ustl::string::string( const TDesC8& ) constructs a string from a Symbian buffer.  Although the internal representation of the string is null-terminated at all times, the buffer may contain arbitrary characters (including nulls).
 - ustl::string::Des8L() answers a (temporary, stack-based) descriptor containing the bytes within the string.

The temporary object returned by DesL or Des8L may be passed to any Symbian function expecting a UNICODE descriptor.  Note that it allocates (and frees) the memory each time, so should be used with discretion.
@section StringWarning Warning!
N.b. For people used to uSTL 0.8, please note that I have changed the semantics of string::length and string::erase to be consistent with code expecting these to work with arbitrary byte collections - they no longer provide the UTF length and erasure.  The previous implementations are now named utf_length and utf_erase.

@b Next: @ref AlgorithmsIntro
*/
/**
@page AlgorithmsIntro Algorithms
<p>
Algorithms are the other half of STL. They are simply templated common
tasks that take iterator arguments, and as a result, work with any
container. Most will take an iterator range, like (v.begin(), v.end()),
but you can, of course operate on a subset of a container by giving a
different one. Because the usual operation is to use the whole container,
uSTL provides versions of most algorithms that take container arguments
instead of the iterator range.  Here are the algorithms you will actually
find useful:
</p><pre>
copy (v1, v2.begin());		// Copies vector v1 to vector v2.
fill (v, 5);			// Fills v with fives.
copy_n (v1, 5, v2.begin());	// Copies first five elements only.
fill_n (v.begin() + 5, 10, 5);	// Fills elements 5-15 with fives.
sort (v);			// Sorts v.
find (v, 14);			// Finds 14 in v, returning its iterator.
binary_search (v, 13);		// Looks up 13 with binary search in a sorted vector.
lower_bound (v, 13);		// Returns the iterator to where you want to insert 13.
iota (v.begin(), v.end(), 0);	// Puts 0,1,2,3,4,... into v.
reverse (v);			// Reverses all the elements in v.
</pre><p>
The rest are rarely of any use. There are obscure mathematical operations,
like inner_product, various set operations that almost never happen
in real code, and lots and lots of predicate algorithms. The latter
are algorithms that take a functor (an object that can be called
like a function) and were supposed to help promote code reuse by
encapsulating common operations.  For example, STL expects you to use
the <tt>for_each</tt> algorithm and write a little functor for all your
iterative tasks:
</p><pre>
class CCompareAndReplace {
public:
    CCompareAndReplace (int minValue, int maxValue, int badValue)
	: m_MinValue (minValue), m_MaxValue (maxValue), m_BadValue (badValue) {}
    void operator (int&amp; v) {
	if (v &lt; m_MinValue || v &gt; m_MaxValue)
	    v = m_BadValue;
    }
private:
    int m_MinValue;
    int m_MaxValue;
    int m_BadValue;
};

for_each (v.begin(), v.end(), CCompareAndReplace (5, 10, 99));
</pre><p>
And yes, it really does work. Doesn't generate any bloat either, since the
compiler can see right through all this trickery and expand the for_each
into a loop without actually creating the functor object. However,
the compiler has a much harder time when you start using containers
of complex objects and operating on their member variables and member
functions.  Since that is what you will most likely have in any real
code outside the academic world, the utility of predicate algorithms is
highly questionable.  Their readability is even more so, considering
that the above fifteen line example can be written as a three line
iterative foreach loop. Finally, there is the problem of where to put the
functor. It just doesn't seem to "belong" anywhere in the object-oriented
world. Sorry, Stepanov, I just don't see how these things can be anything
but an ugly, bloated hindrance.
</p>

@b Next: @ref StreamsIntro
*/
/**
@page StreamsIntro Streams

The C++ standard library provides global stream objects called cin,
cout, and cerr to replace printf and friends for accessing stdin, stdout,
and stderr, respectively. uSTL versions work mostly the same as the
standard ones (yes, the <tt>format</tt> call is a uSTL extension). Most
calls use snprintf for output and thus use whatever locale libc uses.
Fancy operations like stream tying will probably not be supported.
</p><pre>
cout &lt;&lt; "Hello world!" &lt;&lt; endl;
cout &lt;&lt; 456 &lt;&lt; ios::hex &lt;&lt; 0x1234 &lt;&lt; endl;
cerr.format ("You objects are at 0x%08X\n", &amp;o);
</pre><p>
String-writing streams are also available:
</p><pre>
string s;
ostringstream os (s);
os &lt;&lt; "Writing " &lt;&lt; n &lt;&lt; " objects into string s" &lt;&lt; endl;
</pre><p>
And that's pretty much it for the standard streams. File streams are not
implemented yet.  If you need to read a file now, create a string buffer,
read_file, and then attach a stream to it. This way is far friendlier
to merging with asynchronous event-driven frameworks (i.e. pretty much
any X-based UI system) where you're not allowed to sit around waiting for
file input. It also allows abstraction of all file operations in the same
manner as uSTL abstracts memory management from containers, and supporting
cool stuff like data filter chaining, network transparency, etc.
</p><p>
istream and ostream, which are not really usable by themselves in the
standard implementation, are hijacked by uSTL to implement binary data
input and output:
</p><pre>
const size_t writtenSize =
    Align (stream_size_of(number) +
           stream_size_of(ctr)) + 
    stream_size_of(n) +
    stream_size_of(v);
memblock buf (writtenSize);
ostream os (buf);
os &lt;&lt; number &lt;&lt; ctr;
os.align();
os &lt;&lt; n &lt;&lt; v;
</pre><p>
These operations are all very efficient, approaching a straight memcpy
in performance. ostream will not resize the buffer, hence the necessity
to estimate the final size. Most stream_size_of calls are computed at
compile time and thus produce no code. Because the data is written as
is, it is necessary to consider proper data alignment; for example,
a 4 byte int can not be written at stream offset 2. Some architectures
(Macs) actually crash when doing it; Intel processors just do it slowly.
Hence the need to pack the data to a proper "grain". The default align
call will pack to the maximum necessary grain, but can be given an
argument to change that. In case you're wondering, the reason for all
these idiosyncracies is optimization. The smallest and fastest possible
code to dump your stuff into a binary file is produced by this method.
uSTL defines flow operators to write integral values, strings, and
containers, but you can custom-serialize your objects like this:
</p><pre>
namespace myns {

/// Some class I want to serialize
class CMyClass {
public:
    void		read (istream&amp; is);
    void		write (ostream&amp; os) const;
    size_t		stream_size (void) const;
private:
    vector&lt;int&gt;		m_Elements;	///&lt; A bunch of elements.
    size_t		m_SomeSize;	///&lt; Some integral value.
    MyObject		m_SomeObject;	///&lt; Some other streamable object.
}

/// Reads the object from stream \p is.
void CMyClass::read (istream&amp; is)
{
    is &gt;&gt; m_Elements &gt;&gt; m_SomeSize &gt;&gt; m_SomeObject;
}

/// Writes the object to stream \p os.
void CMyClass::write (ostream&amp; os) const
{
    os &lt;&lt; m_Elements &lt;&lt; m_SomeSize &lt;&lt; m_SomeObject;
}

/// Returns the size of the written object.
size_t CMyClass::stream_size (void) const
{
    return (stream_size_of (m_Elements) +
	    stream_size_of (m_SomeSize) +
	    stream_size_of (m_SomeObject));
}

} // namespace myns

STD_STREAMABLE (myns::CMyClass)
</pre>

@b Next: @ref Tuples
*/
/**
@page Tuples Tuples

One last container I'll mention is a <tt>tuple</tt>, which is a fixed-size
array of identical elements. No, it's not the same as the tuple in boost,
which is more like a template-defined struct. I think the name fits
my object better. What are they good for? Graphical objects. Points,
sizes, rectangles, triangles, etc. As a bonus, operations on tuples can
automatically use SIMD instructions if they are available. Any fixed
size-array also works better as a tuple, since it becomes a standard
STL container, which you can use with any algorithm, copy by assignment,
initialize in the constructor, etc.
</p><pre>
typedef int32_t			coord_t;
typedef tuple&lt;2, coord_t&gt;	Point2d;
typedef tuple&lt;2, coord_t&gt;	Size2d;
typedef tuple&lt;2, Point2d&gt;	Rect;

Rect r (Point2d (1,2), Point2d (3,4));
r += Size2d (4, 4);
r[1] -= Size2d (1, 1);
foreach (Rect::iterator, i, r)
    TransformPoint (*i);
Point2d pt (1, 2);
pt += r[0];
pt *= 2;
</pre>

@b Next: @ref Memblocks
*/
/**
@page Memblocks Memblocks and Memlinks

The STL specification is only about containers and algorithms, the stuff
described from here on is totally non-standard, so by using them you'll
have to stick with uSTL as your STL implementation. I think it's worth
it, but, of course, the choice is up to you.
</p><p>
The major difference between the standart STL implementation and uSTL is
that the former has memory management stuff all over the place, while
the latter keeps it all together in the <tt>memblock</tt> class. Normally
STL containers are resized by calling <tt>new</tt> to create more storage
and then copying the elements there from the old one. This method wastes
space by fragmenting memory, wastes time by copying all the existing data
to the new location, and wastes codespace by having to instantiate all
the resizing code for each and every container type you have. This method
is also absolutely necessary to do this resizing in a perfectly object-safe
way. The uSTL way is to manage memory as an opaque, typeless block, and
then use the container templates to cast it to an appropriate pointer type.
</p><p>
This works just fine, except for one little catch: there is one type
of object you can't store in uSTL containers -- the kind that has pointers
to itself. In other implementations, resizing actually creates new objects
in the new location and destroys them in the old location. uSTL simply
memcpys them there without calling the copy constructor. In other words,
the object can not rely on staying at the same address. Most objects really
don't care. Note that this is not the same thing as doing a bitwise copy,
that you were rightly warned against before! It's a bitwise <em>move</em>
that doesn't create a new object, but simply relocates an existing one.
</p><p>
What this one small concession does is allow aggregation of all memory
management in one place, namely, the <tt>memblock</tt> class. All the
containers are thus converted mostly into typecasting wrappers that
exist to ensure type safety. Look at the assembly code and you'll see
mostly calls to memblock's functions. This is precisely the feature
that allows reduction in code instantiated by container templates.
</p><p>
However, memblock's usefulness doesn't end there! It can now replace
all your dynamically allocated buffers that you use for unstructured
data. Need to read a file? Don't use new to allocate memory; use a
memblock! It even has a friendly read_file member function for just
that purpose. Need to write a file? Use the write_file call! Unless
you are working with a database or some really large archive, you
should be able to load all your files this way. Imagine, not having
to worry about file I/O again! It's much nicer to work with data in
memory; you know how long it is, so you know when to stop. You can
seek with impunity, and any operations have the cost of a memcpy.
</p><p>
Memblock is derived from memlink, an object for linking to a memory
block. Now you get to store a pointer and the size of whatever it
points to, but with uSTL you can use a memlink object to keep them
together, reducing source clutter and making your code easier to
read and maintain. You can link to constant blocks too with cmemlink,
from which memlink is derived. Because all three are in a single
hierarchy, you never need to care whether you're working on an
allocated block or on somebody else's allocated block. Pointers are
kept together with block sizes, memory is freed when necessary,
and you never have to call new or delete again. Who needs garbage
collection? Memblocks give you the same functionality at a fraction
of the cost.
</p><p>
Linking is not limited to memlink. You can link memblock objects.
You can link string objects. You can even link containers! Now
you can use alloca to create a vector on the stack; use the
<tt>typed_alloca_link (v, int, 99)</tt> macro. All linked objects
will allocate memory and copy the linked data when you increase their
size. You can also do it explicitly by calling <tt>copy_link</tt>.
Why link? It's cheaper than copying and easier than keeping track
of pointers. For example, here's a line parser:
</p><pre>
string buf, line;
buf.read_file ("some_config_file.txt");
for (uoff_t i = 0; i &lt; buf.size(); i += line.size() + 1) {
    line.link (buf.iat(i), buf.iat (buf.find ('\n',i)));
    process_line (line);
}
</pre><p>
This way process_line gets a string object instead of a pointer and
a size. If you don't rely on the string being null-terminated, which
basically means not using libc functions on it, this is all you need.
Otherwise buf will have to be writable and you can replace the newline
with a null. In either case you are using no extra heap. The overhead
of link is negligible in most cases, but if you really want to do this
in a tight loop, you can use relink call, which expands completely
inline into one or two instructions, avoiding the virtual unlink() call.
</p>

@b Back: @ref intro

*/
/**
@page Contact Bug reporting
<p>
Report bugs through the SourceForge.net
<a href="http://sourceforge.net/projects/ustl">uSTL project page</a> with the
standard bugtracker.
</p>
*/


/// \defgroup Containers Containers
/// Here you'll find all the containers for your objects and data.
///  @sa @ref ContainersIntro
//
///	\defgroup MemoryManagement Memory Management
///	\ingroup Containers
///	Classes that implement low-level memory management and form the base for
///	all containers in the library. Almost all functionality in the containers
///	is reduced to calls to these base classes through a great deal of inline
///	crunching by the compiler, and thus you end up storing all your data in
///	ustl::memblock objects with the container templates as mere syntactic sugar.
//
///	\defgroup Sequences Sequence Containers
///	\ingroup Containers
///	Containers containing sequences of objects.
//
///	\defgroup AssociativeContainers Associative Containers
///	\ingroup Containers
///	Containers containing associations of objects.
//
/// \defgroup Streams Streams
/// Streams convert objects into flat data.
/// @sa @ref StreamsIntro
//
/// 	\defgroup BinaryStreams Binary Streams
///	\ingroup Streams
///	Unlike the C++ standard library,
///	the uSTL default behaviour is very strongly biased toward binary streams. I
///	believe that text formats should be used very sparingly due to numerous
///	problems they cause, such as total lack of structure, buffer overflows,
///	the great multitude of formats and encodings for even the most
///	trivial of things like integers, and the utter lack of readability
///	despite ardent claims to the contrary. Binary formats are well-structured,
///	are simpler to define exhaustively, are aggregates of basic types which
///	are universal to all architectures (with the exception of two types of
///	byte ordering, which I hope to be an issue that will go away soon), and
///	are much more readable (through an appropriate formatting tool equipped
///	to read binary format specifications).
//
///		\defgroup BinaryStreamIterators Binary Stream Iterators
///		\ingroup BinaryStreams
///		\ingroup Iterators
///		Iterators for using STL algorithms with binary streams.
//
///	\defgroup TextStreams TextStreams
///	\ingroup Streams
///	Streams converting objects into streams of text.
//
///		\defgroup DeviceStreams Device Streams
///		\ingroup Streams
///		Standard cout, cerr, and cin implementations for reading
///		and writing text through standard file descriptors.
//
/// \defgroup Iterators Iterators
/// Generalizations of the pointer concept, allowing algorithms to treat
/// all containers in a unified fashion.
///
/// @sa @ref ContainersIntro
//
///	\defgroup IteratorAdaptors Iterator Adaptors
///	\ingroup Iterators
///	Iterators made out of other iterators.
//
/// \defgroup Algorithms Algorithms
/// STL algorithms are the heart of generic programming. The idea is to
/// separate algorithms from containers to take advantage of the fact that
/// there are fewer distinct algorithms than typed containers. This is
/// diametrically opposed to object oriented programming, where each object
/// must contain all functionality related to its internal data. You will
/// find, I think, that in practice, generic programming is not terribly
/// convenient because it prevents you from encapsulating all your data.
/// The best approach is to compromise and have raw data classes that will
/// be manipulated by algorithms and to treat the rest of the objects as
/// stateful data transformers.
///
/// \sa @ref AlgorithmsIntro
//
///	\defgroup MutatingAlgorithms Mutating Algorithms
///	\ingroup Algorithms
///	Algorithms for modifying your data in some way.
//
///		\defgroup SortingAlgorithms Sorting Algorithms
///		\ingroup MutatingAlgorithms
///		Algorithms for sorting containers.
//
///		\defgroup GeneratorAlgorithms Generator Algorithms
///		\ingroup MutatingAlgorithms
///		Algorithms for generating data.
//
///		\defgroup SwapAlgorithms Swap Algorithms
///		\ingroup MutatingAlgorithms
///		Algorithms for swapping elements.
//
///	\defgroup SearchingAlgorithms Searching Algorithms
///	\ingroup Algorithms
///	Algorithms for searching through containers.
//
///	\defgroup RawStorageAlgorithms Raw Storage Algorithms
///	\ingroup Algorithms
///	Algorithms for manipulating unstructured memory.
//

/**
@page SymbianIntro Symbian Overview

By Charles Weir, <a href="http://www.penrillian.com">Penrillian</a>

There is one environment where code size still really matters: mobile phones.  

Mobile phone applications are often downloaded over the air; even if not, user applications
have to be loaded into RAM, and RAM size is limited to a few tens or at most hundreds of MB.

The most important native mobile phone environment, Symbian OS, doesn't as yet have a supported
version of STL.  With the introduction of Symbian 9, it's now become possible to 
port uSTL to Symbian.  This software is that port.  

@ref SymbianInstallation describes how to compile and test the Symbian installation.

@section STDLIB Compatibility.

For Symbian OS, the namespace is '\#define'-ed to be @b std:: rather than @b ustl::, so code using @b std::string and similar works correctly.

The Configure script also creates dummy header files @b vector and similar, so that code using

@code
	\#include <vector>
@endcode
works correctly.


@section SymbianMemory Symbian Memory Handling
We've modified out-of-memory handling for Symbian OS.  
Allocation failures now do a Leave( KErrNoMemory )  in the normal way.  

Thus any operation which may allocate memory (or is documented to throw bad_alloc) 
should be treated for Symbian OS programming purposes as though its name ends in L.

@section SymbianExceptions Symbian Exceptions
Normal Symbian OS code doesn't expect exceptions to be thrown - the TRAP macro Panic's, terminating
the application, if it receives one.
However for practical purposes Symbian programmers can mostly ignore exceptions when using the libraries.

The collection classes, string classes, and vector operations don't throw any exceptions (other than as a Leave for out of memory).  

The stream classes (ostringstream and istringstream), may throw the stream_bounds_exception, signifying an attempt to read/write of bounds.  In practice this probably occurs only with istringstream reading past the end of a string.  

There's also a libc_exception, which wraps errno-based errors, with a subclass file_exception.  Although I've used the file class for convenience in testing, this is not part of the standard, and this and its associated classes are the only places where libc_exceptions are thrown.

@section SymbianWarnings Warnings
Here are some warnings about using uSTL on Symbian OS:

 - Floating point is rendered in ostringstream::operator<< with comma thousand separators (123,456,789.00).  This is incompatible with reading in from the stream.

 - Symbian OS's lack of @b vnsprintf and @b snprintf means that using the ostringstream::printf functionality with %%s pointers to strings longer than 256 chars will corrupt memory.  The 256 byte buffer also wastes memory.

 - There's no direct 64-bit number support for Symbian.  I expect arrays of TInt64 or structs containing them will work fine - but arithmetical expressions and stream output won't work.

 - Symbian OS STDIN/STDOUT/STDERR support is omitted (I don't know of a use for it).

*/

/**
@page SymbianInstallation Symbian OS Installation

@section SymbianBuildingLibrary Deploying the uSTL Library

To build the uSTL library for Symbian OS UIQ 3 or S60 3rd Edition, do the following:

1. Ensure your Symbian 9 environment is set up correctly.

2. In this directory (ustldir\\Symbian), at a DOS prompt, type

@code
	configure
@endcode
This sets up the Symbian include directories, and moves files around to support Symbian compilation.

Then:

@code
	cd S60  //or 'cd UIQ' for UIQ 3
	deploy
@endcode
This builds the emulator and target libraries

@section SymbianUsingLibrary Using uSTL in Symbian OS Applications

As an example:

@code

	#include <vector>
	using namespace ustl;

	static void MainL() 
		{
		console->Write(_L("Vector Example\n"));
		
		vector<int> v;
		v.push_back (1);
		v.push_back (2);
		TBuf<32> buffer;
		buffer.Append(_L("Vector size is "));
		buffer.AppendNum(v.size());
		buffer.Append(_L("\n"));
		console->Write(buffer);
		v[1] = 0;
		v.erase (v.begin() + 1);
		v.pop_back();
		v.insert (v.begin(), 4);
		v.resize (15);
	
		buffer.Zero();
		buffer.Append(_L("First item in Vector is "));
		buffer.AppendNum(v.at(0));
		buffer.Append(_L("\n"));
		console->Write(buffer);
		}

In the MMP file(s) for your application you'll need the following extra:

@code
USERINCLUDE \epoc32\include\ustl
USERINCLUDE \epoc32\include\libc
SYSTEMINCLUDE \epoc32\include\libc

staticlibrary ustl.lib
LIBRARY		estlib.lib 
@endcode

API documentation for available classes can be found <a href="annotated.html">here</a>.

@subsection StdlibMemoryChecking Note on STDLIB memory checking

Because uSTL uses the Symbian C STDLIB, you may get spurious memory leak Panic's when
the emulator application exits (Panic: ALLOC: memoryAddress).

To prevent this (and help you make sure you haven't more important memory leaks), put a call
to CloseSTDLIB() so it's called on application exit (usually in the AppUI destructor):

@code
  #include <libc\sys\reent.h>
...
  CloseSTDLIB();
@endcode

@subsection Beta3Warning Important warning for UIQ3.0 Users (Final version 10 May 2006)

When compiling for the emulator, YOU MUST LINK using:

@code
	abld clean
	abld build winscw udeb
@endcode 

and @b not within the editor environment (this is a bug in the build chain involving stdlib)!
You can safely use the editor environment build for code syntax parsing and debugging.  
Ignore all the "multiply defined sections" error messages when you link as above.

*/

/**
@page SymbianTest Testing the Symbian OS version


@section SymbianEmulatorTests Compiling and Running the Tests

To compile and run the tests (completely optional):

1. Ensure your Symbian 9 environment is set up correctly.

2. In this directory (ustldir\\Symbian), at a DOS prompt, type

@code
	configure
@endcode

This sets up the Symbian include directories, and moves files around to support Symbian compilation.

3. Then to create the test program (and also the emulator library):

@code
	cd S60  //or 'cd UIQ' for UIQ 3
	Build
@endcode


3. You can then run the emulator, and run USTLTEST within.  This creates a set of test output files
in the emulator "C:\\" drive.

To validate the output: 

@code
	popd
	SymbianVerifyOutput
@endcode

The differences are generally minor, and are summarised at the end of this note.

4. You can then do the same for the target hardware builds:

@code
	pushd S60   //or 'pushd UIQ' for UIQ 3
	buildTarget
@endcode

This builds the target (GCCE UREL) library and a test SIS file (ustldir\\Symbian\\UIQ\\%USTLTEST.SISX or ustldir\\Symbian\\S60\\%USTLTEST.SISX).
In Beta 3 ignore warnings "\\EPOC32\\DATA\\Z\\Resource\\Apps given more than once in the same rule."

The SIS file is Self Signed.  See www.symbiansigned.com for more details.

In case of UIQ 3, the resulting files are placed on D:\\ on the phone - on the P990 this is the Memory Stick.  To
validate them, we put the Memory Stick in a card reader, copy them to ...\\EPOC32\\RELEASE\\WINSCW\\C
and used SymbianVerifyOutput as above...

In case of S60 3rd, the resulting files are placed on E:\\ on the phone (as specified on line 56 of \\bvt\\symbiantest.cpp) - on the N95/E90/N73/E61 this is the Memory Card.  To
validate them, we put the Memory Card in a card reader, copy them to ...\\EPOC32\\RELEASE\\WINSCW\\C
and used SymbianVerifyOutput as above...

@section SymbianTestResults Test Results - Summary of Differences

The following are the differences found by SymbianVerifyOutput:
 - Bvt02: Different garbage - irrelevent
 - Bvt03: Irrelevent differences in detailed rendering of double.
 - Bvt05: No 64-bit types.
 - Bvt07: Difference in capacity - we've allocated an extra 256 bytes to get around vnsprintf absence.
 - Bvt09: Stringstream input of doubles rounds to zero significant digits.  Reading a unsigned short integer (16 bit integer) doesn't work correctly. This is because wchar_t is typedef'd to be a the same type, and reading a wchar_t does normal UTF8 conversion.
 - Bvt13: Test omitted (no stdin/stdout/stderr).
 - Bvt18: Testing tuple<8,int8_t>: Symbian chars don't overflow at FFh, so we get the right answer here; the original doesn't.
 
 - Bvt21: No 64 bit types.
 - Bvt23: No 64 bit types.  Also all floats seem to be rendered with 2 decimal places, not 1.   
          In rounding float and doubles to int32_t, Symbian OS rounds 0.5 'up' rather than 'down' (other values are OK). 
          
 - Bvt25: TODO

*/


/**
@page Gotchas Gotchas!
Here is a summary of the main differences between uSTL and STL:

 - There's currently no @b wstring, @b priority_queue, @b complex, @b deque, nor @b stringstream.  (Nor the hash collections: @b hash_set, @b hash_map, @b hash_multimap, @b hash_multiset).

 - There are no @b fstream classes and related functionality.

 - The semantics of ustl::string's functions are not quite standard (some return void rather than a reference to self).

 - In the collections, elements can move - don't keep any pointers to elements in the array.

 - ustl::list is simply a wrapper for ustl::vector - it doesn't have the performance semantics required by the standard.

 - Although ustl::ostringstream derives from ustl::ostream, you mustn't pass an ostringstream as an ostream (functions aren't virtual).   Ostringstream and istringstream don't provide the horrible rdbuf() stuff to return char* buffers.

 - Most of the obscure STL arithmetic functions (@b plus, function combinations, etc) are missing.


*/