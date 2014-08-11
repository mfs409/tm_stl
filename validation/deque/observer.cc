#include <iostream>
#include <string>
#include <deque>
#include <cassert>
#include "tests.h"

/// The deque we will use for our tests
std::deque<int>* allocator_deque = NULL;

/// clone the list to a local array represented by dsize, data[]
#define COPY_DEQUE                              \
    for (auto i : *allocator_deque)                \
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

void observers_test(int id)
{
    global_barrier->arrive(id);

    // a temporary array into which we can copy deque data
    int data[256], dsize;

    if (id == 0)
        printf("Testing get_allocator()\n");

    // This is a really simple test
    global_barrier->arrive(id);
    {
        RESET_LOCAL(-2);
        BEGIN_TX;
        allocator_deque = new std::deque<int>();
        auto a = allocator_deque->get_allocator();
        dsize = allocator_deque->size();
        delete(allocator_deque);
        allocator_deque = NULL;
        END_TX;
    }
    CHECK_SIZE("get_allocator", 0);
}
