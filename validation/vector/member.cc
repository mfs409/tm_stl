#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include "tests.h"
#include "verify.h"

std::vector<int>* member_vec = NULL;

void ctor_dtor_tests(int id)
{
    // print simple output
    global_barrier->arrive(id);
    if (id == 0)
        printf("Testing member vector constructors(10) and destructors(1)\n");

    // the first test is simple ctor and dtor
    //
    // NB: we haven't actually verified size yet, but we use it here.
    global_barrier->arrive(id);
    {
        verifier v;
        int size;
        BEGIN_TX;
        member_vec = new std::vector<int>();
        int size = member_vec->size();
        delete(member_vec);
        member_vec = NULL;
        END_TX;
        v.check_size("basic ctor(1) and dtor(1)", id, size);
    }
#if 0
    // the next test uses a custom allocator with ctor 1
    //
    // NB: we haven't actually verified size yet, but we use it here.
    global_barrier->arrive(id);
    {
        verifier v;
        int size;
        BEGIN_TX;
        member_deque = new std::deque<int>();
        // using get_allocator without checking it
        auto a = member_deque->get_allocator();
        delete(member_deque);
        member_deque = new std::deque<int>(a);
        size = member_deque->size();
        delete(member_deque);
        member_deque = NULL;
        END_TX;
        v.check_size("basic ctor(1) and dtor(1)", id, size);
    }
#endif
    // the next test will call the simple fill constructor and the fill
    // constructor with default value
    //
    // NB: we haven't actually verified iterators yet, but we use them in
    //     COPY_DEQUE
    global_barrier->arrive(id);
    {
        verifier v;
        BEGIN_TX;
        member_vec = new std::vector<int>(4);
        v.insert_all<std::vector<int>>(member_vec);
        delete(member_vec);
        member_vec = new std::vector<int>(4, 98);
        v.insert_all<std::vector<int>>(member_vec);
        delete(member_vec);
        member_vec = NULL;
        END_TX;
        v.check("fill ctor (2a) and fill ctor (2b)", id, 8,
              { 0, 0, 0, 0, 98, 98, 98, 98, -2});
    }
}

void op_eq_tests(int id)
{
}
