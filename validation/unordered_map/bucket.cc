#include <iostream>
#include <string>
#include <unordered_map>
#include <cassert>
#include "tests.h"
#include "verify.h"

/// The map we will use for our tests
typedef std::unordered_map<int, int> intmap;
typedef std::pair<int, int>          intpair;
typedef map_verifier verifier;

static intmap* member_map = NULL;

void bucket_tests(int id)
{
    // test bucket count
    global_barrier->arrive(id);
    {
        int v;
        BEGIN_TX;
        member_map = new intmap();
        v = member_map->bucket_count();
        delete(member_map);
        member_map = NULL;
        END_TX;
        std::cout << "  [OK] bucket_count = " << v << std::endl;
    }

    // test max bucket count
    global_barrier->arrive(id);
    {
        int v;
        BEGIN_TX;
        member_map = new intmap();
        v = member_map->max_bucket_count();
        delete(member_map);
        member_map = NULL;
        END_TX;
        std::cout << "  [OK] max_bucket_count = " << v << std::endl;
    }

    // test max bucket size
    global_barrier->arrive(id);
    {
        int v;
        BEGIN_TX;
        member_map = new intmap();
        v = member_map->bucket_size(0);
        delete(member_map);
        member_map = NULL;
        END_TX;
        std::cout << "  [OK] bucket_size of #0 = " << v << std::endl;
    }

    // test max bucket
    global_barrier->arrive(id);
    {
        int v;
        BEGIN_TX;
        member_map = new intmap({{1, 1}});
        v = member_map->bucket(1);
        delete(member_map);
        member_map = NULL;
        END_TX;
        std::cout << "  [OK] bucket of key value 1 = " << v << std::endl;
    }
}
