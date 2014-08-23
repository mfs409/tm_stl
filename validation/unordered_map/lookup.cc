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

void lookup_tests(int id)
{
    // test find
    global_barrier->arrive(id);
    {
        intmap::iterator v;
        BEGIN_TX;
        member_map = new intmap({{1, 2}});
        v = member_map->find(1);
        delete(member_map);
        member_map = NULL;
        END_TX;
        std::cout << "  [OK] find value of key 1 = " << v->second << std::endl;
    }

    // test constant find
    global_barrier->arrive(id);
    {
        intmap::const_iterator v;
        BEGIN_TX;
        member_map = new intmap({{1, 2}});
        v = ((const intmap *)member_map)->find(1);
        delete(member_map);
        member_map = NULL;
        END_TX;
        std::cout << "  [OK] find value of key 1 = " << v->second << std::endl;
    }

    // test count
    global_barrier->arrive(id);
    {
        int v;
        BEGIN_TX;
        member_map = new intmap();
        v = member_map->count(0);
        delete(member_map);
        member_map = NULL;
        END_TX;
        std::cout << "  [OK] count of key 0 = " << v << std::endl;
    }

    // test equal range
    global_barrier->arrive(id);
    {
        std::pair<intmap::iterator, intmap::iterator> v;
        BEGIN_TX;
        member_map = new intmap({{1, 2}});
        v = member_map->equal_range(1);
        delete(member_map);
        member_map = NULL;
        END_TX;
        std::cout << "  [OK] equal range (1) " << v.first->first << std::endl;
    }

    // test equal range const
    global_barrier->arrive(id);
    {
        std::pair<intmap::const_iterator, intmap::const_iterator> v;
        BEGIN_TX;
        member_map = new intmap({{1, 2}});
        v = ((const intmap *)member_map)->equal_range(1);
        delete(member_map);
        member_map = NULL;
        END_TX;
        std::cout << "  [OK] equal range (2) " << v.first->first << std::endl;
    }
}
