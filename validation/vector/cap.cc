#include <iostream>
#include <vector>
#include "tests.h"
#include "verify.h"

/// The vector we will use for our tests
std::vector<int>* cap_vec = NULL;

void cap_tests(int id)
{
     global_barrier->arrive(id);
    if (id == 0)
        printf("Testing vector capacity functions: size(1), max_size(1), resize(2), capacity(1),empty(1), shrink_to_fit(1)\n");

    // #1: Test size()
    global_barrier->arrive(id);
    {
        int size = 0;
        verifier v;
        BEGIN_TX;
        cap_vec = new std::vector<int>({1, 2, 3});
        size = cap_vec->size();
        delete(cap_vec);
        END_TX;
        if (size != 3)
            printf(" [%d] vector size test failed\n", id);
        else if (id == 0)
            printf(" [OK] %s\n", "vector size()");
    }

     // #3: Test resize()
    global_barrier->arrive(id);
    {
        verifier v;
        BEGIN_TX;
        cap_vec = new std::vector<int>({1, 2, 3});
        cap_vec->resize(1);
        cap_vec->resize(8, 6);
        v.insert_all<std::vector<int>>(cap_vec);
        delete(cap_vec);
        END_TX;
        v.check(" vector resize (1) and (2)", id, 8, { 1, 6, 6, 6, 6, 6, 6, 6 });
    }

    // #4: test empty()
    global_barrier->arrive(id);
    {
        bool ok = true;
        verifier v;
        BEGIN_TX;
        cap_vec = new std::vector<int>();
        ok &= cap_vec->empty();
        cap_vec->resize(8, 6);
        ok &= !cap_vec->empty();
        delete(cap_vec);
        END_TX;
        if (!ok)
            printf(" [%d] vector empty test failed\n", id);
        else if (id == 0)
            printf(" [OK] %s\n", "vector empty()");
    }

    // #5: test shrink_to_fit()
    global_barrier->arrive(id);
    {
        int size = 0;
        verifier v;
        BEGIN_TX;
        cap_vec = new std::vector<int>(100);
        cap_vec->resize(10);
        cap_vec->shrink_to_fit();
        size = cap_vec->size();
        delete(cap_vec);
        END_TX;
        if (size != 10)
            printf(" [%d] vector size test failed: size = %d\n", id, size);
        else if (id == 0)
            printf(" [OK] %s\n", "vector size()");
    }
}
