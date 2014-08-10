#include <iostream>
#include <string>
#include <vector>
#include "sequential_tests.h"
#include "concurrent_tests.h"

/// The vector we will use for our tests
std::vector<int>* swap_vector = NULL;

void swap_test_seq()
{
    // helper message
    std::cout << std::endl
              << "Testing swap" << std::endl;

    // scratch space for holding data when computing whether the test
    // succeeded or failed.
    int data[32], dsize = 0;

    swap_vector = new std::vector<int>({1, 2, 3, 4, 5});
    std::vector<int> l2({7, 8, 9});
    swap(*swap_vector, l2);
    check("Expect {7, 8, 9}", swap_vector);
    delete swap_vector;

    // and just for fun, let's test range iterators
    int vals[5];
    swap_vector = new std::vector<int>({1, 2, 3, 4, 5});
    int i = 0;
    for (auto x : *swap_vector)
        vals[i++] = x;
    bool ok = true;
    for (int i = 0; i < 5; ++i) {
        ok &= (vals[i] == i+1);
    }
    if (!ok)
        std::cout << "Error with range iterator" << std::endl;
    else
        std::cout << " OK range iterators (1)" << std::endl;
    delete swap_vector;
}

/// clone the vector to a local array represented by dsize, data[]
#define COPY_VECTOR                                 \
    dsize = 0;                                    \
    for (auto i : *swap_vector)                     \
        data[dsize++] = i

/// destroy the vector and set the pointer to NULL
#define CLEAR_VECTOR                                \
    delete swap_vector;                             \
    swap_vector = NULL

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

void swap_test_concurrent(int id)
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
    swap_vector = new std::vector<int>({1, 2, 3, 4, 5});
    std::vector<int> l2({7, 8, 9});
    swap(*swap_vector, l2);
    COPY_VECTOR;
    CLEAR_VECTOR;
    END_TX;
    VERIFY("std::swap (1)", 3, {7, 8, 9});

    // and just for fun, let's test range iterators
    int vals[5];
    global_barrier->arrive(id);
    BEGIN_TX;
    swap_vector = new std::vector<int>({1, 2, 3, 4, 5});
    int i = 0;
    for (auto x : *swap_vector)
        vals[i++] = x;
    CLEAR_VECTOR;
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
