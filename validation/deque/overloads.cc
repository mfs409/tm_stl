#include <iostream>
#include <deque>
#include "tests.h"
#include "verify.h"

/// The deques we will use for our tests
std::deque<int>* overload_deque = NULL;

void relational_operator_tests(int id)
{
    global_barrier->arrive(id);
    if (id == 0)
        printf("Testing deque relational operators ==, !=, <, >, <=, >=\n");

    // Test all operators in a single transaction
    global_barrier->arrive(id);
    {
        bool t1, t2, t3, t4, t5, t6;
        BEGIN_TX;
        overload_deque = new std::deque<int>({1, 2, 3});
        std::deque<int> l2({1, 2, 3});
        std::deque<int> l3({2, 3, 4});
        t1 = *overload_deque == l2;
        t2 = l2 != l3;
        t3 = l2 < l3;
        t4 = l3 > l2;
        t5 = *overload_deque >= l2;
        t6 = *overload_deque <= l2;
        delete overload_deque;
        END_TX;
        if (!(t1 && t2 && t3 && t4 && t5 && t6))
            std::cout << "["<<id<<"] error on relational tests" << std::endl;
        else if (id == 0)
            std::cout << " [OK] relational tests" << std::endl;
    }
}

void swap_tests(int id)
{
    global_barrier->arrive(id);
    if (id == 0)
        printf("Testing deque swap\n");

    // Test swap
    global_barrier->arrive(id);
    {
        verifier v;
        BEGIN_TX;
        overload_deque = new std::deque<int>({1, 2, 3, 4, 5});
        std::deque<int> d2({7, 8, 9});
        swap(*overload_deque, d2);
        v.insert_all(overload_deque);
        delete(overload_deque);
        END_TX;
        v.check("std::swap (1)", id, 3, {7, 8, 9});
    }
}
