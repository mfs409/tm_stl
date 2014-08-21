#include <iostream>
#include <map>
#include "tests.h"
#include "verify.h"

/// The maps we will use for our tests
std::map<int, int>* modifier_map = NULL;
struct Foo
{
    int x;
  private:
    explicit Foo(){};
  public:
    Foo(int z) : x(z) { }
};
std::map<int, Foo>* modifier_map_special = NULL;

void modifier_tests(int id)
{
    global_barrier->arrive(id);
    if (id == 0)
        printf("Testing map modifier functions: insert(6), erase(4), "
               "swap(1), clear(1), emplace(1), emplace_hint(1)\n");

    // test insert of single element value_type (1a)
    //
    // mfs: I can't get this to instantiate 1a
    global_barrier->arrive(id);
    {
        map_verifier v;
        BEGIN_TX;
        modifier_map_special = new std::map<int, Foo>({{1, Foo(1)}, {2, Foo(2)}, {4, Foo(4)}, {8, Foo(8)}});
        const std::pair<int, Foo>* t = new std::pair<int, Foo>(3, Foo(3));
        const std::pair<int, Foo>& tt = *t;
        modifier_map_special->insert(tt);
        for (auto i : *modifier_map_special) {
            v.insert(i.first);
            v.insert(i.second.x);
        }
        delete(modifier_map_special);
        END_TX;
        v.check("single element insert (1a)", id, 10, {1, 1, 2, 2, 3, 3, 4, 4, 8, 8, -2});
    }

    // test insert of single element via pair (1b)
    global_barrier->arrive(id);
    {
        map_verifier v;
        BEGIN_TX;
        modifier_map = new std::map<int, int>({{1, 1}, {2, 2}, {4, 4}, {8, 8}});
        modifier_map->insert(std::pair<int, int>(3, 3));
        v.insert_all(modifier_map);
        delete(modifier_map);
        END_TX;
        v.check("single element insert (1b)", id, 10, {1, 1, 2, 2, 3, 3, 4, 4, 8, 8, -2});
    }

    // test insert with hint (2a)
    //
    // [mfs] I can't get this to instantiate 2a
    global_barrier->arrive(id);
    {
        map_verifier v;
        BEGIN_TX;
        modifier_map = new std::map<int, int>({{1, 1}, {2, 2}, {4, 4}, {8, 8}});
        auto i = modifier_map->begin(); i++;
        modifier_map->insert(i, std::make_pair(3, 3));
        v.insert_all(modifier_map);
        delete(modifier_map);
        END_TX;
        v.check("single element insert with hint (2a)", id, 10, {1, 1, 2, 2, 3, 3, 4, 4, 8, 8, -2});
    }

    // test insert with hint (2b)
    global_barrier->arrive(id);
    {
        map_verifier v;
        BEGIN_TX;
        modifier_map = new std::map<int, int>({{1, 1}, {2, 2}, {4, 4}, {8, 8}});
        auto i = modifier_map->begin(); i++;
        modifier_map->insert(i, std::pair<int, int>(3, 3));
        v.insert_all(modifier_map);
        delete(modifier_map);
        END_TX;
        v.check("single element insert with hint (2b)", id, 10, {1, 1, 2, 2, 3, 3, 4, 4, 8, 8, -2});
    }

    // test insert with range
    global_barrier->arrive(id);
    {
        map_verifier v;
        std::map<int, int> tmp({{1, 1}, {2, 2}, {4, 4}, {8, 8}});
        BEGIN_TX;
        modifier_map = new std::map<int, int>();
        modifier_map->insert(tmp.begin(), tmp.end());
        v.insert_all(modifier_map);
        delete(modifier_map);
        END_TX;
        v.check("insert range (3)", id, 8, {1, 1, 2, 2, 4, 4, 8, 8, -2});
    }

    // test insert with initializer list (4)
    global_barrier->arrive(id);
    {
        map_verifier v;
        BEGIN_TX;
        modifier_map = new std::map<int, int>();
        modifier_map->insert({{1, 1}, {2, 2}, {4, 4}, {8, 8}});
        v.insert_all(modifier_map);
        delete(modifier_map);
        END_TX;
        v.check("insert ilist (4)", id, 8, {1, 1, 2, 2, 4, 4, 8, 8, -2});
    }

    // test single element erase (1a)
    global_barrier->arrive(id);
    {
        map_verifier v;
        BEGIN_TX;
        modifier_map = new std::map<int, int>({{16, 16}, {4, 4}, {9, 9}, {25, 25}});
        auto i = modifier_map->cbegin();i++;i++;
        modifier_map->erase(i);
        v.insert_all(modifier_map);
        delete(modifier_map);
        END_TX;
        v.check("erase single element (1a)", id, 6, {4, 4, 9, 9, 25, 25});
    }

    // test single element erase (1b)
    global_barrier->arrive(id);
    {
        map_verifier v;
        BEGIN_TX;
        modifier_map = new std::map<int, int>({{16, 16}, {4, 4}, {9, 9}, {25, 25}});
        auto i = modifier_map->begin();i++;i++;
        modifier_map->erase(i);
        v.insert_all(modifier_map);
        delete(modifier_map);
        END_TX;
        v.check("erase single element (1b)", id, 6, {4, 4, 9, 9, 25, 25});
    }

    // test erase key (2)
    global_barrier->arrive(id);
    {
        map_verifier v;
        BEGIN_TX;
        modifier_map = new std::map<int, int>({{16, 16}, {4, 4}, {9, 9}, {25, 25}});
        modifier_map->erase(16);
        v.insert_all(modifier_map);
        delete(modifier_map);
        END_TX;
        v.check("erase by key (2)", id, 6, {4, 4, 9, 9, 25, 25});
    }

    // test erase range (3)
    global_barrier->arrive(id);
    {
        map_verifier v;
        BEGIN_TX;
        modifier_map = new std::map<int, int>({{16, 16}, {4, 4}, {9, 9}, {25, 25}});
        auto i = modifier_map->begin();i++;i++;
        modifier_map->erase(modifier_map->begin(), i);
        v.insert_all(modifier_map);
        delete(modifier_map);
        END_TX;
        v.check("erase range (3)", id, 4, {16, 16, 25, 25});
    }

    // test swap (1)
    global_barrier->arrive(id);
    {
        map_verifier v;
        int tmp = 0;
        BEGIN_TX;
        modifier_map = new std::map<int, int>({{1, 1}, {2, 2}, {3, 3}});
        std::map<int, int> swapper = {{4, 4}, {6, 6}};
        modifier_map->swap(swapper);
        for (auto i = swapper.begin(); i != swapper.end(); ++i)
            tmp = tmp * 10 + i->first;
        v.insert_all(modifier_map);
        delete(modifier_map);
        END_TX;
        if (tmp != 123)
            std::cout << "["<<id<<"] error in swap()" << std::endl;
        v.check("swap (1)", id, 4, {4, 4, 6, 6});
    }

    // test clear (1)
    global_barrier->arrive(id);
    {
        map_verifier v;
        BEGIN_TX;
        modifier_map = new std::map<int, int>({{1, 1}, {2, 2}, {3, 3}});
        modifier_map->clear();
        v.insert_all(modifier_map);
        delete(modifier_map);
        END_TX;
        v.check("clear (1)", id, 0, {-2, -2});
    }

    // test emplace (1)
    global_barrier->arrive(id);
    {
        map_verifier v;
        BEGIN_TX;
        modifier_map = new std::map<int, int>({{1, 1}, {2, 2}, {3, 3}});
        modifier_map->emplace(8, 8);
        v.insert_all(modifier_map);
        delete(modifier_map);
        END_TX;
        v.check("emplace (1)", id, 8, {1, 1, 2, 2, 3, 3, 8, 8, -2});
    }


    // test emplace_hint (1)
    global_barrier->arrive(id);
    {
        map_verifier v;
        BEGIN_TX;
        modifier_map = new std::map<int, int>({{1, 1}, {8, 8}, {7, 7}});
        auto i = modifier_map->end();
        modifier_map->emplace_hint(i, 30, 30);
        v.insert_all(modifier_map);
        delete(modifier_map);
        END_TX;
        v.check("emplace_hint (1)", id, 8, {1, 1, 7, 7, 8, 8, 30, 30});
    }
}

