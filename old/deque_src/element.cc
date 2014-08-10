#include <iostream>
#include <string>
#include <deque>
#include <cassert>
#include "tests.h"

/// The deques we will use for our tests
std::deque<int>* element_deque = NULL;

/// clone the list to a local array represented by dsize, data[]
#define COPY_DEQUE                              \
    for (auto i : *element_deque)               \
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

void element_tests(int id)
{

}
