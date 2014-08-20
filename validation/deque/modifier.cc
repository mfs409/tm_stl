#include <iostream>
#include <deque>
#include "tests.h"
#include "verify.h"

/// The deques we will use for our tests
std::deque<int>* modifier_deque = NULL;

void modifier_tests(int id)
{
    global_barrier->arrive(id);
    if (id == 0)
        printf("Testing deque modifier functions: assign(3), push_back(2), "
               "push_front(2), pop_back(1), pop_front(1), insert(5), erase(2), "
               "swap(1), clear(1), emplace(1), emplace_front(1), "
               "emplace_back(1)\n");

    // Test range assignment (1)
    global_barrier->arrive(id);
    {
        verifier v;
        BEGIN_TX;
        // make a deque with some elements in it
        modifier_deque = new std::deque<int>(9);
        // create a local deque
        std::deque<int> tmp = {1, 2, 3, 4, 5};
        auto i = tmp.begin();
        i++; i++; i++;
        modifier_deque->assign(tmp.begin(), i);
        v.insert_all(modifier_deque);
        delete(modifier_deque);
        END_TX;
        v.check("assign: range (1)", id, 3, {1, 2, 3});
    }

    // Test range assignment (2)
    global_barrier->arrive(id);
    {
        verifier v;
        BEGIN_TX;
        // make a deque with some elements in it
        modifier_deque = new std::deque<int>();
        modifier_deque->assign(3, 72);
        v.insert_all(modifier_deque);
        delete(modifier_deque);
        END_TX;
        v.check("assign: fill (2)", id, 3, {72, 72, 72, -2});
    }

    // Test ilist assignment (3)
    global_barrier->arrive(id);
    {
        verifier v;
        BEGIN_TX;
        // make a deque with some elements in it
        modifier_deque = new std::deque<int>();
        modifier_deque->assign({9, 8, 12, 5});
        v.insert_all(modifier_deque);
        delete(modifier_deque);
        END_TX;
        v.check("assign: ilist (3)", id, 4, {9, 8, 12, 5, -2});
    }

    // Test push_back (1a, 1b)
    global_barrier->arrive(id);
    {
        verifier v;
        BEGIN_TX;
        // make a deque with some elements in it
        modifier_deque = new std::deque<int>();
        const int x72 = 72;
        modifier_deque->push_back(x72);
        int x99 = 99;
        modifier_deque->push_back(std::move(x99));
        v.insert_all(modifier_deque);
        delete(modifier_deque);
        END_TX;
        v.check("push_back (1a, 1b)", id, 2, {72, 99, -2});
    }

    // Test push_front (1a, 1b)
    global_barrier->arrive(id);
    {
        verifier v;
        BEGIN_TX;
        // make a deque with some elements in it
        modifier_deque = new std::deque<int>();
        const int x72 = 72;
        modifier_deque->push_front(x72);
        int x99 = 99;
        modifier_deque->push_front(std::move(x99));
        v.insert_all(modifier_deque);
        delete(modifier_deque);
        END_TX;
        v.check("push_front (1a, 1b)", id, 2, {99, 72, -2});
    }

    // Test pop_back (1) and pop_front(1)
    global_barrier->arrive(id);
    {
        verifier v;
        BEGIN_TX;
        // make a deque with some elements in it
        modifier_deque = new std::deque<int>({1, 2, 3, 4, 5});
        modifier_deque->pop_back();
        modifier_deque->pop_front();
        v.insert_all(modifier_deque);
        delete(modifier_deque);
        END_TX;
        v.check("pop_back (1), pop_front (1)", id, 3, {2, 3, 4, -2});
    }

    // test insert of single element (1)
    global_barrier->arrive(id);
    {
        verifier v;
        BEGIN_TX;
        modifier_deque = new std::deque<int>({1, 8, 2, 3, 4, 5});
        auto i = modifier_deque->begin();i++;i++;
        const int x9 = 9;
        modifier_deque->insert(i, x9);
        v.insert_all(modifier_deque);
        delete(modifier_deque);
        END_TX;
        v.check("single element insert (1)", id, 7, {1, 8, 9, 2, 3, 4, 5});
    }

    // test fill insert (2)
    global_barrier->arrive(id);
    {
        verifier v;
        BEGIN_TX;
        modifier_deque = new std::deque<int>({1, 8, 9, 2, 3, 4, 5});
        auto i = modifier_deque->begin();i++;i++;i++;
        modifier_deque->insert(i, 3, 18);
        v.insert_all(modifier_deque);
        delete(modifier_deque);
        END_TX;
        v.check("fill insert (2)", id, 10, {1, 8, 9, 18, 18, 18, 2, 3, 4, 5});
    }

    // test range insert (3)
    global_barrier->arrive(id);
    {
        verifier v;
        BEGIN_TX;
        modifier_deque = new std::deque<int>({1, 8, 9, 18, 18, 18, 2, 3, 4, 5});
        auto i = modifier_deque->begin();i++;i++;i++;i++;i++;i++;
        std::deque<int> q({99, 98, 97});
        modifier_deque->insert(i, q.begin(), q.end());
        v.insert_all(modifier_deque);
        delete(modifier_deque);
        END_TX;
        v.check("range insert (3)", id, 13, {1, 8, 9, 18, 18, 18, 99, 98, 97, 2, 3, 4, 5});
    }

    // test insert with move (4)
    global_barrier->arrive(id);
    {
        verifier v;
        BEGIN_TX;
        modifier_deque = new std::deque<int>({1, 8, 9, 18, 18, 18, 99, 98, 97, 2, 3, 4, 5});
        auto i = modifier_deque->begin();i++;i++;
        int zzz = 7;
        modifier_deque->insert(i, std::move(zzz));
        v.insert_all(modifier_deque);
        delete(modifier_deque);
        END_TX;
        v.check("insert with move (4)", id, 14, {1, 8, 7, 9, 18, 18, 18, 99, 98, 97, 2, 3, 4, 5});
    }

    // test insert with initializer list (5)
    global_barrier->arrive(id);
    {
        verifier v;
        BEGIN_TX;
        modifier_deque = new std::deque<int>({1, 8, 9, 18, 16});
        auto i = modifier_deque->begin();i++;i++;
        modifier_deque->insert(i, {14, 15, 16});
        v.insert_all(modifier_deque);
        delete(modifier_deque);
        END_TX;
        v.check("insert with initializer list (5)", id, 8, {1, 8, 14, 15, 16, 9, 18, 16});
    }

    // test single element erase (1)
    global_barrier->arrive(id);
    {
        verifier v;
        BEGIN_TX;
        modifier_deque = new std::deque<int>({1, 8, 9, 18, 16});
        auto i = modifier_deque->begin();i++;i++;
        modifier_deque->erase(i);
        v.insert_all(modifier_deque);
        delete(modifier_deque);
        END_TX;
        v.check("erase single element (1)", id, 4, {1, 8, 18, 16});
    }

    // test range erase (2)
    global_barrier->arrive(id);
    {
        verifier v;
        BEGIN_TX;
        modifier_deque = new std::deque<int>({1, 8, 9, 18, 16});
        auto i = modifier_deque->begin();i++;i++;
        auto j = modifier_deque->end();j--;
        modifier_deque->erase(i, j);
        v.insert_all(modifier_deque);
        delete(modifier_deque);
        END_TX;
        v.check("erase range (2)", id, 3, {1, 8, 16});
    }

    // test swap (1)
    global_barrier->arrive(id);
    {
        verifier v;
        int tmp = 0;
        BEGIN_TX;
        modifier_deque = new std::deque<int>({1, 3, 4});
        std::deque<int> swapper = {9, 7, 3, 5, 2};
        modifier_deque->swap(swapper);
        for (auto i = swapper.begin(); i != swapper.end(); ++i)
            tmp = tmp * 10 + *i;
        v.insert_all(modifier_deque);
        delete(modifier_deque);
        END_TX;
        if (tmp != 134)
            std::cout << "["<<id<<"] error in swap()" << std::endl;
        v.check("swap (1)", id, 5, {9, 7, 3, 5, 2});
    }

    // test clear (1)
    global_barrier->arrive(id);
    {
        verifier v;
        BEGIN_TX;
        modifier_deque = new std::deque<int>({1, 2, 3, 4, 5, 9, 3});
        modifier_deque->clear();
        modifier_deque->push_front(1);
        v.insert_all(modifier_deque);
        delete(modifier_deque);
        END_TX;
        v.check("clear (1)", id, 1, {1});
    }

    // test emplace (1)
    global_barrier->arrive(id);
    {
        verifier v;
        BEGIN_TX;
        modifier_deque = new std::deque<int>({1, 2, 3, 4, 5});
        auto i = modifier_deque->begin(); i++;
        modifier_deque->emplace(i, 8);
        v.insert_all(modifier_deque);
        delete(modifier_deque);
        END_TX;
        v.check("emplace (1)", id, 6, {1, 8, 2, 3, 4, 5});
    }


    // test emplace_front (1) and emplace_back (1)
    global_barrier->arrive(id);
    {
        verifier v;
        BEGIN_TX;
        modifier_deque = new std::deque<int>(4, 9);
        modifier_deque->assign({8, 7, 6, 5, 4, 3, 2, 1});
        modifier_deque->emplace_front(7);
        modifier_deque->emplace_back(9);
        v.insert_all(modifier_deque);
        delete(modifier_deque);
        END_TX;
        v.check("emplace front (1) and emplace back (1)", id, 10, {7, 8, 7, 6, 5, 4, 3, 2, 1, 9});
    }
}
