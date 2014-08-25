#include <iostream>
#include <vector>
#include "tests.h"

/// The vector we will use for our tests
std::vector<int>* element_vec = NULL;

void element_tests(int id)
{
    global_barrier->arrive(id);
    if (id == 0)
        printf("Testing deque element access functions: [](2), at(2), front(2), back(2)\n");

    // Test [] with non-const vector
    global_barrier->arrive(id);
    {
        int ans = -2;
        BEGIN_TX;
        element_vec = new std::vector<int>({1, 2, 3});
        ans = (*element_vec)[1];
        delete(element_vec);
        END_TX;
        if (ans != 2)
            printf(" [%d] vector operator[] test failed\n", id);
        else if (id == 0)
            printf(" [OK] %s\n", "vector operator[](1a)");
    }

    // Test [] with const vector
    global_barrier->arrive(id);
    {
        int ans = -2;
        BEGIN_TX;
        element_vec = new std::vector<int>({1, 2, 3});
        const std::vector<int> ce = *element_vec;
        ans = ce[1];
        delete(element_vec);
        END_TX;
        if (ans != 2)
            printf(" [%d] vector operator[] test failed\n", id);
        else if (id == 0)
            printf(" [OK] %s\n", "vector operator[](1b)");
    }
    // Test at() with non-const vector
    global_barrier->arrive(id);
    {
        int ans = -2;
        BEGIN_TX;
        element_vec = new std::vector<int>({1, 2, 3});
        ans = element_vec->at(1);
        delete(element_vec);
        END_TX;
        if (ans != 2)
            printf(" [%d] vector at() test failed\n", id);
        else if (id == 0)
            printf(" [OK] %s\n", "vector at(1a)");
    }

    // Test at() with const vector
    global_barrier->arrive(id);
    {
        int ans = -2;
        BEGIN_TX;
        element_vec = new std::vector<int>({1, 2, 3});
        const std::vector<int> ce = *element_vec;
        ans = ce.at(1);
        delete(element_vec);
        END_TX;
        if (ans != 2)
            printf(" [%d] vector at() test failed\n", id);
        else if (id == 0)
            printf(" [OK] %s\n", "vector at(1b)");
    }

    // Test front() with non-const vector
    global_barrier->arrive(id);
    {
        int ans = -2;
        BEGIN_TX;
        element_vec = new std::vector<int>({1, 2, 3});
        ans = element_vec->front();
        delete(element_vec);
        END_TX;
        if (ans != 1)
            printf(" [%d] vector front() test failed\n", id);
        else if (id == 0)
            printf(" [OK] %s\n", "vector front(1a)");
    }

    // Test front() with const vector
    global_barrier->arrive(id);
    {
        int ans = -2;
        BEGIN_TX;
        element_vec = new std::vector<int>({1, 2, 3});
        const std::vector<int> ce = *element_vec;
        ans = ce.front();
        delete(element_vec);
        END_TX;
        if (ans != 1)
            printf(" [%d] vector front() test failed\n", id);
        else if (id == 0)
            printf(" [OK] %s\n", "vector front(1b)");
    }

    // Test back() with non-const vector
    global_barrier->arrive(id);
    {
        int ans = -2;
        BEGIN_TX;
        element_vec = new std::vector<int>({1, 2, 3});
        ans = element_vec->back();
        delete(element_vec);
        END_TX;
        if (ans != 3)
            printf(" [%d] vector back() test failed\n", id);
        else if (id == 0)
            printf(" [OK] %s\n", "vector back(1a)");
    }

    // Test back() with const vector
    global_barrier->arrive(id);
    {
        int ans = -2;
        BEGIN_TX;
        element_vec = new std::vector<int>({1, 2, 3});
        const std::vector<int> ce = *element_vec;
        ans = ce.back();
        delete(element_vec);
        END_TX;
        if (ans != 3)
            printf(" [%d] vector back() test failed\n", id);
        else if (id == 0)
            printf(" [OK] %s\n", "vector back(1b)");
    }

    // Test data() with non-const vector
    global_barrier->arrive(id);
    {
        int ans = -2;
        BEGIN_TX;
        element_vec = new std::vector<int>({1, 2, 3});
        ans = *(element_vec->data());
        delete(element_vec);
        END_TX;
        if (ans != 1)
            printf(" [%d] vector data() test failed\n", id);
        else if (id == 0)
            printf(" [OK] %s\n", "vector data(1a)");
    }

    // Test data() with const vector
    global_barrier->arrive(id);
    {
        int ans = -2;
        BEGIN_TX;
        element_vec = new std::vector<int>({1, 2, 3});
        const std::vector<int> ce = *element_vec;
        ans = *(ce.data());
        delete(element_vec);
        END_TX;
        if (ans != 1)
            printf(" [%d] vector data() test failed\n", id);
        else if (id == 0)
            printf(" [OK] %s\n", "vector data(1b)");
    }
}
