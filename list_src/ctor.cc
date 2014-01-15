#include <iostream>
#include <string>
#include <list>
#include "sequential_tests.h"
#include "concurrent_tests.h"
#include <cassert>

/**
 * Here is the sequential ctor test.  It also happens to be the sequential
 * dtor test, since there is only a basic dtor.
 */
void ctor_test_seq()
{
    std::cout << "Testing ctor and dtor" << std::endl;
    std::cout << "Note: we aren't testing the allocator parameter "
              << "of the constructors" << std::endl;

    // test basic constructor (1)
    my_list = new std::list<int>();
    check("Expect an empty list");
    delete my_list;

    // test sized constructor (2)
    my_list = new std::list<int>(5);
    check("Expect 5 default entries");
    delete my_list;

    // test sized constructor with defaults (2)
    my_list = new std::list<int>(5, 1);
    check("Expect 5 ones");
    delete my_list;

    // test range constructor (3)
    int a[] = {1, 2, 3};
    my_list = new std::list<int>(a, a+3);
    check("Expect 1,2,3");

    // test the copy constructor (4)
    std::list<int>* old = my_list;
    my_list = new std::list<int>(*old);
    delete old;
    check("Expect 1,2,3");

    // test the move constructor (5)
    old = my_list;
    my_list = new std::list<int>(std::move(*old));
    std::list<int>* swapper = my_list;
    my_list = old;
    check("Expect empty");
    my_list = swapper;
    check("Expect 1, 2, 3");
    delete old;
    delete swapper;

    // test the initializer list constructor (6)
    my_list = new std::list<int>({1, 2, 3, 4, 5, 6});
    check("Expect 1, 2, 3, 4, 5, 6");
    delete my_list;
}

/// The list we will use for our tests
std::list<int>* ctor_list = NULL;

/// An extra array of lists, which is sometimes useful.  Note that while we
/// treat this as per-thread, it's globally visible, so the compiler can't
/// assume anything...
std::list<int>* per_thread_list[256] = {NULL}; // 256 is way more than enough...

/// clone the list to a local array represented by dsize, data[]
#define COPY_LIST                                 \
    dsize = 0;                                    \
    for (auto i : *ctor_list)                     \
        data[dsize++] = i

/// destroy the list and set the pointer to NULL
#define CLEAR_LIST                                \
    delete ctor_list;                             \
    ctor_list = NULL

/// compare the local array to our expected results
#define VERIFY(testName, size, ...)                                     \
    if (dsize != size)                                                  \
        printf(" [%d] size did not match %d != %d\n", id, dsize, size);  \
    else if (size > 0) {                                                \
        int counter = 0;                                                \
        bool good = true;                                               \
        for (auto i : __VA_ARGS__)                                      \
            good &= (data[counter++] == i);                             \
        if (!good)                                                      \
            std::cout << " ["<<id<<"] Arrays did not match for thread " << id << std::endl; \
        else if (id == 0)                                               \
            std::cout << " [OK] " << testName << std::endl;              \
    }                                                                   \
    else if (id == 0)                                                   \
            std::cout << " [OK] " << testName << std::endl

/**
 * Here is the concurrent ctor test.  It also happens to be the concurrent
 * dtor test, since there is only a basic dtor.
 */
void ctor_test_concurrent(int id)
{
    // invariant: the list starts off null
    assert(ctor_list == NULL);

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
    BEGIN_TX;
    // call the basic constructor, expect empty list
    ctor_list = new std::list<int>();
    COPY_LIST;
    CLEAR_LIST;
    END_TX;
    VERIFY("basic ctor (1)", 0, {0});

    // test sized constructor (2)
    global_barrier->arrive(id);
    // make sure we'll detect errors in data[]
    for (int i = 0; i < 16; ++i)
        data[i] = 17;
    BEGIN_TX;
    // call the sized constructor, expect {0,0,0,0,0}
    // NB: we padded data[] to detect overflow
    ctor_list = new std::list<int>(5);
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
    ctor_list = new std::list<int>(5, 9);
    COPY_LIST;
    CLEAR_LIST;
    END_TX;
    VERIFY("sized ctor (2b)", 5, {9, 9, 9, 9, 9, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17});

    // test range constructor (3)
    global_barrier->arrive(id);
    int a[] = {1, 2, 3};
    BEGIN_TX;
    // use the range constructor, expect {1, 2, 3}
    ctor_list = new std::list<int>(a, a+3);
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
    ctor_list = new std::list<int>(*per_thread_list[id]);
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
        ctor_list = NULL;
    ctor_list = new std::list<int>(std::move(*per_thread_list[id]));
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
    ctor_list = new std::list<int>({6, 3, 8, 2, 9, 1});
    COPY_LIST;
    CLEAR_LIST;
    END_TX;
    VERIFY("ilist ctor (6)", 6, {6, 3, 8, 2, 9, 1});

    global_barrier->arrive(id);
}
