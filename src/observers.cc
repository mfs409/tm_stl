#include <iostream>
#include <string>
#include <list>
#include "sequential_tests.h"

/**
 *  Test std::list's observers
 */
void observers_test_seq()
{
    std::cout << "Testing observers" << std::endl;

    // test get_allocator (1).  Note that we need to use it so that it
    // doesn't get optimized away.

    my_list = new std::list<int>();

    std::list<int>::allocator_type a;
    a = my_list->get_allocator();

    int* p = a.allocate(3);
    for (int i = 0; i < 3; ++i)
        p[i] = i;
    my_list->assign(p, p+3);
    a.deallocate(p, 3);
    check("Expect 0, 1, 2");
    delete my_list;
}
