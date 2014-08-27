#include <unordered_set>
#include "tests.h"
#include "verify.h"

// global pointer to the container
std::unordered_multiset<int>* hash_unordered_multiset = NULL;
const std::unordered_multiset<int>* const_hash_unordered_multiset = NULL;

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
        hash_unordered_multiset = new std::unordered_multiset<int>({1,2,3,4,9,8,7,6});
        load_factor = hash_unordered_multiset->load_factor();
        size = hash_unordered_multiset->size();
        v.insert_all<std::unordered_multiset<int>>(hash_unordered_multiset);
        delete(hash_unordered_multiset);
        hash_unordered_multiset = NULL;
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
        hash_unordered_multiset = new std::unordered_multiset<int>({1,2,3,4,9,8,7,6});
        max_load_factor = hash_unordered_multiset->max_load_factor();
        size = hash_unordered_multiset->size();
        v.insert_all<std::unordered_multiset<int>>(hash_unordered_multiset);
        delete(hash_unordered_multiset);
        hash_unordered_multiset = NULL;
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
        hash_unordered_multiset = new std::unordered_multiset<int>({1,2,3,4,9,8,7,6});
        hash_unordered_multiset->max_load_factor(max_load_factor);
        size = hash_unordered_multiset->size();
        v.insert_all<std::unordered_multiset<int>>(hash_unordered_multiset);
        delete(hash_unordered_multiset);
        hash_unordered_multiset = NULL;
        END_TX;

        v.check_size("max_load_factor set(2)", id, size);
    }

    // the forth test is rehash (1)
    global_barrier->arrive(id);
    {
        verifier v;
        int size;
        BEGIN_TX;
        hash_unordered_multiset = new std::unordered_multiset<int>({1,2,3,4,9,8,7,6});
        hash_unordered_multiset->rehash(20);
        size = hash_unordered_multiset->size();
        v.insert_all<std::unordered_multiset<int>>(hash_unordered_multiset);
        delete(hash_unordered_multiset);
        hash_unordered_multiset = NULL;
        END_TX;

        v.check_size("rehash (1)", id, size);
    }

    // the fifth test is reserve (1)
    global_barrier->arrive(id);
    {
        verifier v;
        int size;
        BEGIN_TX;
        hash_unordered_multiset = new std::unordered_multiset<int>({1,2,3,4,9,8,7,6});
        hash_unordered_multiset->reserve(20);
        hash_unordered_multiset->insert({11,13,17,19,23,29,31,37});
        size = hash_unordered_multiset->size();
        v.insert_all<std::unordered_multiset<int>>(hash_unordered_multiset);
        delete(hash_unordered_multiset);
        hash_unordered_multiset = NULL;
        END_TX;

        v.check_size("reserve (1)", id, size);
    }
}
