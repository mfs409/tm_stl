/*
  Driver to test the transactional version of the std::list interface

  According to http://www.cplusplus.com/reference/list/list/, the std::list
  interface consists of the following:

|--------------------+---------------------+-----------+----------|
| Category           | Method              | # Methods | # Tested |
| (* if not started) |                     |           | with TM  |
|--------------------+---------------------+-----------+----------|
| Member Functions   | (constructor)       |         6 |          |
|                    | (destructor)        |         1 |          |
|                    | operator=           |         3 |          |
|--------------------+---------------------+-----------+----------|
| Iterators          | begin               |         2 |          |
|                    | end                 |         2 |          |
| (note: must be     | rbegin              |         2 |          |
| sure to handle     | rend                |         2 |          |
| all 16 methods     | cbegin              |         1 |          |
| that can be        | cend                |         1 |          |
| called on          | crbegin             |         1 |          |
| an iterator!)      | crend               |         1 |          |
|--------------------+---------------------+-----------+----------|
| Capacity           | empty               |         1 |          |
|                    | size                |         1 |          |
|                    | max_size            |         1 |          |
|--------------------+---------------------+-----------+----------|
| Element access     | front               |         2 |          |
|                    | back                |         2 |          |
|--------------------+---------------------+-----------+----------|
| Modifiers          | assign              |         3 |          |
|                    | emplace             |         1 |          |
|                    | emplace_front       |         1 |          |
|                    | emplace_back        |         1 |          |
|                    | push_front          |         2 |          |
|                    | pop_front           |         1 |          |
|                    | push_back           |         2 |          |
|                    | pop_back            |         1 |          |
|                    | insert              |         5 |          |
|                    | erase               |         2 |          |
|                    | swap                |         1 |          |
|                    | resize              |         2 |          |
|                    | clear               |         1 |          |
|--------------------+---------------------+-----------+----------|
| Operations         | splice              |         6 |          |
|                    | remove              |         1 |          |
|                    | remove_if           |         2 |          |
|                    | unique              |         3 |          |
|                    | merge               |         4 |          |
|                    | sort                |         2 |          |
|                    | reverse             |         1 |          |
|--------------------+---------------------+-----------+----------|
| Observers          | get_allocator       |         1 |          |
|--------------------+---------------------+-----------+----------|
| TODO               |                     |           |          |
|--------------------+---------------------+-----------+----------|
| Non-member         | '=='                |         1 |          |
| function           | '!='                |         1 |          |
| overloads (NMFOs)  | '<'                 |         1 |          |
| (Relational        | '<='                |         1 |          |
| Operators)         | '>'                 |         1 |          |
|                    | '>='                |         1 |          |
|--------------------+---------------------+-----------+----------|
| NMFO (swap)        | swap (forward_list) |         1 |          |
|--------------------+---------------------+-----------+----------|
*/

#include <list>
#include <cstdio>
#include <thread>
#include <atomic>
#include <mutex>
#include <cassert>
#include <iostream>

#include "barrier.h"


// TODO: make this less ugly
std::list<int>* my_list;

void check(std::string s)
{
    std::cout << s << std::endl << " List: ";
    for (auto i : *my_list)
        std::cout << i << ", ";
    std::cout << std::endl;
}

#include "tests.h"

std::list<int>* global_list_ptr = NULL;
std::list<int>  global_list;

barrier* global_barrier;

#ifdef NO_TM
std::mutex global_mutex;
#define BEGIN_TX {std::lock_guard<std::mutex> _g(global_mutex);
#define END_TX   }
#else
#define BEGIN_TX __transaction_atomic {
#define END_TX   }
#endif

void listtest(int id)
{
    // wait for all threads to be ready
    global_barrier->arrive(id);

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

}

/**
 *  The current effort is a bit ad-hoc.  Let's create a sequential test for
 *  *everything*, then transactionalize it piecemeal.
 */
void sequential_test()
{
    // test constructors and destructor
    ctor_test();
    // test assignment operators
    assign_test();
    // test iterators
    iterator_test();
    reverse_iterator_test();
    legacy_const_iterator_test();
    legacy_const_reverse_iterator_test();
    const_iterator_test();
    const_reverse_iterator_test();
    // test capacity
    cap_test();
    // test element access
    element_test();
    // test modifiers
    modifier_test();
    // test operations
    operations_test();
    // test observers
    observers_test();
    // test relational operators
    relational_test();

}

int main()
{
    sequential_test();

    // for now, just do the sequential tests
    return 0;

    // set up a barrier and construct/start the threads
    int num_threads = 4;
    std::thread* threads = new std::thread[num_threads];
    global_barrier = new barrier(num_threads);
    for (int i = 0; i < num_threads; ++i)
        threads[i] = std::thread(listtest, i);
    // wait for the threads to finish
    for (int i = 0; i < num_threads; ++i)
        threads[i].join();

    // print the list...
    for (auto i : global_list)
        printf("item: %d\n", i);
}
