#include <iostream>
#include <string>
#include <vector>
#include "sequential_tests.h"
#include "concurrent_tests.h"

/// The vector we will use for our tests
std::vector<int>* rel_vector = NULL;

void relational_test_seq()
{
    // helper message
    std::cout << std::endl
              << "Testing relational operators" << std::endl;

    // test all relational operators in a single transaction
    bool t1, t2, t3, t4, t5, t6;
    rel_vector = new std::vector<int>({1, 2, 3});
    std::vector<int> l2({1, 2, 3});
    std::vector<int> l3({2, 3, 4});
    t1 = *rel_vector == l2;
    t2 = l2 != l3;
    t3 = l2 < l3;
    t4 = l3 > l2;
    t5 = *rel_vector >= l2;
    t6 = *rel_vector <= l2;
    delete rel_vector;
    if (!(t1 && t2 && t3 && t4 && t5 && t6))
        std::cout << "Error on relational tests" << std::endl;
    else
        std::cout << " OK relational tests" << std::endl;
}

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
    rel_vector = new std::vector<int>({1, 2, 3});
    std::vector<int> l2({1, 2, 3});
    std::vector<int> l3({2, 3, 4});
    t1 = *rel_vector == l2;
    t2 = l2 != l3;
    t3 = l2 < l3;
    t4 = l3 > l2;
    t5 = *rel_vector >= l2;
    t6 = *rel_vector <= l2;
    delete rel_vector;
    END_TX;
    if (!(t1 && t2 && t3 && t4 && t5 && t6))
        std::cout << "["<<id<<"] error on relational tests" << std::endl;
    else if (id == 0)
        std::cout << " [OK] relational tests" << std::endl;

    global_barrier->arrive(id);
}
