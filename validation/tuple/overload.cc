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

void overload_tests(int id)
{
    std::cout << "  Overloaded operators are untraceable\n";
    std::cout << "  Get functions are untraceable\n";

    // test global swap
    global_barrier->arrive(id);
    {
        verifier v;
        inttuple3 p(1, 2, 3);
        BEGIN_TX;
        member_tuple = new inttuple3();
        swap(p, *member_tuple);
        v.insert_all<inttuple3>(member_tuple);
        delete(member_tuple);
        member_tuple = NULL;
        END_TX;
        v.check("global tuple swap (1)", id, 3, {1, 2, 3});
    }
}
