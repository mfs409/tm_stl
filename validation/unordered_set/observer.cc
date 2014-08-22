#include <iostream>
#include <string>
#include <unordered_set>
#include <cassert>
#include "tests.h"
#include "verify.h"

// global pointer to the container
std::unordered_set<int>* observer_unordered_set = NULL;
const std::unordered_set<int>* const_observer_unordered_set = NULL;

void observer_tests(int id)
{
    // print simple output
    global_barrier->arrive(id);
    if (id == 0)
        printf("Testing observers (3)\n");

    // the first test is hash_function (1)
    global_barrier->arrive(id);
    {
        verifier v;
        int size;
	std::unordered_set<int>::hasher hs;
        BEGIN_TX;
        observer_unordered_set = new std::unordered_set<int>({1,2,3,4,9,8,7,6});
	hs = observer_unordered_set->hash_function();
	size = observer_unordered_set->size();
        v.insert_all<std::unordered_set<int>>(observer_unordered_set);
        delete(observer_unordered_set);
        observer_unordered_set = NULL;
        END_TX;

	v.check_size("hash_function (1)", id, size);
    }

    // the second test is key_eq (1)
    global_barrier->arrive(id);
    {
        verifier v;
        int size;
	bool equal = false;
        BEGIN_TX;
        observer_unordered_set = new std::unordered_set<int>({1,2,3,4,9,8,7,6});
	equal = observer_unordered_set->key_eq()(3,6);
	size = observer_unordered_set->size();
        v.insert_all<std::unordered_set<int>>(observer_unordered_set);
        delete(observer_unordered_set);
        observer_unordered_set = NULL;
        END_TX;

	v.check_size("key_eq (1)", id, size);
    }

    // the third test is get_allocator (1)
    global_barrier->arrive(id);
    {
        verifier v;
        int size;
	bool equal = false;
        BEGIN_TX;
        observer_unordered_set = new std::unordered_set<int>({1,2,3,4,9,8,7,6});
	auto a = observer_unordered_set->get_allocator();
	size = observer_unordered_set->size();
        v.insert_all<std::unordered_set<int>>(observer_unordered_set);
        delete(observer_unordered_set);
        observer_unordered_set = NULL;
        END_TX;

	v.check_size("get_allocator (1)", id, size);
    }
}
