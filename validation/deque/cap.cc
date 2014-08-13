#include <iostream>
#include <string>
#include <deque>
#include <cassert>
#include "tests.h"

/// The deques we will use for our tests
std::deque<int>* cap_deque = NULL;

/// clone the list to a local array represented by dsize, data[]
#define COPY_DEQUE                              \
    for (auto i : *cap_deque)                   \
        data[dsize++] = i

#define RESET_LOCAL(val)                        \
    for (int i = 0; i < 256; ++i)               \
        data[i] = val;                          \
    dsize = 0

#define CHECK_SIZE(test_name, size)                                     \
    if (dsize != size)                                                  \
        printf(" [%d] size did not match %d != %d\n", id, dsize, size); \
    else if (id == 0)                                                   \
        printf(" [OK::count] %s\n", test_name)

#define CHECK(test_name, size, content_size, ...)                       \
    bool ok = true;                                                     \
    int  c  = 0;                                                        \
    for (auto i : __VA_ARGS__)                                          \
        ok &= (i == data[c++]);                                         \
    if (dsize != size)                                                  \
        printf(" [%d] size did not match %d != %d\n", id, dsize, size); \
    else if (!ok)                                                       \
        printf(" [%d] array copy did not match\n");                     \
    else if (id == 0)                                                   \
        printf(" [OK::count+data] %s\n", test_name)

/// test the capacity methods of std::deque
void cap_tests(int id)
{
    global_barrier->arrive(id);

    // a temporary array into which we can copy deque data
    int data[256], dsize;

    if (id == 0)
        printf("Testing deque capacity functions: size(1), max_size(1), resize(2), empty(1), shrink_to_fit(1)\n");

    // #1: Test size()
    global_barrier->arrive(id);
    {
        int size = 0;
        RESET_LOCAL(-2);
        BEGIN_TX;
        cap_deque = new std::deque<int>({1, 2, 3});
        size = cap_deque->size();
        delete(cap_deque);
        END_TX;
        if (size != 3)
            printf(" [%d] deque size test failed\n", id);
        else if (id == 0)
            printf(" [OK] %s\n", "deque size()");
    }

    // #2: Test max_size()
    global_barrier->arrive(id);
    {
        int size = 0;
        RESET_LOCAL(-2);
        BEGIN_TX;
        cap_deque = new std::deque<int>({1, 2, 3});
        size = cap_deque->max_size();
        delete(cap_deque);
        END_TX;
        if (size < 1024)
            printf(" [%d] deque max_size test failed %d\n", id, size);
        else if (id == 0)
            printf(" [OK] deque::max_size == %d\n", size);
    }

    // #3: Test resize()
    global_barrier->arrive(id);
    {
        RESET_LOCAL(-2);
        BEGIN_TX;
        cap_deque = new std::deque<int>({1, 2, 3});
        cap_deque->resize(1);
        cap_deque->resize(8, 6);
        COPY_DEQUE;
        delete(cap_deque);
        END_TX;
        CHECK(" deque resize (1) and (2)", 8, 8, { 1, 6, 6, 6, 6, 6, 6, 6 });
    }

    // #4: test empty()
    global_barrier->arrive(id);
    {
        bool ok = true;
        RESET_LOCAL(-2);
        BEGIN_TX;
        cap_deque = new std::deque<int>();
        ok &= cap_deque->empty();
        cap_deque->resize(8, 6);
        ok &= !cap_deque->empty();
        delete(cap_deque);
        END_TX;
        if (!ok)
            printf(" [%d] deque empty test failed\n", id);
        else if (id == 0)
            printf(" [OK] %s\n", "deque empty()");
    }

    // #5: test shrink_to_fit()
    global_barrier->arrive(id);
    {
        int size = 0;
        RESET_LOCAL(-2);
        BEGIN_TX;
        cap_deque = new std::deque<int>(100);
        cap_deque->resize(10);
        cap_deque->shrink_to_fit();
        size = cap_deque->size();
        delete(cap_deque);
        END_TX;
        if (size != 10)
            printf(" [%d] deque size test failed: size = %d\n", id, size);
        else if (id == 0)
            printf(" [OK] %s\n", "deque size()");
    }
}
