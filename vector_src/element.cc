#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include "concurrent_tests.h"

/// The vector we will use for our tests
std::vector<int>* elt_vector = NULL;

/// clone the vector to a local array represented by dsize, data[]
#define COPY_VECTOR                                 \
    dsize = 0;                                    \
    for (auto i : *elt_vector)                     \
        data[dsize++] = i

/// destroy the vector and set the pointer to NULL
#define CLEAR_VECTOR                                \
    delete elt_vector;                             \
    elt_vector = NULL

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

void element_test_seq()
{
    // helper message
  int id = 0;
    std::cout << std::endl
              << "Testing element access methods" << std::endl;

    int data[32], dsize = 0;

    // test data()
    elt_vector = new std::vector<int>({1, 2, 3, 4, 5, 6});
    int* ptr = elt_vector->data();
    *ptr = 15;
    ptr[2] = 16;
    ptr++;
    *ptr = 17;
    COPY_VECTOR;
    CLEAR_VECTOR;
    VERIFY("data (1)", 6, {15, 17, 16, 4, 5, 6});

    // test all accessors at once
    int f1, b1, mb1, ma1, f2, b2, mb2, ma2, f3, b3, mb3, ma3, f4, b4, mb4, ma4;
    elt_vector = new std::vector<int>({1, 2, 3, 4, 5, 6});
    // start with a basic test of front and back
    f1 = elt_vector->front();
    mb1 = (*elt_vector)[2];
    ma1 = elt_vector->at(3);
    b1 = elt_vector->back();
    // now make sure we can get const references
    const std::vector<int>* z = elt_vector;
    std::vector<int>::const_reference cb = z->back();
    std::vector<int>::const_reference cmb = (*z)[2];
    std::vector<int>::const_reference cma = z->at(3);
    std::vector<int>::const_reference cf = z->front();
    f2 = cf;
    mb2 = cmb;
    ma2 = cma;
    b2 = cb;
    // Ensure these don't compile:
    // cf += 11;
    // cb += 11;
    // now test that non-const references can be mutated
    std::vector<int>::reference rb = elt_vector->back();
    std::vector<int>::reference mb = (*elt_vector)[2];
    std::vector<int>::reference ma = elt_vector->at(3);
    std::vector<int>::reference rf = elt_vector->front();
    rb += 11;
    rf -= 11;
    mb += 11;
    ma -= 11;

    f3 = cf;
    b3 = cb;
    mb3 = cmb;
    ma3 = cma;

    f4 = rf;
    b4 = rb;
    mb4 = mb;
    ma4 = ma;
    CLEAR_VECTOR;
    bool ok = (f1 == 1) && (b1 == 6) && (f2 == 1) && (b2 == 6)
        && (f3 == -10) && (b3 == 17) && (f4 == -10) && (b4 == 17)
      && (mb1 == 3) && (ma1 == 4) && (mb2 == 3) && (ma2 == 4)
      && (mb1 == 14) && (ma1 == -7) && (mb2 == 14) && (ma2 == -7);
    if (!ok)
        std::cout << "[ERR] errors with front and back" << std::endl;
    else
        std::cout << " [OK] front and back" << std::endl;
}


void element_test_concurrent(int id)
{
    // helper message
    if (id == 0) {
        std::cout << std::endl
                  << "Testing element access methods" << std::endl;
    }

    int data[32], dsize = 0;

    // test data()
    global_barrier->arrive(id);
    BEGIN_TX;
    elt_vector = new std::vector<int>({1, 2, 3, 4, 5, 6});
    int* ptr = elt_vector->data();
    *ptr = 15;
    ptr[2] = 16;
    ptr++;
    *ptr = 17;
    COPY_VECTOR;
    CLEAR_VECTOR;
    END_TX;
    VERIFY("data (1)", 6, {15, 17, 16, 4, 5, 6});

    // test all accessors at once
    global_barrier->arrive(id);
    int f1, b1, mb1, ma1, f2, b2, mb2, ma2, f3, b3, mb3, ma3, f4, b4, mb4, ma4;
    BEGIN_TX;
    elt_vector = new std::vector<int>({1, 2, 3, 4, 5, 6});
    // start with a basic test of front and back
    f1 = elt_vector->front();
    mb1 = (*elt_vector)[2];
    ma1 = elt_vector->at(3);
    b1 = elt_vector->back();
    // now make sure we can get const references
    const std::vector<int>* z = elt_vector;
    std::vector<int>::const_reference cb = z->back();
    std::vector<int>::const_reference cmb = (*z)[2];
    std::vector<int>::const_reference cma = z->at(3);
    std::vector<int>::const_reference cf = z->front();
    f2 = cf;
    mb2 = cmb;
    ma2 = cma;
    b2 = cb;
    // Ensure these don't compile:
    // cf += 11;
    // cb += 11;
    // now test that non-const references can be mutated
    std::vector<int>::reference rb = elt_vector->back();
    std::vector<int>::reference mb = (*elt_vector)[2];
    std::vector<int>::reference ma = elt_vector->at(3);
    std::vector<int>::reference rf = elt_vector->front();
    rb += 11;
    rf -= 11;
    mb += 11;
    ma -= 11;

    f3 = cf;
    b3 = cb;
    mb3 = cmb;
    ma3 = cma;

    f4 = rf;
    b4 = rb;
    mb4 = mb;
    ma4 = ma;
    CLEAR_VECTOR;
    END_TX;
    bool ok = (f1 == 1) && (b1 == 6) && (f2 == 1) && (b2 == 6)
        && (f3 == -10) && (b3 == 17) && (f4 == -10) && (b4 == 17)
      && (mb1 == 3) && (ma1 == 4) && (mb2 == 3) && (ma2 == 4)
      && (mb1 == 14) && (ma1 == -7) && (mb2 == 14) && (ma2 == -7);
    if (!ok)
        std::cout << "["<<id<<"] errors with front and back" << std::endl;
    else if (id == 0)
        std::cout << " [OK] front and back" << std::endl;

    global_barrier->arrive(id);
}
