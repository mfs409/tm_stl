#include <iostream>
#include <map>
#include "tests.h"
#include "verify.h"

/// The map we will use for our tests
std::map<int, int>* observer_map = NULL;

void observer_tests(int id)
{
    global_barrier->arrive(id);
    if (id == 0)
        printf("Testing get_allocator(1), key_comp(1), value_comp(1)\n");

    // test get_allocator
    global_barrier->arrive(id);
    {
        map_verifier v;
        int size;
        BEGIN_TX;
        observer_map = new std::map<int, int>();
        auto a = observer_map->get_allocator();
        size = observer_map->size();
        delete(observer_map);
        observer_map = NULL;
        END_TX;
        v.check_size("get_allocator (1)", id, size);
    }

    // test keycomp and value_comp
    global_barrier->arrive(id);
    {
        bool ok = true;
        int a;
        BEGIN_TX;
        observer_map = new std::map<int, int>();
        auto k = observer_map->key_comp();
        auto v = observer_map->value_comp();
        ok &= k(1, 2);
        ok &= v(std::make_pair(1, 1), std::make_pair(2, 2));
        delete(observer_map);
        END_TX;
        if (!ok)
            std::cout << "["<<id<<"] error with key_comp or value_comp" << std::endl;
        else if (id == 0)
            std::cout << " [OK] key_comp(1) and value_comp(1)" << std::endl;
    }
}
