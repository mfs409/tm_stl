#include <iostream>
#include <string>
#include <list>
#include <cassert>
#include "tests.h"

/**
 * A functor for remove_if
 */
struct is_even
{
    bool operator() (const int& val) { return (val&1) == 0; }
    bool operator() (int first, int second) { return (first*first) == second; }
};

/**
 * Test the many modifier methods of std::list
 */
void operations_test()
{
    std::cout << "Testing operations" << std::endl;
    // test splice all elements (1)
    my_list = new std::list<int>({1, 2, 5});
    std::list<int> q = {3, 4};
    auto s1i = my_list->begin(); s1i++; s1i++;
    my_list->splice(s1i, q);
    check("Expect 1, 2, 3, 4, 5");
    assert(q.empty());

    // test splice all elements w/ move (2)
    q = {9, 8};
    my_list->splice(s1i, std::move(q));
    check("Expect 1, 2, 3, 4, 9, 8, 5");
    assert(q.empty());

    // test splice one element (3)
    q = {2, 3};
    my_list->splice(my_list->begin(), q, q.begin());
    check("Expect 2, 1, 2, 3, 4, 9, 8, 5");

    // test splice one element w/ move (4)
    q = {2, 3};
    my_list->splice(my_list->begin(), std::move(q), ++q.begin());
    check("Expect 3, 2, 1, 2, 3, 4, 9, 8, 5");

    // test splice range (5)
    q = {6, 7, 8, 9, 10};
    my_list->splice(my_list->begin(), q, q.begin(), ++(++(++q.begin())));
    check("Expect 6, 7, 8, 3, 2, 1, 2, 3, 4, 9, 8, 5");

    // test splice range w/ move (6)
    q = {11, 12, 13, 14, 15, 16, 17};
    my_list->splice(my_list->begin(), std::move(q), q.begin(), ++(++(++q.begin())));
    check("Expect 11, 12, 13, 6, 7, 8, 3, 2, 1, 2, 3, 4, 9, 8, 5");

    // test remove (1)
    my_list->remove(2);
    check("Expect 11, 12, 13, 6, 7, 8, 3, 1, 3, 4, 9, 8, 5");
    delete my_list;

    // test remove_if with function (1)
    my_list = new std::list<int>({1, 2, 3, 4, 5, 6, 7, 8, 9});
    my_list->remove_if([](const int& value)->bool{return value%3==0;});
    check("Expect 1, 2, 4, 5, 7, 8");

    // test remove_if with function object (2)
    my_list->remove_if(is_even());
    check("Expect 1, 5, 7");
    delete my_list;

    // test unique (1)
    my_list = new std::list<int>({1, 1, 2, 3, 4, 4, 5, 6});
    my_list->unique();
    check("Expect 1, 2, 3, 4, 5, 6");
    delete my_list;

    // test unique with function (2)
    my_list = new std::list<int>({2, 4, 3, 9});
    my_list->unique([](int first, int second)->bool{return second == (first*first);});
    check("Expect 2, 3");
    delete my_list;

    // test unique with functor(3)
    my_list = new std::list<int>({2, 4, 3, 9});
    my_list->unique(is_even());
    check("Expect 2, 3");
    delete my_list;

    // test merge (1)
    my_list = new std::list<int>({2, 4, 8});
    std::list<int>*ii = new std::list<int>({1, 6, 7, 19});
    my_list->merge(*ii);
    check("Expect 1, 2, 4, 6, 7, 8, 19");
    delete ii;
    delete my_list;

    // test merge w/ move (2)
    my_list = new std::list<int>({2, 4, 8});
    std::list<int>iii({1, 6, 7, 19});
    my_list->merge(std::move(iii));
    check("Expect 1, 2, 4, 6, 7, 8, 19");
    delete my_list;

    // test merge w/ comparator (3)
    my_list = new std::list<int>({16, 17, 18, 26, 27, 28});
    std::list<int>*zz = new std::list<int>({11, 12, 13, 21, 22, 23});
    my_list->merge(*zz, [](int a, int b)->bool{return a/10 < b/10;});
    check("Expect 16, 17, 18, 11, 12, 13, 26, 27, 28, 21, 22, 23");
    delete my_list;

    // test merge w/ move w/ comparator (4)
    my_list = new std::list<int>({16, 17, 18, 26, 27, 28});
    std::list<int>zzz({11, 12, 13, 21, 22, 23});
    my_list->merge(std::move(zzz), [](int a, int b)->bool{return a/10 < b/10;});
    check("Expect 16, 17, 18, 11, 12, 13, 26, 27, 28, 21, 22, 23");
    delete my_list;

    // test sort (1)
    my_list = new std::list<int>({8, 4, 2, 8, 6, 1});
    my_list->sort();
    check("Expect 1, 2, 4, 6, 8, 8");
    delete my_list;

    // test sort w/ comparator (2)
    my_list = new std::list<int>({16, 28, 17, 27, 18, 26});
    my_list->sort([](int a, int b)->bool{return a/10 < b/10;});
    check("Expect 16, 17, 18, 28, 27, 26");
    delete my_list;

    // test reverse (1)
    my_list = new std::list<int>({1, 2, 3, 4, 5, 6});
    my_list->reverse();
    check("Expect 6, 5, 4, 3, 2, 1");
    delete my_list;
}
