#include <iostream>
#include <map>
#include "tests.h"
#include "verify.h"

std::map<int, int>* overload_map = NULL;

void overload_tests(int id)
{

    global_barrier->arrive(id);
    if (id == 0)
        printf("Testing map relational operators ==, !=, <, >, <=, >=\n");

    // Test all operators in a single transaction
    global_barrier->arrive(id);
    {
        bool t1, t2, t3, t4, t5, t6;
        BEGIN_TX;
        overload_map = new std::map<int, int>({{1, 1}, {2, 2}, {3, 3}});
        std::map<int, int> l2({{1, 1}, {2, 2}, {3, 3}});
        std::map<int, int> l3({{2, 2}, {3, 3}, {4, 4}});
        t1 = *overload_map == l2;
        t2 = l2 != l3;
        t3 = l2 < l3;
        t4 = l3 > l2;
        t5 = *overload_map >= l2;
        t6 = *overload_map <= l2;
        delete overload_map;
        END_TX;
        if (!(t1 && t2 && t3 && t4 && t5 && t6))
            std::cout << "["<<id<<"] error on relational tests" << std::endl;
        else if (id == 0)
            std::cout << " [OK] relational tests" << std::endl;
    }

    global_barrier->arrive(id);
    if (id == 0)
        printf("Testing map swap\n");

    // Test swap
    global_barrier->arrive(id);
    {
        map_verifier v;
        BEGIN_TX;
        overload_map = new std::map<int, int>({{1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5}});
        std::map<int, int> d2({{7, 7}, {8, 8}, {9, 9}});
        swap(*overload_map, d2);
        v.insert_all(overload_map);
        delete(overload_map);
        END_TX;
        v.check("std::swap (1)", id, 6, {7, 7, 8, 8, 9, 9});
    }
}

