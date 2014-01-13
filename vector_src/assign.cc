#include <iostream>
#include <string>
#include <vector>
#include "sequential_tests.h"
#include "concurrent_tests.h"
#include <cassert>

/// The vector we will use for our tests
std::vector<int>* assign_vector = NULL;

std::vector<int> z({1, 2, 3, 4, 5, 6});
std::vector<int> y;

/**
 * Here is the sequential operator= test.
 */
void assign_test_seq()
{
  std::cout << "Testing operator=" << std::endl;
  // create a vector with the initializer vector operator
  std::vector<int> tmp({1, 2, 3, 4, 5, 6});

  // test copy assignment (1)
  y = z;
  std::vector<int> t2 = tmp;
  tmp.clear();
  assign_vector = &tmp;
  check("Expect empty", assign_vector);
  assign_vector = &t2;
  check("Expect 1, 2, 3, 4, 5, 6", assign_vector);

  // test move assignment (2)
  tmp = std::move(t2);
  assign_vector = &t2;
  check("Expect empty", assign_vector);
  assign_vector = &tmp;
  check("Expect 1, 2, 3, 4, 5, 6", assign_vector);

  // test initializer assignment (3)
  t2 = {8, 9, 11};
  assign_vector = &t2;
  check("Expect 8, 9, 11", assign_vector);

  // clean up...
  tmp.clear();
  t2.clear();
}

/// an extra vector
std::vector<int> extra1({1, 2, 3, 4, 5, 6});

/// another extra vector
std::vector<int> extra2;

/// clone the vector to a local array represented by dsize, data[]
#define COPY_VECTOR                                 \
    dsize = 0;                                    \
    for (auto i : *assign_vector)                     \
        data[dsize++] = i

/// destroy the vector and set the pointer to NULL
#define CLEAR_VECTOR                                \
    delete assign_vector;                           \
    assign_vector = NULL

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
void assign_test_concurrent(int id)
{
    // invariant: the vector starts off null
    assert(assign_vector == NULL);

    // helper message
    if (id == 0) {
        std::cout << std::endl
                  << "Testing operator=" << std::endl;
    }

    // scratch space for holding data when computing whether the test
    // succeeded or failed.
    int data[32], dsize = 0;

    // create a vector with the initializer vector operator
    std::vector<int> tmp({1, 2, 3, 4, 5, 6});

    // test copy assignment (1)
    global_barrier->arrive(id);
    BEGIN_TX;
    assign_vector = new std::vector<int>();
    *assign_vector = extra1;
    COPY_VECTOR;
    CLEAR_VECTOR;
    END_TX;
    VERIFY("copy assignment (1)", 6, {1, 2, 3, 4, 5, 6});

    // test move assignment (2)
    global_barrier->arrive(id);
    int i = 9;
    BEGIN_TX;
    assign_vector = new std::vector<int>();
    *assign_vector = std::move(extra1);
    i = extra1.size();
    COPY_VECTOR;
    CLEAR_VECTOR;
    extra1 = {1, 2, 3, 4, 5, 6}; // not tested until (3)
    END_TX;
    assert(i == 0);
    VERIFY("move assignment (2)", 6, {1, 2, 3, 4, 5, 6});

    // test initializer assignment (3)
    global_barrier->arrive(id);
    BEGIN_TX;
    std::vector<int> tmp = {6, 3, 8, 2}; // this is the actual test
    assign_vector = new std::vector<int>();
    *assign_vector = std::move(tmp);
    COPY_VECTOR;
    CLEAR_VECTOR;
    END_TX;
    VERIFY("initializer assignment (3)", 4, {6, 3, 8, 2});

    global_barrier->arrive(id);
}
