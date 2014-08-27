#include <unordered_set>
#include "tests.h"
#include "verify.h"

// global pointer to the container
std::unordered_multiset<int>* bucket_unordered_multiset = NULL;
const std::unordered_multiset<int>* const_bucket_unordered_multiset = NULL;

void bucket_tests(int id)
{
    // print simple output
    global_barrier->arrive(id);
    if (id == 0)
        printf("Testing buckets (4)\n");

    // the first test is bucket_count (1)
    global_barrier->arrive(id);
    {
        verifier v;
        int size;
        int nbuckets;
        BEGIN_TX;
        bucket_unordered_multiset = new std::unordered_multiset<int>({1,2,3,4,9,8,7,6});
        nbuckets = bucket_unordered_multiset->bucket_count();
        size = bucket_unordered_multiset->size();
        v.insert_all<std::unordered_multiset<int>>(bucket_unordered_multiset);
        delete(bucket_unordered_multiset);
        bucket_unordered_multiset = NULL;
        END_TX;

        v.check_size("bucket_count (1)", id, size);
    }

    // the second test is max_bucket_count (1)
    global_barrier->arrive(id);
    {
        verifier v;
        int size;
        int max_nbuckets;
        BEGIN_TX;
        bucket_unordered_multiset = new std::unordered_multiset<int>({1,2,3,4,9,8,7,6});
        max_nbuckets = bucket_unordered_multiset->max_bucket_count();
        size = bucket_unordered_multiset->size();
        v.insert_all<std::unordered_multiset<int>>(bucket_unordered_multiset);
        delete(bucket_unordered_multiset);
        bucket_unordered_multiset = NULL;
        END_TX;

        v.check_size("max_bucket_count (1)", id, size);
    }

    // the third test is bucket_size (1)
    global_barrier->arrive(id);
    {
        verifier v;
        int size;
        int max_bucket_size = -1;
        BEGIN_TX;
        bucket_unordered_multiset = new std::unordered_multiset<int>({1,2,3,4,9,8,7,6});
        int bucket_count = bucket_unordered_multiset->bucket_count();
        for (int i = 0; i < bucket_count; ++i)
        {
            int bucket_size_i = bucket_unordered_multiset->bucket_size(i);
            if (max_bucket_size < bucket_size_i)
                max_bucket_size = bucket_size_i;
        }
        size = bucket_unordered_multiset->size();
        v.insert_all<std::unordered_multiset<int>>(bucket_unordered_multiset);
        delete(bucket_unordered_multiset);
        bucket_unordered_multiset = NULL;
        END_TX;

        v.check_size("bucket_size (1)", id, size);
    }

    // the forth test is bucket (1)
    global_barrier->arrive(id);
    {
        verifier v;
        int size;
        int i_bucket;
        BEGIN_TX;
        bucket_unordered_multiset = new std::unordered_multiset<int>({1,2,3,4,9,8,7,6});
        i_bucket = bucket_unordered_multiset->bucket(9);
        size = bucket_unordered_multiset->size();
        v.insert_all<std::unordered_multiset<int>>(bucket_unordered_multiset);
        delete(bucket_unordered_multiset);
        bucket_unordered_multiset = NULL;
        END_TX;

        v.check_size("bucket (1)", id, size);
    }
}
