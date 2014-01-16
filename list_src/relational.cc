#include <iostream>
#include <string>
#include <list>
#include "concurrent_tests.h"

/// The list we will use for our tests
std::list<int>* rel_list = NULL;

void relational_test_concurrent(int id)
{
    // helper message
    if (id == 0) {
        std::cout << std::endl
                  << "Testing relational operators" << std::endl;
    }

    // test all relational operators in a single transaction
    global_barrier->arrive(id);
    bool t1, t2, t3, t4, t5, t6;
    BEGIN_TX;
    rel_list = new std::list<int>({1, 2, 3});
    std::list<int> l2({1, 2, 3});
    std::list<int> l3({2, 3, 4});
    t1 = *rel_list == l2;
    t2 = l2 != l3;
    t3 = l2 < l3;
    t4 = l3 > l2;
    t5 = *rel_list >= l2;
    t6 = *rel_list <= l2;
    delete rel_list;
    END_TX;
    if (!(t1 && t2 && t3 && t4 && t5 && t6))
        std::cout << "["<<id<<"] error on relational tests" << std::endl;
    else if (id == 0)
        std::cout << " [OK] relational tests" << std::endl;

    global_barrier->arrive(id);
}
