#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include "sequential_tests.h"
#include "concurrent_tests.h"

/// The vector we will use for our tests
std::vector<int>* mod_vector = NULL;

/// clone the vector to a local array represented by dsize, data[]
#define COPY_VECTOR                                 \
    dsize = 0;                                    \
    for (auto i : *mod_vector)                     \
        data[dsize++] = i

/// destroy the vector and set the pointer to NULL
#define CLEAR_VECTOR                                \
    delete mod_vector;                             \
    mod_vector = NULL

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

void modifier_test_seq()
{
    int id = 0;
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
    //barrier
    {
    // create a new vector with 9 elements, so we can be sure the elements get
    // deleted
    mod_vector = new std::vector<int>(9);
    // create another vector, that we can use for assigning into the first vector
    std::vector<int> l = {1, 2, 3, 4, 5};
    auto i = l.begin();
    i++;i++;i++;
    mod_vector->assign(l.begin(), i);
    COPY_VECTOR;
    CLEAR_VECTOR;
    }
    VERIFY("range assignment (1)", 3, {1, 2, 3});

    // test fill assignment (2)
    //barrier
    {
    mod_vector = new std::vector<int>(4, 9);
    mod_vector->assign(8, 32);
    COPY_VECTOR;
    CLEAR_VECTOR;
    }
    VERIFY("fill assignment (2)", 8, {32, 32, 32, 32, 32, 32, 32, 32});

    // test initializer vector assignment (3)
    //barrier
    {
    mod_vector = new std::vector<int>(4, 9);
    mod_vector->assign({8, 7, 6, 5, 4, 3, 2, 1});
    COPY_VECTOR;
    CLEAR_VECTOR;
    }
    VERIFY("initializer vector assignment (3)", 8, {8, 7, 6, 5, 4, 3, 2, 1});

    // test push_back with const value type (1) and with move (2)
    //barrier
    {
    mod_vector = new std::vector<int>({7, 6, 1, 2, 3});
    int x = 9;
    const int x99 = 99;
    mod_vector->push_back(x99);
    mod_vector->push_back(std::move(x));
    COPY_VECTOR;
    CLEAR_VECTOR;
    }
    VERIFY("push_back with const value type (1) and with move (2)", 7, {7, 6, 1, 2, 3, 99, 9});

    // test pop_back
    //barrier
    {
    mod_vector = new std::vector<int>({7, 6, 1, 2, 3, 99, 9});
    mod_vector->pop_back();
    COPY_VECTOR;
    CLEAR_VECTOR;
    }
    VERIFY("pop_back(1)", 6, {7, 6, 1, 2, 3, 99});


    // test insert of single element (1)
    //barrier
    {
    mod_vector = new std::vector<int>({1, 8, 2, 3, 4, 5});
    auto i = mod_vector->begin();i++;i++;
    const int x9 = 9;
    mod_vector->insert(i, x9);
    COPY_VECTOR;
    CLEAR_VECTOR;
    }
    VERIFY("single element insert (1)", 7, {1, 8, 9, 2, 3, 4, 5});

    // test fill insert (2)
    //barrier
    {
    mod_vector = new std::vector<int>({1, 8, 9, 2, 3, 4, 5});
    auto i = mod_vector->begin();i++;i++;i++;
    mod_vector->insert(i, 3, 18);
    COPY_VECTOR;
    CLEAR_VECTOR;
    }
    VERIFY("fill insert (2)", 10, {1, 8, 9, 18, 18, 18, 2, 3, 4, 5});

    // test range insert (3)
    //barrier
    {
    mod_vector = new std::vector<int>({1, 8, 9, 18, 18, 18, 2, 3, 4, 5});
    auto i = mod_vector->begin();i++;i++;i++;i++;i++;i++;
    std::vector<int> q({99, 98, 97});
    mod_vector->insert(i, q.begin(), q.end());
    COPY_VECTOR;
    CLEAR_VECTOR;
    }
    VERIFY("range insert (3)", 13, {1, 8, 9, 18, 18, 18, 99, 98, 97, 2, 3, 4, 5});

    // test insert with move (4)
    //barrier
    {
    mod_vector = new std::vector<int>({1, 8, 9, 18, 18, 18, 99, 98, 97, 2, 3, 4, 5});
    auto i = mod_vector->begin();i++;i++;
    int zzz = 7;
    mod_vector->insert(i, std::move(zzz));
    COPY_VECTOR;
    CLEAR_VECTOR;
    }
    VERIFY("insert with move (4)", 14, {1, 8, 7, 9, 18, 18, 18, 99, 98, 97, 2, 3, 4, 5});

    // test insert with initializer vector (5)
    //barrier
    {
    mod_vector = new std::vector<int>({1, 8, 9, 18, 16});
    auto i = mod_vector->begin();i++;i++;
    mod_vector->insert(i, {14, 15, 16});
    COPY_VECTOR;
    CLEAR_VECTOR;
    }
    VERIFY("insert with initializer vector (5)", 8, {1, 8, 14, 15, 16, 9, 18, 16});

    // test single element erase (1)
    //barrier
    {
    mod_vector = new std::vector<int>({1, 8, 9, 18, 16});
    auto i = mod_vector->begin();i++;i++;
    mod_vector->erase(i);
    COPY_VECTOR;
    CLEAR_VECTOR;
    }
    VERIFY("erase single element (1)", 4, {1, 8, 18, 16});

    // test range erase (2)
    //barrier
    {
    mod_vector = new std::vector<int>({1, 8, 9, 18, 16});
    auto i = mod_vector->begin();i++;i++;
    auto j = mod_vector->end();j--;
    mod_vector->erase(i, j);
    COPY_VECTOR;
    CLEAR_VECTOR;
    }
    VERIFY("erase range (2)", 3, {1, 8, 16});

    // test swap (1)
    //barrier
    int tmp = 0;
    {
    mod_vector = new std::vector<int>({1, 3, 4});
    std::vector<int> swapper = {9, 7, 3, 5, 2};
    mod_vector->swap(swapper);
    for (auto i = swapper.begin(); i != swapper.end(); ++i)
        tmp = tmp * 10 + *i;
    COPY_VECTOR;
    CLEAR_VECTOR;
    }
    if (tmp != 134)
        std::cout << "["<<id<<"] error in swap()" << std::endl;
    else {
        VERIFY("swap (1)", 5, {9, 7, 3, 5, 2});
    }


    // test clear (1)
    //barrier
    {
    mod_vector = new std::vector<int>({1, 2, 3, 4, 5, 9, 3});
    mod_vector->clear();
    mod_vector->push_back(1);
    COPY_VECTOR;
    CLEAR_VECTOR;
    }
    VERIFY("clear (1)", 1, {1});

    // test emplace (1)
    //barrier
    {
    mod_vector = new std::vector<int>({1, 2, 3, 4, 5});
    auto i = mod_vector->begin(); i++;
    mod_vector->emplace(i, 8);
    COPY_VECTOR;
    CLEAR_VECTOR;
    }
    VERIFY("emplace (1)", 6, {1, 8, 2, 3, 4, 5});

    // test emplace_back (1)
    //barrier
    {
    mod_vector = new std::vector<int>(4, 9);
    mod_vector->assign({8, 7, 6, 5, 4, 3, 2, 1});
    mod_vector->emplace_back(9);
    COPY_VECTOR;
    CLEAR_VECTOR;
    }
    VERIFY("emplace front (1) and emplace back (1)", 9, {8, 7, 6, 5, 4, 3, 2, 1, 9});

    //barrier
}




