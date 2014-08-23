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

void iter_create_tests(int id)
{
    // test begin and end
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
        v.check("begin(1a) & end(1a)", id, 4, {1, 1, 2, 2});
    }

    // test const begin and end
    global_barrier->arrive(id);
    {
        verifier v;
        const intmap other({{1, 1}, {2, 2}});
        BEGIN_TX;
        member_map = new intmap();
        member_map->insert(other.begin(), other.end());
        v.insert_all<intmap>(member_map);
        delete(member_map);
        member_map = NULL;
        END_TX;
        v.check("begin(1b) & end(1b)", id, 4, {1, 1, 2, 2});
    }

    // test bucket begin and end
    global_barrier->arrive(id);
    {
        verifier v;
        intmap other({{1, 1}, {2, 2}});
        BEGIN_TX;
        member_map = new intmap();
        member_map->insert(other.begin(0), other.end(0));
        v.insert_all<intmap>(member_map);
        delete(member_map);
        member_map = NULL;
        END_TX;
        std::cout << "  [OK] begin(2a) & end(2a)" << std::endl;
    }

    // test const bucket begin and end
    global_barrier->arrive(id);
    {
        int v;
        const intmap other({{1, 1}, {2, 2}});
        BEGIN_TX;
        v = (other.begin(0) == other.end(0));
        END_TX;
        std::cout << "  [OK] begin(2b) & end(2b) " << v << std::endl;
    }

    // test cbegin and cend
    global_barrier->arrive(id);
    {
        int v;
        const intmap other({{1, 1}, {2, 2}});
        BEGIN_TX;
        v = (other.cbegin() == other.cend());
        END_TX;
        std::cout << "  [OK] cbegin(1) & cend(1) " << v << std::endl;
    }

    // test bucket cbegin and cend
    global_barrier->arrive(id);
    {
        int v;
        const intmap other({{1, 1}, {2, 2}});
        BEGIN_TX;
        v = (other.cbegin(0) == other.cend(0));
        END_TX;
        std::cout << "  [OK] cbegin(2) & cend(2) " << v << std::endl;
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
