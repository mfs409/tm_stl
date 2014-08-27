#include <unordered_set>
#include "tests.h"
#include "verify.h"

// global pointer to the container
std::unordered_multiset<int>* overloads_unordered_multiset = NULL;
const std::unordered_multiset<int>* const_overloads_unordered_multiset = NULL;

void relational_operator_tests(int id)
{
    // print simple output
    global_barrier->arrive(id);
    if (id == 0)
        printf("Testing overloads (2)\n");

    // the first test is operator== (1)
    global_barrier->arrive(id);
    {
        verifier v;
        int size;
        bool equal = false;
        BEGIN_TX;
        std::unordered_multiset<int> tmp({1,2,3,4,10,8,7,6});
        overloads_unordered_multiset = new std::unordered_multiset<int>({1,2,3,4,9,8,7,6});
        if (*overloads_unordered_multiset == tmp)
            equal = true;
        else
            equal = false;

        size = overloads_unordered_multiset->size();
        v.insert_all<std::unordered_multiset<int>>(overloads_unordered_multiset);
        delete(overloads_unordered_multiset);
        overloads_unordered_multiset = NULL;
        END_TX;

        v.check_size("operator== equality(1)", id, size);
    }

    // the second test is operator!= (2)
    global_barrier->arrive(id);
    {
        verifier v;
        int size;
        bool equal = false;
        BEGIN_TX;
        std::unordered_multiset<int> tmp({1,2,3,4,10,8,7,6});
        overloads_unordered_multiset = new std::unordered_multiset<int>({1,2,3,4,9,8,7,6});
        if (*overloads_unordered_multiset != tmp)
            equal = false;
        else
            equal = true;

        size = overloads_unordered_multiset->size();
        v.insert_all<std::unordered_multiset<int>>(overloads_unordered_multiset);
        delete(overloads_unordered_multiset);
        overloads_unordered_multiset = NULL;
        END_TX;

        v.check_size("operator!= inequality(2)", id, size);
    }
}

void swap_tests(int id)
{
    // the first test is swap (unordered_multiset) (1)
    global_barrier->arrive(id);
    {
        verifier v;
        int size;
        BEGIN_TX;
        std::unordered_multiset<int> tmp({11,13,17,19,23,29,31,37});
        overloads_unordered_multiset = new std::unordered_multiset<int>({1,2,3,4,9,8,7,6});
        swap(*overloads_unordered_multiset, tmp);
        size = overloads_unordered_multiset->size();
        v.insert_all<std::unordered_multiset<int>>(overloads_unordered_multiset);
        delete(overloads_unordered_multiset);
        overloads_unordered_multiset = NULL;
        END_TX;

        v.check_size("swap(unordered_multiset) (1)", id, size);
    }
}

