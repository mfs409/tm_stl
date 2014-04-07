#include <map>
#include <cstdio>
#include <thread>
#include <atomic>
#include <mutex>
#include <cassert>
#include <iostream>
#include <unistd.h>

#include "../common/barrier.h"
#include "tests.h"

std::map<int, int>* test_map = NULL;

std::map<int, int>* per_thread_map[256] = {NULL}; // 256 is way more than enough...

void ctor_dtor_tests(int id)
{
    // helper message
    if (id == 0) {
        std::cout << std::endl
                  << "Testing ctor and dtor" << std::endl
                  << "(Note: we aren't testing the allocator parameter "
                  << "of the constructors)" << std::endl;
    }

    // scratch space for holding data when computing whether the test
    // succeeded or failed.
    int data[32], dsize = 0;

    // test empty constructor (1)
    global_barrier->arrive(id);
    BEGIN_TX;
    // call the basic constructor, expect empty map
    test_map = new std::map<int, int>();
    COPY_MAP;
    CLEAR_MAP;
    END_TX;
    VERIFY("empty ctor (1)", 0, {0});

    // test range constructor (2)
    global_barrier->arrive(id);
    std::map<int, int> a; //Tester... These values made sense at one point.
    a[-1] = 5;
    a[0] = 10;
    a[2] = 15;

    BEGIN_TX;
    // call the range constructor, expect {-1, 5, 0, 10, 2, 15}
    test_map = new std::map<int, int>(a.begin(), a.end());
    COPY_MAP;
    CLEAR_MAP;
    END_TX;
    VERIFY("range ctor (2)", 6, {-1, 5, 0, 10, 2, 15});

    // test copy constructor (3)
    global_barrier->arrive(id);
    std::map<int, int> b; //Tester... These values made sense at one point.
    b[1] = 8;
    b[2] = 16;
    b[4] = 32;

    if (per_thread_map[id] == NULL) // always true
        per_thread_map[id] = new std::map<int, int>(b.begin(), b.end());

    BEGIN_TX;
    // call the copy constructor, expect {1, 8, 2, 16, 4, 32}
    test_map = new std::map<int, int>(*per_thread_map[id]);
    delete per_thread_map[id];
    per_thread_map[id] = NULL;
    COPY_MAP;
    CLEAR_MAP;
    END_TX;
    VERIFY("copy ctor (3)", 6, {1, 8, 2, 16, 4, 32});

    // test move constructor (4)
    global_barrier->arrive(id);

    if (per_thread_map[id] == NULL) // always true
        per_thread_map[id] = new std::map<int, int>(a.begin(), a.end());

    BEGIN_TX;
    // Possible GCC bug found in vector testing.
    //if (id < 256)
    //    test_map = NULL;

    // call the move constructor, expect {-1, 5, 0, 10, 2, 15}
    test_map = new std::map<int, int>(std::move(*per_thread_map[id]));
    delete per_thread_map[id];
    per_thread_map[id] = NULL;
    COPY_MAP;
    CLEAR_MAP;
    END_TX;
    VERIFY("move ctor (4)", 6, {-1, 5, 0, 10, 2, 15});

    // test ilist constructor (5)
    global_barrier->arrive(id);
    BEGIN_TX;
    // call the ilist constructor, expect {1, 2, 3, 4}
    test_map = new std::map<int, int>({std::make_pair(1, 2), std::make_pair(3, 4)});
    COPY_MAP;
    CLEAR_MAP;
    END_TX;
    VERIFY("ilist ctor (5)", 4, {1, 2, 3, 4});

    global_barrier->arrive(id);
}

//Just like in vector
std::map<int, int> extra1({std::make_pair(1, 2), std::make_pair(3, 4)});

void op_eq_tests(int id)
{
    // helper message
    if (id == 0) {
        std::cout << std::endl
                  << "Testing operator=" << std::endl;
    }

    // scratch space for holding data when computing whether the test
    // succeeded or failed.
    int data[32], dsize = 0;

    // test copy assign (1)
    global_barrier->arrive(id);
    BEGIN_TX;
    test_map = new std::map<int, int>();
    *test_map = extra1;
    COPY_MAP;
    CLEAR_MAP;
    END_TX;
    VERIFY("copy assign (1)", 4, {1, 2, 3, 4});

    // test move assign (2)
    global_barrier->arrive(id);
    int i = 9;
    BEGIN_TX;
    test_map = new std::map<int, int>();
    *test_map = std::move(extra1);
    i = extra1.size(); //Not tested, technically?
    COPY_MAP;
    CLEAR_MAP;
    END_TX;
    assert(i == 0);
    VERIFY("move assign (2)", 4, {1, 2, 3, 4});

    // test ilist assign (3)
    global_barrier->arrive(id);
    BEGIN_TX;
    std::map<int, int> tmp(); //If we just assign, it calls the ilist ctor
    tmp = {std::make_pair(0, 1), std::make_pair(5, 3)};
    test_map = new std::map<int, int>();
    *test_map = std::move(tmp);
    COPY_MAP;
    CLEAR_MAP;
    END_TX;
    VERIFY("ilist assign (3)", 4, {0, 1, 5, 3});

    global_barrier->arrive(id);
}
