/* -*- C++ -*-*/

#include <mutex>
#include "../common/tm.h"

#pragma once

/**
 * This header is just a convenience for listing all the different concurrent
 * tests that we might run.
 */

// member function tests, from member.cc: ctor tests, dtor tests, and operator= tests
void ctor_dtor_tests(int id);
void op_eq_tests(int id);

// iterator tests, from iter.cc
void iter_create_tests(int id);
void basic_iter_tests(int id);
void const_iter_tests(int id);
void rev_iter_tests(int id);
void const_rev_iter_tests(int id);

// capacity tests, from cap.cc
void cap_tests(int id);

// element access tests, from element.cc
void element_tests(int id);

// modifier tests, from modifier.cc
void modifier_tests(int id);

// observers tests, from observers.cc
void observers_test(int id);

// test for operations, from operations.cc
void operations_tests(int id);

// allocator tests, from allocator.cc
void allocator_tests(int id);

#ifdef NO_TM
#define BEGIN_TX {std::lock_guard<std::mutex> _g(global_mutex);
#define END_TX   }
#else
#define BEGIN_TX __transaction_atomic {
#define END_TX   }
#endif

/// clone the map to a local array represented by dsize, data[]
#define COPY_MAP                                 \
    dsize = 0;                                   \
    for (auto& i : *test_map) {			 \
        data[dsize++] = i.first;                 \
        data[dsize++] = i.second;                \
    }

/// destroy the map and set the pointer to NULL
#define CLEAR_MAP                                \
    delete test_map;                             \
    test_map = NULL

/// compare the local array to our expected results
#define VERIFY(testName, size, ...)                                      \
    if (dsize != size)                                                   \
        printf(" [%d] size did not match %d != %d\n", id, dsize, size);  \
    else if (size > 0) {                                                 \
        int counter = 0;                                                 \
        bool good = true;                                                \
        for (auto i : __VA_ARGS__)                                       \
            good &= (data[counter++] == i);                              \
        if (!good)                                                       \
            std::cout << " ["<<id<<"] data did not match for thread " << id << std::endl; \
        else if (id == 0)                                                \
            std::cout << " [OK] " << testName << std::endl;              \
    }                                                                    \
    else if (id == 0)                                                    \
            std::cout << " [OK] " << testName << std::endl
