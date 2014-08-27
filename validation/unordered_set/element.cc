#include <unordered_set>
#include "tests.h"
#include "verify.h"

// global pointer to the container
std::unordered_set<int>* element_unordered_set = NULL;
const std::unordered_set<int>* const_element_unordered_set = NULL;

void element_tests(int id)
{
    // print simple output
    global_barrier->arrive(id);
    if (id == 0)
        printf("Testing elements (5)\n");

    // the first test is find (1a)
    global_barrier->arrive(id);
    {
        verifier v;
        int size;
        BEGIN_TX;
        element_unordered_set = new std::unordered_set<int>({1,2,3,4,9,8,7,6});
        size = element_unordered_set->size();
        element_unordered_set->find(6);
        v.insert_all<std::unordered_set<int>>(element_unordered_set);
        delete(element_unordered_set);
        element_unordered_set = NULL;
        END_TX;

        v.check_size("find (1a)", id, size);
    }

    // the second test is find (1b)
    global_barrier->arrive(id);
    {
        verifier v;
        int size;
        BEGIN_TX;
        const_element_unordered_set = new std::unordered_set<int>({1,2,3,4,9,8,7,6});
        size = const_element_unordered_set->size();
        const_element_unordered_set->find(6);
        v.insert_all<std::unordered_set<int>>(const_element_unordered_set);
        delete(const_element_unordered_set);
        const_element_unordered_set = NULL;
        END_TX;

        v.check_size("find (1b)", id, size);
    }

    // the third test is count (1)
    global_barrier->arrive(id);
    {
        verifier v;
        int size;
        BEGIN_TX;
        element_unordered_set = new std::unordered_set<int>({1,2,3,4,9,8,7,6});
        size = element_unordered_set->size();
        element_unordered_set->count(6);
        v.insert_all<std::unordered_set<int>>(element_unordered_set);
        delete(element_unordered_set);
        element_unordered_set = NULL;
        END_TX;

        v.check_size("count (1)", id, size);
    }

    // the forth test is equal_range (1a)
    global_barrier->arrive(id);
    {
        verifier v;
        int size;
        BEGIN_TX;
        element_unordered_set = new std::unordered_set<int>({1,2,3,4,9,8,7,6});
        size = element_unordered_set->size();
        element_unordered_set->equal_range(6);
        v.insert_all<std::unordered_set<int>>(element_unordered_set);
        delete(element_unordered_set);
        element_unordered_set = NULL;
        END_TX;

        v.check_size("equal range (1a)", id, size);
    }

    // the fifth test is equal_range (1b)
    global_barrier->arrive(id);
    {
        verifier v;
        int size;
        BEGIN_TX;
        const_element_unordered_set = new std::unordered_set<int>({1,2,3,4,9,8,7,6});
        size = const_element_unordered_set->size();
        const_element_unordered_set->equal_range(6);
        v.insert_all<std::unordered_set<int>>(const_element_unordered_set);
        delete(const_element_unordered_set);
        const_element_unordered_set = NULL;
        END_TX;

        v.check_size("equal_range (1b)", id, size);
    }
}
