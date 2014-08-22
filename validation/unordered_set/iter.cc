#include <iostream>
#include <string>
#include <unordered_set>
#include <cassert>
#include "tests.h"
#include "verify.h"

// global pointer to the container
std::unordered_set<int>* iter_unordered_set = NULL;

int iterator_value(std::unordered_set<int>::const_iterator ci)
{
	return *ci;
}

void iter_create_tests(int id)
{
    // print simple output
    global_barrier->arrive(id);
    if (id == 0)
        printf("Testing iterators (12)\n");

    // the first test is begin, end (1a)
    global_barrier->arrive(id);
    {
        verifier v;
        int size;
        BEGIN_TX;
        iter_unordered_set = new std::unordered_set<int>({1,2,3,4,9,8,7,6});
	size = iter_unordered_set->size();
        v.insert_all<std::unordered_set<int>>(iter_unordered_set);
        delete(iter_unordered_set);
        iter_unordered_set = NULL;
        END_TX;

	v.check_size("begin, end (1a)", id, size);
    }

    // the second test is begin, end (1b)
    global_barrier->arrive(id);
    {
        verifier v;
        int size;
        BEGIN_TX;
        iter_unordered_set = new std::unordered_set<int>({1,2,3,4,9,8,7,6});
	size = iter_unordered_set->size();
	std::unordered_set<int>::const_iterator begin_iter = iter_unordered_set->begin();
	std::unordered_set<int>::const_iterator end_iter = iter_unordered_set->end();
        iterator_value(begin_iter);
	iterator_value(end_iter);

	v.insert_all<std::unordered_set<int>>(iter_unordered_set);
        delete(iter_unordered_set);
        iter_unordered_set = NULL;
        END_TX;

	v.check_size("begin, end (1b)", id, size);
    }
}

void iter_method_tests(int id)
{

}

void iter_operator_tests(int id)
{

}

void iter_overload_tests(int id)
{

}

void iter_function_tests(int id)
{

}
