#include <iostream>
#include <string>
#include <list>
#include <cassert>
#include "tests.h"

/// The list we will use for our tests
std::list<int>* mod_list = NULL;

/// clone the list to a local array represented by dsize, data[]
#define COPY_LIST                                 \
    dsize = 0;                                    \
    for (auto i : *mod_list)                     \
        data[dsize++] = i

/// destroy the list and set the pointer to NULL
#define CLEAR_LIST                                \
    delete mod_list;                             \
    mod_list = NULL

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

void modifier_test(int id)
{
    // helper message
    if (id == 0) {
        std::cout << std::endl
                  << "Testing modifiers" << std::endl;
        std::cout << "Note: non-trivial constructors are not yet tested" << std::endl;
    }

    // scratch space for holding data when computing whether the test
    // succeeded or failed.
    int data[32], dsize = 0;

    // test range assignment (1)
    global_barrier->arrive(id);
    BEGIN_TX;
    // create a new list with 9 elements, so we can be sure the elements get
    // deleted
    mod_list = new std::list<int>(9);
    // create another list, that we can use for assigning into the first list
    std::list<int> l = {1, 2, 3, 4, 5};
    auto i = l.begin();
    i++;i++;i++;
    mod_list->assign(l.begin(), i);
    COPY_LIST;
    CLEAR_LIST;
    END_TX;
    VERIFY("range assignment (1)", 3, {1, 2, 3});

    // test fill assignment (2)
    global_barrier->arrive(id);
    BEGIN_TX;
    mod_list = new std::list<int>(4, 9);
    mod_list->assign(8, 32);
    COPY_LIST;
    CLEAR_LIST;
    END_TX;
    VERIFY("fill assignment (2)", 8, {32, 32, 32, 32, 32, 32, 32, 32});

    // test initializer list assignment (3)
    global_barrier->arrive(id);
    BEGIN_TX;
    mod_list = new std::list<int>(4, 9);
    mod_list->assign({8, 7, 6, 5, 4, 3, 2, 1});
    COPY_LIST;
    CLEAR_LIST;
    END_TX;
    VERIFY("initializer list assignment (3)", 8, {8, 7, 6, 5, 4, 3, 2, 1});

    // test emplace_front (1) and emplace_back (1)
    global_barrier->arrive(id);
    BEGIN_TX;
    mod_list = new std::list<int>(4, 9);
    mod_list->assign({8, 7, 6, 5, 4, 3, 2, 1});
    mod_list->emplace_front(7);
    mod_list->emplace_back(9);
    COPY_LIST;
    CLEAR_LIST;
    END_TX;
    VERIFY("emplace front (1) and emplace back (1)", 10, {7, 8, 7, 6, 5, 4, 3, 2, 1, 9});

    // test push_front with const value type (1)
    global_barrier->arrive(id);
    BEGIN_TX;
    mod_list = new std::list<int>({1, 2, 3});
    const int x6 = 6;
    mod_list->push_front(x6);
    COPY_LIST;
    CLEAR_LIST;
    END_TX;
    VERIFY("push_front with const value type (1)", 4, {6, 1, 2, 3});

    // test push_front with move (2)
    global_barrier->arrive(id);
    BEGIN_TX;
    mod_list = new std::list<int>({6, 1, 2, 3});
    int x = 7;
    mod_list->push_front(std::move(x));
    COPY_LIST;
    CLEAR_LIST;
    END_TX;
    VERIFY("push_front with move (2)", 5, {7, 6, 1, 2, 3});

    // test push_back with const value type (1) and with move (2)
    global_barrier->arrive(id);
    BEGIN_TX;
    mod_list = new std::list<int>({7, 6, 1, 2, 3});
    int x = 9;
    const int x99 = 99;
    mod_list->push_back(x99);
    mod_list->push_back(std::move(x));
    COPY_LIST;
    CLEAR_LIST;
    END_TX;
    VERIFY("push_back with const value type (1) and with move (2)", 7, {7, 6, 1, 2, 3, 99, 9});

    // test pop_front and pop_back
    global_barrier->arrive(id);
    BEGIN_TX;
    mod_list = new std::list<int>({7, 6, 1, 2, 3, 99, 9});
    mod_list->pop_front();
    mod_list->pop_back();
    COPY_LIST;
    CLEAR_LIST;
    END_TX;
    VERIFY("pop_front(1) and pop_back(1)", 5, {6, 1, 2, 3, 99});

    // test emplace (1)
    global_barrier->arrive(id);
    BEGIN_TX;
    mod_list = new std::list<int>({1, 2, 3, 4, 5});
    auto i = mod_list->begin(); i++;
    mod_list->emplace(i, 8);
    COPY_LIST;
    CLEAR_LIST;
    END_TX;
    VERIFY("emplace (1)", 6, {1, 8, 2, 3, 4, 5});

    // test insert of single element (1)
    global_barrier->arrive(id);
    BEGIN_TX;
    mod_list = new std::list<int>({1, 8, 2, 3, 4, 5});
    auto i = mod_list->begin();i++;i++;
    const int x9 = 9;
    mod_list->insert(i, x9);
    COPY_LIST;
    CLEAR_LIST;
    END_TX;
    VERIFY("single element insert (1)", 7, {1, 8, 9, 2, 3, 4, 5});

    // test fill insert (2)
    global_barrier->arrive(id);
    BEGIN_TX;
    mod_list = new std::list<int>({1, 8, 9, 2, 3, 4, 5});
    auto i = mod_list->begin();i++;i++;i++;
    mod_list->insert(i, 3, 18);
    COPY_LIST;
    CLEAR_LIST;
    END_TX;
    VERIFY("fill insert (2)", 10, {1, 8, 9, 18, 18, 18, 2, 3, 4, 5});

    // test range insert (3)
    global_barrier->arrive(id);
    BEGIN_TX;
    mod_list = new std::list<int>({1, 8, 9, 18, 18, 18, 2, 3, 4, 5});
    auto i = mod_list->begin();i++;i++;i++;i++;i++;i++;
    std::list<int> q({99, 98, 97});
    mod_list->insert(i, q.begin(), q.end());
    COPY_LIST;
    CLEAR_LIST;
    END_TX;
    VERIFY("range insert (3)", 13, {1, 8, 9, 18, 18, 18, 99, 98, 97, 2, 3, 4, 5});

    // test insert with move (4)
    global_barrier->arrive(id);
    BEGIN_TX;
    mod_list = new std::list<int>({1, 8, 9, 18, 18, 18, 99, 98, 97, 2, 3, 4, 5});
    auto i = mod_list->begin();i++;i++;
    int zzz = 7;
    mod_list->insert(i, std::move(zzz));
    COPY_LIST;
    CLEAR_LIST;
    END_TX;
    VERIFY("insert with move (4)", 14, {1, 8, 7, 9, 18, 18, 18, 99, 98, 97, 2, 3, 4, 5});

    // test insert with initializer list (5)
    global_barrier->arrive(id);
    BEGIN_TX;
    mod_list = new std::list<int>({1, 8, 9, 18, 16});
    auto i = mod_list->begin();i++;i++;
    mod_list->insert(i, {14, 15, 16});
    COPY_LIST;
    CLEAR_LIST;
    END_TX;
    VERIFY("insert with initializer list (5)", 8, {1, 8, 14, 15, 16, 9, 18, 16});

    // test single element erase (1)
    global_barrier->arrive(id);
    BEGIN_TX;
    mod_list = new std::list<int>({1, 8, 9, 18, 16});
    auto i = mod_list->begin();i++;i++;
    mod_list->erase(i);
    COPY_LIST;
    CLEAR_LIST;
    END_TX;
    VERIFY("erase single element (1)", 4, {1, 8, 18, 16});

    // test range erase (2)
    global_barrier->arrive(id);
    BEGIN_TX;
    mod_list = new std::list<int>({1, 8, 9, 18, 16});
    auto i = mod_list->begin();i++;i++;
    auto j = mod_list->end();j--;
    mod_list->erase(i, j);
    COPY_LIST;
    CLEAR_LIST;
    END_TX;
    VERIFY("erase range (2)", 3, {1, 8, 16});

    // test swap (1)
    global_barrier->arrive(id);
    int tmp = 0;
    BEGIN_TX;
    mod_list = new std::list<int>({1, 3, 4});
    std::list<int> swapper = {9, 7, 3, 5, 2};
    mod_list->swap(swapper);
    for (auto i = swapper.begin(); i != swapper.end(); ++i)
        tmp = tmp * 10 + *i;
    COPY_LIST;
    CLEAR_LIST;
    END_TX;
    if (tmp != 134)
        std::cout << "["<<id<<"] error in swap()" << std::endl;
    else {
        VERIFY("swap (1)", 5, {9, 7, 3, 5, 2});
    }

    // test resize without default (1)
    global_barrier->arrive(id);
    BEGIN_TX;
    mod_list = new std::list<int>({1, 2, 3, 4, 5});
    mod_list->resize(10);
    COPY_LIST;
    CLEAR_LIST;
    END_TX;
    VERIFY("resize (1)", 10, {1, 2, 3, 4, 5, 0, 0, 0, 0, 0});

    // do it again, but shrink this time
    global_barrier->arrive(id);
    BEGIN_TX;
    mod_list = new std::list<int>({1, 2, 3, 4, 5, 9, 8, 7});
    mod_list->resize(5);
    COPY_LIST;
    CLEAR_LIST;
    END_TX;
    VERIFY("resize (2)", 5, {1, 2, 3, 4, 5});

    // test resize with default (2)
    global_barrier->arrive(id);
    BEGIN_TX;
    mod_list = new std::list<int>({1, 2, 3, 4, 5});
    mod_list->resize(8, 2);
    COPY_LIST;
    CLEAR_LIST;
    END_TX;
    VERIFY("resize (3)", 8, {1, 2, 3, 4, 5, 2, 2, 2});

    // do it again, with shrink
    global_barrier->arrive(id);
    BEGIN_TX;
    mod_list = new std::list<int>({1, 2, 3, 4, 5, 9, 3});
    mod_list->resize(5, 2);
    COPY_LIST;
    CLEAR_LIST;
    END_TX;
    VERIFY("resize (4)", 5, {1, 2, 3, 4, 5});

    // test clear (1)
    global_barrier->arrive(id);
    BEGIN_TX;
    mod_list = new std::list<int>({1, 2, 3, 4, 5, 9, 3});
    mod_list->clear();
    mod_list->push_front(1);
    COPY_LIST;
    CLEAR_LIST;
    END_TX;
    VERIFY("clear (1)", 1, {1});

    global_barrier->arrive(id);
}
