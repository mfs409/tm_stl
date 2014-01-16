#include <iostream>
#include <string>
#include <list>
#include "concurrent_tests.h"

/// The list we will use for our tests
std::list<int>* obs_list = NULL;

/// clone the list to a local array represented by dsize, data[]
#define COPY_LIST                                 \
    dsize = 0;                                    \
    for (auto i : *obs_list)                     \
        data[dsize++] = i

/// destroy the list and set the pointer to NULL
#define CLEAR_LIST                                \
    delete obs_list;                             \
    obs_list = NULL

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

void observers_test_concurrent(int id)
{
    // helper message
    if (id == 0) {
        std::cout << std::endl
                  << "Testing observers" << std::endl;
    }

    // scratch space for holding data when computing whether the test
    // succeeded or failed.
    int data[32], dsize = 0;

    // test get_allocator (1).  Note that we need to use it so that it
    // doesn't get optimized away.
    global_barrier->arrive(id);
    BEGIN_TX;
    obs_list = new std::list<int>();
    // use the list's allocator to make an array, populate it, and then use
    // the array to feed our list
    std::list<int>::allocator_type a;
    a = obs_list->get_allocator();
    int* p = a.allocate(3);
    for (int i = 0; i < 3; ++i)
        p[i] = i;
    obs_list->assign(p, p+3);
    a.deallocate(p, 3);
    COPY_LIST;
    CLEAR_LIST;
    END_TX;
    VERIFY("get_allocator (1)", 3, {0, 1, 2});

    global_barrier->arrive(id);
}
