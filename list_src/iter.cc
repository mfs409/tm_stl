#include <iostream>
#include <string>
#include <list>
#include <cassert>
#include "concurrent_tests.h"

/// for testing ->
namespace
{
  struct Q
  {
      int a;
      int b;
      Q() : a(1), b(2) { }
  };
}

/// The list we will use for our tests
std::list<int>* iter_list = NULL;

/// Another list for the tests
std::list<Q> foo;

/// clone the list to a local array represented by dsize, data[]
#define COPY_LIST                                 \
    dsize = 0;                                    \
    for (auto i : *iter_list)                     \
        data[dsize++] = i

/// destroy the list and set the pointer to NULL
#define CLEAR_LIST                                \
    delete iter_list;                             \
    iter_list = NULL

/// compare the local array to our expected results
#define VERIFY(testName, size, ...)                                     \
    if (dsize != size)                                                  \
        printf(" [%d] size did not match %d != %d\n", id, dsize, size);  \
    else if (size > 0) {                                                \
        int counter = 0;                                                \
        bool good = true;                                               \
        for (auto i : __VA_ARGS__)                                      \
            good &= (data[counter++] == i);                             \
        if (!good)                                                      \
            std::cout << " ["<<id<<"] Arrays did not match for thread " << id << std::endl; \
        else if (id == 0)                                               \
            std::cout << " [OK] " << testName << std::endl;              \
    }                                                                   \
    else if (id == 0)                                                   \
            std::cout << " [OK] " << testName << std::endl

/**
 * Transactional test of basic (forward, mutable) iterators
 */
void iterator_test_concurrent(int id)
{
    // invariant: the list starts off null
    assert(iter_list == NULL);

    // helper message
    if (id == 0) {
        std::cout << std::endl
                  << "Testing iterators (note: no verification for copy assignable or swap)"
                  << std::endl;
    }

    // scratch space for holding data when computing whether the test
    // succeeded or failed.
    int data[32], dsize = 0;

    // test begin and end
    global_barrier->arrive(id);
    int tmp = 0;
    BEGIN_TX;
    // make a list
    iter_list = new std::list<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    // use begin and end
    std::list<int>::iterator test_b = iter_list->begin();
    std::list<int>::iterator test_e = iter_list->end();
    // a hack to prevent it being optimized out
    for (std::list<int>::iterator i = test_b; i != test_e; ++i) // using untested ++
        tmp += *i;
    CLEAR_LIST;
    END_TX;
    if (tmp != 55)
        std::cout << "["<<id<<"] did not compute 55" << std::endl;
    else if (id == 0)
        std::cout << " [OK] begin() and end()" << std::endl;

    // test ctors and dtors
    global_barrier->arrive(id);
    BEGIN_TX;
    iter_list = new std::list<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    // iterators should be default constructable (1)
    std::list<int>::iterator i;
    // iterators should be copy constructable(2)
    std::list<int>::iterator b(iter_list->begin());
    // iterators should be copy assignable (3)
    std::list<int>::iterator e = iter_list->end();
    std::list<int>::iterator* pe = new std::list<int>::iterator(e);
    // manually copy the list, so we can be sure to use i, b, pe
    dsize = 0;
    for (i = b; i != *pe; ++i) {
        data[dsize++] = *i;
    }
    // iterators should be destructable (4)
    delete pe;
    CLEAR_LIST;
    END_TX;
    VERIFY("ctors and dtors", 10, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10});

    // iterators should be comparable with == (5) and != (6)
    global_barrier->arrive(id);
    std::list<int>::iterator e;
    if (id == 0) {
        iter_list = new std::list<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    }
    global_barrier->arrive(id);
    e = iter_list->end();
    bool eq, neq;
    BEGIN_TX;
    eq = (e == iter_list->end());
    neq = (e != iter_list->begin());
    END_TX;
    if (!eq || !neq)
        std::cout << "["<<id<<"] did not observe correct equality results" << std::endl;
    else if (id == 0)
        std::cout << " [OK] != and ==" << std::endl;

    // test operator* and operator->
    global_barrier->arrive(id);
    if (id == 0) {
        foo.emplace(foo.begin());
        CLEAR_LIST;
    }
    global_barrier->arrive(id);
    int val1, val2, val3, val4;
    BEGIN_TX;
    iter_list = new std::list<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    std::list<int>::iterator b = iter_list->begin();
    // iterators can be dereferenced as an rvalue if in a dereferenceable
    // state (7)
    val1 = *b;
    // iterators can be dereferenced as an lvalue (8)
    *b = -6;
    val2 = *iter_list->begin();
    // iterator access with ->
    std::list<Q>::iterator x = foo.begin();
    val3 = x->a;
    val4 = x->b;
    CLEAR_LIST;
    END_TX;
    bool ok = (val1 == 1) && (val2 == -6) && (val3 == 1) && (val4 == 2);
    if (!ok)
        std::cout << "["<<id<<"] errors on * or ->" << std::endl;
    else if (id == 0)
        std::cout << " [OK] operators * and ->" << std::endl;
    global_barrier->arrive(id);
    if (id == 0) {
        foo.clear();
    }

    // test ++
    global_barrier->arrive(id);
    val1 = val2 = val3 = val4 = -8;
    int t1, t2, t3, t4;
    ok = true;
    BEGIN_TX;
    iter_list = new std::list<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    std::list<int>::iterator i = iter_list->begin();
    // iterators can be incremented (9)
    i++;
    val1 = *i;
    // iterators can be incremented (10)
    ++i;
    val2 = *i;
    // iterators can be incremented (11)
    val3 = *i++;
    val4 = *i;
    // two iterators that compare equal remain equal after being increased
    // (12)
    std::list<int>::iterator a1(iter_list->begin());
    std::list<int>::iterator a2(iter_list->begin());
    t1 = *a1;
    t2 = *a2;
    ok  &= (a1 == a2) && (t1 == 1) && (t2 == 1);
    a1++;
    a2++;
    t3 = *a1;
    t4 = *a2;
    ok &= (a1 == a2) && (t3 == 2) && (t4 == 2);
    CLEAR_LIST;
    END_TX;
    ok &= (val1 == 2) && (val2 == 3) && (val3 == 3) && (val4 == 4);
    if (!ok)
        std::cout << "["<<id<<"] errors on ++ tests" << std::endl;
    else if (id == 0)
        std::cout << " [OK] operator ++" << std::endl;

    // test operator --
    global_barrier->arrive(id);
    BEGIN_TX;
    iter_list = new std::list<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    std::list<int>::iterator i = iter_list->end();
    // can be decremented (13)
    --i;
    val1 = *i;
    // can be decremented (14)
    i--;
    val2 = *i;
    // can be decremented (15)
    val3 = *i--;
    val4 = *i;
    CLEAR_LIST;
    END_TX;
    ok = (val1 == 10) && (val2 == 9) && (val3 == 9) && (val4 == 8);
    if (!ok)
        std::cout << "["<<id<<"] errors on -- tests" << std::endl;
    else if (id == 0)
        std::cout << " [OK] operator --" << std::endl;

    // test swapping (16)
    global_barrier->arrive(id);
    BEGIN_TX;
    iter_list = new std::list<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    std::list<int>::iterator b = iter_list->begin();
    std::list<int>::iterator e = iter_list->end();
    swap(b, e);
    // manually copy the list, so we can be sure to use i, b, e
    dsize = 0;
    for (std::list<int>::iterator i = e; i != b; ++i) {
        data[dsize++] = *i;
    }
    CLEAR_LIST;
    END_TX;
    VERIFY("swap", 10, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10});

    global_barrier->arrive(id);
}

