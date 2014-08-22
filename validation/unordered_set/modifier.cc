#include <iostream>
#include <string>
#include <unordered_set>
#include <cassert>
#include "tests.h"
#include "verify.h"

// global pointer to the container
std::unordered_set<int>* modifier_unordered_set = NULL;
const std::unordered_set<int>* const_modifier_unordered_set = NULL;

void modifier_tests(int id)
{
    // print simple output
    global_barrier->arrive(id);
    if (id == 0)
        printf("Testing modifiers (14)\n");

    // the first test is emplace (1)
    global_barrier->arrive(id);
    {
        verifier v;
        int size;
        BEGIN_TX;
        modifier_unordered_set = new std::unordered_set<int>({1,2,3,4,9,8,7,6});
	modifier_unordered_set->emplace(7);
	modifier_unordered_set->emplace(11);
	modifier_unordered_set->emplace(13);
	size = modifier_unordered_set->size();
        v.insert_all<std::unordered_set<int>>(modifier_unordered_set);
        delete(modifier_unordered_set);
        modifier_unordered_set = NULL;
        END_TX;

	v.check_size("emplace (1)", id, size);
    }

    // the second test is emplace_hint
    global_barrier->arrive(id);
    {
        verifier v;
        int size;
        BEGIN_TX;
        modifier_unordered_set = new std::unordered_set<int>({1,2,3,4,9,8,7,6});
	modifier_unordered_set->emplace_hint(modifier_unordered_set->begin(), 7);
	modifier_unordered_set->emplace_hint(modifier_unordered_set->begin(), 11);
	modifier_unordered_set->emplace_hint(modifier_unordered_set->begin(), 13);
	size = modifier_unordered_set->size();
        v.insert_all<std::unordered_set<int>>(modifier_unordered_set);
        delete(modifier_unordered_set);
        modifier_unordered_set = NULL;
        END_TX;

	v.check_size("emplace_hint (1)", id, size);
    }

    // the third test is insert (1)
    global_barrier->arrive(id);
    {
        verifier v;
        int size;
        BEGIN_TX;
        modifier_unordered_set = new std::unordered_set<int>({1,2,3,4,9,8,7,6});
	const int new_int = 11;
	modifier_unordered_set->insert(new_int);
	size = modifier_unordered_set->size();
        v.insert_all<std::unordered_set<int>>(modifier_unordered_set);
        delete(modifier_unordered_set);
        modifier_unordered_set = NULL;
        END_TX;

	v.check_size("insert (1)", id, size);
    }

    // the 4th test is insert (2)
    global_barrier->arrive(id);
    {
        verifier v;
        int size;
        BEGIN_TX;
        modifier_unordered_set = new std::unordered_set<int>({1,2,3,4,9,8,7,6});
	modifier_unordered_set->insert(11);
	size = modifier_unordered_set->size();
        v.insert_all<std::unordered_set<int>>(modifier_unordered_set);
        delete(modifier_unordered_set);
        modifier_unordered_set = NULL;
        END_TX;

	v.check_size("insert (2)", id, size);
    }

    // the 5th test is insert (3)
    global_barrier->arrive(id);
    {
        verifier v;
        int size;
        BEGIN_TX;
        modifier_unordered_set = new std::unordered_set<int>({1,2,3,4,9,8,7,6});
	const int new_int = 11;
	modifier_unordered_set->insert(modifier_unordered_set->begin(), new_int);
	size = modifier_unordered_set->size();
        v.insert_all<std::unordered_set<int>>(modifier_unordered_set);
        delete(modifier_unordered_set);
        modifier_unordered_set = NULL;
        END_TX;

	v.check_size("insert (3)", id, size);
    }

    // the 6th test is insert (4)
    global_barrier->arrive(id);
    {
        verifier v;
        int size;
        BEGIN_TX;
        modifier_unordered_set = new std::unordered_set<int>({1,2,3,4,9,8,7,6});
	modifier_unordered_set->insert(modifier_unordered_set->begin(), 11);
	size = modifier_unordered_set->size();
        v.insert_all<std::unordered_set<int>>(modifier_unordered_set);
        delete(modifier_unordered_set);
        modifier_unordered_set = NULL;
        END_TX;

	v.check_size("insert (4)", id, size);
    }

    // the 7th test is insert (5)
    global_barrier->arrive(id);
    {
        verifier v;
        int size;
        BEGIN_TX;
	std::unordered_set<int> tmp({11,13,17,19});
        modifier_unordered_set = new std::unordered_set<int>({1,2,3,4,9,8,7,6});
	modifier_unordered_set->insert(tmp.begin(), tmp.end());
	size = modifier_unordered_set->size();
        v.insert_all<std::unordered_set<int>>(modifier_unordered_set);
        delete(modifier_unordered_set);
        modifier_unordered_set = NULL;
        END_TX;

	v.check_size("insert (5)", id, size);
    }

    // the 8th test is insert (6)
    global_barrier->arrive(id);
    {
        verifier v;
        int size;
        BEGIN_TX;
        modifier_unordered_set = new std::unordered_set<int>({1,2,3,4,9,8,7,6});
	modifier_unordered_set->insert({11,13,17,19});
	size = modifier_unordered_set->size();
        v.insert_all<std::unordered_set<int>>(modifier_unordered_set);
        delete(modifier_unordered_set);
        modifier_unordered_set = NULL;
        END_TX;

	v.check_size("insert (6)", id, size);
    }

    // the 9th test is erase (1a)
    global_barrier->arrive(id);
    {
        verifier v;
        int size;
        BEGIN_TX;
        modifier_unordered_set = new std::unordered_set<int>({1,2,3,4,9,8,7,6});
	std::unordered_set<int>::const_iterator itr = modifier_unordered_set->begin();
	modifier_unordered_set->erase(itr);
	size = modifier_unordered_set->size();
        v.insert_all<std::unordered_set<int>>(modifier_unordered_set);
        delete(modifier_unordered_set);
        modifier_unordered_set = NULL;
        END_TX;

	v.check_size("erase by position(1a)", id, size);
    }

    // the 10th test is erase (1b)
    global_barrier->arrive(id);
    {
        verifier v;
        int size;
        BEGIN_TX;
        modifier_unordered_set = new std::unordered_set<int>({1,2,3,4,9,8,7,6});
	modifier_unordered_set->erase(modifier_unordered_set->begin());
	size = modifier_unordered_set->size();
        v.insert_all<std::unordered_set<int>>(modifier_unordered_set);
        delete(modifier_unordered_set);
        modifier_unordered_set = NULL;
        END_TX;

	v.check_size("erase by position(1b)", id, size);
    }

    // the 11th test is erase (2)
    global_barrier->arrive(id);
    {
        verifier v;
        int size;
        BEGIN_TX;
        modifier_unordered_set = new std::unordered_set<int>({1,2,3,4,9,8,7,6});
	modifier_unordered_set->erase(7);
	size = modifier_unordered_set->size();
        v.insert_all<std::unordered_set<int>>(modifier_unordered_set);
        delete(modifier_unordered_set);
        modifier_unordered_set = NULL;
        END_TX;

	v.check_size("erase by key(2)", id, size);
    }
    
    // the 12th test is erase (3)
    global_barrier->arrive(id);
    {
        verifier v;
        int size;
        BEGIN_TX;
        modifier_unordered_set = new std::unordered_set<int>({1,2,3,4,9,8,7,6});
	modifier_unordered_set->erase(++modifier_unordered_set->begin(), modifier_unordered_set->end());
	size = modifier_unordered_set->size();
        v.insert_all<std::unordered_set<int>>(modifier_unordered_set);
        delete(modifier_unordered_set);
        modifier_unordered_set = NULL;
        END_TX;

	v.check_size("erase range(3)", id, size);
    }

    // the 13th test is clear (1)
    global_barrier->arrive(id);
    {
        verifier v;
        int size;
        BEGIN_TX;
        modifier_unordered_set = new std::unordered_set<int>({1,2,3,4,9,8,7,6});
	modifier_unordered_set->clear();
	size = modifier_unordered_set->size();
        v.insert_all<std::unordered_set<int>>(modifier_unordered_set);
        delete(modifier_unordered_set);
        modifier_unordered_set = NULL;
        END_TX;

	v.check_size("clear (1)", id, size);
    }

    // the 13th test is swap (1)
    global_barrier->arrive(id);
    {
        verifier v;
        int size;
        BEGIN_TX;
	std::unordered_set<int> tmp({3,5,7,11});
        modifier_unordered_set = new std::unordered_set<int>({1,2,3,4,9,8,7,6});
	modifier_unordered_set->swap(tmp);
	size = modifier_unordered_set->size();
        v.insert_all<std::unordered_set<int>>(modifier_unordered_set);
        delete(modifier_unordered_set);
        modifier_unordered_set = NULL;
        END_TX;

	v.check_size("swap (1)", id, size);
    }
}
