#include <iostream>
#include <string>
#include <vector>
#include "sequential_tests.h"
#include "concurrent_tests.h"
#include <cassert>

/// The vector we will use for our tests
std::vector<int>* ctor_vector = NULL;

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
    ctor_vector = new std::vector<int>();
    check("Expect an empty vector", ctor_vector);
    delete ctor_vector;

    // test sized constructor (2)
    ctor_vector = new std::vector<int>(5);
    check("Expect 5 default entries", ctor_vector);
    delete ctor_vector;

    // test sized constructor with defaults (2)
    ctor_vector = new std::vector<int>(5, 1);
    check("Expect 5 ones", ctor_vector);
    delete ctor_vector;

    // test range constructor (3)
    int a[] = {1, 2, 3};
    ctor_vector = new std::vector<int>(a, a+3);
    check("Expect 1,2,3", ctor_vector);

    // test the copy constructor (4)
    std::vector<int>* old = ctor_vector;
    ctor_vector = new std::vector<int>(*old);
    delete old;
    check("Expect 1,2,3", ctor_vector);

    // test the move constructor (5)
    old = ctor_vector;
    ctor_vector = new std::vector<int>(std::move(*old));
    std::vector<int>* swapper = ctor_vector;
    ctor_vector = old;
    check("Expect empty", ctor_vector);
    ctor_vector = swapper;
    check("Expect 1, 2, 3", ctor_vector);
    delete old;
    delete swapper;

    // test the initializer vector constructor (6)
    ctor_vector = new std::vector<int>({1, 2, 3, 4, 5, 6});
    check("Expect 1, 2, 3, 4, 5, 6", ctor_vector);
    delete ctor_vector;
}

/// An extra array of vectors, which is sometimes useful.  Note that while we
/// treat this as per-thread, it's globally visible, so the compiler can't
/// assume anything...
std::vector<int>* per_thread_vector[256] = {NULL}; // 256 is way more than enough...

/// clone the vector to a local array represented by dsize, data[]
#define COPY_VECTOR                                 \
    dsize = 0;                                    \
    for (auto i : *ctor_vector)                     \
        data[dsize++] = i

/// destroy the vector and set the pointer to NULL
#define CLEAR_VECTOR                                \
    delete ctor_vector;                             \
    ctor_vector = NULL

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
    // invariant: the vector starts off null
    assert(ctor_vector == NULL);

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
    // call the basic constructor, expect empty vector
    ctor_vector = new std::vector<int>();
    COPY_VECTOR;
    CLEAR_VECTOR;
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
    ctor_vector = new std::vector<int>(5);
    COPY_VECTOR;
    CLEAR_VECTOR;
    END_TX;
    VERIFY("sized ctor (2)", 5, {0, 0, 0, 0, 0, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17, 17});

    // test range constructor (3)
    global_barrier->arrive(id);
    int a[] = {1, 2, 3};
    BEGIN_TX;
    // use the range constructor, expect {1, 2, 3}
    ctor_vector = new std::vector<int>(a, a+3);
    COPY_VECTOR;
    CLEAR_VECTOR;
    END_TX;
    VERIFY("range ctor (3)", 3, {1, 2, 3});

    // test the copy constructor (4)
    global_barrier->arrive(id);
    int b[] = {7, 8, 9, 2};
    if (per_thread_vector[id] == NULL) // always true
        per_thread_vector[id] = new std::vector<int>({b, b+4});
    BEGIN_TX;
    ctor_vector = new std::vector<int>(*per_thread_vector[id]);
    delete per_thread_vector[id];
    per_thread_vector[id] = NULL;
    COPY_VECTOR;
    CLEAR_VECTOR;
    END_TX;
    VERIFY("copy ctor  (4)", 4, {7, 8, 9, 2});

    // test the move constructor (5)
    global_barrier->arrive(id);
    if (per_thread_vector[id] == NULL) // always true
        per_thread_vector[id] = new std::vector<int>({a, a+3});
    int tmp;
    BEGIN_TX;
    // Possible GCC bug... nulling the null vector before we start causes an
    // early write, without which we get concurrency errors
    if (id < 256)
        ctor_vector = NULL;
    ctor_vector = new std::vector<int>(std::move(*per_thread_vector[id]));
    tmp = per_thread_vector[id]->size(); // expect 0
    delete per_thread_vector[id];
    per_thread_vector[id] = NULL;
    COPY_VECTOR;
    CLEAR_VECTOR;
    END_TX;
    assert(tmp == 0);
    VERIFY("move ctor  (5)", 3, {1, 2, 3});

    // test the initializer vector constructor (6)
    global_barrier->arrive(id);
    BEGIN_TX;
    ctor_vector = new std::vector<int>({6, 3, 8, 2, 9, 1});
    COPY_VECTOR;
    CLEAR_VECTOR;
    END_TX;
    VERIFY("ilist ctor (6)", 6, {6, 3, 8, 2, 9, 1});

    global_barrier->arrive(id);
}
