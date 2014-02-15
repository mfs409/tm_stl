#include <iostream>
#include <string>
#include <list>
#include <cassert>
#include "tests.h"

/// The list we will use for our tests
std::list<int>* elt_list = NULL;

/// clone the list to a local array represented by dsize, data[]
#define COPY_LIST                                 \
    dsize = 0;                                    \
    for (auto i : *elt_list)                     \
        data[dsize++] = i

/// destroy the list and set the pointer to NULL
#define CLEAR_LIST                                \
    delete elt_list;                             \
    elt_list = NULL

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

void element_test(int id)
{
    // helper message
    if (id == 0) {
        std::cout << std::endl
                  << "Testing element access methods" << std::endl;
    }

    // test all accessors at once
    global_barrier->arrive(id);
    int f1, b1, f2, b2, f3, b3, f4, b4;
    BEGIN_TX;
    elt_list = new std::list<int>({1, 2, 3, 4, 5, 6});
    // start with a basic test of front and back
    f1 = elt_list->front();
    b1 = elt_list->back();
    // now make sure we can get const references
    const std::list<int>* z = elt_list;
    std::list<int>::const_reference cb = z->back();
    std::list<int>::const_reference cf = z->front();
    f2 = cf;
    b2 = cb;
    // Ensure these don't compile:
    // cf += 11;
    // cb += 11;
    // now test that non-const references can be mutated
    std::list<int>::reference rb = elt_list->back();
    std::list<int>::reference rf = elt_list->front();
    rb += 11;
    rf -= 11;
    f3 = cf;
    b3 = cb;
    f4 = rf;
    b4 = rb;
    CLEAR_LIST;
    END_TX;
    bool ok = (f1 == 1) && (b1 == 6) && (f2 == 1) && (b2 == 6)
        && (f3 == -10) && (b3 == 17) && (f4 == -10) && (b4 == 17);
    if (!ok)
        std::cout << "["<<id<<"] errors with front and back" << std::endl;
    else if (id == 0)
        std::cout << " [OK] front and back" << std::endl;

    global_barrier->arrive(id);
}