///~~~~~




void modifier_test_concurrent(int id)
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
    // create a new vector with 9 elements, so we can be sure the elements get
    // deleted
    mod_vector = new std::vector<int>(9);
    // create another vector, that we can use for assigning into the first vector
    std::vector<int> l = {1, 2, 3, 4, 5};
    auto i = l.begin();
    i++;i++;i++;
    mod_vector->assign(l.begin(), i);
    COPY_VECTOR;
    CLEAR_VECTOR;
    END_TX;
    VERIFY("range assignment (1)", 3, {1, 2, 3});

    // test fill assignment (2)
    global_barrier->arrive(id);
    BEGIN_TX;
    mod_vector = new std::vector<int>(4, 9);
    mod_vector->assign(8, 32);
    COPY_VECTOR;
    CLEAR_VECTOR;
    END_TX;
    VERIFY("fill assignment (2)", 8, {32, 32, 32, 32, 32, 32, 32, 32});

    // test initializer vector assignment (3)
    global_barrier->arrive(id);
    BEGIN_TX;
    mod_vector = new std::vector<int>(4, 9);
    mod_vector->assign({8, 7, 6, 5, 4, 3, 2, 1});
    COPY_VECTOR;
    CLEAR_VECTOR;
    END_TX;
    VERIFY("initializer vector assignment (3)", 8, {8, 7, 6, 5, 4, 3, 2, 1});

    // test push_back with const value type (1) and with move (2)
    global_barrier->arrive(id);
    BEGIN_TX;
    mod_vector = new std::vector<int>({7, 6, 1, 2, 3});
    int x = 9;
    const int x99 = 99;
    mod_vector->push_back(x99);
    mod_vector->push_back(std::move(x));
    COPY_VECTOR;
    CLEAR_VECTOR;
    END_TX;
    VERIFY("push_back with const value type (1) and with move (2)", 7, {7, 6, 1, 2, 3, 99, 9});

    // test pop_back
    global_barrier->arrive(id);
    BEGIN_TX;
    mod_vector = new std::vector<int>({7, 6, 1, 2, 3, 99, 9});
    mod_vector->pop_back();
    COPY_VECTOR;
    CLEAR_VECTOR;
    END_TX;
    VERIFY("pop_back(1)", 6, {7, 6, 1, 2, 3, 99});


    // test insert of single element (1)
    global_barrier->arrive(id);
    BEGIN_TX;
    mod_vector = new std::vector<int>({1, 8, 2, 3, 4, 5});
    auto i = mod_vector->begin();i++;i++;
    const int x9 = 9;
    mod_vector->insert(i, x9);
    COPY_VECTOR;
    CLEAR_VECTOR;
    END_TX;
    VERIFY("single element insert (1)", 7, {1, 8, 9, 2, 3, 4, 5});

    // test fill insert (2)
    global_barrier->arrive(id);
    BEGIN_TX;
    mod_vector = new std::vector<int>({1, 8, 9, 2, 3, 4, 5});
    auto i = mod_vector->begin();i++;i++;i++;
    mod_vector->insert(i, 3, 18);
    COPY_VECTOR;
    CLEAR_VECTOR;
    END_TX;
    VERIFY("fill insert (2)", 10, {1, 8, 9, 18, 18, 18, 2, 3, 4, 5});

    // test range insert (3)
    global_barrier->arrive(id);
    BEGIN_TX;
    mod_vector = new std::vector<int>({1, 8, 9, 18, 18, 18, 2, 3, 4, 5});
    auto i = mod_vector->begin();i++;i++;i++;i++;i++;i++;
    std::vector<int> q({99, 98, 97});
    mod_vector->insert(i, q.begin(), q.end());
    COPY_VECTOR;
    CLEAR_VECTOR;
    END_TX;
    VERIFY("range insert (3)", 13, {1, 8, 9, 18, 18, 18, 99, 98, 97, 2, 3, 4, 5});

    // test insert with move (4)
    global_barrier->arrive(id);
    BEGIN_TX;
    mod_vector = new std::vector<int>({1, 8, 9, 18, 18, 18, 99, 98, 97, 2, 3, 4, 5});
    auto i = mod_vector->begin();i++;i++;
    int zzz = 7;
    mod_vector->insert(i, std::move(zzz));
    COPY_VECTOR;
    CLEAR_VECTOR;
    END_TX;
    VERIFY("insert with move (4)", 14, {1, 8, 7, 9, 18, 18, 18, 99, 98, 97, 2, 3, 4, 5});

    // test insert with initializer vector (5)
    global_barrier->arrive(id);
    BEGIN_TX;
    mod_vector = new std::vector<int>({1, 8, 9, 18, 16});
    auto i = mod_vector->begin();i++;i++;
    mod_vector->insert(i, {14, 15, 16});
    COPY_VECTOR;
    CLEAR_VECTOR;
    END_TX;
    VERIFY("insert with initializer vector (5)", 8, {1, 8, 14, 15, 16, 9, 18, 16});

    // test single element erase (1)
    global_barrier->arrive(id);
    BEGIN_TX;
    mod_vector = new std::vector<int>({1, 8, 9, 18, 16});
    auto i = mod_vector->begin();i++;i++;
    mod_vector->erase(i);
    COPY_VECTOR;
    CLEAR_VECTOR;
    END_TX;
    VERIFY("erase single element (1)", 4, {1, 8, 18, 16});

    // test range erase (2)
    global_barrier->arrive(id);
    BEGIN_TX;
    mod_vector = new std::vector<int>({1, 8, 9, 18, 16});
    auto i = mod_vector->begin();i++;i++;
    auto j = mod_vector->end();j--;
    mod_vector->erase(i, j);
    COPY_VECTOR;
    CLEAR_VECTOR;
    END_TX;
    VERIFY("erase range (2)", 3, {1, 8, 16});

    // test swap (1)
    global_barrier->arrive(id);
    int tmp = 0;
    BEGIN_TX;
    mod_vector = new std::vector<int>({1, 3, 4});
    std::vector<int> swapper = {9, 7, 3, 5, 2};
    mod_vector->swap(swapper);
    for (auto i = swapper.begin(); i != swapper.end(); ++i)
        tmp = tmp * 10 + *i;
    COPY_VECTOR;
    CLEAR_VECTOR;
    END_TX;
    if (tmp != 134)
        std::cout << "["<<id<<"] error in swap()" << std::endl;
    else {
        VERIFY("swap (1)", 5, {9, 7, 3, 5, 2});
    }


    // test clear (1)
    global_barrier->arrive(id);
    BEGIN_TX;
    mod_vector = new std::vector<int>({1, 2, 3, 4, 5, 9, 3});
    mod_vector->clear();
    mod_vector->push_back(1);
    COPY_VECTOR;
    CLEAR_VECTOR;
    END_TX;
    VERIFY("clear (1)", 1, {1});

    // test emplace (1)
    global_barrier->arrive(id);
    BEGIN_TX;
    mod_vector = new std::vector<int>({1, 2, 3, 4, 5});
    auto i = mod_vector->begin(); i++;
    mod_vector->emplace(i, 8);
    COPY_VECTOR;
    CLEAR_VECTOR;
    END_TX;
    VERIFY("emplace (1)", 6, {1, 8, 2, 3, 4, 5});

    // test emplace_back (1)
    global_barrier->arrive(id);
    BEGIN_TX;
    mod_vector = new std::vector<int>(4, 9);
    mod_vector->assign({8, 7, 6, 5, 4, 3, 2, 1});
    mod_vector->emplace_back(9);
    COPY_VECTOR;
    CLEAR_VECTOR;
    END_TX;
    VERIFY("emplace front (1) and emplace back (1)", 9, {8, 7, 6, 5, 4, 3, 2, 1, 9});

    global_barrier->arrive(id);
}
