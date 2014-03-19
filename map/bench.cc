/*
  Driver to test the transactional version of the std::map interface

  To fill out the following table, use
  http://www.cplusplus.com/reference/map/map/, as well as the source code at


  We use this list for a few purposes.

  ** Status 1: Make sure that every method of std::map is listed in the
     Functions and Count columns.  This part is supposed to correlate to what
     is listed on cplusplus.com (Spec), and what is present in the
     implementation (Impl).  Be sure to annotate those places where things
     don't make sense, and be sure to capture everything public in the
     header.  In particular, pay close attention to iterators and overloads
     of std:: functions

  ** Status 2: Make sure that every public function in the GCC STL source
     code is listed in Trace Location, with the location of the line of code
     where the TRACE macro was placed.  The TRACE call should indicate which
     version of a method is being called, so that the output will allow us to
     conclude that we have complete coverage.

  ** Status 3: Write a test program that causes every TRACE macro for the
     category to lead to the appropriate printf being called, when the
     '_notm' version of the code was run.  At that point, we know that every
     method within the category is being called.

  ** Status 4: Get the '_tm' version of the code to compile and run.  This is
     likely where difficulties will arise, with unexpectedly
     not-transaction-safe code cropping up.  When we reach this, we've
     discovered every obstacle to transaction-safety.

|------------------------+-----------------+--------+--------+--------------------+--------|
| Category               | Functions       |  Count | Count  | Trace Location     | Status |
|                        |                 | (Spec) | (Impl) | (default: bits/??? |        |
|------------------------+-----------------+--------+--------+--------------------+--------|
| Member Functions       | (constructor)   |      8 |        |                    |        |
|                        | (destructor)    |      1 |        |                    |        |
|                        | operator=       |      3 |        |                    |        |
|------------------------+-----------------+--------+--------+--------------------+--------|
| Iterators              | begin           |      2 |        |                    |        |
|                        | end             |      2 |        |                    |        |
|                        | rbegin          |      2 |        |                    |        |
|                        | rend            |      2 |        |                    |        |
|                        | cbegin          |      1 |        |                    |        |
|                        | cend            |      1 |        |                    |        |
|                        | crbegin         |      1 |        |                    |        |
|                        | crend           |      1 |        |                    |        |
|------------------------+-----------------+--------+--------+--------------------+--------|
| Iterator               | ctor            |        |        |                    |        |
| Methods                | operator*       |        |        |                    |        |
|                        | operator->      |        |        |                    |        |
|                        | operator++      |        |        |                    |        |
|                        | operator--      |        |        |                    |        |
|                        | operator+=      |        |        |                    |        |
|                        | operator+       |        |        |                    |        |
|                        | operator-=      |        |        |                    |        |
|                        | operator-       |        |        |                    |        |
|                        | operator[]      |        |        |                    |        |
|------------------------+-----------------+--------+--------+--------------------+--------|
| Iterator               | operator==      |        |        |                    |        |
| Overloads              | operator!=      |        |        |                    |        |
|                        | operator<       |        |        |                    |        |
|                        | operator>       |        |        |                    |        |
|                        | operator<=      |        |        |                    |        |
|                        | operator>=      |        |        |                    |        |
|                        | operator-       |        |        |                    |        |
|                        | operator+       |        |        |                    |        |
|------------------------+-----------------+--------+--------+--------------------+--------|
| Iterator               | copy-assignable |        |        |                    |        |
| Other                  | destructible    |        |        |                    |        |
|                        | swappable       |        |        |                    |        |
|------------------------+-----------------+--------+--------+--------------------+--------|
| Const Iterator         |                 |        |        |                    |        |
| Methods                |                 |        |        |                    |        |
| (subsumed by above)    |                 |        |        |                    |        |
|------------------------+-----------------+--------+--------+--------------------+--------|
| reverse_iterator*      |                 |        |        |                    |        |
| methods                |                 |        |        |                    |        |
|------------------------+-----------------+--------+--------+--------------------+--------|
| Const Reverse Iterator |                 |        |        |                    |        |
| Methods                |                 |        |        |                    |        |
| (subsumed by above)    |                 |        |        |                    |        |
|------------------------+-----------------+--------+--------+--------------------+--------|
| Capacity               | empty           |      1 |        |                    |        |
|                        | size            |      1 |        |                    |        |
|                        | max_size        |      1 |        |                    |        |
|------------------------+-----------------+--------+--------+--------------------+--------|
| Element access         | operator[]      |      2 |        |                    |        |
|                        | at              |      2 |        |                    |        |
|------------------------+-----------------+--------+--------+--------------------+--------|
| Modifiers              | insert          |      6 |        |                    |        |
|                        | erase           |      3 |        |                    |        |
|                        | swap            |      1 |        |                    |        |
|                        | clear           |      1 |        |                    |        |
|                        | emplace         |      1 |        |                    |        |
|                        | emplace_hint    |      1 |        |                    |        |
|------------------------+-----------------+--------+--------+--------------------+--------|
| Observers              | key_comp        |      1 |        |                    |        |
|                        | value_comp      |      1 |        |                    |        |
|------------------------+-----------------+--------+--------+--------------------+--------|
| Operations             | find            |      2 |        |                    |        |
|                        | count           |      1 |        |                    |        |
|                        | lower_bound     |      2 |        |                    |        |
|                        | upper_bound     |      2 |        |                    |        |
|                        | equal_range     |      2 |        |                    |        |
|------------------------+-----------------+--------+--------+--------------------+--------|
| Allocators             | get_allocator   |      1 |        |                    |        |
|------------------------+-----------------+--------+--------+--------------------+--------|
*/

