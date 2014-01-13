/*
  Driver to test the transactional version of the std::vector interface

  According to http://www.cplusplus.com/reference/vector/vector/, the
  std::vector interface consists of the following:

|--------------------+---------------+-----------+----------------|
| Category           | Method        | # Methods | Location       |
| (* if not started) |               |           | (default: ???) |
|--------------------+---------------+-----------+----------------|
| Member Functions   | ctor          | 6*        |                |
|                    | dtor          | 1*        |                |
|                    | operator =    | 3*        |                |
|--------------------+---------------+-----------+----------------|
| Iterators          | begin         | 2*        |                |
|                    | end           | 2*        |                |
|                    | rbegin        | 2*        |                |
|                    | rend          | 2*        |                |
|                    | cbegin        | 1*        |                |
|                    | cend          | 1*        |                |
|                    | crbegin       | 1*        |                |
|                    | crend         | 1*        |                |
|--------------------+---------------+-----------+----------------|
| Capacity           | size          | 1*        |                |
|                    | max_size      | 1*        |                |
|                    | resize        | 2*        |                |
|                    | capacity      | 1*        |                |
|                    | empty         | 1*        |                |
|                    | reserve       | 1*        |                |
|                    | shrink_to_fit | 1*        |                |
|--------------------+---------------+-----------+----------------|
| Element access     | operator []   | 2*        |                |
|                    | at            | 2*        |                |
|                    | front         | 2*        |                |
|                    | back          | 2*        |                |
|                    | data          | 2*        |                |
|--------------------+---------------+-----------+----------------|
| Modifiers          | assign        | 3*        |                |
|                    | push_back     | 2*        |                |
|                    | pop_back      | 1*        |                |
|                    | insert        | 5*        |                |
|                    | erase         | 2*        |                |
|                    | swap          | 1*        |                |
|                    | clear         | 1*        |                |
|                    | emplace       | 1*        |                |
|                    | emplace_back  | 1*        |                |
|--------------------+---------------+-----------+----------------|
| Allocator          | get_allocator | 1*        |                |
|--------------------+---------------+-----------+----------------|
| Non-member         | '=='          | 1*        |                |
| function           | '!='          | 1*        |                |
| overloads (NMFOs)  | '<'           | 1*        |                |
| (Relational        | '<='          | 1*        |                |
| Operators)         | '>'           | 1*        |                |
|                    | '>='          | 1*        |                |
|--------------------+---------------+-----------+----------------|
| NMFO (swap)        | swap          | 1*        |                |
|--------------------+---------------+-----------+----------------|
*/

#include <vector>
#include <cstdio>
#include <thread>
#include <atomic>
#include <mutex>
#include <cassert>
#include <iostream>
#include <unistd.h>

#include "../common/barrier.h"
#include "sequential_tests.h"
#include "concurrent_tests.h"

using std::cout;
using std::endl;

/// configured via command line args: run sequential tests?
bool run_sequential = true;

/// configured via command line args: run concurrent tests?
bool run_concurrent = false;

/// configured via command line args: number of threads
int  num_threads = 4;

/// the barrier to use when we are in concurrent mode
barrier* global_barrier;

/// the mutex to use when we are in concurrent mode with tm turned off
std::mutex global_mutex;

/// a helper to print our list when visually checking for correctness
void check(std::string s, std::vector<int>* my_vector)
{
    std::cout << s << std::endl << " List: ";
    for (auto i : *my_vector)
        std::cout << i << ", ";
    std::cout << std::endl;
}

/// Report on how to use the command line to configure this program
void usage()
{
    cout << "Command-Line Options:" << endl
         << "  -s       : run the sequential tests" << endl
         << "  -c       : run the concurrent tests" << endl
         << "  -n <int> : specify the number of threads" << endl
         << "  -h       : display this message" << endl << endl;
    exit(0);
}

/// Parse command line arguments using getopt()
void parseargs(int argc, char** argv)
{
    // parse the command-line options
    int opt;
    while ((opt = getopt(argc, argv, "scn:h")) != -1) {
        switch (opt) {
          case 's': run_sequential = true; run_concurrent = false; break;
          case 'c': run_concurrent = true; run_sequential = false; break;
          case 'n': num_threads = atoi(optarg);                    break;
          case 'h': usage();                                       break;
        }
    }
}

/// A sequential test for exercising every method of std::list
void sequential_test()
{
    // test constructors and destructor
    ctor_test_seq();
    // test assignment operators
    assign_test_seq();
#if 0
    // test iterators
    iterator_test_seq();
    reverse_iterator_test_seq();
    legacy_const_iterator_test_seq();
    legacy_const_reverse_iterator_test_seq();
    const_iterator_test_seq();
    const_reverse_iterator_test_seq();
    // test capacity
    cap_test_seq();
    // test element access
    element_test_seq();
    // test modifiers
    modifier_test_seq();
    // test operations
    operations_test_seq();
    // test observers
    observers_test_seq();
    // test relational operators
    relational_test_seq();
    // test swap
    swap_test_seq();
#endif
}

/// A concurrent test for exercising every method of std::list.  This is
/// called by every thread
void thread_concurrent_test(int id)
{
    // wait for all threads to be ready
    global_barrier->arrive(id);
    // test constructors and destructors
    ctor_test_concurrent(id);
    // test assignment operators
    assign_test_concurrent(id);
#if 0
    // test iterators
    iterator_test_concurrent(id);
    reverse_iterator_test_concurrent(id);
    legacy_const_iterator_test_concurrent(id);
    legacy_const_reverse_iterator_test_concurrent(id);
    const_iterator_test_concurrent(id);
    const_reverse_iterator_test_concurrent(id);
    // test capacity
    cap_test_concurrent(id);
    // test element access
    element_test_concurrent(id);
    // test modifiers
    modifier_test_concurrent(id);
    // test operations
    operations_test_concurrent(id);
    // test observers
    observers_test_concurrent(id);
    // test relational operators
    relational_test_concurrent(id);
    // test swap
    swap_test_concurrent(id);
#endif
}

/// Kick off a test with many threads simultaneously invoking every possible
/// method of std::list.
///
/// This function just creates the threads and waits for them to finish
void concurrent_test()
{
    // set up a barrier and construct/start the threads
    std::thread* threads = new std::thread[num_threads];
    global_barrier = new barrier(num_threads);
    for (int i = 0; i < num_threads; ++i)
        threads[i] = std::thread(thread_concurrent_test, i);
    // wait for the threads to finish
    for (int i = 0; i < num_threads; ++i)
        threads[i].join();
}

/// main() just parses arguments and picks the test to run...
int main(int argc, char** argv)
{
    parseargs(argc, argv);

    if (run_sequential)
        sequential_test();
    if (run_concurrent)
        concurrent_test();
}
