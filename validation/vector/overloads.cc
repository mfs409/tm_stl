#include <iostream>
#include <vector>
#include "tests.h"
#include "verify.h"

/// The vector we will use for our tests
std::vector<int>* overload_vec = NULL;

void relational_operator_tests(int id)
{
     global_barrier->arrive(id);
    if (id == 0)
        printf("Testing vector relational operators ==, !=, <, >, <=, >=\n");

    // Test all operators in a single transaction
    global_barrier->arrive(id);
    {
        bool t1, t2, t3, t4, t5, t6;
        BEGIN_TX;
        overload_vec = new std::vector<int>({1, 2, 3});
        std::vector<int> l2({1, 2, 3});
        std::vector<int> l3({2, 3, 4});
        t1 = *overload_vec == l2;
        t2 = l2 != l3;
        t3 = l2 < l3;
        t4 = l3 > l2;
        t5 = *overload_vec >= l2;
        t6 = *overload_vec <= l2;
        delete overload_vec;
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
        printf("Testing vector swap\n");

    // Test swap
    global_barrier->arrive(id);
    {
        verifier v;
        BEGIN_TX;
        overload_vec = new std::vector<int>({1, 2, 3, 4, 5});
        std::vector<int> d2({7, 8, 9});
        swap(*overload_vec, d2);
        v.insert_all(overload_vec);
        delete(overload_vec);
        END_TX;
        v.check("std::swap (1)", id, 3, {7, 8, 9});
    }
}

