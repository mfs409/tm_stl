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
    // test emplace
    global_barrier->arrive(id);
    {
        verifier v;
        BEGIN_TX;
        member_map = new intmap();
        member_map->emplace(42, 42);
        v.insert_all<intmap>(member_map);
        delete(member_map);
        member_map = NULL;
        END_TX;
        v.check("emplace pair (1)", id, 2, {42, 42});
    }

    // test emplace with hint
    global_barrier->arrive(id);
    {
        verifier v;
        BEGIN_TX;
        member_map = new intmap();
        member_map->emplace_hint(member_map->begin(), 42, 42);
        v.insert_all<intmap>(member_map);
        delete(member_map);
        member_map = NULL;
        END_TX;
        v.check("emplace_hint pair (1)", id, 2, {42, 42});
    }

    // test insertion of a pair (copy)
    global_barrier->arrive(id);
    {
        verifier v;
        BEGIN_TX;
        member_map = new intmap();
        member_map->insert({42, 42});
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

    // test insertion of a pair with hint (copy)
    global_barrier->arrive(id);
    {
        verifier v;
        BEGIN_TX;
        member_map = new intmap();
        member_map->insert(member_map->begin(), {42, 42});
        v.insert_all<intmap>(member_map);
        delete(member_map);
        member_map = NULL;
        END_TX;
        v.check("insert pair (3)", id, 2, {42, 42});
    }

    // test insertion of a pair with hint (move)
    global_barrier->arrive(id);
    {
        verifier v;
        BEGIN_TX;
        member_map = new intmap();
        member_map->insert(member_map->begin(), std::make_pair(23, 23));
        v.insert_all<intmap>(member_map);
        delete(member_map);
        member_map = NULL;
        END_TX;
        v.check("insert pair (4)", id, 2, {23, 23});
    }

    // test range insertion (move)
    global_barrier->arrive(id);
    {
        verifier v;
        intmap other({{1, 1}, {2, 2}});
        BEGIN_TX;
        member_map = new intmap();
        member_map->insert(other.begin(), other.end());
        v.insert_all<intmap>(member_map);
        delete(member_map);
        member_map = NULL;
        END_TX;
        v.check("insert range (5)", id, 4, {1, 1, 2, 2});
    }

    // test range insertion
    global_barrier->arrive(id);
    {
        verifier v;
        BEGIN_TX;
        member_map = new intmap();
        member_map->insert({{1, 1}, {2, 2}});
        v.insert_all<intmap>(member_map);
        delete(member_map);
        member_map = NULL;
        END_TX;
        v.check("insert range (6)", id, 4, {1, 1, 2, 2});
    }

    // test erase (position)
    global_barrier->arrive(id);
    {
        verifier v;
        BEGIN_TX;
        member_map = new intmap({{1, 1}, {2, 2}, {3, 3}});
        member_map->erase(member_map->begin());
        v.insert_all<intmap>(member_map);
        delete(member_map);
        member_map = NULL;
        END_TX;
        v.check("erase position (1)", id, 4, {2, 2, 3, 3});
    }

    // test erase (key)
    global_barrier->arrive(id);
    {
        verifier v;
        BEGIN_TX;
        member_map = new intmap({{1, 1}, {2, 2}, {3, 3}});
        member_map->erase(1);
        v.insert_all<intmap>(member_map);
        delete(member_map);
        member_map = NULL;
        END_TX;
        v.check("erase key (2)", id, 4, {2, 2, 3, 3});
    }

    // test erase (range)
    global_barrier->arrive(id);
    {
        verifier v;
        BEGIN_TX;
        member_map = new intmap({{1, 1}, {2, 2}, {3, 3}});
        member_map->erase(member_map->begin(), member_map->end());
        v.insert_all<intmap>(member_map);
        delete(member_map);
        member_map = NULL;
        END_TX;
        v.check_size("erase range (3)", id, 0);
    }

    // test swap
    global_barrier->arrive(id);
    {
        verifier v;
        intmap other({{4, 4}, {5, 5}});
        BEGIN_TX;
        member_map = new intmap({{1, 1}, {2, 2}, {3, 3}});
        member_map->swap(other);
        v.insert_all<intmap>(member_map);
        delete(member_map);
        member_map = NULL;
        END_TX;
        v.check("swap (1)", id, 4, {4, 4, 5, 5});
    }

    // test clear
    global_barrier->arrive(id);
    {
        verifier v;
        BEGIN_TX;
        member_map = new intmap({{1, 1}, {2, 2}, {3, 3}});
        member_map->clear();
        v.insert_all<intmap>(member_map);
        delete(member_map);
        member_map = NULL;
        END_TX;
        v.check_size("clear (1)", id, 0);
    }

}
