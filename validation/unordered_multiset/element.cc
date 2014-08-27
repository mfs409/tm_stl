#include <unordered_set>
#include "tests.h"
#include "verify.h"

// global pointer to the container
std::unordered_multiset<int>* element_unordered_multiset = NULL;
const std::unordered_multiset<int>* const_element_unordered_multiset = NULL;

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
        element_unordered_multiset = new std::unordered_multiset<int>({1,2,3,4,9,8,7,6});
        size = element_unordered_multiset->size();
        element_unordered_multiset->find(6);
        v.insert_all<std::unordered_multiset<int>>(element_unordered_multiset);
        delete(element_unordered_multiset);
        element_unordered_multiset = NULL;
        END_TX;

        v.check_size("find (1a)", id, size);
    }

    // the second test is find (1b)
    global_barrier->arrive(id);
    {
        verifier v;
        int size;
        BEGIN_TX;
        const_element_unordered_multiset = new std::unordered_multiset<int>({1,2,3,4,9,8,7,6});
        size = const_element_unordered_multiset->size();
        const_element_unordered_multiset->find(6);
        v.insert_all<std::unordered_multiset<int>>(const_element_unordered_multiset);
        delete(const_element_unordered_multiset);
        const_element_unordered_multiset = NULL;
        END_TX;

        v.check_size("find (1b)", id, size);
    }

    // the third test is count (1)
    global_barrier->arrive(id);
    {
        verifier v;
        int size;
        BEGIN_TX;
        element_unordered_multiset = new std::unordered_multiset<int>({1,2,3,4,9,8,7,6});
        size = element_unordered_multiset->size();
        element_unordered_multiset->count(6);
        v.insert_all<std::unordered_multiset<int>>(element_unordered_multiset);
        delete(element_unordered_multiset);
        element_unordered_multiset = NULL;
        END_TX;

        v.check_size("count (1)", id, size);
    }

    // the forth test is equal_range (1a)
    global_barrier->arrive(id);
    {
        verifier v;
        int size;
        BEGIN_TX;
        element_unordered_multiset = new std::unordered_multiset<int>({1,2,3,4,9,8,7,6});
        size = element_unordered_multiset->size();
        element_unordered_multiset->equal_range(6);
        v.insert_all<std::unordered_multiset<int>>(element_unordered_multiset);
        delete(element_unordered_multiset);
        element_unordered_multiset = NULL;
        END_TX;

        v.check_size("equal range (1a)", id, size);
    }

    // the fifth test is equal_range (1b)
    global_barrier->arrive(id);
    {
        verifier v;
        int size;
        BEGIN_TX;
        const_element_unordered_multiset = new std::unordered_multiset<int>({1,2,3,4,9,8,7,6});
        size = const_element_unordered_multiset->size();
        const_element_unordered_multiset->equal_range(6);
        v.insert_all<std::unordered_multiset<int>>(const_element_unordered_multiset);
        delete(const_element_unordered_multiset);
        const_element_unordered_multiset = NULL;
        END_TX;

        v.check_size("equal_range (1b)", id, size);
    }
}
