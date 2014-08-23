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

void hash_tests(int id)
{
    // test load factor
    global_barrier->arrive(id);
    {
        float v;
        BEGIN_TX;
        member_map = new intmap();
        v = member_map->load_factor();
        delete(member_map);
        member_map = NULL;
        END_TX;
        std::cout << "  [OK] load_factor = " << v << std::endl;
    }

    // test load factor
    global_barrier->arrive(id);
    {
        float v;
        BEGIN_TX;
        member_map = new intmap();
        member_map->max_load_factor(0.2);
        v = member_map->max_load_factor();
        delete(member_map);
        member_map = NULL;
        END_TX;
        std::cout << "  [OK] max_load_factor = " << v << std::endl;
    }

    // test rehash
    global_barrier->arrive(id);
    {
        int v;
        BEGIN_TX;
        member_map = new intmap();
        member_map->rehash(100);
        v = member_map->bucket_count();
        delete(member_map);
        member_map = NULL;
        END_TX;
        std::cout << "  [OK] bucket count after rehash = " << v << std::endl;
    }

    // test reserve
    global_barrier->arrive(id);
    {
        int v;
        BEGIN_TX;
        member_map = new intmap();
        member_map->reserve(100);
        v = member_map->bucket_count();
        delete(member_map);
        member_map = NULL;
        END_TX;
        std::cout << "  [OK] bucket count after reserve = " << v << std::endl;
    }
}
