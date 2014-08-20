/*
  Driver to test the transactional version of the std::map interface

  According to http://www.cplusplus.com/reference/map/map/, the
  std::map interface consists of the following:

  Steps:
    1 - Put TBD traces into all of GCC's map functions
    2 - For one category at a time, replace TBDs and update columns 3 and 4
        of table below
    3 - Write test code for ensuring that every traced function is called,
        and then write DONE in the category

|-------------------+-----------------------+--------------------+------------------------|
| Category          | Functions             | C++14              | GCC                    |
|                   |                       | Expected           | Actual                 |
|-------------------+-----------------------+--------------------+------------------------|
| Member Functions  | (constructor)         | 1a, 1b, 1c, 2a, 2b | 1a, 1b, 1c, 2a, 2b, 2c |
| (DONE)            |                       | 3a, 3b, 4a, 4b     | 3a, 3b, 4a, 4b         |
|                   |                       | 5a, 5b             | 5a, 5b                 |
|                   | (destructor)          | 1                  | NONE                   |
|                   | operator=             | 1, 2, 3            | 1, 2, 3                |
|-------------------+-----------------------+--------------------+------------------------|
| Iterators         | begin                 | 1a, 1b             | 1a, 1b                 |
|                   | end                   | 1a, 1b             | 1a, 1b                 |
|                   | rbegin                | 1a, 1b             | 1a, 1b                 |
|                   | rend                  | 1a, 1b             | 1a, 1b                 |
|                   | cbegin                | 1                  | 1                      |
|                   | cend                  | 1                  | 1                      |
|                   | crbegin               | 1                  | 1                      |
|                   | crend                 | 1                  | 1                      |
|-------------------+-----------------------+--------------------+------------------------|
| Iterator          | default constructable |                    |                        |
| Methods           | copy constructable    |                    |                        |
|                   | copy assignable       |                    |                        |
|                   | destructible          |                    |                        |
|                   | swappable             |                    |                        |
|-------------------+-----------------------+--------------------+------------------------|
| Iterator          | operator*             |                    |                        |
| Operators         | operator->            |                    |                        |
|                   | operator++            |                    |                        |
|                   | operator--            |                    |                        |
|                   | operator+=            |                    |                        |
|                   | operator+             |                    |                        |
|                   | operator-=            |                    |                        |
|                   | operator-             |                    |                        |
|                   | operator[]            |                    |                        |
|-------------------+-----------------------+--------------------+------------------------|
| Iterator          | operator==            |                    |                        |
| Overloads         | operator!=            |                    |                        |
|                   | operator<             |                    |                        |
|                   | operator>             |                    |                        |
|                   | operator<=            |                    |                        |
|                   | operator>=            |                    |                        |
|                   | operator-             |                    |                        |
|                   | operator+             |                    |                        |
|-------------------+-----------------------+--------------------+------------------------|
| Const             |                       |                    |                        |
| Iterator          |                       |                    |                        |
| Methods           |                       |                    |                        |
|-------------------+-----------------------+--------------------+------------------------|
| Reverse           |                       |                    |                        |
| Iterator          |                       |                    |                        |
| Methods           |                       |                    |                        |
|-------------------+-----------------------+--------------------+------------------------|
| Const Reverse     |                       |                    |                        |
| Iterator          |                       |                    |                        |
| Methods           |                       |                    |                        |
|-------------------+-----------------------+--------------------+------------------------|
| Capacity          | empty                 | 1                  | 1                      |
|                   | size                  | 1                  | 1                      |
|                   | max_size              | 1                  | 1                      |
|-------------------+-----------------------+--------------------+------------------------|
| Element           | operator[]            | 1a, 1b             | 1a, 1b                 |
| Access            | at                    | 1a, 1b             | 1a, 1b                 |
|-------------------+-----------------------+--------------------+------------------------|
| Modifiers         | insert                | 1a, 1b, 2a, 2b     | 1a, 1b, 2a, 2b         |
|                   |                       | 3, 4               | 3, 4                   |
|                   | erase                 | 1, 2, 3            | 1a, 1b, 2, 3           |
|                   | swap                  | 1                  | 1                      |
|                   | clear                 | 1                  | 1                      |
|                   | emplace               | 1                  | 1                      |
|                   | emplace_hint          | 1                  | 1                      |
|-------------------+-----------------------+--------------------+------------------------|
| Observers         | get_allocator         | 1                  | 1                      |
|                   | key_comp              | 1                  | 1                      |
|                   | value_comp            | 1                  | 1                      |
|-------------------+-----------------------+--------------------+------------------------|
| Operations        | find                  | 1a, 1b             | 1a, 1b                 |
|                   | count                 | 1                  | 1                      |
|                   | lower_bound           | 1a, 1b             | 1a, 1b                 |
|                   | upper_bound           | 1a, 1b             | 1a, 1b                 |
|                   | equal_range           | 1a, 1b             | 1a, 1b                 |
|-------------------+-----------------------+--------------------+------------------------|
| Non-member        | '=='                  |                    | 1                      |
| function          | '!='                  |                    | 1                      |
| overloads (NMFOs) | '<'                   |                    | 1                      |
|                   | '<='                  |                    | 1                      |
|                   | '>'                   |                    | 1                      |
|                   | '>='                  |                    | 1                      |
|                   | swap                  |                    | 1                      |
|-------------------+-----------------------+--------------------+------------------------|
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
         << "               3 iterator creation" << endl
         << "               4 iterator methods" << endl
         << "               5 iterator operators" << endl
         << "               6 iterator overloads" << endl
         << "               7 iterator functions" << endl
         << "               8 capacity methods" << endl
         << "               9 element access methods" << endl
         << "              10 modifier methods" << endl
         << "              11 observer methods" << endl
         << "              12 operations methods" << endl
         << endl
         << "  Note: const, reverse, and const reverse iterators not tested"
         << endl
         << endl;
    exit(0);
}

#define NUM_TESTS 13
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
    operation_tests                                     // operations.cc
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
