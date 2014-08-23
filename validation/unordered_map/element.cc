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

void element_tests(int id)
{
    global_barrier->arrive(id);
    if (id == 0)
        printf("Testing unordered map element access functions: [](2), at(2)\n");

    // Test [] with const key
    global_barrier->arrive(id);
    {
        int ans = -2;
        BEGIN_TX;
        member_map = new intmap({{1, 1}, {3, 3}, {2, 2}});
        const int x = 1;
        ans = (*member_map)[x];
        delete(member_map);
        END_TX;
        if (ans != 1)
            printf(" [%d] map operator[] test failed\n", id);
        else if (id == 0)
            printf(" [OK] %s\n", "map operator[](1a)");
    }

    // Test [] with move key
    global_barrier->arrive(id);
    {
        int ans = -2;
        BEGIN_TX;
        member_map = new intmap({{1, 1}, {3, 3}, {2, 2}});
        int x = 4;
        ans = (*member_map)[std::move(x)];
        delete(member_map);
        END_TX;
        if (ans != 0)
            printf(" [%d] map operator[] test failed\n", id);
        else if (id == 0)
            printf(" [OK] %s\n", "map operator[](1b)");
    }

    // Test at() with non-const map
    global_barrier->arrive(id);
    {
        int ans = -2;
        BEGIN_TX;
        member_map = new intmap({{1, 1}, {3, 3}, {2, 2}});
        ans = member_map->at(1);
        delete(member_map);
        END_TX;
        if (ans != 1)
            printf(" [%d] map at() test failed\n", id);
        else if (id == 0)
            printf(" [OK] %s\n", "map at(1a)");
    }

    // Test at() with const map
    global_barrier->arrive(id);
    {
        int ans = -2;
        BEGIN_TX;
        member_map = new intmap({{1, 1}, {3, 3}, {2, 2}});
        const intmap ce = *member_map;
        ans = ce.at(3);
        delete(member_map);
        END_TX;
        if (ans != 3)
            printf(" [%d] map at() test failed\n", id);
        else if (id == 0)
            printf(" [OK] %s\n", "map at(1b)");
    }
}
