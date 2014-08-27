#include <iostream>
#include <unordered_set>
#include "tests.h"
#include "verify.h"

// global pointer to the container
std::unordered_multiset<int>* member_unordered_multiset = NULL;

void ctor_dtor_tests(int id)
{
    // print simple output
    global_barrier->arrive(id);
    if (id == 0)
        printf("Testing constructors(/15) and destructors(1)\n");

    // the empty ctor with size (1b)
    global_barrier->arrive(id);
    {
        verifier v;
        int size;
        BEGIN_TX;
        member_unordered_multiset = new std::unordered_multiset<int>(18);
        int size = member_unordered_multiset->size();
        delete(member_unordered_multiset);
        member_unordered_multiset = NULL;
        END_TX;
        v.check_size("empty ctor (1b)", id, size);
    }

    // the empty constructor with allocator parameter (1c)
    global_barrier->arrive(id);
    {
        verifier v;
        int size;
        BEGIN_TX;
        member_unordered_multiset = new std::unordered_multiset<int>();
        auto a = member_unordered_multiset->get_allocator();
        delete(member_unordered_multiset);
        member_unordered_multiset = new std::unordered_multiset<int>(a);
        size = member_unordered_multiset->size();
        delete(member_unordered_multiset);
        member_unordered_multiset = NULL;
        END_TX;
        v.check_size("empty constructor + allocator (1c)", id, size);
    }

    // the range constructor (2a)
    global_barrier->arrive(id);
    {
        verifier v;
        BEGIN_TX;
        std::unordered_multiset<int> tmp({1, 2, 3, 4, 100, 99, 98, 97});
        member_unordered_multiset = new std::unordered_multiset<int>(tmp.begin(), tmp.end());
        v.insert_all<std::unordered_multiset<int>>(member_unordered_multiset);
        delete(member_unordered_multiset);
        member_unordered_multiset = NULL;
        END_TX;

        v.check("range ctor (2a)", id, 8,
                {1, 2, 3, 4, 100, 99, 98, 97, -2});
    }

    // the copy constructor (3b)
    global_barrier->arrive(id);
    {
        verifier v;
        int size;
        BEGIN_TX;
        std::unordered_multiset<int> tmp({1, 2, 3, 4, 9, 8, 7, 6});
        auto a = tmp.get_allocator();
        member_unordered_multiset = new std::unordered_multiset<int>(tmp, a);
        size = member_unordered_multiset->size();
        v.insert_all<std::unordered_multiset<int>>(member_unordered_multiset);
        delete(member_unordered_multiset);
        member_unordered_multiset = NULL;
        END_TX;
        v.check_size("copy constructor with allocator (3b)", id, size);
    }

    // the move constructor (4b)
    global_barrier->arrive(id);
    {
        verifier v;
        int size;
        BEGIN_TX;
        std::unordered_multiset<int> tmp({1,2,3,4,9,8,7,6});
        auto a = tmp.get_allocator();
        member_unordered_multiset = new std::unordered_multiset<int>(std::move(tmp), a);
        size = member_unordered_multiset->size();
        v.insert_all<std::unordered_multiset<int>>(member_unordered_multiset);
        delete(member_unordered_multiset);
        member_unordered_multiset = NULL;
        END_TX;

        v.check_size("the move constructor (4b)", id, size);
    }

    // the initializer list constructor (5a)
    global_barrier->arrive(id);
    {
        verifier v;
        BEGIN_TX;
        member_unordered_multiset = new std::unordered_multiset<int>({1, 2, 3, 4, 100, 99, 98, 97});
        v.insert_all<std::unordered_multiset<int>>(member_unordered_multiset);
        delete(member_unordered_multiset);
        member_unordered_multiset = NULL;
        END_TX;

        v.check("initializer list ctor (5a)", id, 8,
                { 1, 2, 3, 4, 100, 99, 98, 97, -2});
    }
}

void op_eq_tests(int id)
{
    global_barrier->arrive(id);
    {
        verifier v;
        int size;
        BEGIN_TX;

        std::unordered_multiset<int> tmp;
        tmp = {1,2,3,4,9,8,7,6};

        member_unordered_multiset = new std::unordered_multiset<int>(tmp);
        size = member_unordered_multiset->size();
        v.insert_all<std::unordered_multiset<int>>(member_unordered_multiset);
        delete(member_unordered_multiset);
        member_unordered_multiset  = NULL;
        END_TX;

        v.check_size("initializer list operator= (3)", id, size);
    }
}
