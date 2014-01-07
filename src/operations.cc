#include <iostream>
#include <string>
#include <list>
#include <cassert>
#include "sequential_tests.h"
#include "concurrent_tests.h"

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
void operations_test_seq()
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

/// The list we will use for our tests
std::list<int>* ops_list = NULL;

/// clone the list to a local array represented by dsize, data[]
#define COPY_LIST                                 \
    dsize = 0;                                    \
    for (auto i : *ops_list)                     \
        data[dsize++] = i

/// destroy the list and set the pointer to NULL
#define CLEAR_LIST                                \
    delete ops_list;                             \
    ops_list = NULL

/// compare the local array to our expected results
#define VERIFY(testName, size, ...)                                     \
    if (dsize != size)                                                  \
        printf(" [%d] size did not match %d != %d\n", id, dsize, size);  \
    else if (size > 0) {                                                \
        int counter = 0;                                                \
        bool good = true;                                               \
        for (auto i : __VA_ARGS__)                                      \
            good &= (data[counter++] == i);                             \
        if (!good)                                                      \
            std::cout << " ["<<id<<"] Arrays did not match for thread " << id << std::endl; \
        else if (id == 0)                                               \
            std::cout << " [OK] " << testName << std::endl;              \
    }                                                                   \
    else if (id == 0)                                                   \
            std::cout << " [OK] " << testName << std::endl

