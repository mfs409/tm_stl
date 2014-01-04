tm_stl
======

A scratchpad for working on making some of the C++11 STL code from gcc 4.9 transaction-safe

Folders
----

inc/:
   We use -nostdinc when building, so that GCC does not use any of its
   standard header files.  We then manually copy any include files that our
   program needs into this folder tree, so that we can easily track which
   (hopefully few) files actually require modification in order to achieve a
   fully transactional collection.
   
inc/stdinc:
   This folder stores unmodified copies of the files in
   <gcc_install>/include/c++/4.9.0/.  These are the bulk of the stl
   implementation.

inc/platform_inc:
   This folder stores unmodified copies of the files in
   <gcc_install>/include/c++/4.9.0/x86_64-unknown-linux-gnu/.  These are the
   parts of the stl implementation that are platform-dependent.

inc/libinc:
   This folder stores unmodified copies of the files in
   <gcc_install>/lib/gcc/x86_64-unknown-linux-gnu/4.9.0/include/.

inc/tm_stdinc:
   If we require a modification to something in stdinc, we first move it
   here, then we modify it.  It should be the case that the only required
   modifications are annotations, though there are also times where we must
   wrap abort calls in a pure wrapper.

lib/:
   Some portions of the stl are not implemented in headers.  When those parts
   become transaction_safe, we need to re-compile the appropriate parts of
   libstdc++.so.6.  The easiest way to do it for now is to copy the relevent
   .cc files here, compile them during the benchmark build, and then
   statically link them.

lib/libstdc++-v3:
   A home for the files from gcc's libstdc++ implementation that we need to
   recompile.  It should be the case that these files do not require
   modification.

src/:
   It ought to be the case that we can just mark headers as transaction-safe,
   and then everything will "just work".  In reality, this is insufficient,
   because so much of the STL is implemented in templates, and thus a "safe"
   template function isn't statically checked for safety until it is actually
   instantiated.  Consequently, we require a program that calls every method
   of the collection, to ensure that the transaction_safe annotations do not
   generate errors due to unsafe calls within safe code.  That program is
   implemented in this folder.

Status
----

### std::list (in progress)

  + The first step is to produce a sequential program that instantiates every
    method of std::list (matching not just names, but parameter types).  This
    effort is underway.  We are using www.cplusplus.com as a reference to
    identify every public method in std::list, in all of its forms.  Note
    that we are only concerned with C++11... C++98-only versions mentioned on
    the site are being ignored. **Status: DONE**

  + The second step is to create a multithreaded program that also
    instantiates everything in std::list.  There is an initial ad-hoc attempt
    at this.  The ad-hoc code will be deleted once we finish step 1, because
    it would be better to use step 1 as a starting point for doing step 2
    correctly.

  + There are currently annotations in the files in inc/tm_stdinc.  These
    annotations are legacy, from when we were doing things in a more ad-hoc
    fashion.  All annotations will be discarded, and the effort re-done, once
    we finish step 1.
