// Driver to test the transactional version of the std::list interface
//
// according to http://www.cplusplus.com/reference/list/list/, the std::list
// interface consists of the following:

/*
| Member functions                           | Status        |
|--------------------------------------------+---------------|
| (constructor), (destructor), and operator= | Done (step 1) |

| Iterators:     | Status |
|----------------+--------|
| begin, end     |        |
| rbegin, rend   |        |
| cbegin, cend   |        |
| crbegin, crend |        |

| Capacity:             | Status |
|-----------------------+--------|
| empty, size, max_size |        |

| Element access: | Status        |
|-----------------+---------------|
| front, back     | Done (step 2) |

| Modifiers:                            | Status        |
|---------------------------------------+---------------|
| assign,                               |               |
| emplace, emplace_front, emplace_back, |               |
| push_front, pop_front                 |               |
| push_back, pop_back                   | Done (step 2) |
| insert, erase, swap, resize, clear    |               |

| Operations:                                             | Status |
|---------------------------------------------------------+--------|
| splice, remove, remove_if, unique, merge, sort, reverse |        |

| Observers:    | Status |
|---------------+--------|
| get_allocator |        |

| Non-member function overloads:      | Status |
|-------------------------------------+--------|
| relational operators (forward_list) |        |
| swap (forward_list)                 |        |
*/
#include <list>
#include <cstdio>
#include <thread>
#include <atomic>
#include <mutex>
#include <cassert>

#include "barrier.h"

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
    // notify that we've started, then wait for all threads to be ready
    printf("Hello from thread %d\n", id);
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
    // we know who constructed.  Also push onto the heap list.  Then everyone pushes, pops, and pushes again
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


}


int main()
{
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
