#include <iostream>
#include <string>
#include <list>
#include "sequential_tests.h"

/**
 * Here is the test of the swap operation from forward_list
 */
void swap_test_seq()
{
    std::cout << "Testing swap" << std::endl;

    my_list = new std::list<int>({1, 2, 3, 4, 5});
    check("Expect 1, 2, 3, 4, 5");

    std::list<int> l2({7, 8, 9});
    swap(*my_list, l2);
    check("Expect 7, 8, 9");
    delete my_list;

    // and just for fun, let's test range iterators
    int vals[5];
    my_list = new std::list<int>({1, 2, 3, 4, 5});
    int i = 0;
    for (auto x : *my_list)
        vals[i++] = x;

    bool ok = true;
    for (i = 0; i < 5; ++i) {
        ok &= (vals[i] == i+1);
    }
    std::cout << "Range check " << (ok?"passed":"failed") << std::endl;
}
