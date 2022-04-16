# B_dk
A (family of) B compilers.

I have always been fascinated by "typeless" languages.  As a student of "C" I have been particularly fascinated by C's predecessor, "B".  Not only am I fascinated by B, but I think that it can be a very useful language, especially on small systems.  The reasoning for that will be detailed later.

If you have programmed in C for very long, you have likely heard that it developed from B.  The story is much longer, of course.  And it is interesting.  Here is a short summary, as interpreted by me.  A lot of this is fuzzy and some details are almost certainly wrong, but this is the gist of what happened.

In the early 60s at Cambridge a language was defined that was intended to be a do-all language (shades of PL/I).  More to follow on that later.  For various reasons it ran into troubles.  But one researcher, Martin Richards, created a simplified version.  The original language was named either "Cambridge Programming Language" or "Combined Programming Language" or both.  In short, "CPL".  Richards created a simplified, typeless version and called it "Basic CPL", or BCPL.  It was implemented around 1967.  About the sametime Ken Thompson and Dennis Ritchie from Bell Labs were working on the Multics project.  They were apparently exposed to BCPL during that time.  Ritchie (and others?) apparently created a BCPL compiler for Bell Labs around that time.  When Bell Labs pulled out of the Multics project, Ken Thompson took some of what they had done there and created the very first version of UNIX on a PDP-7 that was "abandoned" at Bell Labs.  It was a very small machine (something like 8 K words of 18 bit memory.)  Originally everything was written in assembly language, which would be rather tedious even on a modern machine.  On a small, slow PDP-7 in the 60s it would be even worse.  At any rate, the pair decided they needed a high-level language.  Thompson (?) took BCPL and stripped it down to bare essentials, probably to make it practical on the very small machine.  Other changes were made, probably to his liking.  B was born.  The origin of the name is open to debate.

What does B look like?

B, as might be expected, looks a lot like C.  But it is quite different in many ways from modern C.  For instance, the following code is perfectly legal in B:

square(x) return(x*x);

savesquare(y) { *1024 = square(4); }

It defines two functions, square and savesquare.  Notice there are no types listed.  square has a single statement, not enclosed in braces.  savesquare calls square with the argument 4 and writes the returned value (16) to the memory word at address 1024.

The language is in itself very permissive.  That's the nature of the beast.  The compiler(s) will at first be even more permissive in the interest of making them small and simple.  Some restrictions will likely come later, but not too many;  the idea is to keep the flavor of the language, not turn it into (modern) C.

An example of the nature of permissiveness is this: there is no distinction between an integer and a pointer.  There is only one type of operand in the language, the machine word.  A variable is a variable is a variable.  It holds a number.  How you use that number is up to you.  If you use it as an integer, fine.  If you use it as a character (or two!) that is fine as well.  If you use it as a pointer (address) to a number, a character, a string, a 
function, another pointer, or whatever, that is fine.  Or a boolean value.  Or a set of boolean values.  It is a LOT like assembly language.  In fact, many people have called C a "high level assembly language."  That is a lot more true of B than C.

The Plan
First, I am writing a B compiler in C.  The C version will be used to bootstrap B.  I am defining a type "word" that is a 16 bit value.  The C code will use "word" as (almost) the only type in the program.  The C code will end up looking very much like the equivalent program in B.  This first compiler will emit code for a virtual machine rather than machine code for any real machine.  That means I have to write an interpreter for the virtual machine.  That doesn't really need to be limited in any way.  At that point we can write, compile, and run B programs on a PC.

The next step is to port the C code for the compiler into B.  At that point we have a compiler for B written in B that can emit code for the virtual machine, therefore the compiler can compile itself.

We can, at this stage, write a virtual machine in whatever language is convenient for any machine and be able to run B programs on that machine.  I may or may not do that for some chosen target(s).  

The bigger and ultimate goal is to change the compiler to produce machine code for some real machine.  I plan the first target to be the RCA 1802 COSMAC.  
At that point we will have a B compiler written in B that emits code for some specific machine.  At first it is a "cross compiler" that runs on one machine and produces code for a different machine.  But, we can use it to compile itself FOR THAT MACHINE!  Then we have a B compiler for that machine that runs natively on that machine.  For the example above, we would have a B compiler that runs on an 1802 and produces code for the 1802.  And that is the main goal.

After that, there are many things we can do.  We can improve the compiler with better checking or restrictions.  We can make it optimize code.  We can do different things to the different versions (native vs cross.)  Whatever.  But those decisions will come later.  The main reason I'm writing this is to have a good language native on the 1802 that is a good fit for it.  The 1802 is an interesting processor, but not very fast and has some limitations.  I think B is a better fit than C.  A C compiler is by necessity a bigger and slower compiler.  And with a rather slow processor and 64K of address space, big programs aren't a good fit, either.

The C bootstrap version will be in C_bootstrap.  Documentation will be in docs.  The rest is yet to be determined.

Note:
This compiler isn't intended as an education in compiler writing, but it may be educational to read it.  It is much smaller and simpler than most "real" compilers.  
