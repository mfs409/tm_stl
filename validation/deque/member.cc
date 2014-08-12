#include <iostream>
#include <string>
#include <deque>
#include <cassert>
#include "tests.h"
#include "support.h"

/// The deque we will use for our tests
std::deque<int>* member_deque = NULL;

/**
 * Test 7 ctors and 1 dtor
 */
void ctor_dtor_tests(int id)
{
    global_barrier->arrive(id);

    // a temporary array into which we can copy deque data
    int data[256], dsize;

    if (id == 0)
        printf("Testing member deque constructors(8) and destructors(1)\n");

    // the first test is simple ctor and dtor
    //
    // NB: we haven't actually verified size yet, but we use it here.
    global_barrier->arrive(id);
    {
        RESET_LOCAL(-2, data, dsize);
        BEGIN_TX;
        member_deque = new std::deque<int>();
        dsize = member_deque->size();
        delete(member_deque);
        member_deque = NULL;
        END_TX;
        CHECK_SIZE("basic ctor(1) and dtor(1)", 0, dsize, id);
    }

    // the next test uses a custom allocator with ctor 1
    //
    // NB: we haven't actually verified size yet, but we use it here.
    global_barrier->arrive(id);
    {
        RESET_LOCAL(-2, data, dsize);
        BEGIN_TX;
        member_deque = new std::deque<int>();
        // using get_allocator without checking it
        auto a = member_deque->get_allocator();
        delete(member_deque);
        member_deque = new std::deque<int>(a);
        dsize = member_deque->size();
        delete(member_deque);
        member_deque = NULL;
        END_TX;
        CHECK_SIZE("basic ctor(1) and dtor(1)", 0, dsize, id);
    }

    // the next test will call the simple fill constructor and the fill
    // constructor with default value
    //
    // NB: we haven't actually verified iterators yet, but we use them in
    //     COPY_DEQUE
    global_barrier->arrive(id);
    {
        RESET_LOCAL(-2, data, dsize);
        BEGIN_TX;
        member_deque = new std::deque<int>(4);
        COPY_DEQUE(member_deque, data, dsize);
        delete(member_deque);
        member_deque = new std::deque<int>(4, 98);
        COPY_DEQUE(member_deque, data, dsize);
        delete(member_deque);
        member_deque = NULL;
        END_TX;
        CHECK("fill ctor (2a) and fill ctor (2b)", 8, dsize, data, id,
              { 0, 0, 0, 0, 98, 98, 98, 98, -2});
    }

    // the next test will use the range ctor
    global_barrier->arrive(id);
    {
        RESET_LOCAL(-2, data, dsize);
        int q [] = { 9, 8, 7 };
        BEGIN_TX;
        member_deque = new std::deque<int>(q, q+3);
        COPY_DEQUE(member_deque, data, dsize);
        delete(member_deque);
        member_deque = NULL;
        END_TX;
        CHECK("range ctor (3)", 3, dsize, data, id, { 9, 8, 7, -2 });
    }

    // the next test will use the copy ctor
    global_barrier->arrive(id);
    {
        RESET_LOCAL(-2, data, dsize);
        std::deque<int> local = { 3, 4, 5, 6 };
        BEGIN_TX;
        member_deque = new std::deque<int>(local);
        COPY_DEQUE(member_deque, data, dsize);
        delete(member_deque);
        member_deque = NULL;
        END_TX;
        CHECK("copy ctor (4a) -- there is no 4b", 4, dsize, data, id, {3, 4, 5, 6, -2});
    }

    // the next test is the move ctor
    global_barrier->arrive(id);
    {
        RESET_LOCAL(-2, data, dsize);
        std::deque<int> local = {5, 4, 3};
        BEGIN_TX;
        member_deque = new std::deque<int>(std::move(local));
        COPY_DEQUE(member_deque, data, dsize);
        delete(member_deque);
        member_deque = NULL;
        END_TX;
        CHECK("move ctor (5a) -- there is no 5b", 3, dsize, data, id, {5, 4, 3, -2});
    }

    // the next test is the ilist ctor
    global_barrier->arrive(id);
    {
        RESET_LOCAL(-2, data, dsize);
        BEGIN_TX;
        member_deque = new std::deque<int>({11, 13, 15, 17});
        COPY_DEQUE(member_deque, data, dsize);
        delete(member_deque);
        member_deque = NULL;
        END_TX;
        CHECK("move ilist ctor (6)", 4, dsize, data, id, {11, 13, 15, 17, -2});
    }
}

/**
 *  Test 3 operator= functions
 */
void op_eq_tests(int id)
{
    global_barrier->arrive(id);

    // a temporary array into which we can copy deque data
    int data[256], dsize;

    if (id == 0)
        printf("Testing operator= (3)\n");

    // test #1 is operator= copy
    //
    // NB: we haven't actually verified iterators yet, but we use them...
    global_barrier->arrive(id);
    {
        RESET_LOCAL(-2, data, dsize);
        std::deque<int> local = { 3, 4, 5, 6 };
        BEGIN_TX;
        member_deque = new std::deque<int>();
        *member_deque = local;
        COPY_DEQUE(member_deque, data, dsize);
        delete(member_deque);
        member_deque = NULL;
        END_TX;
        CHECK("copy operator= (1)", 4, dsize, data, id, {3, 4, 5, 6, -2});
    }

    // test #2 is operator= move
    global_barrier->arrive(id);
    {
        RESET_LOCAL(-2, data, dsize);
        std::deque<int> local = { 9, 8, 7, 6 };
        BEGIN_TX;
        member_deque = new std::deque<int>();
        *member_deque = std::move(local);
        COPY_DEQUE(member_deque, data, dsize);
        delete(member_deque);
        member_deque = NULL;
        END_TX;
        CHECK("move operator= (2)", 4, dsize, data, id, {9, 8, 7, 6, -2});
    }

    // test #3 is operator= ilist
    global_barrier->arrive(id);
    {
        RESET_LOCAL(-2, data, dsize);
        BEGIN_TX;
        member_deque = new std::deque<int>();
        *member_deque = { 13, 14, 15, 16 };
        COPY_DEQUE(member_deque, data, dsize);
        delete(member_deque);
        member_deque = NULL;
        END_TX;
        CHECK("ilist operator= (3)", 4, dsize, data, id, {13, 14, 15, 16, -2});
    }
}
