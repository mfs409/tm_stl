#include <iostream>
#include <map>
#include "tests.h"
#include "verify.h"

/// The maps we will use for our tests
std::map<int, int>* cap_map = NULL;

/// test the capacity methods of std::map
void cap_tests(int id)
{
    global_barrier->arrive(id);
    if (id == 0)
        printf("Testing map capacity functions: size(1), max_size(1), empty(1)\n");

    // #1: Test size()
    global_barrier->arrive(id);
    {
        int size = 0;
        BEGIN_TX;
        cap_map = new std::map<int, int>({{1, 1}, {2, 2}, {3, 3}});
        size = cap_map->size();
        delete(cap_map);
        END_TX;
        if (size != 3)
            printf(" [%d] map size test failed\n", id);
        else if (id == 0)
            printf(" [OK] %s\n", "map size()");
    }

    // #2: Test max_size()
    global_barrier->arrive(id);
    {
        int size = 0;
        BEGIN_TX;
        cap_map = new std::map<int, int>({{1, 1}, {2, 2}, {3, 3}});
        size = cap_map->max_size();
        delete(cap_map);
        END_TX;
        if (size < 1024)
            printf(" [%d] map max_size test failed %d\n", id, size);
        else if (id == 0)
            printf(" [OK] map::max_size == %d\n", size);
    }

    // #3: test empty()
    global_barrier->arrive(id);
    {
        bool ok = true;
        BEGIN_TX;
        cap_map = new std::map<int, int>({{1, 1}, {2, 2}, {3, 3}});
        ok &= !cap_map->empty();
        cap_map->clear();
        ok &= cap_map->empty();
        delete(cap_map);
        END_TX;
        if (!ok)
            printf(" [%d] map empty test failed\n", id);
        else if (id == 0)
            printf(" [OK] %s\n", "map empty()");
    }
}
