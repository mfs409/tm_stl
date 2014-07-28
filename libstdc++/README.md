About
======

This folder consists entirely of source code from gcc.  It has been
restructured a bit, so that one can compile the C++ standard library without
having a full GCC source tree.  This is beneficial because we want to be able
to have multiple versions of this library at the same time.

Folders
-------

include/:
  These are include files needed to build libstdc++-v3

libgcc/:
  These are the few tiny parts of the GCC source tree's libgcc folder that
  are needed to build libstdc++-v3

libiberty/:
  These are the few tiny parts of the GCC source tree's libiberty folder that
  are needed to build libstdc++-v3

libstdc++-v3/:
  This is the full libstdc++-v3 source tree.  It includes an 'include'
  folder, which holds all of the headers needed when using libstdc++, the
  'libsupc++' folder needed for some low-level stuff, and the 'src' folder
  containing the code that must be compiled to build libstdc++.  Note that
  the actual files in all subfolders were chosen by tracing symlinks after
  configuring on a 64-bit x86 Linux box.
