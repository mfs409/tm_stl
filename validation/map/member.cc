#include <iostream>
#include <map>
#include "tests.h"
#include "verify.h"

/// The maps we will use for our tests
std::map<int, int>* member_map = NULL;
// NB: we must make the comparison function transaction_safe, or we'll get
//     lots of errors.  This is slightly annoying.
typedef bool(*safecomp)(int, int) __attribute__((transaction_safe));
std::map<int, int, safecomp>* member_map_comparison = NULL;

// a comparison function
bool intcomp(int lhs, int rhs) { return lhs < rhs;}

void ctor_dtor_tests(int id)
{
    // print simple output
    global_barrier->arrive(id);
    if (id == 0)
        printf("Testing member map constructors(12/11) and destructors(0/1)\n");

    // set up our comparison function pointer:
    safecomp comparison = intcomp;

    // test simple ctor and dtor
    //
    // NB: we haven't actually verified size yet, but we use it here.
    global_barrier->arrive(id);
    {
        map_verifier v;
        int size;
        BEGIN_TX;
        member_map = new std::map<int, int>();
        int size = member_map->size();
        delete(member_map);
        END_TX;
        v.check_size("basic ctor(1a) and dtor(0)", id, size);
    }

    // test construction with explicit comparison function
    global_barrier->arrive(id);
    {
        map_verifier v;
        int size;
        BEGIN_TX;
        member_map_comparison = new std::map<int, int, safecomp>(comparison);
        int size = member_map_comparison->size();
        delete(member_map_comparison);
        END_TX;
        v.check_size("basic ctor(1b)", id, size);
    }

    // test ctor with explicit allocator
    global_barrier->arrive(id);
    {
        map_verifier v;
        int size;
        std::map<int, int> tmp;
        BEGIN_TX;
        member_map = new std::map<int, int>(tmp.get_allocator());
        int size = member_map->size();
        delete(member_map);
        END_TX;
        v.check_size("basic ctor(1c)", id, size);
    }

    // test range ctor with explicit comparison function
    global_barrier->arrive(id);
    {
        map_verifier v;
        std::map<int, int> tmp({{1, 1}, {2, 2}, {3, 3}});
        BEGIN_TX;
        member_map_comparison = new std::map<int, int, safecomp>(tmp.begin(), tmp.end(), comparison);
        v.insert_all(member_map_comparison);
        delete(member_map_comparison);
        END_TX;
        v.check("range ctor(2a)", id, 6, {1, 1, 2, 2, 3, 3});
    }

    // test range ctor with explicit allocator function
    global_barrier->arrive(id);
    {
        map_verifier v;
        std::map<int, int> tmp({{1, 1}, {2, 2}, {3, 3}});
        BEGIN_TX;
        member_map = new std::map<int, int>(tmp.begin(), tmp.end(), tmp.get_allocator());
        v.insert_all(member_map);
        delete(member_map);
        END_TX;
        v.check("range ctor(2b)", id, 6, {1, 1, 2, 2, 3, 3});
    }

    // test range ctor with implicit allocator and comparison functions
    global_barrier->arrive(id);
    {
        map_verifier v;
        std::map<int, int> tmp({{1, 1}, {2, 2}, {3, 3}});
        BEGIN_TX;
        member_map = new std::map<int, int>(tmp.begin(), tmp.end());
        v.insert_all(member_map);
        delete(member_map);
        END_TX;
        v.check("range ctor(2c)", id, 6, {1, 1, 2, 2, 3, 3});
    }

    // test copy ctor
    global_barrier->arrive(id);
    {
        map_verifier v;
        std::map<int, int> tmp({{1, 1}, {2, 2}, {3, 3}});
        BEGIN_TX;
        member_map = new std::map<int, int>(tmp);
        v.insert_all(member_map);
        delete(member_map);
        END_TX;
        v.check("copy ctor(3a)", id, 6, {1, 1, 2, 2, 3, 3});
    }

    // test copy ctor with explicit allocator
    global_barrier->arrive(id);
    {
        map_verifier v;
        std::map<int, int> tmp({{1, 1}, {2, 2}, {3, 3}});
        BEGIN_TX;
        member_map = new std::map<int, int>(tmp, tmp.get_allocator());
        v.insert_all(member_map);
        delete(member_map);
        END_TX;
        v.check("copy ctor(3b)", id, 6, {1, 1, 2, 2, 3, 3});
    }

    // test move ctor
    global_barrier->arrive(id);
    {
        map_verifier v;
        std::map<int, int> tmp({{1, 1}, {2, 2}, {3, 3}});
        BEGIN_TX;
        member_map = new std::map<int, int>(std::move(tmp));
        v.insert_all(member_map);
        delete(member_map);
        END_TX;
        v.check("move ctor(4a)", id, 6, {1, 1, 2, 2, 3, 3});
    }

    // test move ctor with explicit allocator
    global_barrier->arrive(id);
    {
        map_verifier v;
        std::map<int, int> tmp({{1, 1}, {2, 2}, {3, 3}});
        BEGIN_TX;
        member_map = new std::map<int, int>(std::move(tmp), tmp.get_allocator());
        v.insert_all(member_map);
        delete(member_map);
        END_TX;
        v.check("copy ctor(4b)", id, 6, {1, 1, 2, 2, 3, 3});
    }

    // test initializer list ctor with explicit comparison function
    global_barrier->arrive(id);
    {
        map_verifier v;
        BEGIN_TX;
        member_map_comparison = new std::map<int, int, safecomp>({{1, 1}, {2, 2}, {3, 3}}, comparison);
        v.insert_all(member_map_comparison);
        delete(member_map_comparison);
        END_TX;
        v.check("ilist ctor(5a)", id, 6, {1, 1, 2, 2, 3, 3});
    }

    // test initializer list ctor with default comparison function
    global_barrier->arrive(id);
    {
        map_verifier v;
        std::map<int, int> tmp({{1, 1}, {2, 2}, {3, 3}});
        BEGIN_TX;
        member_map = new std::map<int, int>({{1, 1}, {2, 2}, {3, 3}}, tmp.get_allocator());
        v.insert_all(member_map);
        delete(member_map);
        END_TX;
        v.check("ilist ctor(5b)", id, 6, {1, 1, 2, 2, 3, 3});
    }
}

