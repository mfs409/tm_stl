#include <iostream>
#include <unordered_set>
#include "tests.h"
#include "verify.h"

// global pointer to the container
std::unordered_multiset<int>* iter_unordered_multiset = NULL;
const std::unordered_multiset<int>* const_iter_unordered_multiset = NULL;

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
        iter_unordered_multiset = new std::unordered_multiset<int>({1,2,3,4,9,8,7,6});
        size = iter_unordered_multiset->size();
        v.insert_all<std::unordered_multiset<int>>(iter_unordered_multiset);
        delete(iter_unordered_multiset);
        iter_unordered_multiset = NULL;
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
        const_iter_unordered_multiset = new std::unordered_multiset<int>({1,2,3,4,9,8,7,6});
        size = const_iter_unordered_multiset->size();
        for (auto local_itr = const_iter_unordered_multiset->begin(); local_itr != const_iter_unordered_multiset->end(); ++ local_itr)
        {
            sum += *local_itr;
        }

        v.insert_all<std::unordered_multiset<int>>(const_iter_unordered_multiset);
        delete(const_iter_unordered_multiset);
        const_iter_unordered_multiset = NULL;
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
        iter_unordered_multiset = new std::unordered_multiset<int>({1,2,3,4,9,8,7,6}, 3);
        size = iter_unordered_multiset->size();
        for ( unsigned i = 0; i < iter_unordered_multiset->bucket_count(); ++i)
        {
            for (auto local_it = iter_unordered_multiset->begin(i); local_it != iter_unordered_multiset->end(i); ++local_it)
            {
                sum += *local_it;
            }
        }

        v.insert_all<std::unordered_multiset<int>>(iter_unordered_multiset);
        delete(iter_unordered_multiset);
        iter_unordered_multiset = NULL;
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
        const_iter_unordered_multiset = new std::unordered_multiset<int>({1,2,3,4,9,8,7,6}, 3);
        size = const_iter_unordered_multiset->size();
        for ( unsigned i = 0; i < const_iter_unordered_multiset->bucket_count(); ++i)
        {
            for (auto local_it = const_iter_unordered_multiset->begin(i); local_it != const_iter_unordered_multiset->end(i); ++local_it)
            {
                sum += *local_it;
            }
        }

        v.insert_all<std::unordered_multiset<int>>(const_iter_unordered_multiset);
        delete(const_iter_unordered_multiset);
        const_iter_unordered_multiset = NULL;
        END_TX;

        v.check_size("begin, end (2b)", id, size);
    }

    // cbegin, cend (1)
    global_barrier->arrive(id);
    {
        verifier v;
        int size;
        int sum = 0;
        BEGIN_TX;
        const_iter_unordered_multiset = new std::unordered_multiset<int>({1,2,3,4,9,8,7,6});
        size = const_iter_unordered_multiset->size();
        for (auto local_itr = const_iter_unordered_multiset->cbegin(); local_itr != const_iter_unordered_multiset->cend(); ++ local_itr)
        {
            sum += *local_itr;
        }

        v.insert_all<std::unordered_multiset<int>>(const_iter_unordered_multiset);
        delete(const_iter_unordered_multiset);
        const_iter_unordered_multiset = NULL;
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
        const_iter_unordered_multiset = new std::unordered_multiset<int>({1,2,3,4,9,8,7,6}, 3);
        size = const_iter_unordered_multiset->size();
        for ( unsigned i = 0; i < const_iter_unordered_multiset->bucket_count(); ++i)
        {
            for (auto local_it = const_iter_unordered_multiset->cbegin(i); local_it != const_iter_unordered_multiset->cend(i); ++local_it)
            {
                sum += *local_it;
            }
        }

        v.insert_all<std::unordered_multiset<int>>(const_iter_unordered_multiset);
        delete(const_iter_unordered_multiset);
        const_iter_unordered_multiset = NULL;
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
