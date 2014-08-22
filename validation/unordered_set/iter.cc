#include <iostream>
#include <string>
#include <unordered_set>
#include <cassert>
#include "tests.h"
#include "verify.h"

// global pointer to the container
std::unordered_set<int>* iter_unordered_set = NULL;
const std::unordered_set<int>* const_iter_unordered_set = NULL;

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
	int sum = 0;
        BEGIN_TX;
        const_iter_unordered_set = new std::unordered_set<int>({1,2,3,4,9,8,7,6});
	size = const_iter_unordered_set->size();
	for (auto local_itr = const_iter_unordered_set->begin(); local_itr != const_iter_unordered_set->end(); ++ local_itr)
	{
		sum += *local_itr;
	}

	v.insert_all<std::unordered_set<int>>(const_iter_unordered_set);
        delete(const_iter_unordered_set);
        const_iter_unordered_set = NULL;
        END_TX;

	v.check_size("begin, end (1b)", id, size);
    }

    // the third test is begin, end (2a)
    global_barrier->arrive(id);
    {
        verifier v;
        int size;
	int sum = 0;
        BEGIN_TX;
        iter_unordered_set = new std::unordered_set<int>({1,2,3,4,9,8,7,6}, 3);
	size = iter_unordered_set->size();
	for ( unsigned i = 0; i < iter_unordered_set->bucket_count(); ++i)
	{
		for (auto local_it = iter_unordered_set->begin(i); local_it != iter_unordered_set->end(i); ++local_it)
		{
			sum += *local_it;
		}
	}

	v.insert_all<std::unordered_set<int>>(iter_unordered_set);
        delete(iter_unordered_set);
        iter_unordered_set = NULL;
        END_TX;

	v.check_size("begin, end (2a)", id, size);
    }
    
    // the third test is begin, end (2b)
    global_barrier->arrive(id);
    {
        verifier v;
        int size;
	int sum = 0;
        BEGIN_TX;
        const_iter_unordered_set = new std::unordered_set<int>({1,2,3,4,9,8,7,6}, 3);
	size = const_iter_unordered_set->size();
	for ( unsigned i = 0; i < const_iter_unordered_set->bucket_count(); ++i)
	{
		for (auto local_it = const_iter_unordered_set->begin(i); local_it != const_iter_unordered_set->end(i); ++local_it)
		{
			sum += *local_it;
		}
	}

	v.insert_all<std::unordered_set<int>>(const_iter_unordered_set);
        delete(const_iter_unordered_set);
        const_iter_unordered_set = NULL;
        END_TX;

	v.check_size("begin, end (2b)", id, size);
    }

    // the second test is cbegin, cend (1)
    global_barrier->arrive(id);
    {
        verifier v;
        int size;
	int sum = 0;
        BEGIN_TX;
        const_iter_unordered_set = new std::unordered_set<int>({1,2,3,4,9,8,7,6});
	size = const_iter_unordered_set->size();
	for (auto local_itr = const_iter_unordered_set->cbegin(); local_itr != const_iter_unordered_set->cend(); ++ local_itr)
	{
		sum += *local_itr;
	}

	v.insert_all<std::unordered_set<int>>(const_iter_unordered_set);
        delete(const_iter_unordered_set);
        const_iter_unordered_set = NULL;
        END_TX;

	v.check_size("cbegin, cend (1)", id, size);
    }

    // the third test is cbegin, cend (2)
    global_barrier->arrive(id);
    {
        verifier v;
        int size;
	int sum = 0;
        BEGIN_TX;
        const_iter_unordered_set = new std::unordered_set<int>({1,2,3,4,9,8,7,6}, 3);
	size = const_iter_unordered_set->size();
	for ( unsigned i = 0; i < const_iter_unordered_set->bucket_count(); ++i)
	{
		for (auto local_it = const_iter_unordered_set->cbegin(i); local_it != const_iter_unordered_set->cend(i); ++local_it)
		{
			sum += *local_it;
		}
	}

	v.insert_all<std::unordered_set<int>>(const_iter_unordered_set);
        delete(const_iter_unordered_set);
        const_iter_unordered_set = NULL;
        END_TX;

	v.check_size("cbegin, cend (2)", id, size);
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
