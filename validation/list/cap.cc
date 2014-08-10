#include <iostream>
#include <string>
#include <list>
#include <cassert>
#include "tests.h"

/// The list we will use for our tests
std::list<int>* cap_list = NULL;

/// clone the list to a local array represented by dsize, data[]
#define COPY_LIST                                 \
    dsize = 0;                                    \
    for (auto i : *cap_list)                     \
        data[dsize++] = i

/// destroy the list and set the pointer to NULL
#define CLEAR_LIST                                \
    delete cap_list;                             \
    cap_list = NULL

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

void cap_test(int id)
{
    assert(cap_list == NULL);
    if (id == 0) {
        std::cout << std::endl << "Testing capacity methods" << std::endl;
    }

    // scratch space for holding data when computing whether the test
    // succeeded or failed.
    int data[32], dsize = 0;

    // test empty() (1)
    global_barrier->arrive(id);
    bool e1, e2;
    BEGIN_TX;
    cap_list = new std::list<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    e1 = cap_list->empty();
    CLEAR_LIST;
    cap_list = new std::list<int>();
    e2 = cap_list->empty();
    CLEAR_LIST;
    END_TX;
    if (e1 || !e2)
        std::cout << "["<<id<<"] errors on empty()" << std::endl;
    else if (id == 0)
        std::cout << " [OK] empty()" << std::endl;


    // test size() (2)
    global_barrier->arrive(id);
    int s1 = -1, s2 = -1;
    BEGIN_TX;
    cap_list = new std::list<int>({8, 3, 7, 1});
    s1 = cap_list->size();
    CLEAR_LIST;
    cap_list = new std::list<int>();
    s2 = cap_list->size();;
    CLEAR_LIST;
    END_TX;
    if (s1 != 4 || s2 != 0)
        std::cout << "["<<id<<"] errors on size()" << std::endl;
    else if (id == 0)
        std::cout << " [OK] size()" << std::endl;

    // test max_size() (3)
    global_barrier->arrive(id);
    int z = -1;
    BEGIN_TX;
    cap_list = new std::list<int>({9, 5, 1});
    z = cap_list->max_size();
    CLEAR_LIST;
    END_TX;
    if (z <= 65536) // good enough... it's not cap_list->size(), and it's positive
        std::cout << "["<<id<<"] errors on max_size()" << std::endl;
    else if (id == 0)
        std::cout << " [OK] max_size() == " << z << std::endl;
    global_barrier->arrive(id);
}
