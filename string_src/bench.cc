/*
  Driver to test the transactional version of the std::string interface

  According to http://www.cplusplus.com/reference/string, there are quite
  a few things that are declared in <string>.  We're only going to worry
  about std::string for now.
  Per http://www.cplusplus.com/reference/string/string/, we must address
  the following (noting that std::string is actually basic_string<char>):

|------------------------+-------------------+-----------+--------------------------------|
| Category               | Method            | # Methods | Location                       |
| (* if not started)     |                   |           | (default: bits/basic_string.h) |
|------------------------+-------------------+-----------+--------------------------------|
| Member Functions       | (constructor)     |         9 | 444, 461->???                  |
|                        | (destructor)      |         1 |                                |
|                        | operator=         |         5 |                                |
|------------------------+-------------------+-----------+--------------------------------|
| Iterators              | begin             |         2 |                                |
|                        | end               |         2 |                                |
|                        | rbegin            |         2 |                                |
|                        | rend              |         2 |                                |
|                        | cbegin            |         1 |                                |
|                        | cend              |         1 |                                |
|                        | crbegin           |         1 |                                |
|                        | crend             |         1 |                                |
|------------------------+-------------------+-----------+--------------------------------|
| iterator               | TBD               |           |                                |
| (methods)              |                   |           |                                |
|------------------------+-------------------+-----------+--------------------------------|
| const_iterator         | TBD               |           |                                |
| (methods)              |                   |           |                                |
|------------------------+-------------------+-----------+--------------------------------|
| reverse_iterator       | TBD               |           |                                |
| (methods)              |                   |           |                                |
|------------------------+-------------------+-----------+--------------------------------|
| const_reverse_iterator | TBD               |           |                                |
| (methods)              |                   |           |                                |
|------------------------+-------------------+-----------+--------------------------------|
| Capacity               | size              |         1 |                                |
|                        | length            |         1 |                                |
|                        | max_size          |         1 |                                |
|                        | resize            |         2 |                                |
|                        | capacity          |         1 |                                |
|                        | reserve           |         1 |                                |
|                        | clear             |         1 |                                |
|                        | empty             |         1 |                                |
|                        | shrink_to_fit     |         1 |                                |
|------------------------+-------------------+-----------+--------------------------------|
| Element access         | operator[]        |         2 |                                |
|                        | at                |         2 |                                |
|                        | back              |         2 |                                |
|                        | front             |         2 |                                |
|------------------------+-------------------+-----------+--------------------------------|
| Modifiers              | operator+=        |         4 |                                |
|                        | append            |         7 |                                |
|                        | push_back         |         1 |                                |
|                        | assign            |         8 |                                |
|                        | insert            |         9 |                                |
|                        | erase             |         3 |                                |
|                        | replace           |        11 |                                |
|                        | swap              |         1 |                                |
|                        | pop_back          |         1 |                                |
|------------------------+-------------------+-----------+--------------------------------|
| String Operations      | c_str             |         1 |                                |
|                        | data              |         1 |                                |
|                        | get_allocator     |         1 |                                |
|                        | copy              |         1 |                                |
|                        | find              |         4 |                                |
|                        | rfind             |         4 |                                |
|                        | find_first_of     |         4 |                                |
|                        | find_last_of      |         4 |                                |
|                        | find_first_not_of |         4 |                                |
|                        | find_last_not_of  |         4 |                                |
|                        | substr            |         1 |                                |
|                        | compare           |         6 |                                |
|------------------------+-------------------+-----------+--------------------------------|
| Non-member             | operator==        |         3 |                                |
| function               | operator!=        |         3 |                                |
| overloads (NMFOs)      | operator<         |         3 |                                |
| (Relational            | operator<=        |         3 |                                |
| Operators)             | operator>         |         3 |                                |
|                        | operator>=        |         3 |                                |
|------------------------+-------------------+-----------+--------------------------------|
| NMFO                   | operator+         |        12 |                                |
| (Other Operators)      | swap              |         1 |                                |
|                        | operator>>        |         1 |                                |
|                        | operator<<        |         1 |                                |
|                        | getline           |         4 |                                |
|------------------------+-------------------+-----------+--------------------------------|
*/

#include <string>
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

/// A concurrent test for exercising every method of std::string.  This is
/// called by every thread
void thread_concurrent_test(int id)
{
    // wait for all threads to be ready
    global_barrier->arrive(id);
    // test constructors and destructors
    ctor_test(id);
    /*
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
    */
}

/// main() just parses arguments, makes a barrier, and starts threads
int main(int argc, char** argv)
{
    // figure out what we're doing
    parseargs(argc, argv);

    // set up a barrier
    global_barrier = new barrier(num_threads);

    // make threads
    std::thread* threads = new std::thread[num_threads];
    for (int i = 0; i < num_threads; ++i)
        threads[i] = std::thread(thread_concurrent_test, i);

    // wait for the threads to finish
    for (int i = 0; i < num_threads; ++i)
        threads[i].join();
}
