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

void cap_tests(int id)
{
    // test empty
    global_barrier->arrive(id);
    {
        int v;
        BEGIN_TX;
        member_map = new intmap();
        v = member_map->empty();
        delete(member_map);
        member_map = NULL;
        END_TX;
        std::cout << "  [OK] empty = " << v << std::endl;
    }

    // test size
    global_barrier->arrive(id);
    {
        int v;
        BEGIN_TX;
        member_map = new intmap();
        v = member_map->size();
        delete(member_map);
        member_map = NULL;
        END_TX;
        std::cout << "  [OK] size = " << v << std::endl;
    }

    // test max size
    global_barrier->arrive(id);
    {
        int v;
        BEGIN_TX;
        member_map = new intmap();
        v = member_map->max_size();
        delete(member_map);
        member_map = NULL;
        END_TX;
        std::cout << "  [OK] max_size = " << v << std::endl;
    }
}
