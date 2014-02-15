#include <iostream>
#include <string>
#include <list>
#include <cassert>
#include "tests.h"

/// The list we will use for our tests
std::list<int>* assign_list = NULL;

/// an extra list
std::list<int> extra1({1, 2, 3, 4, 5, 6});

/// another extra list
std::list<int> extra2;

/// clone the list to a local array represented by dsize, data[]
#define COPY_LIST                                 \
    dsize = 0;                                    \
    for (auto i : *assign_list)                     \
        data[dsize++] = i

/// destroy the list and set the pointer to NULL
#define CLEAR_LIST                                \
    delete assign_list;                           \
    assign_list = NULL

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
 * Here is the sequential operator= test.
 */
void assign_test(int id)
{
    // invariant: the list starts off null
    assert(assign_list == NULL);

    // helper message
    if (id == 0) {
        std::cout << std::endl
                  << "Testing operator=" << std::endl;
    }

    // scratch space for holding data when computing whether the test
    // succeeded or failed.
    int data[32], dsize = 0;

    // create a list with the initializer list operator
    std::list<int> tmp({1, 2, 3, 4, 5, 6});

    // test copy assignment (1)
    global_barrier->arrive(id);
    BEGIN_TX;
    assign_list = new std::list<int>();
    *assign_list = extra1;
    COPY_LIST;
    CLEAR_LIST;
    END_TX;
    VERIFY("copy assignment (1)", 6, {1, 2, 3, 4, 5, 6});

    // test move assignment (2)
    global_barrier->arrive(id);
    int i = 9;
    BEGIN_TX;
    assign_list = new std::list<int>();
    *assign_list = std::move(extra1);
    i = extra1.size();
    COPY_LIST;
    CLEAR_LIST;
    extra1 = {1, 2, 3, 4, 5, 6}; // not tested until (3)
    END_TX;
    assert(i == 0);
    VERIFY("move assignment (2)", 6, {1, 2, 3, 4, 5, 6});

    // test initializer assignment (3)
    global_barrier->arrive(id);
    BEGIN_TX;
    std::list<int> tmp = {6, 3, 8, 2}; // this is the actual test
    assign_list = new std::list<int>();
    *assign_list = std::move(tmp);
    COPY_LIST;
    CLEAR_LIST;
    END_TX;
    VERIFY("initializer assignment (3)", 4, {6, 3, 8, 2});

    global_barrier->arrive(id);
}
