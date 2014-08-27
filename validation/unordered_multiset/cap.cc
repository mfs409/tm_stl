#include <iostream>
#include <unordered_set>
#include "tests.h"
#include "verify.h"

// global pointer to the container
std::unordered_multiset<int>* cap_unordered_multiset = NULL;
const std::unordered_multiset<int>* const_cap_unordered_multiset = NULL;

void cap_tests(int id)
{
    // print simple output
    global_barrier->arrive(id);
    if (id == 0)
        printf("Testing capacity (3)\n");

    // the first test is empty, size, and max_size (1)
    global_barrier->arrive(id);
    {
        verifier v;
        int size;
    bool empty;
    int max_size;
        BEGIN_TX;
        cap_unordered_multiset = new std::unordered_multiset<int>({1,2,3,4,9,8,7,6});
    size = cap_unordered_multiset->size();
    empty = cap_unordered_multiset->empty();
    max_size = cap_unordered_multiset->max_size();

        v.insert_all<std::unordered_multiset<int>>(cap_unordered_multiset);
        delete(cap_unordered_multiset);
        cap_unordered_multiset = NULL;
        END_TX;

    v.check_size("empty, size, max_size (1)", id, size);
    }

}
