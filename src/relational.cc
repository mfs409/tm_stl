#include <iostream>
#include <string>
#include <list>
#include "sequential_tests.h"

/**
 * Here is the test of relational operators from forward_list.
 */
void relational_test_seq()
{
    std::cout << "Testing relational operators" << std::endl;

    my_list = new std::list<int>({1, 2, 3});
    std::list<int> l2({1, 2, 3});
    std::list<int> l3({2, 3, 4});

    bool t;

    t = *my_list == l2;
    std::cout << "Testing == ... " << (t?"OK":"FAILED") << std::endl;

    t = l2 != l3;
    std::cout << "Testing != ... " << (t?"OK":"FAILED") << std::endl;

    t = l2 < l3;
    std::cout << "Testing < ... " << (t?"OK":"FAILED") << std::endl;

    t = l3 > l2;
    std::cout << "Testing > ... " << (t?"OK":"FAILED") << std::endl;

    t = *my_list >= l2;
    std::cout << "Testing >= ... " << (t?"OK":"FAILED") << std::endl;

    t = *my_list <= l2;
    std::cout << "Testing <= ... " << (t?"OK":"FAILED") << std::endl;

    delete my_list;
}
