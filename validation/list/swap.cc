#include <iostream>
#include <string>
#include <list>
#include "tests.h"

/// The list we will use for our tests
std::list<int>* swap_list = NULL;

/// clone the list to a local array represented by dsize, data[]
#define COPY_LIST                                 \
    dsize = 0;                                    \
    for (auto i : *swap_list)                     \
        data[dsize++] = i

/// destroy the list and set the pointer to NULL
#define CLEAR_LIST                                \
    delete swap_list;                             \
    swap_list = NULL

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

void swap_test(int id)
{
    // helper message
    if (id == 0) {
        std::cout << std::endl
                  << "Testing swap" << std::endl;
    }

    // scratch space for holding data when computing whether the test
    // succeeded or failed.
    int data[32], dsize = 0;

    global_barrier->arrive(id);
    BEGIN_TX;
    swap_list = new std::list<int>({1, 2, 3, 4, 5});
    std::list<int> l2({7, 8, 9});
    swap(*swap_list, l2);
    COPY_LIST;
    CLEAR_LIST;
    END_TX;
    VERIFY("std::swap (1)", 3, {7, 8, 9});

    // and just for fun, let's test range iterators
    int vals[5];
    global_barrier->arrive(id);
    BEGIN_TX;
    swap_list = new std::list<int>({1, 2, 3, 4, 5});
    int i = 0;
    for (auto x : *swap_list)
        vals[i++] = x;
    CLEAR_LIST;
    END_TX;
    bool ok = true;
    for (int i = 0; i < 5; ++i) {
        ok &= (vals[i] == i+1);
    }
    if (!ok)
        std::cout << "["<<id<<"] error with range iterator" << std::endl;
    else if (id == 0)
        std::cout << " [OK] range iterators (1)" << std::endl;

    global_barrier->arrive(id);
}