/**
 *  Transactional test of mutable reverse iterators
 */
void reverse_iterator_test_concurrent(int id)
{
    // invariant: the list starts off null
    assert(iter_list == NULL);

    // helper message
    if (id == 0) {
        std::cout << std::endl
                  << "Testing reverse iterators" << std::endl;
    }

    // scratch space for holding data when computing whether the test
    // succeeded or failed.
    int data[32], dsize = 0;

    // test rbegin and rend
    global_barrier->arrive(id);
    int tmp = 0;
    BEGIN_TX;
    // make a list
    iter_list = new std::list<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    // use rbegin and rend
    std::list<int>::reverse_iterator test_b = iter_list->rbegin();
    std::list<int>::reverse_iterator test_e = iter_list->rend();
    // a hack to prevent it being optimized out
    for (std::list<int>::reverse_iterator i = test_b; i != test_e; ++i) // using untested ++
        tmp += *i;
    CLEAR_LIST;
    END_TX;
    if (tmp != 55)
        std::cout << "["<<id<<"] did not compute 55" << std::endl;
    else if (id == 0)
        std::cout << " [OK] rbegin() and rend()" << std::endl;

    // test ctors and dtors
    global_barrier->arrive(id);
    BEGIN_TX;
    iter_list = new std::list<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    // iterators should be default constructable (1)
    std::list<int>::reverse_iterator i;
    // iterators should be copy constructable(2)
    std::list<int>::reverse_iterator b(iter_list->rbegin());
    // iterators should be copy assignable (3)
    std::list<int>::reverse_iterator e = iter_list->rend();
    std::list<int>::reverse_iterator* pe = new std::list<int>::reverse_iterator(e);
    // manually copy the list, so we can be sure to use i, b, pe
    dsize = 0;
    for (i = b; i != *pe; ++i) {
        data[dsize++] = *i;
    }
    // iterators should be destructable (4)
    delete pe;
    CLEAR_LIST;
    END_TX;
    VERIFY("ctors and dtors", 10, {10, 9, 8, 7, 6, 5, 4, 3, 2, 1});

    // iterators should be comparable with == (5) and != (6)
    global_barrier->arrive(id);
    std::list<int>::reverse_iterator e;
    if (id == 0) {
        iter_list = new std::list<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    }
    global_barrier->arrive(id);
    e = iter_list->rend();
    bool eq, neq;
    BEGIN_TX;
    eq = (e == iter_list->rend());
    neq = (e != iter_list->rbegin());
    END_TX;
    if (!eq || !neq)
        std::cout << "["<<id<<"] did not observe correct equality results" << std::endl;
    else if (id == 0)
        std::cout << " [OK] != and ==" << std::endl;

    // test operator* and operator->
    global_barrier->arrive(id);
    if (id == 0) {
        foo.emplace(foo.begin());
        CLEAR_LIST;
    }
    global_barrier->arrive(id);
    int val1 = -9, val2 = -9, val3 = -9, val4 = -9;
    BEGIN_TX;
    iter_list = new std::list<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    std::list<int>::reverse_iterator b = iter_list->rbegin();
    // iterators can be dereferenced as an rvalue if in a dereferenceable
    // state (7)
    val1 = *b;
    // iterators can be dereferenced as an lvalue (8)
    *b = -6;
    val2 = *iter_list->rbegin();
    // iterator access with ->
    std::list<Q>::reverse_iterator x = foo.rbegin();
    val3 = x->a;
    val4 = x->b;
    CLEAR_LIST;
    END_TX;
    bool ok = (val1 == 10) && (val2 == -6) && (val3 == 1) && (val4 == 2);
    if (!ok)
        std::cout << "["<<id<<"] errors on * or ->" << std::endl;
    else if (id == 0)
        std::cout << " [OK] operators * and ->" << std::endl;
    global_barrier->arrive(id);
    if (id == 0) {
        foo.clear();
    }
    // test ++
    global_barrier->arrive(id);
    val1 = val2 = val3 = val4 = -8;
    int t1, t2, t3, t4;
    ok = true;
    BEGIN_TX;
    iter_list = new std::list<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    std::list<int>::reverse_iterator i = iter_list->rbegin();
    // iterators can be incremented (9)
    i++;
    val1 = *i;
    // iterators can be incremented (10)
    ++i;
    val2 = *i;
    // iterators can be incremented (11)
    val3 = *i++;
    val4 = *i;
    // two iterators that compare equal remain equal after being increased
    // (12)
    std::list<int>::reverse_iterator a1(iter_list->rbegin());
    std::list<int>::reverse_iterator a2(iter_list->rbegin());
    t1 = *a1;
    t2 = *a2;
    ok  &= (a1 == a2) && (t1 == 10) && (t2 == 10);
    a1++;
    a2++;
    t3 = *a1;
    t4 = *a2;
    ok &= (a1 == a2) && (t3 == 9) && (t4 == 9);
    CLEAR_LIST;
    END_TX;
    ok &= (val1 == 9) && (val2 == 8) && (val3 == 8) && (val4 == 7);
    if (!ok)
        std::cout << "["<<id<<"] errors on ++ tests" << std::endl;
    else if (id == 0)
        std::cout << " [OK] operator ++" << std::endl;

    // test operator --
    global_barrier->arrive(id);
    BEGIN_TX;
    iter_list = new std::list<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    std::list<int>::reverse_iterator i = iter_list->rbegin();
    i++;i++;i++;
    // can be decremented (13)
    --i;
    val1 = *i;
    // can be decremented (14)
    i--;
    val2 = *i;
    // can be decremented (15)
    val3 = *i--;
    val4 = *i;
    CLEAR_LIST;
    END_TX;
    ok = (val1 == 8) && (val2 == 9) && (val3 == 9) && (val4 == 10);
    if (!ok)
        std::cout << "["<<id<<"] errors on -- tests" << std::endl;
    else if (id == 0)
        std::cout << " [OK] operator --" << std::endl;
    global_barrier->arrive(id);
    if (id == 0)
        delete iter_list;

    // test swapping (16)
    global_barrier->arrive(id);
    BEGIN_TX;
    iter_list = new std::list<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    std::list<int>::reverse_iterator b = iter_list->rbegin();
    std::list<int>::reverse_iterator e = iter_list->rend();
    swap(b, e);
    // manually copy the list, so we can be sure to use i, b, e
    dsize = 0;
    for (std::list<int>::reverse_iterator i = e; i != b; ++i) {
        data[dsize++] = *i;
    }
    CLEAR_LIST;
    END_TX;
    VERIFY("swap", 10, {10, 9, 8, 7, 6, 5, 4, 3, 2, 1});

    global_barrier->arrive(id);
}