#include <cstdio>
#include <thread>
#include <atomic>
#include <mutex>
#include <cassert>
#include <iostream>
#include <unistd.h>

#include "../common/barrier.h"
#include "tests.h"

using std::cout;
using std::endl;

/// configured via command line args: number of threads
int  num_threads = 1;

/// the barrier to use when we are in concurrent mode
barrier* global_barrier;

/// the mutex to use when we are in concurrent mode with tm turned off
std::mutex global_mutex;

/// Report on how to use the command line to configure this program
void usage()
{
    cout << "Command-Line Options:" << endl
         << "  -n <int> : specify the number of threads" << endl
         << "  -h       : display this message" << endl
         << "  -T       : enable all tests" << endl
         << "  -t <int> : enable a specific test" << endl
         << "               1 constructors and destructors" << endl
         << "               2 operator=" << endl
         << "               3 iterator creation" << endl
         << "               4 basic iterator use" << endl
         << "               5 const iterator use" << endl
         << "               6 reverse iterator use" << endl
         << "               7 const reverse iterator use" << endl
         << "               8 capacity methods" << endl
         << "               9 element access methods" << endl
         << "              10 modifier methods" << endl
         << "              11 observer methods" << endl
         << "              12 relational operator use" << endl
         << "              13 swap use" << endl
         << endl;
    exit(0);
}

#define NUM_TESTS 14
bool test_flags[NUM_TESTS] = {false};

void (*test_names[NUM_TESTS])(int) = {
    NULL,
    ctor_dtor_tests,                                    // member.cc
    op_eq_tests,                                        // member.cc
    iter_create_tests,                                  // iter.cc
    basic_iter_tests,                                   // iter.cc
    const_iter_tests,                                   // iter.cc
    rev_iter_tests,                                     // iter.cc
    const_rev_iter_tests,                               // iter.cc
    cap_tests,                                          // cap.cc
    element_tests,                                      // element.cc
    modifier_tests,                                     // modifier.cc
    observers_test,                                     // observer.cc
    operations_tests,                                   // operations.cc
    allocator_tests                                     // allocator.cc
};

/// Parse command line arguments using getopt()
void parseargs(int argc, char** argv)
{
    // parse the command-line options
    int opt;
    while ((opt = getopt(argc, argv, "n:ht:")) != -1) {
        switch (opt) {
          case 'n': num_threads = atoi(optarg); break;
          case 'h': usage();                    break;
          case 't': test_flags[atoi(optarg)] = true; break;
          case 'T': for (int i = 0; i < NUM_TESTS; ++i) test_flags[i] = true; break;
        }
    }
}

/// A concurrent test for exercising every method of std::list.  This is
/// called by every thread
void per_thread_test(int id)
{
    // wait for all threads to be ready
    global_barrier->arrive(id);

    // run the tests that were requested on the command line
    for (int i = 0; i < NUM_TESTS; ++i)
        if (test_flags[i])
            test_names[i](id);
}

/// main() just parses arguments, makes a barrier, and starts threads
int main(int argc, char** argv)
{
    // figure out what we're doing
    parseargs(argc, argv);

    // set up the barrier
    global_barrier = new barrier(num_threads);

    // make threads
    std::thread* threads = new std::thread[num_threads];
    for (int i = 0; i < num_threads; ++i)
        threads[i] = std::thread(per_thread_test, i);

    // wait for the threads to finish
    for (int i = 0; i < num_threads; ++i)
        threads[i].join();
}
