/*
  Driver to test the transactional version of the std::queue interface

  According to http://www.cplusplus.com/reference/queue/queue/, the std::queue
  interface consists of the following:

|-------------------------+----------------+-----------+----------------|
| Category                | Method         | # Methods | Trace Location |
| (* if not traced )      |                |           | (default: TBD) |
|-------------------------+----------------+-----------+----------------|
| Member Functions        | (constructor)  |           |                |
| (11 total... OK)        |                |           |                |
|                         | (destructor)   |           |                |
|                         | operator=      |           |                |
|                         |                |           |                |
|-------------------------+----------------+-----------+----------------|
| Iterators               | begin          |           |                |
| (ctors only)            | end            |           |                |
| (12 total... OK)        | rbegin         |           |                |
|                         | rend           |           |                |
|                         | cbegin         |           |                |
|                         | cend           |           |                |
|                         | crbegin        |           |                |
|                         | crend          |           |                |
|-------------------------+----------------+-----------+----------------|
| iterator                | ctor           |           |                |
| (methods)               | dtor           |           |                |
| (10 total... OK)        | "== !="        |           |                |
|                         | lval * and ->  |           |                |
|                         | *              |           |                |
|                         | ->             |           |                |
|                         | ++             |           |                |
|                         | --             |           |                |
|-------------------------+----------------+-----------+----------------|
| const_iterator          | ctor           |           |                |
| (methods)               | dtor           |           |                |
| (11 total... OK)        | "== !="        |           |                |
|                         | *              |           |                |
|                         | ->             |           |                |
|                         | ++             |           |                |
|                         | --             |           |                |
|-------------------------+----------------+-----------+----------------|
| reverse_iterator*       | see bits/      |           |                |
| (methods)               | stl_iterator.h |           |                |
|-------------------------+----------------+-----------+----------------|
| const_reverse_iterator* |                |           |                |
| (methods)               |                |           |                |
|-------------------------+----------------+-----------+----------------|
| Capacity                | empty          |           |                |
| (3 total... OK)         | size           |           |                |
|                         | max_size       |           |                |
|-------------------------+----------------+-----------+----------------|
| Element access          | front          |           |                |
| (4 total... OK)         | back           |           |                |
|-------------------------+----------------+-----------+----------------|
| Modifiers               | assign         |           |                |
| (23 total... OK)        | emplace        |           |                |
|                         | emplace_front  |           |                |
|                         | emplace_back   |           |                |
|                         | push_front     |           |                |
|                         | pop_front      |           |                |
|                         | push_back      |           |                |
|                         | pop_back       |           |                |
|                         | insert         |           |                |
|                         |                |           |                |
|                         |                |           |                |
|                         | erase          |           |                |
|                         |                |           |                |
|                         | swap           |           |                |
|                         | resize         |           |                |
|                         |                |           |                |
|                         | clear          |           |                |
|-------------------------+----------------+-----------+----------------|
| Operations              | splice         |           |                |
| (17 total... OK)        |                |           |                |
|                         | remove         |           |                |
|                         | remove_if      |           |                |
|                         | unique         |           |                |
|                         |                |           |                |
|                         | merge          |           |                |
|                         |                |           |                |
|                         |                |           |                |
|                         |                |           |                |
|                         | sort           |           |                |
|                         |                |           |                |
|                         | reverse        |           |                |
|-------------------------+----------------+-----------+----------------|
| Observers               | get_allocator  |           |                |
| (1 total... OK)         |                |           |                |
|-------------------------+----------------+-----------+----------------|
| Non-member              | '=='           |           |                |
| function                | '!='           |           |                |
| overloads (NMFOs)       | '<'            |           |                |
| (7 total... OK)         | '<='           |           |                |
|                         | '>'            |           |                |
|                         | '>='           |           |                |
|                         | swap           |           |                |
|-------------------------+----------------+-----------+----------------|
| Other                   | ==, != for     |           |                |
| (2 total... OK)         | const/normal   |           |                |
|                         | iterators      |           |                |
|-------------------------+----------------+-----------+----------------|
*/

#include <queue>
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
         << "  -h       : display this message" << endl << endl;
    exit(0);
}

/// Parse command line arguments using getopt()
void parseargs(int argc, char** argv)
{
    // parse the command-line options
    int opt;
    while ((opt = getopt(argc, argv, "n:h")) != -1) {
        switch (opt) {
          case 'n': num_threads = atoi(optarg); break;
          case 'h': usage();                    break;
        }
    }
}

/// A concurrent test for exercising every method of std::list.  This is
/// called by every thread
void thread_concurrent_test(int id)
{
    // wait for all threads to be ready
    global_barrier->arrive(id);

    // test constructors and destructors
    ctor_test(id);
#if 0
    // test assignment operators
    assign_test_concurrent(id);

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

/// main() just parses arguments, makes a barrier, and starts threads
int main(int argc, char** argv)
{
    // figure out what we're doing
    parseargs(argc, argv);

    // set up the barrier
    global_barrier = new barrier(1);

    // make threads
    std::thread* threads = new std::thread[num_threads];
    for (int i = 0; i < num_threads; ++i)
        threads[i] = std::thread(thread_concurrent_test, i);

    // wait for the threads to finish
    for (int i = 0; i < num_threads; ++i)
        threads[i].join();
}