/**
 * Transactional test of legacy const forward iterators
 */
void legacy_const_iterator_test_concurrent(int id)
{
    // invariant: the list starts off null
    assert(iter_list == NULL);

    // helper message
    if (id == 0) {
        std::cout << std::endl
                  << "Testing legacy const iterators" << std::endl;
    }

    // scratch space for holding data when computing whether the test
    // succeeded or failed.
    int data[32], dsize = 0;

    // test begin and end
    global_barrier->arrive(id);
    int tmp = 0;
    BEGIN_TX;
    // make a list
    iter_list = new std::list<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    // use begin and end
    const std::list<int>* l2 = iter_list;
    std::list<int>::const_iterator test_b = l2->begin();
    std::list<int>::const_iterator test_e = l2->end();
    // a hack to prevent it being optimized out
    for (std::list<int>::const_iterator i = test_b; i != test_e; ++i) // using untested ++
        tmp += *i;
    CLEAR_LIST;
    END_TX;
    if (tmp != 55)
        std::cout << "["<<id<<"] did not compute 55" << std::endl;
    else if (id == 0)
        std::cout << " [OK] begin() and end()" << std::endl;

    // test ctors and dtors
    global_barrier->arrive(id);
    BEGIN_TX;
    iter_list = new std::list<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    // iterators should be default constructable (1)
    std::list<int>::const_iterator i;
    // iterators should be copy constructable(2)
    std::list<int>::const_iterator b(iter_list->begin());
    // iterators should be copy assignable (3)
    std::list<int>::const_iterator e = iter_list->end();
    std::list<int>::const_iterator* pe = new std::list<int>::const_iterator(e);
    // manually copy the list, so we can be sure to use i, b, pe
    dsize = 0;
    for (i = b; i != *pe; ++i) {
        data[dsize++] = *i;
    }
    // iterators should be destructable (4)
    delete pe;
    CLEAR_LIST;
    END_TX;
    VERIFY("ctors and dtors", 10, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10});

    // iterators should be comparable with == (5) and != (6)
    global_barrier->arrive(id);
    std::list<int>::const_iterator e;
    if (id == 0) {
        iter_list = new std::list<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    }
    global_barrier->arrive(id);
    e = iter_list->end();
    bool eq, neq;
    BEGIN_TX;
    eq = (e == iter_list->end());
    neq = (e != iter_list->begin());
    END_TX;
    if (!eq || !neq)
        std::cout << "["<<id<<"] did not observe correct equality results" << std::endl;
    else if (id == 0)
        std::cout << " [OK] != and ==" << std::endl;

    // test operator* and operator->
    global_barrier->arrive(id);
    if (id == 0) {
        foo.emplace(foo.begin());
        CLEAR_LIST;
    }
    global_barrier->arrive(id);
    int val1, val2, val3, val4;
    BEGIN_TX;
    iter_list = new std::list<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    std::list<int>::const_iterator b = iter_list->begin();
    // iterators can be dereferenced as an rvalue if in a dereferenceable
    // state (7)
    val1 = *b;
