tm_stl
======

A scratchpad for working on making the C++11 STL code from gcc 4.10
transaction-safe.

To that end, this project entails two efforts:
   1. Write driver programs that we can prove call every method of the STL
   2. Make the necessary modifications so that every STL method can be called from an atomic transaction

To achieve the first goal, we have a version of libstdc++ in which there is a
`printf` within every STL function.  We call this the **trace** version.
Then, for each STL collection, there is a subfolder within the **validation**
folder.  When the code in the appropriate subfolder of validation is linked
to the library in the trace folder, and run, output should indicate that
every STL function is being called.

To achieve the second goal, we flip a `#define` so that the validation code
attempts to call STL functions from within `__transaction_atomic` blocks.  We
then link against the **tm** version of libstdc++.  It should be the case
that there are no link errors, and the program executes without any errors.

Note: there is a libstdc++ folder containing unmodified gcc 4.10 source code.

Notice
----
On 9 Aug 2014, this repository was heavily restructured.  Please note that
most file locations have changed to fit with the above description.

Folders
----

libstdc++/:
   This is the original libstdc++ code from GCC.  Note that it pulls from a
   few parts of GCC, in order to be compilable.

libstdc++_tm/:
   This version of libstdc++ has been annotated or otherwise modified as
   little as possible to make all STL calls transaction-safe.

libstdc++_trace/:
   This version of libstdc++ has `printf` statements in every method call, so
   that we can be sure that we have 100% coverage of the STL functions.
   
old/:
   Any work from before 9 Aug 2014 that has not yet been ported to the new
   layout now resides in this folder.  Once it is migrated, it will no longer
   reside in this folder.

   
validation/:
   In the subfolders of validation are the programs for testing individual
   STL containers.

Status
----

### std::list:   Complete
### std::deque:  Incomplete, in old/ folder
### std::map:    Incomplete, in old/ folder
### std::queue:  Incomplete, in old/ folder
### std::string: Incomplete, in old/ folder
   + When last we looked, this wasn't going to work due to std::string not
   conforming to C++11 requirements (it is still reference counted!)

### std::vector: Incomplete, in old/ folder
