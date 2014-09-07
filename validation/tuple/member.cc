#include <iostream>
#include <string>
#include <tuple>
#include <cassert>
#include "tests.h"
#include "verify.h"

/// The tuple we will use for our tests
typedef std::tuple<int, int, int> inttuple3;
typedef std::tuple<int, int>      inttuple2;
static inttuple3 * member_tuple  = NULL;
static inttuple2 * member_tuple2 = NULL;

void ctor_dtor_tests(int id)
{
    // test ctor 1'
    global_barrier->arrive(id);
    {
        verifier v;
        BEGIN_TX;
        member_tuple = new inttuple3(std::allocator_arg, NULL);
        v.insert_all<inttuple3>(member_tuple);
        delete(member_tuple);
        member_tuple = NULL;
        END_TX;
        v.check_size("tuple ctor (1')", id, 3);
    }

    // test ctor 2a'
    global_barrier->arrive(id);
    {
        verifier v;
        const inttuple3 p(1, 2, 3);
        BEGIN_TX;
        member_tuple = new inttuple3(std::allocator_arg, NULL, p);
        v.insert_all<inttuple3>(member_tuple);
        delete(member_tuple);
        member_tuple = NULL;
        END_TX;
        v.check("tuple ctor (2a')", id, 3, {1, 2, 3});
    }

    // test ctor 2b'
    global_barrier->arrive(id);
    {
        verifier v;
        const inttuple3 p(1, 2, 3);
        BEGIN_TX;
        member_tuple = new inttuple3(std::allocator_arg, NULL, std::make_tuple(1, 2, 3));
        v.insert_all<inttuple3>(member_tuple);
        delete(member_tuple);
        member_tuple = NULL;
        END_TX;
        v.check("tuple ctor (2b')", id, 3, {1, 2, 3});
    }

    // test ctor 3a'
    global_barrier->arrive(id);
    {
        verifier v;
        const std::tuple<char, char, char> p(1, 2, 3);
        BEGIN_TX;
        member_tuple = new inttuple3(std::allocator_arg, NULL, p);
        v.insert_all<inttuple3>(member_tuple);
        delete(member_tuple);
        member_tuple = NULL;
        END_TX;
        v.check("tuple ctor (3a')", id, 3, {1, 2, 3});
    }

    // test ctor 3b'
    global_barrier->arrive(id);
    {
        verifier v;
        BEGIN_TX;
        member_tuple = new inttuple3(std::allocator_arg, NULL, std::make_tuple((char)1, 2, 3));
        v.insert_all<inttuple3>(member_tuple);
        delete(member_tuple);
        member_tuple = NULL;
        END_TX;
        v.check("tuple ctor (3b')", id, 3, {1, 2, 3});
    }

    // test ctor 4a'
    global_barrier->arrive(id);
    {
        verifier v;
        const int a = 1;
        BEGIN_TX;
        member_tuple = new inttuple3(std::allocator_arg, NULL, a, a, a);
        v.insert_all<inttuple3>(member_tuple);
        delete(member_tuple);
        member_tuple = NULL;
        END_TX;
        v.check("tuple ctor (4a')", id, 3, {1, 1, 1});
    }

    // test ctor 4b'
    global_barrier->arrive(id);
    {
        verifier v;
        BEGIN_TX;
        member_tuple = new inttuple3(std::allocator_arg, NULL, 1, 2, 3);
        v.insert_all<inttuple3>(member_tuple);
        delete(member_tuple);
        member_tuple = NULL;
        END_TX;
        v.check("tuple ctor (4b')", id, 3, {1, 2, 3});
    }

    // test ctor 5a'
    global_barrier->arrive(id);
    {
        verifier v;
        const std::pair<int, int> p(1, 2);
        BEGIN_TX;
        member_tuple2 = new inttuple2(std::allocator_arg, NULL, p);
        v.insert(std::get<0>(*member_tuple2));
        v.insert(std::get<1>(*member_tuple2));
        delete(member_tuple2);
        member_tuple2 = NULL;
        END_TX;
        v.check("tuple ctor (5a')", id, 2, {1, 2});
    }

    // test ctor 5b'
    global_barrier->arrive(id);
    {
        verifier v;
        BEGIN_TX;
        member_tuple2 = new inttuple2(std::allocator_arg, NULL, std::make_pair(1, 2));
        v.insert(std::get<0>(*member_tuple2));
        v.insert(std::get<1>(*member_tuple2));
        delete(member_tuple2);
        member_tuple2 = NULL;
        END_TX;
        v.check("tuple ctor (5b')", id, 2, {1, 2});
    }
}