#if 0 // does not apply to const iterators
    // iterators can be dereferenced as an lvalue (8)
    *b = -6;
    val2 = *iter_list->begin();
#endif
    // iterator access with ->
    std::list<Q>::const_iterator x = foo.begin();
    val3 = x->a;
    val4 = x->b;
    CLEAR_LIST;
    END_TX;
    bool ok = (val1 == 1) && (val3 == 1) && (val4 == 2);
    if (!ok)
        std::cout << "["<<id<<"] errors on * or ->" << std::endl;
    else if (id == 0)
        std::cout << " [OK] operators * and ->" << std::endl;
    global_barrier->arrive(id);
    if (id == 0) {
        foo.clear();
    }

    // test ++
    global_barrier->arrive(id);
    val1 = val2 = val3 = val4 = -8;
    int t1, t2, t3, t4;
    ok = true;
    BEGIN_TX;
    iter_list = new std::list<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    std::list<int>::const_iterator i = iter_list->begin();
    // iterators can be incremented (9)
    i++;
    val1 = *i;
    // iterators can be incremented (10)
    ++i;
    val2 = *i;
    // iterators can be incremented (11)
    val3 = *i++;
    val4 = *i;
    // two iterators that compare equal remain equal after being increased
    // (12)
    std::list<int>::const_iterator a1(iter_list->begin());
    std::list<int>::const_iterator a2(iter_list->begin());
    t1 = *a1;
    t2 = *a2;
    ok  &= (a1 == a2) && (t1 == 1) && (t2 == 1);
    a1++;
    a2++;
    t3 = *a1;
    t4 = *a2;
    ok &= (a1 == a2) && (t3 == 2) && (t4 == 2);
    CLEAR_LIST;
    END_TX;
    ok &= (val1 == 2) && (val2 == 3) && (val3 == 3) && (val4 == 4);
    if (!ok)
        std::cout << "["<<id<<"] errors on ++ tests" << std::endl;
    else if (id == 0)
        std::cout << " [OK] operator ++" << std::endl;

    // test operator --
    global_barrier->arrive(id);
    BEGIN_TX;
    iter_list = new std::list<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    std::list<int>::const_iterator i = iter_list->end();
    // can be decremented (13)
    --i;
    val1 = *i;
    // can be decremented (14)
    i--;
    val2 = *i;
    // can be decremented (15)
    val3 = *i--;
    val4 = *i;
    CLEAR_LIST;
    END_TX;
    ok = (val1 == 10) && (val2 == 9) && (val3 == 9) && (val4 == 8);
    if (!ok)
        std::cout << "["<<id<<"] errors on -- tests" << std::endl;
    else if (id == 0)
        std::cout << " [OK] operator --" << std::endl;

    // test swapping (16)
    global_barrier->arrive(id);
    BEGIN_TX;
    iter_list = new std::list<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    std::list<int>::const_iterator b = iter_list->begin();
    std::list<int>::const_iterator e = iter_list->end();
    swap(b, e);
    // manually copy the list, so we can be sure to use i, b, e
    dsize = 0;
    for (std::list<int>::const_iterator i = e; i != b; ++i) {
        data[dsize++] = *i;
    }
    CLEAR_LIST;
    END_TX;
    VERIFY("swap", 10, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10});

    global_barrier->arrive(id);
}

