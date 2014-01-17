/*
  Driver to test the transactional version of the std::list interface

  According to http://www.cplusplus.com/reference/list/list/, the std::list
  interface consists of the following:

|-------------------------+----------------+--------------+------------------------------|
| Category                | Method         |    # Methods | Trace Location               |
| (* if not traced )      |                |              | (default: stl_list.h)        |
|-------------------------+----------------+--------------+------------------------------|
| Member Functions        | (constructor)  |            7 | 542, 555, 568, 594,          |
| (11 total... OK)        |                |              | 605, 618, 637                |
|                         | (destructor)   |            1 | 394                          |
|                         | operator=      |            3 | list.tcc:228,                |
|                         |                |              | 678, 695                     |
|-------------------------+----------------+--------------+------------------------------|
| Iterators               | begin          |            2 | 769, 778                     |
| (ctors only)            | end            |            2 | 787, 796                     |
| (12 total... OK)        | rbegin         |            2 | 805, 814                     |
|                         | rend           |            2 | 823, 832                     |
|                         | cbegin         |            1 | 842                          |
|                         | cend           |            1 | 851                          |
|                         | crbegin        |            1 | 860                          |
|                         | crend          |            1 | 869                          |
|-------------------------+----------------+--------------+------------------------------|
| iterator                | ctor           |            2 | 144, 148                     |
| (methods)               | dtor           |     (1 test) | N/A                          |
| (10 total... OK)        | "== !="        |            2 | 195, 199                     |
|                         | lval * and ->  |     (1 test) | Is there test code yet?      |
|                         | *              |            1 | 157                          |
|                         | ->             |            1 | 161                          |
|                         | ++             |  2 (3 tests) | 165, 172                     |
|                         | --             |  2 (3 tests) | 180, 187                     |
|-------------------------+----------------+--------------+------------------------------|
| const_iterator          | ctor           |            3 | 224, 229, 232                |
| (methods)               | dtor           |     (1 test) | N/A                          |
| (11 total... OK)        | "== !="        |            2 | 280, 284                     |
|                         | *              |            1 | 242                          |
|                         | ->             |            1 | 246                          |
|                         | ++             |  2 (3 tests) | 250, 257                     |
|                         | --             |  2 (3 tests) | 265, 272                     |
|-------------------------+----------------+--------------+------------------------------|
| reverse_iterator*       | see bits/      |              | NOT VERIFIED (code looks OK) |
| (methods)               | stl_iterator.h |              |                              |
|-------------------------+----------------+--------------+------------------------------|
| const_reverse_iterator* |                |              | NOT VERIFIED (code looks OK) |
| (methods)               |                |              |                              |
|-------------------------+----------------+--------------+------------------------------|
| Capacity                | empty          |            1 | 879                          |
| (3 total... OK)         | size           |            1 | 884                          |
|                         | max_size       |            1 | 889                          |
|-------------------------+----------------+--------------+------------------------------|
| Element access          | front          |            2 | 938, 946                     |
| (4 total... OK)         | back           |            2 | 954, 958                     |
|-------------------------+----------------+--------------+------------------------------|
| Modifiers               | assign         |            3 | 713, 732, 754                |
| (23 total... OK)        | emplace        |            1 | list.tcc:89                  |
|                         | emplace_front  |            1 | 995                          |
|                         | emplace_back   |            1 | 1036                         |
|                         | push_front     |            2 | 985, 990                     |
|                         | pop_front      |            1 | 1012                         |
|                         | push_back      |            2 | 1026, 1031                   |
|                         | pop_back       |            1 | 1052                         |
|                         | insert         |            5 | list.tcc:104                 |
|                         |                |              | 1114, 1133, list.tcc:115     |
|                         |                |              | list.tcc:132                 |
|                         | erase          |            2 | list.tcc:152                 |
|                         |                |              | 1265                         |
|                         | swap           |            1 | 1282                         |
|                         | resize         |            2 | list.tcc:182                 |
|                         |                |              | list.tcc:197                 |
|                         | clear          |            1 | 1300                         |
|-------------------------+----------------+--------------+------------------------------|
| Operations              | splice         |            6 | 1323, 1336, 1365, 1391       |
| (17 total... OK)        |                |              | 1428, 1457                   |
|                         | remove         |            1 | list.tcc:282                 |
|                         | remove_if      | 1 (func/obj) | list.tcc:435                 |
|                         | unique         | 2 (func/obj) | list.tcc:310                 |
|                         |                |              | list.tcc:453                 |
|                         | merge          |            4 | list.tcc:334                 |
|                         |                |              | 1533                         |
|                         |                |              | list.tcc:368                 |
|                         |                |              | 1560                         |
|                         | sort           |            2 | list.tcc:397                 |
|                         |                |              | list.tcc:474                 |
|                         | reverse        |            1 | 1574                         |
|-------------------------+----------------+--------------+------------------------------|
| Observers               | get_allocator  |            1 | 752                          |
| (1 total... OK)         |                |              |                              |
|-------------------------+----------------+--------------+------------------------------|
| Non-member              | '=='           |            1 | 1727                         |
| function                | '!='           |            1 | 1763                         |
| overloads (NMFOs)       | '<'            |            1 | 1756                         |
| (7 total... OK)         | '<='           |            1 | 1775                         |
|                         | '>'            |            1 | 1769                         |
|                         | '>='           |            1 | 1781                         |
|                         | swap           |            1 | 1780                         |
|-------------------------+----------------+--------------+------------------------------|
| Other                   | ==, != for     |            2 | 294, 300                     |
| (2 total... OK)         | const/normal   |              | note: no test code yet!      |
|                         | iterators      |              |                              |
|-------------------------+----------------+--------------+------------------------------|
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
