/*
  Driver to test the transactional version of the std::list interface

  According to http://www.cplusplus.com/reference/list/list/, the std::list
  interface consists of the following:

|--------------------+---------------+-----------+-------------------------+----------|
| Category           | Method        | # Methods | Location                | # Tested |
| (* if not started) |               |           | (default: stl_list.h)   | with TM  |
|--------------------+---------------+-----------+-------------------------+----------|
| Member Functions   | (constructor) |         6 | 534, 546, 558/572, 587, |          |
|                    |               |           | 597, 608, 627/633       |          |
|                    | (destructor)  |         1 | 387                     |          |
|                    | operator=     |         3 | 658->list.tcc:225,      |          |
|                    |               |           | 669, 686               |          |
|--------------------+---------------+-----------+-------------------------+----------|
| Iterators          | begin         |         2 |                         |          |
|                    | end           |         2 |                         |          |
| (note: must be     | rbegin        |         2 |                         |          |
| sure to handle     | rend          |         2 |                         |          |
| all 16 methods     | cbegin        |         1 |                         |          |
| that can be        | cend          |         1 |                         |          |
| called on          | crbegin       |         1 |                         |          |
| an iterator!)      | crend         |         1 |                         |          |
|--------------------+---------------+-----------+-------------------------+----------|
| Capacity           | empty         |         1 |                         |          |
|                    | size          |         1 |                         |          |
|                    | max_size      |         1 |                         |          |
|--------------------+---------------+-----------+-------------------------+----------|
| Element access     | front         |         2 |                         |          |
|                    | back          |         2 |                         |          |
|--------------------+---------------+-----------+-------------------------+----------|
| Modifiers          | assign        |         3 |                         |          |
|                    | emplace       |         1 |                         |          |
|                    | emplace_front |         1 |                         |          |
|                    | emplace_back  |         1 |                         |          |
|                    | push_front    |         2 |                         |          |
|                    | pop_front     |         1 |                         |          |
|                    | push_back     |         2 |                         |          |
|                    | pop_back      |         1 |                         |          |
|                    | insert        |         5 |                         |          |
|                    | erase         |         2 |                         |          |
|                    | swap          |         1 |                         |          |
|                    | resize        |         2 |                         |          |
|                    | clear         |         1 |                         |          |
|--------------------+---------------+-----------+-------------------------+----------|
| Operations         | splice        |         6 |                         |          |
|                    | remove        |         1 |                         |          |
|                    | remove_if     |         2 |                         |          |
|                    | unique        |         3 |                         |          |
|                    | merge         |         4 |                         |          |
|                    | sort          |         2 |                         |          |
|                    | reverse       |         1 |                         |          |
|--------------------+---------------+-----------+-------------------------+----------|
| Observers          | get_allocator |         1 |                         |          |
|--------------------+---------------+-----------+-------------------------+----------|
| Non-member         | '=='          |         1 |                         |          |
| function           | '!='          |         1 |                         |          |
| overloads (NMFOs)  | '<'           |         1 |                         |          |
| (Relational        | '<='          |         1 |                         |          |
| Operators)         | '>'           |         1 |                         |          |
|                    | '>='          |         1 |                         |          |
|--------------------+---------------+-----------+-------------------------+----------|
| NMFO (swap)        | swap          |         1 |                         |          |
|--------------------+---------------+-----------+-------------------------+----------|
*/

#include <list>
#include <cstdio>
#include <thread>
#include <atomic>
#include <mutex>
#include <cassert>
#include <iostream>
#include <unistd.h>

#include "barrier.h"
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

/// the list that all our sequential tests use
std::list<int>* my_list;

/// a helper to print our list when visually checking for correctness
void check(std::string s)
{
    std::cout << s << std::endl << " List: ";
    for (auto i : *my_list)
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
}

/// A concurrent test for exercising every method of std::list.  This is
/// called by every thread
void thread_concurrent_test(int id)
{
    // wait for all threads to be ready
    global_barrier->arrive(id);

    // test constructors and destructors
    ctor_test_concurrent(id);
#if 0
    // @step: 1
    //
    // Test construction, destruction, and operator=; be sure to test
    // ctor/dtor for stack and heap allocations.  Note that stl_list.h has
    // three operator=() methods, but we're only testing one of them
    bool success = false;
    BEGIN_TX;
    if (global_list_ptr == NULL) {
        // a locally constructed list... it should be constructed and
        // destructed in this scope
        std::list<int> my_other_list();
        // operator= on the global list
        global_list = std::list<int>();
        // construct and destruct the list on the heap
        global_list_ptr = new std::list<int>();
        delete(global_list_ptr);
        // make another one, and this time, keep it
        global_list_ptr = new std::list<int>();
        success = true;
    }
    END_TX;
    global_barrier->arrive(id);

    // @step: 2
    //
    // The thread who succeeded in the insertion will push an entry, so that
    // we know who constructed.  Also push onto the heap list.  Then everyone
    // pushes, pops, and pushes again
    BEGIN_TX;
    if (success) {
        global_list.push_back(id);
        global_list_ptr->push_back(id);
    }
    END_TX;
    global_barrier->arrive(id);
    BEGIN_TX;
    global_list.push_back(id);
    global_list_ptr->push_back(id);
    END_TX;
    global_barrier->arrive(id);
    int i0, i1, i2;
    BEGIN_TX;
    i0 = global_list.front();
    i1 = global_list.back(); global_list.pop_back();
    i2 = global_list_ptr->back(); global_list_ptr->pop_back();
    END_TX;
    global_barrier->arrive(id);
    printf("thread %d popped %d/%d, head is %d\n", id, i1, i2, i0);
    BEGIN_TX;
    global_list.push_back(id);
    global_list_ptr->push_back(id);
    END_TX;
    global_barrier->arrive(id);

    // @step: 3
    //
    // Now let's clear the list and start trying to do more fun stuff with it
    if (id == 0) {
        BEGIN_TX;
        global_list.clear();
        global_list_ptr->clear();
        END_TX;
        // initialize the list with all ints between 0 and 50, except for
        // those divisible by 5
        BEGIN_TX;
        for (int i = 0; i < 50; ++i) {
            if (i%5 != 0) {
                global_list.push_back(i);
                global_list_ptr->push_back(i);
            }
        }
        END_TX;
    }
    global_barrier->arrive(id);
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