void op_eq_tests(int id)
{
    // print simple output
    global_barrier->arrive(id);
    if (id == 0)
        printf("Testing member map operator=(3)\n");

    // test #1 is operator= copy
    //
    // NB: we haven't actually verified iterators yet, but we use them...
    global_barrier->arrive(id);
    {
        map_verifier v;
        std::map<int, int> local = { {1, 1}, {4, 4}, {2, 2}};
        BEGIN_TX;
        member_map = new std::map<int, int>();
        *member_map = local;
        v.insert_all<std::map<int, int>>(member_map);
        delete(member_map);
        member_map = NULL;
        END_TX;
        v.check("copy operator= (1)", id, 6, {1, 1, 2, 2, 4, 4, -2});
    }

    // test #2 is operator= move
    global_barrier->arrive(id);
    {
        map_verifier v;
        std::map<int, int> local = { {1, 1}, {4, 4}, {2, 2}};
        BEGIN_TX;
        member_map = new std::map<int, int>();
        *member_map = std::move(local);
        v.insert_all<std::map<int, int>>(member_map);
        delete(member_map);
        member_map = NULL;
        END_TX;
        v.check("move operator= (2)", id, 6, {1, 1, 2, 2, 4, 4, -2});
    }

    // test #3 is operator= ilist
    global_barrier->arrive(id);
    {
        map_verifier v;
        BEGIN_TX;
        member_map = new std::map<int, int>();
        *member_map = { {1, 1}, {4, 4}, {2, 2}};
        v.insert_all<std::map<int, int>>(member_map);
        delete(member_map);
        member_map = NULL;
        END_TX;
        v.check("ilist operator= (3)", id, 6, {1, 1, 2, 2, 4, 4, -2});
    }
}
