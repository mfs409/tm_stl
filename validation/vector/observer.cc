#include <iostream>
#include <vector>
#include "tests.h"
#include "verify.h"

std::vector<int>* allocator_vec = NULL;


void observer_tests(int id)
{
    global_barrier->arrive(id);
    if (id == 0)
        printf("Testing get_allocator()\n");

    // This is a really simple test
    global_barrier->arrive(id);
    {
        verifier v;
        int size;
        BEGIN_TX;
        allocator_vec = new std::vector<int>();
        auto a = allocator_vec->get_allocator();
        size = allocator_vec->size();
        delete(allocator_vec);
        allocator_vec = NULL;
        END_TX;
        v.check_size("get_allocator", id, size);
    }
}
