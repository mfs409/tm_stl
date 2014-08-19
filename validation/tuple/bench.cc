/*
  Driver to test the transactional version of the std::tuple interface

  According to http://www.cplusplus.com/reference/tuple/tuple/, the
  std::tuple interface consists of the following:

  Steps:
    1 - Put TBD traces into all of GCC's tuple functions
    2 - For one category at a time, replace TBDs and update columns 3 and 4
        of table below
    3 - Write test code for ensuring that every traced function is called,
        and then write DONE in the category

|-------------------+------------------+----------+--------|
| Category          | Functions        | C++14    | GCC    |
|                   |                  | Expected | Actual |
|-------------------+------------------+----------+--------|
| Member Functions  | (constructor)    |          |        |
|                   |                  |          |        |
|                   | (destructor)     |          |        |
|                   | operator=        |          |        |
|                   | swap             |          |        |
|-------------------+------------------+----------+--------|
| Non-member        | '=='             |          |        |
| function          | '!='             |          |        |
| overloads (NMFOs) | '<'              |          |        |
|                   | '<='             |          |        |
|                   | '>'              |          |        |
|                   | '>='             |          |        |
|                   | swap             |          |        |
|                   | get              |          |        |
|-------------------+------------------+----------+--------|
| Non-member        | uses_allocator   |          |        |
| class             |                  |          |        |
| specializations   |                  |          |        |
|-------------------+------------------+----------+--------|
| Non-member        | make_tuple       |          |        |
| Functions         | forward_as_tuple |          |        |
|                   | tie              |          |        |
|                   | tuple_cat        |          |        |
|                   | get              |          |        |
|-------------------+------------------+----------+--------|
*/

#include <list>
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
         << "               3 swap" << endl
         << "               4 non-member function overloads" << endl
         << "               5 class specializations" << endl
         << "               6 non-member functions" << endl
         << endl;
    exit(0);
}

#define NUM_TESTS 7
bool test_flags[NUM_TESTS] = {false};

void (*test_names[NUM_TESTS])(int) = {
    NULL,
    ctor_dtor_tests,                                    // member.cc
    op_eq_tests,                                        // member.cc
    swap_tests,                                         // member.cc
    overload_tests,                                     // overload.cc
    specialize_tests,                                   // specialize.cc
    function_tests,                                     // function.cc
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
