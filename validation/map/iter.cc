#include <iostream>
#include <map>
#include "tests.h"
#include "verify.h"

std::map<int, int>* iter_map = NULL;

void iter_create_tests(int id)
{
    global_barrier->arrive(id);
    if (id == 0)
        printf("Testing iterator begin/end functions\n");

    // the first test will simply ensure that we can call begin() and end()
    // correctly
    global_barrier->arrive(id);
    {
        bool ok = false;
        map_verifier v;
        BEGIN_TX;
        iter_map = new std::map<int, int>();
        std::map<int, int>::iterator b = iter_map->begin();
        std::map<int, int>::iterator e = iter_map->end();
        ok = (b == e);
        delete(iter_map);
        iter_map = NULL;
        END_TX;
        if (!ok)
            printf(" [%d] iterator begin and end did not match for empty deque", id);
        else if (id == 0)
            printf(" [OK] %s\n", "basic iterator begin and end");
    }

    // now test the legacy const begin() and end() calls
    global_barrier->arrive(id);
    {
        bool ok = false;
        map_verifier v;
        BEGIN_TX;
        iter_map = new std::map<int, int>();
        const std::map<int, int>* cd = iter_map;
        std::map<int, int>::const_iterator b = cd->begin();
        std::map<int, int>::const_iterator e = cd->end();
        ok = (b == e);
        delete(iter_map);
        iter_map = NULL;
        END_TX;
        if (!ok)
            printf(" [%d] iterator begin and end did not match for empty deque", id);
        else if (id == 0)
            printf(" [OK] %s\n", "legacy const iterator begin and end");
    }

    // the first test will simply ensurec that we can call begin() and end()
    // correctly
    global_barrier->arrive(id);
    {
        bool ok = false;
        map_verifier v;
        BEGIN_TX;
        iter_map = new std::map<int, int>();
        std::map<int, int>::reverse_iterator b = iter_map->rbegin();
        std::map<int, int>::reverse_iterator e = iter_map->rend();
        ok = (b == e);
        delete(iter_map);
        iter_map = NULL;
        END_TX;
        if (!ok)
            printf(" [%d] iterator rbegin and rend did not match for empty deque", id);
        else if (id == 0)
            printf(" [OK] %s\n", "basic reverse iterator rbegin and rend");
    }

    // now test the legacy const rbegin() and rend() calls
    global_barrier->arrive(id);
    {
        bool ok = false;
        map_verifier v;
        BEGIN_TX;
        iter_map = new std::map<int, int>();
        const std::map<int, int>* cd = iter_map;
        std::map<int, int>::const_reverse_iterator b = cd->rbegin();
        std::map<int, int>::const_reverse_iterator e = cd->rend();
        ok = (b == e);
        delete(iter_map);
        iter_map = NULL;
        END_TX;
        if (!ok)
            printf(" [%d] iterator rbegin and rend did not match for empty deque", id);
        else if (id == 0)
            printf(" [OK] %s\n", "legacy const reverse iterator rbegin and rend");
    }

    // now test the c++11 const begin() and end() calls
    global_barrier->arrive(id);
    {
        bool ok = false;
        map_verifier v;
        BEGIN_TX;
        iter_map = new std::map<int, int>();
        const std::map<int, int>* cd = iter_map;
        std::map<int, int>::const_iterator b = cd->cbegin();
        std::map<int, int>::const_iterator e = cd->cend();
        ok = (b == e);
        delete(iter_map);
        iter_map = NULL;
        END_TX;
        if (!ok)
            printf(" [%d] iterator cbegin and cend did not match for empty deque", id);
        else if (id == 0)
            printf(" [OK] %s\n", "c++11 cbegin and cend");
    }

    // now test the c++11 crbegin() and crend() calls
    global_barrier->arrive(id);
    {
        bool ok = false;
        map_verifier v;
        BEGIN_TX;
        iter_map = new std::map<int, int>();
        const std::map<int, int>* cd = iter_map;
        std::map<int, int>::const_reverse_iterator b = cd->crbegin();
        std::map<int, int>::const_reverse_iterator e = cd->crend();
        ok = (b == e);
        delete(iter_map);
        iter_map = NULL;
        END_TX;
        if (!ok)
            printf(" [%d] iterator crbegin and crend did not match for empty deque", id);
        else if (id == 0)
            printf(" [OK] %s\n", "c++11 crbegin and crend");
    }
}

void iter_method_tests(int id)
{
}

void iter_operator_tests(int id)
{
}

void iter_overload_tests(int id)
{
}

void iter_function_tests(int id)
{
}
