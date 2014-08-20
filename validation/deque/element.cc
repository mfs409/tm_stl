#include <iostream>
#include <string>
#include <deque>
#include <cassert>
#include "tests.h"
#include "verify.h"

/// The deques we will use for our tests
std::deque<int>* element_deque = NULL;

void element_tests(int id)
{
    global_barrier->arrive(id);
    if (id == 0)
        printf("Testing deque element access functions: [](2), at(2), front(2), back(2)\n");

    // Test [] with non-const deque
    global_barrier->arrive(id);
    {
        int ans = -2;
        verifier v;
        BEGIN_TX;
        element_deque = new std::deque<int>({1, 2, 3});
        ans = (*element_deque)[1];
        delete(element_deque);
        END_TX;
        if (ans != 2)
            printf(" [%d] deque operator[] test failed\n", id);
        else if (id == 0)
            printf(" [OK] %s\n", "deque operator[](1a)");
    }

    // Test [] with const deque
    global_barrier->arrive(id);
    {
        int ans = -2;
        verifier v;
        BEGIN_TX;
        element_deque = new std::deque<int>({1, 2, 3});
        const std::deque<int> ce = *element_deque;
        ans = ce[1];
        delete(element_deque);
        END_TX;
        if (ans != 2)
            printf(" [%d] deque operator[] test failed\n", id);
        else if (id == 0)
            printf(" [OK] %s\n", "deque operator[](1b)");
    }

    // Test at() with non-const deque
    global_barrier->arrive(id);
    {
        int ans = -2;
        verifier v;
        BEGIN_TX;
        element_deque = new std::deque<int>({1, 2, 3});
        ans = element_deque->at(1);
        delete(element_deque);
        END_TX;
        if (ans != 2)
            printf(" [%d] deque at() test failed\n", id);
        else if (id == 0)
            printf(" [OK] %s\n", "deque at(1a)");
    }

    // Test at() with const deque
    global_barrier->arrive(id);
    {
        int ans = -2;
        verifier v;
        BEGIN_TX;
        element_deque = new std::deque<int>({1, 2, 3});
        const std::deque<int> ce = *element_deque;
        ans = ce.at(1);
        delete(element_deque);
        END_TX;
        if (ans != 2)
            printf(" [%d] deque at() test failed\n", id);
        else if (id == 0)
            printf(" [OK] %s\n", "deque at(1b)");
    }

    // Test front() with non-const deque
    global_barrier->arrive(id);
    {
        int ans = -2;
        verifier v;
        BEGIN_TX;
        element_deque = new std::deque<int>({1, 2, 3});
        ans = element_deque->front();
        delete(element_deque);
        END_TX;
        if (ans != 1)
            printf(" [%d] deque front() test failed\n", id);
        else if (id == 0)
            printf(" [OK] %s\n", "deque front(1a)");
    }

    // Test front() with const deque
    global_barrier->arrive(id);
    {
        int ans = -2;
        verifier v;
        BEGIN_TX;
        element_deque = new std::deque<int>({1, 2, 3});
        const std::deque<int> ce = *element_deque;
        ans = ce.front();
        delete(element_deque);
        END_TX;
        if (ans != 1)
            printf(" [%d] deque front() test failed\n", id);
        else if (id == 0)
            printf(" [OK] %s\n", "deque front(1b)");
    }

    // Test back() with non-const deque
    global_barrier->arrive(id);
    {
        int ans = -2;
        verifier v;
        BEGIN_TX;
        element_deque = new std::deque<int>({1, 2, 3});
        ans = element_deque->back();
        delete(element_deque);
        END_TX;
        if (ans != 3)
            printf(" [%d] deque back() test failed\n", id);
        else if (id == 0)
            printf(" [OK] %s\n", "deque back(1a)");
    }

    // Test back() with const deque
    global_barrier->arrive(id);
    {
        int ans = -2;
        verifier v;
        BEGIN_TX;
        element_deque = new std::deque<int>({1, 2, 3});
        const std::deque<int> ce = *element_deque;
        ans = ce.back();
        delete(element_deque);
        END_TX;
        if (ans != 3)
            printf(" [%d] deque back() test failed\n", id);
        else if (id == 0)
            printf(" [OK] %s\n", "deque back(1b)");
    }
}
