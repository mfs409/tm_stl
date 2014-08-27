#include <unordered_set>
#include "tests.h"
#include "verify.h"

// global pointer to the container
std::unordered_set<int>* cap_unordered_set = NULL;
const std::unordered_set<int>* const_cap_unordered_set = NULL;

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
        cap_unordered_set = new std::unordered_set<int>({1,2,3,4,9,8,7,6});
        size = cap_unordered_set->size();
        empty = cap_unordered_set->empty();
        max_size = cap_unordered_set->max_size();

        v.insert_all<std::unordered_set<int>>(cap_unordered_set);
        delete(cap_unordered_set);
        cap_unordered_set = NULL;
        END_TX;

        v.check_size("empty, size, max_size (1)", id, size);
    }

}
