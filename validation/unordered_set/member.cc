#include <iostream>
#include <string>
#include <unordered_set>
#include <cassert>
#include "tests.h"
#include "verify.h"

// global pointer to the container
std::unordered_set<int>* member_unordered_set = NULL;

void ctor_dtor_tests(int id)
{
    // print simple output
    global_barrier->arrive(id);
    if (id == 0)
        printf("Testing member vector constructors(10) and destructors(1)\n");

    // the first test is empty ctor (1b)
    global_barrier->arrive(id);
    {
        verifier v;
        int size;
        BEGIN_TX;
        member_unordered_set = new std::unordered_set<int>();
        int size = member_unordered_set->size();
        delete(member_unordered_set);
        member_unordered_set = NULL;
        END_TX;
        v.check_size("empty ctor (1b)", id, size);
    }

    // the range constructor (2a)
    global_barrier->arrive(id);
    {
        verifier v;
        BEGIN_TX;
        std::unordered_set<int> tmp({1, 2, 3, 4, 100, 99, 98, 97});
	member_unordered_set = new std::unordered_set<int>(tmp.begin(), tmp.end());
	v.insert_all<std::unordered_set<int>>(member_unordered_set);
        delete(member_unordered_set);
        member_unordered_set = NULL;
        END_TX;

	v.check("range ctor (2a)", id, 8,
		{1, 2, 3, 4, 100, 99, 98, 97, -2});
    }

    // the empty constructor (1c)
    global_barrier->arrive(id);
    {
	verifier v;
	int size;
	BEGIN_TX;
	member_unordered_set = new std::unordered_set<int>();
	auto a = member_unordered_set->get_allocator();
	delete(member_unordered_set);
	member_unordered_set = new std::unordered_set<int>(a);
	size = member_unordered_set->size();
	delete(member_unordered_set);
	member_unordered_set = NULL;
	END_TX;

	v.check_size("the empty constructor (1c)", id, size);
    }

    // the copy constructor (3b)
    global_barrier->arrive(id);
    {
	verifier v;
	int size;
	BEGIN_TX;
	std::unordered_set<int> tmp({1, 2, 3, 4, 9, 8, 7, 6});
	auto a = tmp.get_allocator();
	member_unordered_set = new std::unordered_set<int>(tmp, a);
	size = member_unordered_set->size();
	v.insert_all<std::unordered_set<int>>(member_unordered_set);
	delete(member_unordered_set);
	member_unordered_set = NULL;
	END_TX;

        v.check_size("the copy constructor (3b)", id, size);
    }

    // the move constructor (4b)
    global_barrier->arrive(id);
    {
	verifier v;
	int size;
	BEGIN_TX;
	std::unordered_set<int> tmp({1,2,3,4,9,8,7,6});
	auto a = tmp.get_allocator();
	member_unordered_set = new std::unordered_set<int>(std::move(tmp), a);
	size = member_unordered_set->size();
	v.insert_all<std::unordered_set<int>>(member_unordered_set);
	delete(member_unordered_set);
	member_unordered_set = NULL;
	END_TX;

	v.check_size("the move constructor (4b)", id, size);
    }

    // the simple initializer list constructor (5a)
    global_barrier->arrive(id);
    {
        verifier v;
        BEGIN_TX;
        member_unordered_set = new std::unordered_set<int>({1, 2, 3, 4, 100, 99, 98, 97});
        v.insert_all<std::unordered_set<int>>(member_unordered_set);
        delete(member_unordered_set);
        member_unordered_set = NULL;
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

	std::unordered_set<int> tmp;
	tmp = {1,2,3,4,9,8,7,6};

	member_unordered_set = new std::unordered_set<int>(tmp);
	size = member_unordered_set->size();
	v.insert_all<std::unordered_set<int>>(member_unordered_set);
	delete(member_unordered_set);
	member_unordered_set = NULL;
	END_TX;

	v.check_size("initializer list operator= (3)", id, size);
    }
}
