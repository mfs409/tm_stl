#include <iostream>
#include <string>
#include <vector>
#include "sequential_tests.h"
#include "concurrent_tests.h"

/// The vector we will use for our tests
std::vector<int>* obs_vector = NULL;

void observers_test_seq()
{
    // helper message
    std::cout << std::endl
              << "Testing observers" << std::endl;

    // test get_allocator (1).  Note that we need to use it so that it
    // doesn't get optimized away.
    obs_vector = new std::vector<int>();
    // use the vector's allocator to make an array, populate it, and then use
    // the array to feed our vector
    std::vector<int>::allocator_type a;
    a = obs_vector->get_allocator();
    int* p = a.allocate(3);
    for (int i = 0; i < 3; ++i)
        p[i] = i;
    obs_vector->assign(p, p+3);
    a.deallocate(p, 3);

    check("Expect {0, 1, 2}", obs_vector);

    delete obs_vector;
}

/// clone the vector to a local array represented by dsize, data[]
#define COPY_VECTOR                                 \
    dsize = 0;                                    \
    for (auto i : *obs_vector)                     \
        data[dsize++] = i

/// destroy the vector and set the pointer to NULL
#define CLEAR_VECTOR                                \
    delete obs_vector;                             \
    obs_vector = NULL

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
    obs_vector = new std::vector<int>();
    // use the vector's allocator to make an array, populate it, and then use
    // the array to feed our vector
    std::vector<int>::allocator_type a;
    a = obs_vector->get_allocator();
    int* p = a.allocate(3);
    for (int i = 0; i < 3; ++i)
        p[i] = i;
    obs_vector->assign(p, p+3);
    a.deallocate(p, 3);
    COPY_VECTOR;
    CLEAR_VECTOR;
    END_TX;
    VERIFY("get_allocator (1)", 3, {0, 1, 2});

    global_barrier->arrive(id);
}
