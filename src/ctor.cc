#include <iostream>
#include <string>
#include <list>
#include "tests.h"

/**
 * Here is the sequential ctor test.  It also happens to be the sequential
 * dtor test, since there is only a basic dtor.
 */
void ctor_test()
{
    std::cout << "Testing ctor and dtor" << std::endl;
    std::cout << "Note: we aren't testing the allocator parameter "
              << "of the constructors" << std::endl;

    // test basic constructor (1)
    my_list = new std::list<int>();
    check("Expect an empty list");
    delete my_list;

    // test sized constructor (2)
    my_list = new std::list<int>(5);
    check("Expect 5 default entries");
    delete my_list;

    // test sized constructor with defaults (2)
    my_list = new std::list<int>(5, 1);
    check("Expect 5 ones");
    delete my_list;

    // test range constructor (3)
    int a[] = {1, 2, 3};
    my_list = new std::list<int>(a, a+3);
    check("Expect 1,2,3");

    // test the copy constructor (4)
    std::list<int>* old = my_list;
    my_list = new std::list<int>(*old);
    delete old;
    check("Expect 1,2,3");

    // test the move constructor (5)
    old = my_list;
    my_list = new std::list<int>(std::move(*old));
    std::list<int>* swapper = my_list;
    my_list = old;
    check("Expect empty");
    my_list = swapper;
    check("Expect 1, 2, 3");
    delete old;
    delete swapper;

    // test the initializer list constructor (6)
    my_list = new std::list<int>({1, 2, 3, 4, 5, 6});
    check("Expect 1, 2, 3, 4, 5, 6");
    delete my_list;
}

/**
 *  The list that all our sequential tests use
 */
std::list<int>* my_list;

/**
 *  A helper to print our list when visually checking for correctness
 */
void check(std::string s)
{
    std::cout << s << std::endl << " List: ";
    for (auto i : *my_list)
        std::cout << i << ", ";
    std::cout << std::endl;
}
