#include <iostream>
#include <string>
#include <list>
#include "sequential_tests.h"

/**
 * Test the many modifier methods of std::list
 */
void modifier_test_seq()
{
    std::cout << "Testing modifiers" << std::endl;
    std::cout << "Note: non-trivial constructors are not yet tested" << std::endl;

    // create a new list with 9 elements, so we can be sure the elements get
    // deleted
    my_list = new std::list<int>(9);

    // create another list, that we can use for assigning into the first list
    std::list<int> l = {1, 2, 3, 4, 5};

    // test range assignment (1)
    auto i = l.begin();
    i++;i++;i++;
    my_list->assign(l.begin(), i);
    check("Expect 1, 2, 3");

    // test fill assignment (2)
    my_list->assign(8, 32);
    check("Expect 32, 32, 32, 32, 32, 32, 32, 32");

    // test initializer list assignment (3)
    my_list->assign({8, 7, 6, 5, 4, 3, 2, 1});
    check("Expect 8, 7, 6, 5, 4, 3, 2, 1");

    // test emplace_front (1) and emplace_back (1)
    my_list->emplace_front(7);
    my_list->emplace_back(9);
    check("Expect 7, 8, 7, 6, 5, 4, 3, 2, 1, 9");

    // test push_front with const value type (1)
    delete my_list;
    my_list = new std::list<int>({1, 2, 3});
    const int x6 = 6;
    my_list->push_front(x6);
    check("Expect 6, 1, 2, 3");

    // test push_front with move (2)
    int x = 7;
    my_list->push_front(std::move(x));
    check("Expect 7, 6, 1, 2, 3");

    // test push_back with const value type (1) and with move (2)
    x = 9;
    const int x99 = 99;
    my_list->push_back(x99);
    my_list->push_back(std::move(x));
    check("Expect 7, 6, 1, 2, 3, 99, 9");

    // test pop_front and pop_back
    my_list->pop_front();
    my_list->pop_back();
    check("Expect 6, 1, 2, 3, 99");
    delete my_list;

    // test emplace (1)
    my_list = new std::list<int>({1, 2, 3, 4, 5});
    i = my_list->begin(); i++;
    my_list->emplace(i, 8);
    check("Expect 1, 8, 2, 3, 4, 5");

    // test insert of single element (1)
    const int x9 = 9;
    my_list->insert(i, x9);
    check("Expect 1, 8, 9, 2, 3, 4, 5");

    // test fill insert (2)
    my_list->insert(i, 3, 18);
    check("Expect 1, 8, 9, 18, 18, 18, 2, 3, 4, 5");

    // test range insert (3)
    std::list<int> q({99, 98, 97});
    my_list->insert(i, q.begin(), q.end());
    check("Expect 1, 8, 9, 18, 18, 18, 99, 98, 97, 2, 3, 4, 5");

    // test insert with move (4)
    int zzz = 9;
    my_list->insert(i, std::move(zzz));
    check("Expect 1, 8, 9, 18, 18, 18, 99, 98, 97, 9, 2, 3, 4, 5");

    // test insert with initializer list (5)
    my_list->insert(i, {14, 15, 16});
    check("Expect 1, 8, 9, 18, 18, 18, 99, 98, 97, 9, 14, 15, 16, 2, 3, 4, 5");

    // test single element erase (1)
    my_list->erase(i);
    check("Expect 1, 8, 9, 18, 18, 18, 99, 98, 97, 9, 14, 15, 16, 3, 4, 5");

    // test range erase (2)
    i = my_list->begin();
    i++;i++;i++;i++;
    my_list->erase(my_list->begin(), i);
    check("Expect 18, 18, 99, 98, 97, 9, 14, 15, 16, 3, 4, 5");

    // test swap (1)
    std::list<int> swapper = {1, 2, 3, 4, 5};
    my_list->swap(swapper);
    swapper.clear();
    check("Expect 1, 2, 3, 4, 5");

    // test resize without default (1)
    my_list->resize(10);
    check("Expect 1, 2, 3, 4, 5, 0, 0, 0, 0, 0");
    my_list->resize(5);
    check("Expect 1, 2, 3, 4, 5");
    // test resize with default (2)
    my_list->resize(8, 2);
    check("Expect 1, 2, 3, 4, 5, 2, 2, 2");
    my_list->resize(5, 2);
    check("Expect 1, 2, 3, 4, 5");

    // test clear (1)
    my_list->clear();
    my_list->push_front(1);
    check("Expect 1");
    delete my_list;
}

void modifier_test_concurrent(int id)
{
    // helper message
    if (id == 0) {
        std::cout << std::endl
                  << "Testing modifiers [TODO]" << std::endl;
    }
}
