#include <iostream>
#include <string>
#include <unordered_set>
#include <cassert>
#include "tests.h"
#include "verify.h"

std::unordered_set<int>* member_vec = NULL;

void ctor_dtor_tests(int id)
{
    // print simple output
    global_barrier->arrive(id);
    if (id == 0)
        printf("Testing member vector constructors(10) and destructors(1)\n");

    // the first test is simple ctor and dtor
    global_barrier->arrive(id);
    {
        verifier v;
        int size;
        BEGIN_TX;
        member_vec = new std::unordered_set<int>(10);
        int size = member_vec->size();
        delete(member_vec);
        member_vec = NULL;
        END_TX;
        v.check_size("empty ctor (1b)", id, size);
    }

    // the next test will call the simple fill constructor and the fill
    // constructor with default value
    //
    // NB: we haven't actually verified iterators yet, but we use them in
    //     COPY_DEQUE
    global_barrier->arrive(id);
    {
        verifier v;
        BEGIN_TX;
        member_vec = new std::unordered_set<int>({1, 2, 3, 4, 100, 99, 98, 97});
        v.insert_all<std::unordered_set<int>>(member_vec);
        delete(member_vec);
        member_vec = NULL;
        END_TX;
	
        v.check("initializer list ctor (5a)", id, 8,
              { 1, 2, 3, 4, 100, 99, 98, 97, -2});
    }
}

void op_eq_tests(int id)
{
}
