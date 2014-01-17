/*
  Driver to test the transactional version of the std::list interface

  According to http://www.cplusplus.com/reference/list/list/, the std::list
  interface consists of the following:

|-------------------------+---------------+-----------+------------------------------|
| Category                | Method        | # Methods | Trace Location               |
| (* if not traced )      |               |           | (default: stl_list.h)        |
|-------------------------+---------------+-----------+------------------------------|
| Member Functions        | (constructor) |         6 | 542, 555, 568, 594,          |
|                         |               |           | 605, 618, 637                |
|                         | (destructor)  |         1 | 394                          |
|                         | operator=     |         3 | list.tcc:228,                |
|                         |               |           | 678, 695                     |
|-------------------------+---------------+-----------+------------------------------|
| Iterators               | begin         |         2 | 769, 778                     |
| (ctors only)            | end           |         2 | 787, 796                     |
|                         | rbegin        |         2 | 805, 814                     |
|                         | rend          |         2 | 823, 832                     |
|                         | cbegin        |         1 | 842                          |
|                         | cend          |         1 | 851                          |
|                         | crbegin       |         1 | 860                          |
|                         | crend         |         1 | 869                          |
|-------------------------+---------------+-----------+------------------------------|
| iterator                | ctor          |         3 | 144, 148                     |
| (methods)               | dtor          |         1 | N/A                          |
|                         | "== !="       |         2 | 195, 199                     |
|                         | lval * and -> |       N/A | Is there test code yet?      |
|                         | *             |         1 | 157                          |
|                         | ->            |         1 | 161                          |
|                         | ++            |         3 | 165, 172                     |
|                         | --            |         3 | 180, 187                     |
|-------------------------+---------------+-----------+------------------------------|
| const_iterator          | ctor          |         3 | 224, 229, 232                |
| (methods)               | dtor          |         1 | N/A                          |
|                         | "== !="       |         2 | 280, 284                     |
|                         | *             |         1 | 242                          |
|                         | ->            |         1 | 246                          |
|                         | ++            |         3 | 250, 257                     |
|                         | --            |         3 | 265, 272                     |
|-------------------------+---------------+-----------+------------------------------|
| reverse_iterator*       |               |           | NOT VERIFIED (code looks OK) |
| (methods)               |               |           |                              |
|-------------------------+---------------+-----------+------------------------------|
| const_reverse_iterator* |               |         2 | NOT VERIFIED (code looks OK) |
| (methods)               |               |         1 |                              |
|-------------------------+---------------+-----------+------------------------------|
| Capacity                | empty         |         1 | 879                          |
|                         | size          |         1 | 884                          |
|                         | max_size      |         1 | 889                          |
|-------------------------+---------------+-----------+------------------------------|
| Element access          | front         |         2 | 938, 946                     |
|                         | back          |         2 | 954, 958                     |
|-------------------------+---------------+-----------+------------------------------|
| Modifiers*              | assign        |         3 | 713, 732, 754                |
|                         | emplace       |         1 | *1062->list.tcc:88           |
|                         | emplace_front |         1 | 995                          |
|                         | emplace_back  |         1 | *1028                        |
|                         | push_front    |         2 | 985, 990                     |
|                         | pop_front     |         1 | 1012                         |
|                         | push_back     |         2 | *1018, 1023                  |
|                         | pop_back      |         1 | *1044                        |
|                         | insert        |         5 | *1076/1090->list.tcc:98      |
|                         |               |           | *1106, 1145->list.tcc/1160:  |
|                         |               |           | *1186->list.tcc:130/1204     |
|                         | erase         |         2 | *1229/1231:list.tcc:148      |
|                         |               |           | *1254/1256                   |
|                         | swap          |         1 | *1274                        |
|                         | resize        |         2 | list.tcc:182                 |
|                         |               |           | list.tcc:197                 |
|                         | clear         |         1 | *1292                        |
|-------------------------+---------------+-----------+------------------------------|
| Operations              | splice        |         6 | *1312, 1329, 1345, 1384      |
|                         |               |           | *1403, 1449                  |
|                         | remove        |         1 | list.tcc:282                 |
|                         | remove_if     |         2 | *1481->list.tcc:434          |
|                         | unique        |         3 | list.tcc:310                 |
|                         |               |           | *1510->list.tcc:452          |
|                         | merge         |         4 | *1523->list.tcc:330          |
|                         |               |           | *1526                        |
|                         |               |           | *1549->list.tcc:364          |
|                         |               |           | *1553                        |
|                         | sort          |         2 | *1577->list.tcc:396          |
|                         |               |           | *1587->list.tcc:473          |
|                         | reverse       |         1 | *1567                        |
|-------------------------+---------------+-----------+------------------------------|
| Observers               | get_allocator |         1 | 752                          |
|-------------------------+---------------+-----------+------------------------------|
| Non-member*             | '=='          |         1 | 1720                         |
| function                | '!='          |         1 | 1757                         |
| overloads (NMFOs)       | '<'           |         1 | 1749                         |
| (Relational             | '<='          |         1 | 1768                         |
| Operators)              | '>'           |         1 | 1762                         |
|                         | '>='          |         1 | 1774                         |
|-------------------------+---------------+-----------+------------------------------|
| NMFO (swap)*            | swap          |         1 | 1780                         |
|-------------------------+---------------+-----------+------------------------------|
| Other*                  | ==, != for    |         2 | 294, 300                     |
|                         | const/normal  |           | note: no test code yet!      |
|                         | iterators     |           |                              |
|-------------------------+---------------+-----------+------------------------------|
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
#include "concurrent_tests.h"

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
    ctor_test_concurrent(id);

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