/**
 * Transactional test of legacy const reverse iterators
 */
void legacy_const_reverse_iterator_test_concurrent(int id)
{
    // invariant: the list starts off null
    assert(iter_list == NULL);

    // helper message
    if (id == 0) {
        std::cout << std::endl
                  << "Testing legacy const reverse iterators" << std::endl;
    }

    // scratch space for holding data when computing whether the test
    // succeeded or failed.
    int data[32], dsize = 0;

    // test begin and end
    global_barrier->arrive(id);
    int tmp = 0;
    BEGIN_TX;
    // make a list
    iter_list = new std::list<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    // use begin and end
    const std::list<int>* l2 = iter_list;
    std::list<int>::const_reverse_iterator test_b = l2->rbegin();
    std::list<int>::const_reverse_iterator test_e = l2->rend();
    // a hack to prevent it being optimized out
    for (std::list<int>::const_reverse_iterator i = test_b; i != test_e; ++i) // using untested ++
        tmp += *i;
    CLEAR_LIST;
    END_TX;
    if (tmp != 55)
        std::cout << "["<<id<<"] did not compute 55" << std::endl;
    else if (id == 0)
        std::cout << " [OK] begin() and end()" << std::endl;

    // test ctors and dtors
    global_barrier->arrive(id);
    BEGIN_TX;
    iter_list = new std::list<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    // iterators should be default constructable (1)
    std::list<int>::const_reverse_iterator i;
    // iterators should be copy constructable(2)
    std::list<int>::const_reverse_iterator b(iter_list->rbegin());
    // iterators should be copy assignable (3)
    std::list<int>::const_reverse_iterator e = iter_list->rend();
    std::list<int>::const_reverse_iterator* pe = new std::list<int>::const_reverse_iterator(e);
    // manually copy the list, so we can be sure to use i, b, pe
    dsize = 0;
    for (i = b; i != *pe; ++i) {
        data[dsize++] = *i;
    }
    // iterators should be destructable (4)
    delete pe;
    CLEAR_LIST;
    END_TX;
    VERIFY("ctors and dtors", 10, {10, 9, 8, 7, 6, 5, 4, 3, 2, 1});

    // iterators should be comparable with == (5) and != (6)
    global_barrier->arrive(id);
    std::list<int>::const_reverse_iterator e;
    if (id == 0) {
        iter_list = new std::list<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    }
    global_barrier->arrive(id);
    e = iter_list->rend();
    bool eq, neq;
    BEGIN_TX;
    eq = (e == iter_list->rend());
    neq = (e != iter_list->rbegin());
    END_TX;
    if (!eq || !neq)
        std::cout << "["<<id<<"] did not observe correct equality results" << std::endl;
    else if (id == 0)
        std::cout << " [OK] != and ==" << std::endl;

    // test operator* and operator->
    global_barrier->arrive(id);
    if (id == 0) {
        foo.emplace(foo.begin());
        CLEAR_LIST;
    }
    global_barrier->arrive(id);
    int val1, val2, val3, val4;
    BEGIN_TX;
    iter_list = new std::list<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    std::list<int>::const_reverse_iterator b = iter_list->rbegin();
    // iterators can be dereferenced as an rvalue if in a dereferenceable
    // state (7)
    val1 = *b;
#if 0 // does not apply to const iterators
    // iterators can be dereferenced as an lvalue (8)
    *b = -6;
    val2 = *iter_list->begin();
#endif
    // iterator access with ->
    std::list<Q>::const_reverse_iterator x = foo.rbegin();
    val3 = x->a;
    val4 = x->b;
    CLEAR_LIST;
    END_TX;
    bool ok = (val1 == 10) && (val3 == 1) && (val4 == 2);
    if (!ok)
        std::cout << "["<<id<<"] errors on * or ->" << std::endl;
    else if (id == 0)
        std::cout << " [OK] operators * and ->" << std::endl;
    global_barrier->arrive(id);
    if (id == 0) {
        foo.clear();
    }

    // test ++
    global_barrier->arrive(id);
    val1 = val2 = val3 = val4 = -8;
    int t1, t2, t3, t4;
    ok = true;
    BEGIN_TX;
    iter_list = new std::list<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    std::list<int>::const_reverse_iterator i = iter_list->rbegin();
    // iterators can be incremented (9)
    i++;
    val1 = *i;
    // iterators can be incremented (10)
    ++i;
    val2 = *i;
    // iterators can be incremented (11)
    val3 = *i++;
    val4 = *i;
    // two iterators that compare equal remain equal after being increased
    // (12)
    std::list<int>::const_reverse_iterator a1(iter_list->rbegin());
    std::list<int>::const_reverse_iterator a2(iter_list->rbegin());
    t1 = *a1;
    t2 = *a2;
    ok  &= (a1 == a2) && (t1 == 10) && (t2 == 10);
    a1++;
    a2++;
    t3 = *a1;
    t4 = *a2;
    ok &= (a1 == a2) && (t3 == 9) && (t4 == 9);
    CLEAR_LIST;
    END_TX;
    ok &= (val1 == 9) && (val2 == 8) && (val3 == 8) && (val4 == 7);
    if (!ok)
        std::cout << "["<<id<<"] errors on ++ tests" << std::endl;
    else if (id == 0)
        std::cout << " [OK] operator ++" << std::endl;

    // test operator --
    global_barrier->arrive(id);
    BEGIN_TX;
    iter_list = new std::list<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    std::list<int>::const_reverse_iterator i = iter_list->rbegin();
    i++;i++;i++;
    // can be decremented (13)
    --i;
    val1 = *i;
    // can be decremented (14)
    i--;
    val2 = *i;
    // can be decremented (15)
    val3 = *i--;
    val4 = *i;
    CLEAR_LIST;
    END_TX;
    ok = (val1 == 8) && (val2 == 9) && (val3 == 9) && (val4 == 10);
    if (!ok)
        std::cout << "["<<id<<"] errors on -- tests" << std::endl;
    else if (id == 0)
        std::cout << " [OK] operator --" << std::endl;

    // test swapping (16)
    global_barrier->arrive(id);
    BEGIN_TX;
    iter_list = new std::list<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    std::list<int>::const_reverse_iterator b = iter_list->rbegin();
    std::list<int>::const_reverse_iterator e = iter_list->rend();
    swap(b, e);
    // manually copy the list, so we can be sure to use i, b, e
    dsize = 0;
    for (std::list<int>::const_reverse_iterator i = e; i != b; ++i) {
        data[dsize++] = *i;
    }
    CLEAR_LIST;
    END_TX;
    VERIFY("swap", 10, {10, 9, 8, 7, 6, 5, 4, 3, 2, 1});

    global_barrier->arrive(id);
}

