/*
  Driver to test the transactional version of the std::deque interface

  According to http://www.cplusplus.com/reference/deque/deque/, the
  std::deque interface consists of the following:

  We use this list for a few purposes.

  ** First, we make sure that every method of std::deque is listed in the
     Method and # Methods column.  This part is supposed to correlate to what
     is listed on cplusplus.com.

  ** Second, in Trace Location, we make sure that every instance in the GCC
     STL source code is listed in Trace Location, with the location of the
     line of code where the TRACE macro was placed.

  ** Third, in Category, we update VERIFICATION once we've had a successful
     run in which every TRACE macro for a given category led to the
     appropriate printf being called.  At that point, we know that every
     method within the category is now transaction_safe.

|------------------------+---------------+-----------+-----------------------------|
| Category               | Method        | # Methods | Trace Location              |
| (* if not traced )     |               |           | (default: bits/stl_deque.h  |
|------------------------+---------------+-----------+-----------------------------|
| Member Functions       | (constructor) |         9 | 790, 803, 816, 842, 855,    |
|                        |               |           | 871, 898 (no 4b, 5b in GCC) |
| (VERIFICATION: TODO)   | (destructor)  |         1 | 917                         |
|                        | operator=     |         3 | 927->deque.gcc:94, 939, 960 |
|------------------------+---------------+-----------+-----------------------------|
| Iterators              | begin         |         2 |                             |
| (ctors only)           | end           |         2 |                             |
| (VERIFICATION: TODO)   | rbegin        |         2 |                             |
|                        | rend          |         2 |                             |
|                        | cbegin        |         1 |                             |
|                        | cend          |         1 |                             |
|                        | crbegin       |         1 |                             |
|                        | crend         |         1 |                             |
|------------------------+---------------+-----------+-----------------------------|
| iterator               |               |           |                             |
| methods: TODO          |               |           |                             |
|------------------------+---------------+-----------+-----------------------------|
| const_iterator         |               |           |                             |
| methods: TODO          |               |           |                             |
|------------------------+---------------+-----------+-----------------------------|
| reverse_iterator*      |               |           |                             |
| methods: TODO          |               |           |                             |
|------------------------+---------------+-----------+-----------------------------|
| const_reverse_iterator |               |           |                             |
| methods: TODO          |               |           |                             |
|------------------------+---------------+-----------+-----------------------------|
| Capacity               | size          |         1 |                             |
| (VERIFICATION: TODO)   | max_size      |         1 |                             |
|                        | resize        |         2 |                             |
|                        | empty         |         1 |                             |
|                        | shrink_to_fit |         1 |                             |
|------------------------+---------------+-----------+-----------------------------|
| Element access         | operator[]    |         2 |                             |
| (VERIFICATION: TODO)   | at            |         2 |                             |
|                        | front         |         2 |                             |
|                        | back          |         2 |                             |
|------------------------+---------------+-----------+-----------------------------|
| Modifiers              | assign        |         3 |                             |
| (VERIFICATION: TODO)   | push_back     |         2 |                             |
|                        | push_front    |         2 |                             |
|                        | pop_back      |         1 |                             |
|                        | pop_front     |         1 |                             |
|                        | insert        |         5 |                             |
|                        | erase         |         2 |                             |
|                        | swap          |         1 |                             |
|                        | clear         |         1 |                             |
|                        | emplace       |         1 |                             |
|                        | emplace_front |         1 |                             |
|                        | emplace_back  |         1 |                             |
|------------------------+---------------+-----------+-----------------------------|
| Allocators             | get_allocator |         1 |                             |
|------------------------+---------------+-----------+-----------------------------|
| Non-member             | '=='          |         1 |                             |
| function               | '!='          |         1 |                             |
| overloads (NMFOs)      | '<'           |         1 |                             |
| (7 total... OK)        | '<='          |         1 |                             |
|                        | '>'           |         1 |                             |
|                        | '>='          |         1 |                             |
|                        | swap          |         1 |                             |
|------------------------+---------------+-----------+-----------------------------|
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
void per_thread_test(int id)
{
    // wait for all threads to be ready
    global_barrier->arrive(id);

    // test member functions (ctor, dtor, operator=)
    member_test(id);

#if 0
    // test assignment operators
    assign_test(id);

    // test iterators
    iterator_test(id);
    reverse_iterator_test(id);
    legacy_const_iterator_test(id);
    legacy_const_reverse_iterator_test(id);
    const_iterator_test(id);
    const_reverse_iterator_test(id);

    // test capacity
    cap_test(id);

    // test element access
    element_test(id);

    // test modifiers
    modifier_test(id);

    // test operations
    operations_test(id);

    // test observers
    observers_test(id);

    // test relational operators
    relational_test(id);

    // test swap
    swap_test(id);
#endif
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
