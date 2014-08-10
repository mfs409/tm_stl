#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include "sequential_tests.h"
#include "concurrent_tests.h"

/// The vector we will use for our tests
std::vector<int>* cap_vector = NULL;

/// clone the vector to a local array represented by dsize, data[]
#define COPY_VECTOR                                 \
    dsize = 0;                                    \
    for (auto i : *cap_vector)                     \
        data[dsize++] = i

/// destroy the vector and set the pointer to NULL
#define CLEAR_VECTOR                                \
    delete cap_vector;                             \
    cap_vector = NULL

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

void cap_test_seq()
{
    int id = 0;
    assert(cap_vector == NULL);
    if (id == 0) {
        std::cout << std::endl << "Testing capacity methods" << std::endl;
    }

    // scratch space for holding data when computing whether the test
    // succeeded or failed.
    int data[32], dsize = 0;

    // test size() (1)
    //barrier
    int s1 = -1, s2 = -1;
    {
    cap_vector = new std::vector<int>({8, 3, 7, 1});
    s1 = cap_vector->size();
    CLEAR_VECTOR;
    cap_vector = new std::vector<int>();
    s2 = cap_vector->size();;
    CLEAR_VECTOR;
    }
    if (s1 != 4 || s2 != 0)
        std::cout << "["<<id<<"] errors on size()" << std::endl;
    else if (id == 0)
        std::cout << " [OK] size()" << std::endl;

    // test max_size() (1)
    //barrier
    int z = -1;
    {
    cap_vector = new std::vector<int>({9, 5, 1});
    z = cap_vector->max_size();
    CLEAR_VECTOR;
    }
    if (z <= 65536) // good enough... it's not cap_vector->size(), and it's positive
        std::cout << "["<<id<<"] errors on max_size()" << std::endl;
    else if (id == 0)
        std::cout << " [OK] max_size() == " << z << std::endl;

    // test resize without default (1)
    //barrier
    {
    cap_vector = new std::vector<int>({1, 2, 3, 4, 5});
    cap_vector->resize(10);
    COPY_VECTOR;
    CLEAR_VECTOR;
    }
    VERIFY("resize (1)", 10, {1, 2, 3, 4, 5, 0, 0, 0, 0, 0});

    // do it again, but shrink this time
    //barrier
    {
    cap_vector = new std::vector<int>({1, 2, 3, 4, 5, 9, 8, 7});
    cap_vector->resize(5);
    COPY_VECTOR;
    CLEAR_VECTOR;
    }
    VERIFY("resize (2)", 5, {1, 2, 3, 4, 5});

    // test resize with default (2)
    //barrier
    {
    cap_vector = new std::vector<int>({1, 2, 3, 4, 5});
    cap_vector->resize(8, 2);
    COPY_VECTOR;
    CLEAR_VECTOR;
    }
    VERIFY("resize (3)", 8, {1, 2, 3, 4, 5, 2, 2, 2});

    // do it again, with shrink
    //barrier
    {
    cap_vector = new std::vector<int>({1, 2, 3, 4, 5, 9, 3});
    cap_vector->resize(5, 2);
    COPY_VECTOR;
    CLEAR_VECTOR;
    }
    VERIFY("resize (4)", 5, {1, 2, 3, 4, 5});

    //test capacity() (1)
    //barrier
    bool e;
    int c;
    {
    cap_vector = new std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    c = cap_vector->capacity();
    e = (c == 16);
    CLEAR_VECTOR;
    }
    if (e)
        std::cout << "["<<id<<"] errors on capacity()" << std::endl;
    else if (id == 0)
        std::cout << " [OK] capacity()" << std::endl;

    // test empty() (1)
    //barrier
    bool e1, e2;
    {
    cap_vector = new std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    e1 = cap_vector->empty();
    CLEAR_VECTOR;
    cap_vector = new std::vector<int>();
    e2 = cap_vector->empty();
    CLEAR_VECTOR;
    }
    if (e1 || !e2)
        std::cout << "["<<id<<"] errors on empty()" << std::endl;
    else if (id == 0)
        std::cout << " [OK] empty()" << std::endl;

    //test reserve() (1)
    //barrier
    //bool e;
    //int c;
    {
    cap_vector = new std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    cap_vector->reserve(21);
    c = cap_vector->capacity();
    e = (c == 21);
    CLEAR_VECTOR;
    }
    if (e)
        std::cout << "["<<id<<"] errors on reserve()" << std::endl;
    else if (id == 0)
        std::cout << " [OK] reserve()" << std::endl;

    //test shrink_to_fit() (1)
    //barrier
    //bool e;
    //int c;
    {
    cap_vector = new std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    cap_vector->reserve(21);
    cap_vector->shrink_to_fit();
    c = cap_vector->capacity();
    e = (c == 10);
    CLEAR_VECTOR;
    }
    if (e)
        std::cout << "["<<id<<"] errors on shrink_to_fit()" << std::endl;
    else if (id == 0)
        std::cout << " [OK] shrink_to_fit()" << std::endl;

    //barrier
}


