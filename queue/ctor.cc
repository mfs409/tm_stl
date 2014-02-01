#include <iostream>
#include <string>
#include <queue>
#include <cassert>
#include "tests.h"

/// The list we will use for our tests
std::queue<int>* ctor_queue = NULL;

/**
 * Here is the concurrent ctor test.  It also happens to be the concurrent
 * dtor test, since there is only a basic dtor.
 */
void ctor_test(int id)
{
    // invariant: the list starts off null
    assert(ctor_queue == NULL);

    // helper message
    if (id == 0) {
        std::cout << std::endl
                  << "Testing ctor and dtor" << std::endl
                  << "(Note: we aren't testing the allocator parameter "
                  << "of the constructors)" << std::endl;
    }

    // scratch space for holding data when computing whether the test
    // succeeded or failed.
    int data[32], dsize = 0;

    // test basic constructor (1)
    global_barrier->arrive(id);
    int s = -1;
    BEGIN_TX;
    // call the basic constructor, expect empty list
    ctor_queue = new std::queue<int>();
    s = ctor_queue->size();
    delete ctor_queue;
    ctor_queue = NULL;
    END_TX;
    if (s == 0)
        printf("[OK] basic ctor(1)\n");
    else
        printf("basic ctor(1) failed");
#if 0
    // test sized constructor (2)
    global_barrier->arrive(id);
    // make sure we'll detect errors in data[]
    for (int i = 0; i < 16; ++i)
        data[i] = 17;
    BEGIN_TX;
    // call the sized constructor, expect {0,0,0,0,0}
    // NB: we padded data[] to detect overflow
    ctor_queue = new std::list<int>(5);
    COPY_LIST;
    CLEAR_LIST;
    END_TX;
    VERIFY("sized ctor (2a)", 5, {0, 0, 0, 0, 0, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17});

    // test sized constructor (2b)
    global_barrier->arrive(id);
    // make sure we'll detect errors in data[]
    for (int i = 0; i < 16; ++i)
        data[i] = 17;
    BEGIN_TX;
    // call the sized constructor, expect {0,0,0,0,0}
    // NB: we padded data[] to detect overflow
    ctor_queue = new std::list<int>(5, 9);
    COPY_LIST;
    CLEAR_LIST;
    END_TX;
    VERIFY("sized ctor (2b)", 5, {9, 9, 9, 9, 9, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17});

    // test range constructor (3)
    global_barrier->arrive(id);
    int a[] = {1, 2, 3};
    BEGIN_TX;
    // use the range constructor, expect {1, 2, 3}
    ctor_queue = new std::list<int>(a, a+3);
    COPY_LIST;
    CLEAR_LIST;
    END_TX;
    VERIFY("range ctor (3)", 3, {1, 2, 3});

    // test the copy constructor (4)
    global_barrier->arrive(id);
    int b[] = {7, 8, 9, 2};
    if (per_thread_list[id] == NULL) // always true
        per_thread_list[id] = new std::list<int>({b, b+4});
    BEGIN_TX;
    ctor_queue = new std::list<int>(*per_thread_list[id]);
    delete per_thread_list[id];
    per_thread_list[id] = NULL;
    COPY_LIST;
    CLEAR_LIST;
    END_TX;
    VERIFY("copy ctor  (4)", 4, {7, 8, 9, 2});

    // test the move constructor (5)
    global_barrier->arrive(id);
    if (per_thread_list[id] == NULL) // always true
        per_thread_list[id] = new std::list<int>({a, a+3});
    int tmp;
    BEGIN_TX;
    // Possible GCC bug... nulling the null list before we start causes an
    // early write, without which we get concurrency errors
    if (id < 256)
        ctor_queue = NULL;
    ctor_queue = new std::list<int>(std::move(*per_thread_list[id]));
    tmp = per_thread_list[id]->size(); // expect 0
    delete per_thread_list[id];
    per_thread_list[id] = NULL;
    COPY_LIST;
    CLEAR_LIST;
    END_TX;
    assert(tmp == 0);
    VERIFY("move ctor  (5)", 3, {1, 2, 3});

    // test the initializer list constructor (6)
    global_barrier->arrive(id);
    BEGIN_TX;
    ctor_queue = new std::list<int>({6, 3, 8, 2, 9, 1});
    COPY_LIST;
    CLEAR_LIST;
    END_TX;
    VERIFY("ilist ctor (6)", 6, {6, 3, 8, 2, 9, 1});
#endif
    global_barrier->arrive(id);
}
