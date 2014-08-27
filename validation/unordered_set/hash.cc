#include <unordered_set>
#include "tests.h"
#include "verify.h"

// global pointer to the container
std::unordered_set<int>* hash_unordered_set = NULL;
const std::unordered_set<int>* const_hash_unordered_set = NULL;

void hash_tests(int id)
{
    // print simple output
    global_barrier->arrive(id);
    if (id == 0)
        printf("Testing hash (5)\n");

    // the first test is load_factor (1)
    global_barrier->arrive(id);
    {
        verifier v;
        int size;
        float load_factor;
        BEGIN_TX;
        hash_unordered_set = new std::unordered_set<int>({1,2,3,4,9,8,7,6});
        load_factor = hash_unordered_set->load_factor();
        size = hash_unordered_set->size();
        v.insert_all<std::unordered_set<int>>(hash_unordered_set);
        delete(hash_unordered_set);
        hash_unordered_set = NULL;
        END_TX;

        v.check_size("load_factor (1)", id, size);
    }

    // the second test is max_load_factor (1)
    global_barrier->arrive(id);
    {
        verifier v;
        int size;
        float max_load_factor;
        BEGIN_TX;
        hash_unordered_set = new std::unordered_set<int>({1,2,3,4,9,8,7,6});
        max_load_factor = hash_unordered_set->max_load_factor();
        size = hash_unordered_set->size();
        v.insert_all<std::unordered_set<int>>(hash_unordered_set);
        delete(hash_unordered_set);
        hash_unordered_set = NULL;
        END_TX;

        v.check_size("max_load_factor get(1)", id, size);
    }

    // the third test is max_load_factor (2)
    global_barrier->arrive(id);
    {
        verifier v;
        int size;
        float max_load_factor = 0.5;
        BEGIN_TX;
        hash_unordered_set = new std::unordered_set<int>({1,2,3,4,9,8,7,6});
        hash_unordered_set->max_load_factor(max_load_factor);
        size = hash_unordered_set->size();
        v.insert_all<std::unordered_set<int>>(hash_unordered_set);
        delete(hash_unordered_set);
        hash_unordered_set = NULL;
        END_TX;

        v.check_size("max_load_factor set(2)", id, size);
    }

    // the forth test is rehash (1)
    global_barrier->arrive(id);
    {
        verifier v;
        int size;
        BEGIN_TX;
        hash_unordered_set = new std::unordered_set<int>({1,2,3,4,9,8,7,6});
        hash_unordered_set->rehash(20);
        size = hash_unordered_set->size();
        v.insert_all<std::unordered_set<int>>(hash_unordered_set);
        delete(hash_unordered_set);
        hash_unordered_set = NULL;
        END_TX;

        v.check_size("rehash (1)", id, size);
    }

    // the fifth test is reserve (1)
    global_barrier->arrive(id);
    {
        verifier v;
        int size;
        BEGIN_TX;
        hash_unordered_set = new std::unordered_set<int>({1,2,3,4,9,8,7,6});
        hash_unordered_set->reserve(20);
        hash_unordered_set->insert({11,13,17,19,23,29,31,37});
        size = hash_unordered_set->size();
        v.insert_all<std::unordered_set<int>>(hash_unordered_set);
        delete(hash_unordered_set);
        hash_unordered_set = NULL;
        END_TX;

        v.check_size("reserve (1)", id, size);
    }
}