void cap_test_concurrent(int id)
{
    assert(cap_vector == NULL);
    if (id == 0) {
        std::cout << std::endl << "Testing capacity methods" << std::endl;
    }

    // scratch space for holding data when computing whether the test
    // succeeded or failed.
    int data[32], dsize = 0;

    // test size() (1)
    global_barrier->arrive(id);
    int s1 = -1, s2 = -1;
    BEGIN_TX;
    cap_vector = new std::vector<int>({8, 3, 7, 1});
    s1 = cap_vector->size();
    CLEAR_VECTOR;
    cap_vector = new std::vector<int>();
    s2 = cap_vector->size();;
    CLEAR_VECTOR;
    END_TX;
    if (s1 != 4 || s2 != 0)
        std::cout << "["<<id<<"] errors on size()" << std::endl;
    else if (id == 0)
        std::cout << " [OK] size()" << std::endl;

    // test max_size() (1)
    global_barrier->arrive(id);
    int z = -1;
    BEGIN_TX;
    cap_vector = new std::vector<int>({9, 5, 1});
    z = cap_vector->max_size();
    CLEAR_VECTOR;
    END_TX;
    if (z <= 65536) // good enough... it's not cap_vector->size(), and it's positive
        std::cout << "["<<id<<"] errors on max_size()" << std::endl;
    else if (id == 0)
        std::cout << " [OK] max_size() == " << z << std::endl;

    // test resize without default (1)
    global_barrier->arrive(id);
    BEGIN_TX;
    cap_vector = new std::vector<int>({1, 2, 3, 4, 5});
    cap_vector->resize(10);
    COPY_VECTOR;
    CLEAR_VECTOR;
    END_TX;
    VERIFY("resize (1)", 10, {1, 2, 3, 4, 5, 0, 0, 0, 0, 0});

    // do it again, but shrink this time
    global_barrier->arrive(id);
    BEGIN_TX;
    cap_vector = new std::vector<int>({1, 2, 3, 4, 5, 9, 8, 7});
    cap_vector->resize(5);
    COPY_VECTOR;
    CLEAR_VECTOR;
    END_TX;
    VERIFY("resize (2)", 5, {1, 2, 3, 4, 5});

    // test resize with default (2)
    global_barrier->arrive(id);
    BEGIN_TX;
    cap_vector = new std::vector<int>({1, 2, 3, 4, 5});
    cap_vector->resize(8, 2);
    COPY_VECTOR;
    CLEAR_VECTOR;
    END_TX;
    VERIFY("resize (3)", 8, {1, 2, 3, 4, 5, 2, 2, 2});

    // do it again, with shrink
    global_barrier->arrive(id);
    BEGIN_TX;
    cap_vector = new std::vector<int>({1, 2, 3, 4, 5, 9, 3});
    cap_vector->resize(5, 2);
    COPY_VECTOR;
    CLEAR_VECTOR;
    END_TX;
    VERIFY("resize (4)", 5, {1, 2, 3, 4, 5});

    //test capacity() (1)
    global_barrier->arrive(id);
    bool e;
    int c;
    BEGIN_TX;
    cap_vector = new std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    c = cap_vector->capacity();
    e = (c == 16);
    CLEAR_VECTOR;
    END_TX;
    if (e)
        std::cout << "["<<id<<"] errors on capacity()" << std::endl;
    else if (id == 0)
        std::cout << " [OK] capacity()" << std::endl;

    // test empty() (1)
    global_barrier->arrive(id);
    bool e1, e2;
    BEGIN_TX;
    cap_vector = new std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    e1 = cap_vector->empty();
    CLEAR_VECTOR;
    cap_vector = new std::vector<int>();
    e2 = cap_vector->empty();
    CLEAR_VECTOR;
    END_TX;
    if (e1 || !e2)
        std::cout << "["<<id<<"] errors on empty()" << std::endl;
    else if (id == 0)
        std::cout << " [OK] empty()" << std::endl;

    //test reserve() (1)
    global_barrier->arrive(id);
    //bool e;
    //int c;
    BEGIN_TX;
    cap_vector = new std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    cap_vector->reserve(21);
    c = cap_vector->capacity();
    e = (c == 21);
    CLEAR_VECTOR;
    END_TX;
    if (e)
        std::cout << "["<<id<<"] errors on reserve()" << std::endl;
    else if (id == 0)
        std::cout << " [OK] reserve()" << std::endl;

    //test shrink_to_fit() (1)
    global_barrier->arrive(id);
    //bool e;
    //int c;
    BEGIN_TX;
    cap_vector = new std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    cap_vector->reserve(21);
    cap_vector->shrink_to_fit();
    c = cap_vector->capacity();
    e = (c == 10);
    CLEAR_VECTOR;
    END_TX;
    if (e)
        std::cout << "["<<id<<"] errors on shrink_to_fit()" << std::endl;
    else if (id == 0)
        std::cout << " [OK] shrink_to_fit()" << std::endl;

    global_barrier->arrive(id);
}
