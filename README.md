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