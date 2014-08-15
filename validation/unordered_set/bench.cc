/*
  Driver to test the transactional version of the std::unordered_set
  interface

  According to
  http://www.cplusplus.com/reference/unordered_set/unordered_set/, the
  std::unordered_set interface consists of the following:

  Steps:
    1 - Put TBD traces into all of GCC's unordered_set functions
    2 - For one category at a time, replace TBDs and update columns 3 and 4
        of table below
    3 - Write test code for ensuring that every traced function is called,
        and then write DONE in the category

|-------------------+------------------+----------+--------|
| Category          | Functions        | C++14    | GCC    |
|                   |                  | Expected | Actual |
|-------------------+------------------+----------+--------|
| Member            | (constructor)    |          |        |
| Functions         |                  |          |        |
|                   | (destructor)     |          |        |
|                   | operator=        |          |        |
|-------------------+------------------+----------+--------|
| Iterators         | begin            |          |        |
|                   | end              |          |        |
|                   | cbegin           |          |        |
|                   | cend             |          |        |
|-------------------+------------------+----------+--------|
| Iterator          | ctor             |          |        |
| Methods           | dtor             |          |        |
|                   | operator*        |          |        |
|                   | operator->       |          |        |
|                   | operator++       |          |        |
|                   | operator--       |          |        |
|                   | operator+=       |          |        |
|                   | operator+        |          |        |
|                   | operator-=       |          |        |
|                   | operator-        |          |        |
|                   | operator[]       |          |        |
|-------------------+------------------+----------+--------|
| Iterator          | operator==       |          |        |
| Operators         | operator!=       |          |        |
|                   | operator<        |          |        |
|                   | operator>        |          |        |
|                   | operator<=       |          |        |
|                   | operator>=       |          |        |
|                   | operator-        |          |        |
|                   | operator+        |          |        |
|-------------------+------------------+----------+--------|
| Iterator          | copy-assignable  |          |        |
| Overloads         | destructible     |          |        |
|                   | swappable        |          |        |
|-------------------+------------------+----------+--------|
| Iterator          | fill             |          |        |
| Functions         | copy             |          |        |
|                   | copy_backward    |          |        |
|                   | move             |          |        |
|                   | move_backward    |          |        |
|-------------------+------------------+----------+--------|
| Const             |                  |          |        |
| Iterator          |                  |          |        |
| Methods           |                  |          |        |
|-------------------+------------------+----------+--------|
| Capacity          | empty            |          |        |
|                   | size             |          |        |
|                   | max_size         |          |        |
|-------------------+------------------+----------+--------|
| Element           | find             |          |        |
| Lookup            | count            |          |        |
|                   | equal_range      |          |        |
|-------------------+------------------+----------+--------|
| Modifiers         | emplace          |          |        |
|                   | emplace_hint     |          |        |
|                   | insert           |          |        |
|                   | erase            |          |        |
|                   | clear            |          |        |
|                   | swap             |          |        |
|-------------------+------------------+----------+--------|
| Buckets           | bucket_count     |          |        |
|                   | max_bucket_count |          |        |
|                   | bucket_size      |          |        |
|                   | bucket           |          |        |
|-------------------+------------------+----------+--------|
| Hash Policy       | load_factor      |          |        |
|                   | max_load_factor  |          |        |
|                   | rehash           |          |        |
|                   | reserve          |          |        |
|-------------------+------------------+----------+--------|
| Observers         | hash_function    |          |        |
|                   | key_eq           |          |        |
|                   | get_allocator    |          |        |
|-------------------+------------------+----------+--------|
| Non-member        | '=='             |          |        |
| function          | '!='             |          |        |
| overloads (NMFOs) | swap             |          |        |
|-------------------+------------------+----------+--------|
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
         << "               4 iterator methods" << endl
         << "               5 iterator operators" << endl
         << "               6 iterator overloads" << endl
         << "               7 iterator functions" << endl
         << "               8 capacity methods" << endl
         << "               9 element access methods" << endl
         << "              10 modifier methods" << endl
         << "              11 bucket methods" << endl
         << "              12 hash methods" << endl
         << "              13 observer methods" << endl
         << "              14 relational operator use" << endl
         << "              15 swap use" << endl
         << endl
         << "  Note: const, reverse, and const reverse iterators not tested"
         << endl
         << endl;
    exit(0);
}

#define NUM_TESTS 16
bool test_flags[NUM_TESTS] = {false};

void (*test_names[NUM_TESTS])(int) = {
    NULL,
    ctor_dtor_tests,                                    // member.cc
    op_eq_tests,                                        // member.cc
    iter_create_tests,                                  // iter.cc
    iter_method_tests,                                  // iter.cc
    iter_operator_tests,                                // iter.cc
    iter_overload_tests,                                // iter.cc
    iter_function_tests,                                // iter.cc
    cap_tests,                                          // cap.cc
    element_tests,                                      // element.cc
    modifier_tests,                                     // modifier.cc
    bucket_tests,                                       // bucket.cc
    hash_tests,                                         // hash.cc
    observer_tests,                                     // observer.cc
    relational_operator_tests,                          // overloads.cc
    swap_tests                                          // overloads.cc
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

/// A concurrent test for exercising every method.  This is called by every
/// thread
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