/**
 * Transactional test of C++ const forward iterators with cbegin
 */
void const_iterator_test_concurrent(int id)
{
    // invariant: the list starts off null
    assert(iter_list == NULL);

    // helper message
    if (id == 0) {
        std::cout << std::endl
                  << "Testing C++11 const iterators" << std::endl;
    }

    // scratch space for holding data when computing whether the test
    // succeeded or failed.
    int data[32], dsize = 0;

    // test begin and end
    global_barrier->arrive(id);
    int tmp = 0;
    BEGIN_TX;
    // make a list
    iter_list = new std::list<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    // use begin and end
    const std::list<int>* l2 = iter_list;
    std::list<int>::const_iterator test_b = l2->cbegin();
    std::list<int>::const_iterator test_e = l2->cend();
    // a hack to prevent it being optimized out
    for (std::list<int>::const_iterator i = test_b; i != test_e; ++i) // using untested ++
        tmp += *i;
    CLEAR_LIST;
    END_TX;
    if (tmp != 55)
        std::cout << "["<<id<<"] did not compute 55" << std::endl;
    else if (id == 0)
        std::cout << " [OK] cbegin() and cend()" << std::endl;

    // test ctors and dtors
    global_barrier->arrive(id);
    BEGIN_TX;
    iter_list = new std::list<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    // iterators should be default constructable (1)
    std::list<int>::const_iterator i;
    // iterators should be copy constructable(2)
    std::list<int>::const_iterator b(iter_list->cbegin());
    // iterators should be copy assignable (3)
    std::list<int>::const_iterator e = iter_list->cend();
    std::list<int>::const_iterator* pe = new std::list<int>::const_iterator(e);
    // manually copy the list, so we can be sure to use i, b, pe
    dsize = 0;
    for (i = b; i != *pe; ++i) {
        data[dsize++] = *i;
    }
    // iterators should be destructable (4)
    delete pe;
    CLEAR_LIST;
    END_TX;
    VERIFY("ctors and dtors", 10, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10});

    // iterators should be comparable with == (5) and != (6)
    global_barrier->arrive(id);
    std::list<int>::const_iterator e;
    if (id == 0) {
        iter_list = new std::list<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    }
    global_barrier->arrive(id);
    e = iter_list->cend();
    bool eq, neq;
    BEGIN_TX;
    eq = (e == iter_list->cend());
    neq = (e != iter_list->cbegin());
    END_TX;
    if (!eq || !neq)
        std::cout << "["<<id<<"] did not observe correct equality results" << std::endl;
    else if (id == 0)
        std::cout << " [OK] != and ==" << std::endl;

    // test operator* and operator->
    global_barrier->arrive(id);
    if (id == 0) {
        foo.emplace(foo.cbegin());
        CLEAR_LIST;
    }
    global_barrier->arrive(id);
    int val1, val2, val3, val4;
    BEGIN_TX;
    iter_list = new std::list<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    std::list<int>::const_iterator b = iter_list->cbegin();
    // iterators can be dereferenced as an rvalue if in a dereferenceable
    // state (7)
    val1 = *b;
