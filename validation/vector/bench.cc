/*
  Driver to test the transactional version of the std::vector interface

  According to http://www.cplusplus.com/reference/vector/vector/, the
  std::vector interface consists of the following:

  Steps:
    1 - Put TBD traces into all of GCC's vector functions
    2 - For one category at a time, replace TBDs and update columns 3 and 4
        of table below
    3 - Write test code for ensuring that every traced function is called,
        and then write DONE in the category

|-------------------+-----------------+-------------------+----------------|
| Category          | Functions       | C++14             | GCC            |
|                   |                 | Expected          | Actual         |
|-------------------+-----------------+-------------------+----------------|
| Member            | (constructor)   | 1a, 1b, 2a, 2b, 3 | 1a, 1b, 2a, 2b, 3 |
| Functions         |                 | 4a, 4b, 5a, 5b, 6 | 4a, 4b, 5a, 5b, 6 |
|                   | (destructor)    | 1                 | 1              |
|                   | operator=       | 1, 2, 3           | 2, 3           |
|-------------------+-----------------+-------------------+----------------|
| Iterators         | begin           | 1a, 1b            | 1a, 1b         |
|                   | end             | 1a, 1b            | 1a, 1b         |
|                   | rbegin          | 1a, 1b            | 1a, 1b         |
|                   | rend            | 1a, 1b            | 1a, 1b         |
|                   | cbegin          | 1                 | 1              |
|                   | cend            | 1                 | 1              |
|                   | crbegin         | 1                 | 1              |
|                   | crend           | 1                 | 1              |
|-------------------+-----------------+-------------------+----------------|
| Iterator          | ctor            |                   |                |
| Methods           | dtor            |                   |                |
|                   | operator*       |                   |                |
|                   | operator->      |                   |                |
|                   | operator++      |                   |                |
|                   | operator--      |                   |                |
|                   | operator+=      |                   |                |
|                   | operator+       |                   |                |
|                   | operator-=      |                   |                |
|                   | operator-       |                   |                |
|                   | operator[]      |                   |                |
|-------------------+-----------------+-------------------+----------------|
| Iterator          | operator==      |                   |                |
| Operators         | operator!=      |                   |                |
|                   | operator<       |                   |                |
|                   | operator>       |                   |                |
|                   | operator<=      |                   |                |
|                   | operator>=      |                   |                |
|                   | operator-       |                   |                |
|                   | operator+       |                   |                |
|-------------------+-----------------+-------------------+----------------|
| Iterator          | copy-assignable |                   |                |
| Overloads         | destructible    |                   |                |
|                   | swappable       |                   |                |
|-------------------+-----------------+-------------------+----------------|
| Iterator          | fill            |                   |                |
| Functions         | copy            |                   |                |
|                   | copy_backward   |                   |                |
|                   | move            |                   |                |
|                   | move_backward   |                   |                |
|-------------------+-----------------+-------------------+----------------|
| Const             |                 |                   |                |
| Iterator          |                 |                   |                |
| Methods           |                 |                   |                |
|-------------------+-----------------+-------------------+----------------|
| Reverse           |                 |                   |                |
| Iterator          |                 |                   |                |
| Methods           |                 |                   |                |
|-------------------+-----------------+-------------------+----------------|
| Const Reverse     |                 |                   |                |
| Iterator          |                 |                   |                |
| Methods           |                 |                   |                |
|-------------------+-----------------+-------------------+----------------|
| Capacity          | size            | 1                 | 1              |
|                   | max_size        | 1                 | 1              |
|                   | resize          | 1a, 1b            | 1a, 1b         |
|                   | capacity        | 1                 | 1              |
|                   | empty           | 1                 | 1              |
|                   | reserve         | 1                 |                |
|                   | shrink_to_fit   | 1                 | 1              |
|-------------------+-----------------+-------------------+----------------|
| Element access    | operator[]      | 1a, 1b            | 1a, 1b         |
|                   | at              | 1a, 1b            | 1a, 1b         |
|                   | front           | 1a, 1b            | 1a, 1b         |
|                   | back            | 1a, 1b            | 1a, 1b         |
|                   | data            | 1a, 1b            | 1a, 1b         |
|-------------------+-----------------+-------------------+----------------|
| Modifiers         | assign          | 1, 2, 3           | 1, 2, 3        |
|                   | push_back       | 1a, 1b            | 1a, 1b         |
|                   | pop_back        | 1                 | 1              |
|                   | insert          | 1, 2, 3, 4, 5     | 2, 3, 4, 5     |
|                   | erase           | 1a, 1b            | 1a, 1b         |
|                   | swap            | 1                 | 1              |
|                   | clear           | 1                 | 1              |
|                   | emplace         | 1                 |                |
|                   | emplace_back    | 1                 |                |
|-------------------+-----------------+-------------------+----------------|
| Allocators        | get_allocator   | 1                 |                |
|                   |                 |                   |                |
|-------------------+-----------------+-------------------+----------------|
| Non-member        | '=='            |  1                | 1              |
| function          | '!='            |  2                | 2              |
| overloads (NMFOs) | '<'             |  3                | 3              |
|                   | '<='            |  4                | 4              |
|                   | '>'             |  5a               | 5a             |
|                   | '>='            |  5b               | 5b             |
|                   | swap            |  1                | 1              |
|-------------------+-----------------+-------------------+----------------|
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
         << "              11 observer methods" << endl
         << "              12 relational operator use" << endl
         << "              13 swap use" << endl
         << endl
         << "  Note: const, reverse, and const reverse iterators not tested"
         << endl
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
    iter_method_tests,                                  // iter.cc
    iter_operator_tests,                                // iter.cc
    iter_overload_tests,                                // iter.cc
    iter_function_tests,                                // iter.cc
    cap_tests,                                          // cap.cc
    element_tests,                                      // element.cc
    modifier_tests,                                     // modifier.cc
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
