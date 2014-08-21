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

void modifier_tests(int id)
{
    // test insertion of a pair (copy)
    global_barrier->arrive(id);
    {
        verifier v;
        BEGIN_TX;
        member_map = new intmap();
        member_map->insert({23, 23});
        v.insert_all<intmap>(member_map);
        delete(member_map);
        member_map = NULL;
        END_TX;
        v.check("insert pair (1)", id, 2, {42, 42});
    }

    // test insertion of a pair (move)
    global_barrier->arrive(id);
    {
        verifier v;
        BEGIN_TX;
        member_map = new intmap();
        member_map->insert(std::make_pair(23, 23));
        v.insert_all<intmap>(member_map);
        delete(member_map);
        member_map = NULL;
        END_TX;
        v.check("insert pair (2)", id, 2, {23, 23});
    }
}