#if 0 // does not apply to const iterators
    // iterators can be dereferenced as an lvalue (8)
    *b = -6;
    val2 = *iter_list->cbegin();
#endif
    // iterator access with ->
    std::list<Q>::const_iterator x = foo.cbegin();
    val3 = x->a;
    val4 = x->b;
    CLEAR_LIST;
    END_TX;
    bool ok = (val1 == 1) && (val3 == 1) && (val4 == 2);
    if (!ok)
        std::cout << "["<<id<<"] errors on * or ->" << std::endl;
    else if (id == 0)
        std::cout << " [OK] operators * and ->" << std::endl;
    global_barrier->arrive(id);
    if (id == 0) {
        foo.clear();
    }

    // test ++
    global_barrier->arrive(id);
    val1 = val2 = val3 = val4 = -8;
    int t1, t2, t3, t4;
    ok = true;
    BEGIN_TX;
    iter_list = new std::list<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    std::list<int>::const_iterator i = iter_list->cbegin();
    // iterators can be incremented (9)
    i++;
    val1 = *i;
    // iterators can be incremented (10)
    ++i;
    val2 = *i;
    // iterators can be incremented (11)
    val3 = *i++;
    val4 = *i;
    // two iterators that compare equal remain equal after being increased
    // (12)
    std::list<int>::const_iterator a1(iter_list->cbegin());
    std::list<int>::const_iterator a2(iter_list->cbegin());
    t1 = *a1;
    t2 = *a2;
    ok  &= (a1 == a2) && (t1 == 1) && (t2 == 1);
    a1++;
    a2++;
    t3 = *a1;
    t4 = *a2;
    ok &= (a1 == a2) && (t3 == 2) && (t4 == 2);
    CLEAR_LIST;
    END_TX;
    ok &= (val1 == 2) && (val2 == 3) && (val3 == 3) && (val4 == 4);
    if (!ok)
        std::cout << "["<<id<<"] errors on ++ tests" << std::endl;
    else if (id == 0)
        std::cout << " [OK] operator ++" << std::endl;

    // test operator --
    global_barrier->arrive(id);
    BEGIN_TX;
    iter_list = new std::list<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    std::list<int>::const_iterator i = iter_list->cend();
    // can be decremented (13)
    --i;
    val1 = *i;
    // can be decremented (14)
    i--;
    val2 = *i;
    // can be decremented (15)
    val3 = *i--;
    val4 = *i;
    CLEAR_LIST;
    END_TX;
    ok = (val1 == 10) && (val2 == 9) && (val3 == 9) && (val4 == 8);
    if (!ok)
        std::cout << "["<<id<<"] errors on -- tests" << std::endl;
    else if (id == 0)
        std::cout << " [OK] operator --" << std::endl;

    // test swapping (16)
    global_barrier->arrive(id);
    BEGIN_TX;
    iter_list = new std::list<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    std::list<int>::const_iterator b = iter_list->cbegin();
    std::list<int>::const_iterator e = iter_list->cend();
    swap(b, e);
    // manually copy the list, so we can be sure to use i, b, e
    dsize = 0;
    for (std::list<int>::const_iterator i = e; i != b; ++i) {
        data[dsize++] = *i;
    }
    CLEAR_LIST;
    END_TX;
    VERIFY("swap", 10, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10});

    global_barrier->arrive(id);
}

/**
 * Transactional test of C++11 const reverse iterators with crbegin
 */
