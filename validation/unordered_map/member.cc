#include <iostream>
#include <string>
#include <unordered_map>
#include <cassert>
#include "tests.h"
#include "verify.h"

/// The map we will use for our tests
typedef std::unordered_map<int, int> intmap;
typedef map_verifier verifier;

intmap* member_map = NULL;

void ctor_dtor_tests(int id)
{
    // print simple output
    global_barrier->arrive(id);
    if (id == 0)
        printf("Testing member map constructors(8) and destructors(1)\n");

    // the first test is simple ctor and dtor
    //
    // NB: we haven't actually verified size yet, but we use it here.
    global_barrier->arrive(id);
    {
        verifier v;
        int size;
        BEGIN_TX;
        member_map = new intmap();
        int size = member_map->size();
        delete(member_map);
        member_map = NULL;
        END_TX;
        v.check_size("basic ctor(1) and dtor(1)", id, size);
    }

    // the next test uses a custom allocator with ctor 1
    //
    // NB: we haven't actually verified size yet, but we use it here.
    global_barrier->arrive(id);
    {
        verifier v;
        int size;
        BEGIN_TX;
        member_map = new intmap();
        // using get_allocator without checking it
        auto a = member_map->get_allocator();
        delete(member_map);
        member_map = new intmap(a);
        size = member_map->size();
        delete(member_map);
        member_map = NULL;
        END_TX;
        v.check_size("basic ctor(1) and dtor(1)", id, size);
    }

    // the next test will use the ilist & range ctor
    global_barrier->arrive(id);
    {
        verifier v;
        intmap q ({{1,1},{2,2},{3,3}});
        BEGIN_TX;
        member_map = new intmap(q.begin(), q.end());
        v.insert_all<intmap>(member_map);
        delete(member_map);
        member_map = NULL;
        END_TX;
        v.check("range ctor (2)", id, 6, { 1, 1, 2, 2, 3, 3 });
    }

    // the next test will use the copy ctor
    global_barrier->arrive(id);
    {
        verifier v;
        intmap local ({{3,3},{4,4}});
        BEGIN_TX;
        member_map = new intmap(local, member_map->get_allocator());
        v.insert_all<intmap>(member_map);
        delete(member_map);
        member_map = NULL;
        END_TX;
        v.check("copy ctor (3b) -- 3a is default", id, 4, {3, 3, 4, 4});
    }

    // the next test is the move ctor
    global_barrier->arrive(id);
    {
        verifier v;
        intmap local ({{5,5},{4,4},{3,3}});
        BEGIN_TX;
        member_map = new intmap(std::move(local), member_map->get_allocator());
        v.insert_all<intmap>(member_map);
        delete(member_map);
        member_map = NULL;
        END_TX;
        v.check("move ctor (4b) -- 4a is default", id, 6, {5, 5, 4, 4, 3, 3});
    }
}

void op_eq_tests(int id)
{
    // test #3 is operator= ilist
    global_barrier->arrive(id);
    {
        verifier v;
        BEGIN_TX;
        member_map = new intmap();
        *member_map = { {13, 14}, {15, 16} };
        v.insert_all<intmap>(member_map);
        delete(member_map);
        member_map = NULL;
        END_TX;
        v.check("ilist operator= (3)", id, 4, {13, 14, 15, 16});
    }
}
