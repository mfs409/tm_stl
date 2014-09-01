#include <iostream>
#include <string>
#include <utility>
#include <cassert>
#include "tests.h"
#include "verify.h"

/// The pair we will use for our tests
typedef std::pair<int, int> intpair;
static intpair * member_pair = NULL;

void ctor_dtor_tests(int id)
{
    // print simple output
    global_barrier->arrive(id);
    if (id == 0)
        printf("Testing member pair constructors and destructors\n");

    // the first test is simple ctor and dtor
    std::cout << "  No constructor is untraceable\n";
}

void op_eq_tests(int id)
{
    // test operator= (copy)
    global_barrier->arrive(id);
    {
        verifier v;
        const intpair p(1, 2);
        BEGIN_TX;
        member_pair = new intpair();
        *member_pair = p;
        v.insert_all<intpair>(member_pair);
        delete(member_pair);
        member_pair = NULL;
        END_TX;
        v.check("pair operator= (1a)", id, 2, {1, 2});
    }

    // test operator= (copy)
    global_barrier->arrive(id);
    {
        verifier v;
        const std::pair<char, char> p(1, 2);
        BEGIN_TX;
        member_pair = new intpair();
        *member_pair = p;
        v.insert_all<intpair>(member_pair);
        delete(member_pair);
        member_pair = NULL;
        END_TX;
        v.check("pair operator= (1b)", id, 2, {1, 2});
    }

    // test operator= (move)
    global_barrier->arrive(id);
    {
        verifier v;
        intpair p(1, 2);
        BEGIN_TX;
        member_pair = new intpair();
        *member_pair = std::make_pair(1, 2);
        v.insert_all<intpair>(member_pair);
        delete(member_pair);
        member_pair = NULL;
        END_TX;
        v.check("pair operator= (2a)", id, 2, {1, 2});
    }

    // test operator= (move)
    global_barrier->arrive(id);
    {
        verifier v;
        intpair p(1, 2);
        BEGIN_TX;
        member_pair = new intpair();
        *member_pair = std::make_pair<char, char>(1, 2);
        v.insert_all<intpair>(member_pair);
        delete(member_pair);
        member_pair = NULL;
        END_TX;
        v.check("pair operator= (2b)", id, 2, {1, 2});
    }
}

void swap_tests(int id)
{
    // test swap
    global_barrier->arrive(id);
    {
        verifier v;
        intpair p(1, 2);
        BEGIN_TX;
        member_pair = new intpair(3, 4);
        member_pair->swap(p);
        v.insert_all<intpair>(member_pair);
        delete(member_pair);
        member_pair = NULL;
        END_TX;
        v.check("pair swap", id, 2, {1, 2});
    }
}
