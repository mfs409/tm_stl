tm_stl
======

A scratchpad for working on making some of the C++11 STL code from gcc 4.9 transaction-safe

Folders
----

libinc:
   This folder stores unmodified copies of the files in
   <gcc_install>/lib/gcc/x86_64-unknown-linux-gnu/4.9.0/include/

platform_inc:
   This folder stores unmodified copies of the files in
   <gcc_install>/include/c++/4.9.0/x86_64-unknown-linux-gnu/

stdinc:
   This folder stores unmodified copies of the files in
   <gcc_install>/include/c++/4.9.0/

tm_stdinc:
   If we require a modification to something in stdinc, we first move it
   here, then we modify it.  All changes are annotated via comments with
   [TM] and the @step at which this change was made.

tm_libstdc++-v3:
   In some cases, the tm_stdinc header file changes require us to recompile
   parts of libstdc++.  When that happens, we copy the corresponding .cc
   files here, so that we know exactly what had to be recompiled.
