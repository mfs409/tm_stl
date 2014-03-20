/*
  Driver to test the transactional version of the std::deque interface

  According to http://www.cplusplus.com/reference/deque/deque/, the
  std::deque interface consists of the following:

  We use this list for a few purposes.

  ** Status 1: Make sure that every method of std::deque is listed in the
     Functions and Count columns.  This part is supposed to correlate to what
     is listed on cplusplus.com.  Be sure to annotate those places where
     things don't make sense, and be sure to capture everything public in the
     header.  In particular, pay close attention to iterators and overloads
     of std:: functions

  ** Status 2: Make sure that every public function in the GCC STL source
     code is listed in Trace Location, with the location of the line of code
     where the TRACE macro was placed.  The TRACE call should indicate which
     version of a method is being called, so that the output will allow us to
     conclude that we have complete coverage.

  ** Status 3: Write a test program that causes every TRACE macro for the
     category to lead to the appropriate printf being called, when the
     '_notm' version of the code was run.  At that point, we know that every
     method within the category is being called.

  ** Status 4: Get the '_tm' version of the code to compile and run.  This is
     likely where difficulties will arise, with unexpectedly
     not-transaction-safe code cropping up.  When we reach this, we've
     discovered every obstacle to transaction-safety.

|------------------------+-----------------+-------+-----------------------------+--------|
| Category               | Functions       | Count | Trace Location              | Status |
|                        |                 |       | (default: bits/stl_deque.h  |        |
|------------------------+-----------------+-------+-----------------------------+--------|
| Member Functions       | (constructor)   |     9 | 790, 803, 816, 842, 855,    |      4 |
|                        |                 |       | 871, 898 (no 4b, 5b in GCC) |        |
|                        | (destructor)    |     1 | 917                         |      4 |
|                        | operator=       |     3 | 927->deque.gcc:94, 939, 960 |      4 |
|------------------------+-----------------+-------+-----------------------------+--------|
| Iterators              | begin           |     2 | 1037, 1045                  |      4 |
|                        | end             |     2 | 1054, 1063                  |      4 |
|                        | rbegin          |     2 | 1072, 1081                  |      4 |
|                        | rend            |     2 | 1090, 1099                  |      4 |
|                        | cbegin          |     1 | 1108                        |      4 |
|                        | cend            |     1 | 1117                        |      4 |
|                        | crbegin         |     1 | 1126                        |      4 |
|                        | crend           |     1 | 1135                        |      4 |
|------------------------+-----------------+-------+-----------------------------+--------|
| Iterator               | ctor            |     3 | 130, 133, 137               |      2 |
| Methods                | operator*       |     1 | 145                         |      2 |
|                        | operator->      |     1 | 149                         |      2 |
|                        | operator++      |     2 | 153, 165                    |      2 |
|                        | operator--      |     2 | 173, 185                    |      2 |
|                        | operator+=      |     1 | 193                         |      2 |
|                        | operator+       |     1 | 212                         |      2 |
|                        | operator-=      |     1 | 219                         |      2 |
|                        | operator-       |     1 | 223                         |      2 |
|                        | operator[]      |     1 | 230                         |      2 |
|------------------------+-----------------+-------+-----------------------------+--------|
| Iterator               | operator==      |     2 | 253, 260                    |      2 |
| Overloads              | operator!=      |     2 | 266, 273                    |      2 |
|                        | operator<       |     2 | 279, 287                    |      2 |
|                        | operator>       |     2 | 294, 301                    |      2 |
|                        | operator<=      |     2 | 307, 314                    |      2 |
|                        | operator>=      |     2 | 320, 327                    |      2 |
|                        | operator-       |     2 | 327, 349                    |      2 |
|                        | operator+       |     1 | 360                         |      2 |
|------------------------+-----------------+-------+-----------------------------+--------|
| Iterator               | copy-assignable |       |                             |      2 |
| Other                  | destructible    |       |                             |      4 |
|                        | swappable       |       |                             |      0 |
|------------------------+-----------------+-------+-----------------------------+--------|
| Const Iterator         |                 |       |                             |      0 |
| Methods                |                 |       |                             |        |
| (subsumed by above)    |                 |       |                             |        |
|------------------------+-----------------+-------+-----------------------------+--------|
| reverse_iterator*      |                 |       |                             |      0 |
| methods                |                 |       |                             |        |
|------------------------+-----------------+-------+-----------------------------+--------|
| Const Reverse Iterator |                 |       |                             |      0 |
| Methods                |                 |       |                             |        |
| (subsumed by above)    |                 |       |                             |        |
|------------------------+-----------------+-------+-----------------------------+--------|
| Capacity               | size            |     1 | 1142                        |      4 |
|                        | max_size        |     1 | 1147                        |      4 |
|                        | resize          |     2 | 1161, 1183                  |      4 |
|                        | empty           |     1 | 1228                        |      2 |
|                        | shrink_to_fit   |     1 | 1219                        |      2 |
|------------------------+-----------------+-------+-----------------------------+--------|
| Element access         | operator[]      |     2 | 1244, 1259                  |      2 |
|                        | at              |     2 | 1287, 1305                  |      2 |
|                        | front           |     2 | 1316, 1324                  |      2 |
|                        | back            |     2 | 1332, 1344                  |      2 |
|------------------------+-----------------+-------+-----------------------------+--------|
| Modifiers              | assign          |     3 | 978, 997, 1022              |      2 |
|                        | push_back       |     2 | 1393, 1407                  |      2 |
|                        | push_front      |     2 | 1362, 1375                  |      2 |
|                        | pop_back        |     1 |                             |      2 |
|                        | pop_front       |     1 | 1424, 1445                  |      2 |
|                        | insert          |     5 | 1480->deque.tcc:179, 1507,  |      2 |
|                        |                 |       | 1520, 1536, 1573            |        |
|                        | erase           |     2 | 1620, 1644                  |      2 |
|                        | swap            |     1 | 1657                        |      2 |
|                        | clear           |     1 | 1677                        |      2 |
|                        | emplace         |     1 | 1468->deque.tcc:152         |      2 |
|                        | emplace_front   |     1 | 1379->deque.tcc:117         |      2 |
|                        | emplace_back    |     1 | 1411->deque.tcc:133         |      2 |
|------------------------+-----------------+-------+-----------------------------+--------|
| Allocators             | get_allocator   |     1 | 1028                        |      2 |
|------------------------+-----------------+-------+-----------------------------+--------|
| Non-member             | '=='            |     1 | 2025                        |      2 |
| function               | '!='            |     1 | 2051                        |        |
| overloads (NMFOs)      | '<'             |     1 | 2058                        |        |
|                        | '<='            |     1 | 2065                        |        |
|                        | '>'             |     1 | 2043                        |        |
|                        | '>='            |     1 | 2072                        |        |
|                        | swap            |     1 | 2078                        |        |
|------------------------+-----------------+-------+-----------------------------+--------|
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
         << "               4 basic iterator use" << endl
         << "               5 const iterator use" << endl
         << "               6 reverse iterator use" << endl
         << "               7 const reverse iterator use" << endl
         << "               8 capacity methods" << endl
         << "               9 element access methods" << endl
         << "              10 modifier methods" << endl
         << "              11 observer methods" << endl
         << "              12 relational operator use" << endl
         << "              13 swap use" << endl
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
    basic_iter_tests,                                   // iter.cc
    const_iter_tests,                                   // iter.cc
    rev_iter_tests,                                     // iter.cc
    const_rev_iter_tests,                               // iter.cc
    cap_tests,                                          // cap.cc
    element_tests,                                      // element.cc
    modifier_tests,                                     // modifier.cc
    observers_test,                                     // observer.cc
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
