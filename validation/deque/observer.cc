#include <iostream>
#include <string>
#include <deque>
#include <cassert>
#include "tests.h"
#include "verify.h"

/// The deque we will use for our tests
std::deque<int>* allocator_deque = NULL;

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
        allocator_deque = new std::deque<int>();
        auto a = allocator_deque->get_allocator();
        size = allocator_deque->size();
        delete(allocator_deque);
        allocator_deque = NULL;
        END_TX;
        v.check_size("get_allocator", id, size);
    }
}