void operations_test_concurrent(int id)
{
    // helper message
    if (id == 0) {
        std::cout << std::endl
                  << "Testing operations" << std::endl;
    }

    // scratch space for holding data when computing whether the test
    // succeeded or failed.
    int data[32], dsize = 0;

    // test splice all elements (1)
    global_barrier->arrive(id);
    bool check = false;
    BEGIN_TX;
    ops_list = new std::list<int>({1, 2, 5});
    std::list<int> q = {3, 4};
    auto i = ops_list->begin(); i++; i++;
    ops_list->splice(i, q);
    check = q.empty();
    COPY_LIST;
    CLEAR_LIST;
    END_TX;
    if (check == false)
        std::cout << "["<<id<<"] empty check failed" << std::endl;
    else {
        VERIFY("splice (1)", 5, {1, 2, 3, 4, 5});
    }

    // test splice all elements w/ move (2)
    global_barrier->arrive(id);
    BEGIN_TX;
    ops_list = new std::list<int>({1, 2, 5});
    std::list<int> q = {9, 8};
    auto i = ops_list->begin(); i++; i++;
    ops_list->splice(i, std::move(q));
    check = q.empty();
    COPY_LIST;
    CLEAR_LIST;
    END_TX;
    if (check == false)
        std::cout << "["<<id<<"] empty check failed" << std::endl;
    else {
        VERIFY("splice with move (2)", 5, {1, 2, 9, 8, 5});
    }

    // test splice one element (3)
    global_barrier->arrive(id);
    BEGIN_TX;
    ops_list = new std::list<int>({1, 2, 5});
    std::list<int> q = {2, 3};
    ops_list->splice(ops_list->begin(), q, q.begin());
    COPY_LIST;
    CLEAR_LIST;
    END_TX;
    VERIFY("splice one element (3)", 4, {2, 1, 2, 5});

    // test splice one element w/ move (4)
    global_barrier->arrive(id);
    BEGIN_TX;
    ops_list = new std::list<int>({1, 2, 5});
    std::list<int> q = {2, 3};
    ops_list->splice(ops_list->begin(), std::move(q), ++q.begin());
    COPY_LIST;
    CLEAR_LIST;
    END_TX;
    VERIFY("splice one element with move (4)", 4, {3, 1, 2, 5});

    // test splice range (5)
    global_barrier->arrive(id);
    BEGIN_TX;
    ops_list = new std::list<int>({1, 2, 5});
    std::list<int> q = {6, 7, 8, 9, 10};
    ops_list->splice(ops_list->begin(), q, q.begin(), ++(++(++q.begin())));
    COPY_LIST;
    CLEAR_LIST;
    END_TX;
    VERIFY("splice range (5)", 6, {6, 7, 8, 1, 2, 5});

    // test splice range w/ move (6)
    global_barrier->arrive(id);
    BEGIN_TX;
    ops_list = new std::list<int>({1, 2, 5});
    std::list<int> q = {11, 12, 13, 14, 15, 16, 17};
    ops_list->splice(ops_list->begin(), std::move(q), q.begin(), ++(++(++q.begin())));
    COPY_LIST;
    CLEAR_LIST;
    END_TX;
    VERIFY("splice range with move (6)", 6, {11, 12, 13, 1, 2, 5});

    // test remove (1)
    global_barrier->arrive(id);
    BEGIN_TX;
    ops_list = new std::list<int>({1, 2, 5, 2, 9});
    ops_list->remove(2);
    COPY_LIST;
    CLEAR_LIST;
    END_TX;
    VERIFY("remove", 3, {1, 5, 9});

    // test remove_if with function (1)
    global_barrier->arrive(id);
    BEGIN_TX;
    ops_list = new std::list<int>({1, 2, 3, 4, 5, 6, 7, 8, 9});
    ops_list->remove_if([](const int& value)->bool{return value%3==0;});
    COPY_LIST;
    CLEAR_LIST;
    END_TX;
    VERIFY("remove_if with function (1)", 6, {1, 2, 4, 5, 7, 8});

    // test remove_if with function object (2)
    global_barrier->arrive(id);
    BEGIN_TX;
    ops_list = new std::list<int>({1, 2, 4, 5, 7, 8});
    ops_list->remove_if(is_even());
    COPY_LIST;
    CLEAR_LIST;
    END_TX;
    VERIFY("remove_if with function object (2)", 3, {1, 5, 7});

    // test unique (1)
    global_barrier->arrive(id);
    BEGIN_TX;
    ops_list = new std::list<int>({1, 1, 2, 3, 4, 4, 5, 6});
    ops_list->unique();
    COPY_LIST;
    CLEAR_LIST;
    END_TX;
    VERIFY("unique (1)", 6, {1, 2, 3, 4, 5, 6});

    // test unique with function (2)
    global_barrier->arrive(id);
    BEGIN_TX;
    ops_list = new std::list<int>({2, 4, 3, 9});
    ops_list->unique([](int first, int second)->bool{return second == (first*first);});
    COPY_LIST;
    CLEAR_LIST;
    END_TX;
    VERIFY("unique with function (2)", 2, {2, 3});

    // test unique with functor(3)
    global_barrier->arrive(id);
    BEGIN_TX;
    ops_list = new std::list<int>({2, 4, 3, 9});
    ops_list->unique(is_even());
    COPY_LIST;
    CLEAR_LIST;
    END_TX;
    VERIFY("unique with function object", 2, {2, 3});

    // test merge (1)
    global_barrier->arrive(id);
    BEGIN_TX;
    ops_list = new std::list<int>({2, 4, 8});
    std::list<int>*tmp = new std::list<int>({1, 6, 7, 19});
    ops_list->merge(*tmp);
    COPY_LIST;
    CLEAR_LIST;
    delete tmp;
    END_TX;
    VERIFY("merge (1)", 7, {1, 2, 4, 6, 7, 8, 19});

    // test merge w/ move (2)
    global_barrier->arrive(id);
    BEGIN_TX;
    ops_list = new std::list<int>({2, 4, 8});
    std::list<int>tmp({1, 6, 7, 19});
    ops_list->merge(std::move(tmp));
    COPY_LIST;
    CLEAR_LIST;
    END_TX;
    VERIFY("merge with move (2)", 7, {1, 2, 4, 6, 7, 8, 19});

    // test merge w/ comparator (3)
    global_barrier->arrive(id);
    BEGIN_TX;
    ops_list = new std::list<int>({16, 17, 18, 26, 27, 28});
    std::list<int>*tmp = new std::list<int>({11, 12, 13, 21, 22, 23});
    ops_list->merge(*tmp, [](int a, int b)->bool{return a/10 < b/10;});
    COPY_LIST;
    CLEAR_LIST;
    delete tmp;
    END_TX;
    VERIFY("merge with comparator (3)", 12, {16, 17, 18, 11, 12, 13, 26, 27, 28, 21, 22, 23});

    // test merge w/ move w/ comparator (4)
    global_barrier->arrive(id);
    BEGIN_TX;
    ops_list = new std::list<int>({16, 17, 18, 26, 27, 28});
    std::list<int>tmp({11, 12, 13, 21, 22, 23});
    ops_list->merge(std::move(tmp), [](int a, int b)->bool{return a/10 < b/10;});
    COPY_LIST;
    CLEAR_LIST;
    END_TX;
    VERIFY("merge with comparator with move (4)", 12, {16, 17, 18, 11, 12, 13, 26, 27, 28, 21, 22, 23});

    // test sort (1)
    global_barrier->arrive(id);
    BEGIN_TX;
    ops_list = new std::list<int>({8, 4, 2, 8, 6, 1});
    ops_list->sort();
    COPY_LIST;
    CLEAR_LIST;
    END_TX;
    VERIFY("sort (1)", 6, {1, 2, 4, 6, 8, 8});

    // test sort w/ comparator (2)
    global_barrier->arrive(id);
    BEGIN_TX;
    ops_list = new std::list<int>({16, 28, 17, 27, 18, 26});
    ops_list->sort([](int a, int b)->bool{return a/10 < b/10;});
    COPY_LIST;
    CLEAR_LIST;
    END_TX;
    VERIFY("sort with comparator (2)", 6, {16, 17, 18, 28, 27, 26});

    // test reverse (1)
    global_barrier->arrive(id);
    BEGIN_TX;
    ops_list = new std::list<int>({1, 2, 3, 4, 5, 6});
    ops_list->reverse();
    COPY_LIST;
    CLEAR_LIST;
    END_TX;
    VERIFY("reverse (1)", 6, {6, 5, 4, 3, 2, 1});

    global_barrier->arrive(id);
}
