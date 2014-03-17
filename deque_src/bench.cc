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
|                        | (destructor)    |     1 | 917                         |        |
|                        | operator=       |     3 | 927->deque.gcc:94, 939, 960 |        |
|------------------------+-----------------+-------+-----------------------------+--------|
| Iterators              | begin           |     2 | 1037, 1045                  |      3 |
|                        | end             |     2 | 1054, 1063                  |      3 |
|                        | rbegin          |     2 | 1072, 1081                  |      2 |
|                        | rend            |     2 | 1090, 1099                  |      2 |
|                        | cbegin          |     1 | 1108                        |      2 |
|                        | cend            |     1 | 1117                        |      2 |
|                        | crbegin         |     1 | 1126                        |      2 |
|                        | crend           |     1 | 1135                        |      2 |
|------------------------+-----------------+-------+-----------------------------+--------|
| Iterator               | ctor            |     3 | 130, 133, 137               |      2 |
| Methods                | operator*       |     1 | 145                         |        |
|                        | operator->      |     1 | 149                         |        |
|                        | operator++      |     2 | 153, 165                    |        |
|                        | operator--      |     2 | 173, 185                    |        |
|                        | operator+=      |     1 | 193                         |        |
|                        | operator+       |     1 | 212                         |        |
|                        | operator-=      |     1 | 219                         |        |
|                        | operator-       |     1 | 223                         |        |
|                        | operator[]      |     1 | 230                         |        |
|------------------------+-----------------+-------+-----------------------------+--------|
| Iterator               | operator==      |     2 | 253, 260                    |      2 |
| Overloads              | operator!=      |     2 | 266, 273                    |        |
|                        | operator<       |     2 | 279, 287                    |        |
|                        | operator>       |     2 | 294, 301                    |        |
|                        | operator<=      |     2 | 307, 314                    |        |
|                        | operator>=      |     2 | 320, 327                    |        |
|                        | operator-       |     2 | 327, 349                    |        |
|                        | operator+       |     1 | 360                         |        |
|------------------------+-----------------+-------+-----------------------------+--------|
| Iterator               | copy-assignable |       |                             |      4 |
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
| Capacity               | size            |     1 | 1142                        |      2 |
|                        | max_size        |     1 | 1147                        |        |
|                        | resize          |     2 | 1161, 1183                  |        |
|                        | empty           |     1 | 1228                        |        |
|                        | shrink_to_fit   |     1 | 1219                        |        |
|------------------------+-----------------+-------+-----------------------------+--------|
| Element access         | operator[]      |     2 | 1244, 1259                  |      2 |
|                        | at              |     2 | 1287, 1305                  |        |
|                        | front           |     2 | 1316, 1324                  |        |
|                        | back            |     2 | 1332, 1344                  |        |
|------------------------+-----------------+-------+-----------------------------+--------|
| Modifiers              | assign          |     3 | 978, 997, 1022              |      2 |
|                        | push_back       |     2 | 1393, 1407                  |        |
|                        | push_front      |     2 | 1362, 1375                  |        |
|                        | pop_back        |     1 |                             |        |
|                        | pop_front       |     1 | 1424, 1445                  |        |
|                        | insert          |     5 | 1480->deque.tcc:179, 1507,  |        |
|                        |                 |       | 1520, 1536, 1573            |        |
|                        | erase           |     2 | 1620, 1644                  |        |
|                        | swap            |     1 | 1657                        |        |
|                        | clear           |     1 | 1677                        |        |
|                        | emplace         |     1 | 1468->deque.tcc:152         |        |
|                        | emplace_front   |     1 | 1379->deque.tcc:117         |        |
|                        | emplace_back    |     1 | 1411->deque.tcc:133         |        |
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
         << "  -n <int>  : specify the number of threads" << endl
         << "  -h        : display this message" << endl
         << "  -t <name> : enable a test" << endl
         << "               f: member functions" << endl
         << "               i: iterators " << endl
         << "               c: capacity" << endl
         << "               e: element access " << endl
         << "               m: modifiers" << endl
         << "               a: allocators " << endl
         << "               n: non-member function overleads " << endl
         << "               *: all" << endl << endl;
    exit(0);
}

bool tests[7] = {false};

void enable_test(char which)
{
    switch (which) {
      case 'f': tests[0] = true; break;
      case 'i': tests[1] = true; break;
      case 'c': tests[2] = true; break;
      case 'e': tests[3] = true; break;
      case 'm': tests[4] = true; break;
      case 'a': tests[5] = true; break;
      case 'n': tests[6] = true; break;
      case '*': for (int i = 0; i < 7; ++i) tests[i] = true; break;
    }
}

/// Parse command line arguments using getopt()
void parseargs(int argc, char** argv)
{
    // parse the command-line options
    int opt;
    while ((opt = getopt(argc, argv, "n:ht:")) != -1) {
        switch (opt) {
          case 'n': num_threads = atoi(optarg); break;
          case 'h': usage();                    break;
          case 't': enable_test(*optarg);  break;
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
    if (tests[0])
        member_test(id);

    // test iterators
    if (tests[1]) {
        iter_test(id);
        legacy_const_iter_test(id);
    }

#if 0
    reverse_iterator_test(id);
    legacy_const_iterator_test(id);
    legacy_const_reverse_iterator_test(id);
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