void const_reverse_iterator_test_concurrent(int id)
{
    // invariant: the list starts off null
    assert(iter_list == NULL);

    // helper message
    if (id == 0) {
        std::cout << std::endl
                  << "Testing legacy const reverse iterators" << std::endl;
    }

    // scratch space for holding data when computing whether the test
    // succeeded or failed.
    int data[32], dsize = 0;

    // test begin and end
    global_barrier->arrive(id);
    int tmp = 0;
    BEGIN_TX;
    // make a list
    iter_list = new std::list<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    // use begin and end
    const std::list<int>* l2 = iter_list;
    std::list<int>::const_reverse_iterator test_b = l2->crbegin();
    std::list<int>::const_reverse_iterator test_e = l2->crend();
    // a hack to prevent it being optimized out
    for (std::list<int>::const_reverse_iterator i = test_b; i != test_e; ++i) // using untested ++
        tmp += *i;
    CLEAR_LIST;
    END_TX;
    if (tmp != 55)
        std::cout << "["<<id<<"] did not compute 55" << std::endl;
    else if (id == 0)
        std::cout << " [OK] crbegin() and crend()" << std::endl;

    // test ctors and dtors
    global_barrier->arrive(id);
    BEGIN_TX;
    iter_list = new std::list<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    // iterators should be default constructable (1)
    std::list<int>::const_reverse_iterator i;
    // iterators should be copy constructable(2)
    std::list<int>::const_reverse_iterator b(iter_list->crbegin());
    // iterators should be copy assignable (3)
    std::list<int>::const_reverse_iterator e = iter_list->crend();
    std::list<int>::const_reverse_iterator* pe = new std::list<int>::const_reverse_iterator(e);
    // manually copy the list, so we can be sure to use i, b, pe
    dsize = 0;
    for (i = b; i != *pe; ++i) {
        data[dsize++] = *i;
    }
    // iterators should be destructable (4)
    delete pe;
    CLEAR_LIST;
    END_TX;
    VERIFY("ctors and dtors", 10, {10, 9, 8, 7, 6, 5, 4, 3, 2, 1});

    // iterators should be comparable with == (5) and != (6)
    global_barrier->arrive(id);
    std::list<int>::const_reverse_iterator e;
    if (id == 0) {
        iter_list = new std::list<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    }
    global_barrier->arrive(id);
    e = iter_list->crend();
    bool eq, neq;
    BEGIN_TX;
    eq = (e == iter_list->crend());
    neq = (e != iter_list->crbegin());
    END_TX;
    if (!eq || !neq)
        std::cout << "["<<id<<"] did not observe correct equality results" << std::endl;
    else if (id == 0)
        std::cout << " [OK] != and ==" << std::endl;

    // test operator* and operator->
    global_barrier->arrive(id);
    if (id == 0) {
        foo.emplace(foo.begin());
        CLEAR_LIST;
    }
    global_barrier->arrive(id);
    int val1, val2, val3, val4;
    BEGIN_TX;
    iter_list = new std::list<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    std::list<int>::const_reverse_iterator b = iter_list->crbegin();
    // iterators can be dereferenced as an rvalue if in a dereferenceable
    // state (7)
    val1 = *b;
#if 0 // does not apply to const iterators
    // iterators can be dereferenced as an lvalue (8)
    *b = -6;
    val2 = *iter_list->begin();
#endif
    // iterator access with ->
    std::list<Q>::const_reverse_iterator x = foo.crbegin();
    val3 = x->a;
    val4 = x->b;
    CLEAR_LIST;
    END_TX;
    bool ok = (val1 == 10) && (val3 == 1) && (val4 == 2);
    if (!ok)
        std::cout << "["<<id<<"] errors on * or ->" << std::endl;
    else if (id == 0)
        std::cout << " [OK] operators * and ->" << std::endl;
    global_barrier->arrive(id);
    if (id == 0) {
        foo.clear();
    }

    // test ++
    global_barrier->arrive(id);
    val1 = val2 = val3 = val4 = -8;
    int t1, t2, t3, t4;
    ok = true;
    BEGIN_TX;
    iter_list = new std::list<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    std::list<int>::const_reverse_iterator i = iter_list->crbegin();
    // iterators can be incremented (9)
    i++;
    val1 = *i;
    // iterators can be incremented (10)
    ++i;
    val2 = *i;
    // iterators can be incremented (11)
    val3 = *i++;
    val4 = *i;
    // two iterators that compare equal remain equal after being increased
    // (12)
    std::list<int>::const_reverse_iterator a1(iter_list->crbegin());
    std::list<int>::const_reverse_iterator a2(iter_list->crbegin());
    t1 = *a1;
    t2 = *a2;
    ok  &= (a1 == a2) && (t1 == 10) && (t2 == 10);
    a1++;
    a2++;
    t3 = *a1;
    t4 = *a2;
    ok &= (a1 == a2) && (t3 == 9) && (t4 == 9);
    CLEAR_LIST;
    END_TX;
    ok &= (val1 == 9) && (val2 == 8) && (val3 == 8) && (val4 == 7);
    if (!ok)
        std::cout << "["<<id<<"] errors on ++ tests" << std::endl;
    else if (id == 0)
        std::cout << " [OK] operator ++" << std::endl;

    // test operator --
    global_barrier->arrive(id);
    BEGIN_TX;
    iter_list = new std::list<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    std::list<int>::const_reverse_iterator i = iter_list->crbegin();
    i++;i++;i++;
    // can be decremented (13)
    --i;
    val1 = *i;
    // can be decremented (14)
    i--;
    val2 = *i;
    // can be decremented (15)
    val3 = *i--;
    val4 = *i;
    CLEAR_LIST;
    END_TX;
    ok = (val1 == 8) && (val2 == 9) && (val3 == 9) && (val4 == 10);
    if (!ok)
        std::cout << "["<<id<<"] errors on -- tests" << std::endl;
    else if (id == 0)
        std::cout << " [OK] operator --" << std::endl;

    // test swapping (16)
    global_barrier->arrive(id);
    BEGIN_TX;
    iter_list = new std::list<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    std::list<int>::const_reverse_iterator b = iter_list->crbegin();
    std::list<int>::const_reverse_iterator e = iter_list->crend();
    swap(b, e);
    // manually copy the list, so we can be sure to use i, b, e
    dsize = 0;
    for (std::list<int>::const_reverse_iterator i = e; i != b; ++i) {
        data[dsize++] = *i;
    }
    CLEAR_LIST;
    END_TX;
    VERIFY("swap", 10, {10, 9, 8, 7, 6, 5, 4, 3, 2, 1});

    global_barrier->arrive(id);
}
