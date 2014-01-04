#include <iostream>
#include <string>
#include <list>
#include "sequential_tests.h"

/**
 * Test std::list capacity methods
 */
void cap_test_seq()
{
    std::cout << "Testing capacity methods" << std::endl;

    // use the initializer list constructor
    my_list = new std::list<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});

    // test empty() (1)
    bool e = my_list->empty();
    delete my_list;
    my_list = new std::list<int>();
    bool e2 = my_list->empty();
    std::cout << "Expect 0, 1: " << e << ", " << e2 << std::endl;

    // test size() (2)
    std::list<int>::size_type s = my_list->size();
    delete my_list;
    my_list = new std::list<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    std::list<int>::size_type s2 = my_list->size();
    std::cout << "Expect 0, 10: " << s << ", " << s2 << std::endl;

    // test max_size() (3)
    s = my_list->max_size();
    delete my_list;
    my_list = new std::list<int>();
    s2 = my_list->max_size();
    std::cout << "Expect matching large value: " << s << ", " << s2 << std::endl;
}