void op_eq_tests(int id)
{
    // test operator= 1a
    global_barrier->arrive(id);
    {
        verifier v;
        const inttuple3 p(1, 2, 3);
        BEGIN_TX;
        member_tuple = new inttuple3();
        *member_tuple = p;
        v.insert_all<inttuple3>(member_tuple);
        delete(member_tuple);
        member_tuple = NULL;
        END_TX;
        v.check("tuple operator= (1a)", id, 3, {1, 2, 3});
    }

    // test operator= 1b
    global_barrier->arrive(id);
    {
        verifier v;
        BEGIN_TX;
        member_tuple = new inttuple3();
        *member_tuple = std::make_tuple(1, 2, 3);
        v.insert_all<inttuple3>(member_tuple);
        delete(member_tuple);
        member_tuple = NULL;
        END_TX;
        v.check("tuple operator= (1b)", id, 3, {1, 2, 3});
    }

    // test operator= 2a
    global_barrier->arrive(id);
    {
        verifier v;
        const std::tuple<char, char, char> p(1, 2, 3);
        BEGIN_TX;
        member_tuple = new inttuple3();
        *member_tuple = p;
        v.insert_all<inttuple3>(member_tuple);
        delete(member_tuple);
        member_tuple = NULL;
        END_TX;
        v.check("tuple operator= (2a)", id, 3, {1, 2, 3});
    }

    // test operator= 2b
    global_barrier->arrive(id);
    {
        verifier v;
        BEGIN_TX;
        member_tuple = new inttuple3();
        *member_tuple = std::make_tuple((char)1, 2, 3);
        v.insert_all<inttuple3>(member_tuple);
        delete(member_tuple);
        member_tuple = NULL;
        END_TX;
        v.check("tuple operator= (2b)", id, 3, {1, 2, 3});
    }

    // test operator= 3a
    global_barrier->arrive(id);
    {
        verifier v;
        const std::pair<char, char> p(1, 2);
        BEGIN_TX;
        member_tuple2 = new inttuple2();
        *member_tuple2 = p;
        v.insert(std::get<0>(*member_tuple2));
        v.insert(std::get<1>(*member_tuple2));
        delete(member_tuple2);
        member_tuple2 = NULL;
        END_TX;
        v.check("tuple operator= (3a)", id, 2, {1, 2});
    }

    // test operator= 3b
    global_barrier->arrive(id);
    {
        verifier v;
        BEGIN_TX;
        member_tuple2 = new inttuple2();
        *member_tuple2 = std::make_pair((char)1, 2);;
        v.insert(std::get<0>(*member_tuple2));
        v.insert(std::get<1>(*member_tuple2));
        delete(member_tuple2);
        member_tuple2 = NULL;
        END_TX;
        v.check("tuple operator= (3b)", id, 2, {1, 2});
    }
}

void swap_tests(int id)
{
    // test swap
    global_barrier->arrive(id);
    {
        verifier v;
        inttuple3 p(1, 2, 3);
        BEGIN_TX;
        member_tuple = new inttuple3();
        member_tuple->swap(p);
        v.insert_all<inttuple3>(member_tuple);
        delete(member_tuple);
        member_tuple = NULL;
        END_TX;
        v.check("tuple swap (1)", id, 3, {1, 2, 3});
    }
}
