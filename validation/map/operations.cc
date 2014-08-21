#include <iostream>
#include <map>
#include "tests.h"
#include "verify.h"

/// The maps we will use for our tests
std::map<int, int>* operations_map = NULL;

void operation_tests(int id)
{
    global_barrier->arrive(id);
    if (id == 0)
        printf("Testing map operations: find(2), count(1), "
               "lower_bound(2), upper_bound(), equal_range(2)\n");

    // test find(1a)
    global_barrier->arrive(id);
    {
        map_verifier v;
        bool ok = true;
        BEGIN_TX;
        operations_map = new std::map<int, int>({{1, 1}, {2, 2}, {4, 4}, {8, 8}});
        auto i = operations_map->find(2);
        ok &= i->second == 2;
        delete(operations_map);
        END_TX;
        if (!ok)
            std::cout << "["<<id<<"] error with find(1a)" << std::endl;
        else if (id == 0)
            std::cout << " [OK] find(1a)" << std::endl;
    }

    // test find(1b) with const iterator
    global_barrier->arrive(id);
    {
        map_verifier v;
        bool ok = true;
        BEGIN_TX;
        operations_map = new std::map<int, int>({{1, 1}, {2, 2}, {4, 4}, {8, 8}});
        const std::map<int, int>* c = operations_map;
        auto i = c->find(2);
        ok &= i->second == 2;
        delete(operations_map);
        END_TX;
        if (!ok)
            std::cout << "["<<id<<"] error with find(1a)" << std::endl;
        else if (id == 0)
            std::cout << " [OK] find(1b)" << std::endl;
    }

    // test count(1)
    global_barrier->arrive(id);
    {
        map_verifier v;
        int size, size2;
        BEGIN_TX;
        operations_map = new std::map<int, int>({{1, 1}, {2, 2}, {4, 4}, {8, 8}});
        size = operations_map->count(1);
        size2 = operations_map->count(9);
        delete(operations_map);
        END_TX;
        if (size != 1 || size2 != 0)
            std::cout << "["<<id<<"] error with count(1)" << std::endl;
        else if (id == 0)
            std::cout << " [OK] count(1)" << std::endl;
    }

    // test lower_bound(1a) and upper_bound(1a)
    global_barrier->arrive(id);
    {
        map_verifier v;
        BEGIN_TX;
        operations_map = new std::map<int, int>({{1, 1}, {2, 2}, {4, 4}, {8, 8}});
        auto l = operations_map->lower_bound(2);
        auto u = operations_map->upper_bound(7);
        operations_map->erase(l, u);
        v.insert_all(operations_map);
        delete(operations_map);
        END_TX;
        v.check(" lower_bound(1a) and upper_bound(1a)", id, 4, {1, 1, 8, 8});
    }

    // test lower_bound(1b) and upper_bound(1b)
    global_barrier->arrive(id);
    {
        map_verifier v;
        BEGIN_TX;
        operations_map = new std::map<int, int>({{1, 1}, {2, 2}, {4, 4}, {8, 8}});
        const std::map<int, int>* c = operations_map;
        auto l = c->lower_bound(2);
        auto u = c->upper_bound(7);
        operations_map->erase(l, u);
        v.insert_all(operations_map);
        delete(operations_map);
        END_TX;
        v.check(" lower_bound(1b) and upper_bound(1b)", id, 4, {1, 1, 8, 8});
    }

    // test equal_range(1a, 1b)
    global_barrier->arrive(id);
    {
        map_verifier v;
        BEGIN_TX;
        operations_map = new std::map<int, int>({{1, 1}, {2, 2}, {4, 4}, {8, 8}});
        const std::map<int, int>* c = operations_map;
        auto nt = operations_map->equal_range(2);
        operations_map->erase(nt.first, nt.second);
        auto ct = c->equal_range(8);
        operations_map->erase(ct.first, ct.second);
        v.insert_all(operations_map);
        delete(operations_map);
        END_TX;
        v.check(" equal_range(1a, 1b)", id, 4, {1, 1, 4, 4});
    }
}
