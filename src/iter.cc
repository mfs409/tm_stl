#include <iostream>
#include <string>
#include <list>
#include "sequential_tests.h"

/**
 * Verify that our iterators support the features described in
 * http://www.cplusplus.com/reference/iterator/BidirectionalIterator/
 */

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

/**
 *  This is for basic (forward, mutable) iterators
 */
void iterator_test_seq()
{
    std::cout << "Testing iterators" << std::endl;

    // use the initializer list constructor
    my_list = new std::list<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});

    // test begin and end
    std::list<int>::iterator test_b = my_list->begin();
    std::list<int>::iterator test_e = my_list->end();
    // a hack to prevent it being optimized out
    int tmp = 0;
    for (auto i = test_b; i != test_e; ++i)
        tmp += *i;
    std::cout << "computed " << tmp << " expected 55" << std::endl;

    // iterators should be default constructable (1)
    std::list<int>::iterator i;
    // iterators should be copy constructable(2)
    std::list<int>::iterator b(my_list->begin());
    // iterators should be copy assignable (3)
    std::list<int>::iterator e = my_list->end();
    // note: above code also tested begin() and end()

    // iterators should be destructable (4)
    std::list<int>::iterator* pe = new std::list<int>::iterator(e);
    delete pe;

    // iterators should be comparable with == (5) and != (6)
    bool eq, neq;
    eq = (e == my_list->end());
    neq = (e != my_list->begin());
    std::cout << "Equality test " << (eq ? "passed" : "failed") << std::endl;
    std::cout << "Inequality test " << (neq ? "passed" : "failed") << std::endl;

    // iterators can be dereferenced as an rvalue if in a dereferenceable
    // state (7)
    int val = *b;
    std::cout << "Expecting 1, found " << val << std::endl;

    // iterators can be dereferenced as an lvalue (8)
    *b = -6;
    val = *my_list->begin();
    std::cout << "Expecting -6, found " << val << std::endl;

    // iterator access with ->
    std::list<Q> foo;
    foo.emplace(foo.begin());
    auto x = foo.begin();
    std::cout << "Expecting 1, 2: " << x->a << ", " << x->b << std::endl;

    // iterators can be incremented (9)
    i = b;
    i++;
    val = *i;
    std::cout << "Expecting 2, found " << val << std::endl;
    // iterators can be incremented (10)
    ++i;
    val = *i;
    std::cout << "Expecting 3, found " << val << std::endl;
    // iterators can be incremented (11)
    val = *i++;
    std::cout << "Expecting 3, found " << val << std::endl;
    val = *i;
    std::cout << "Expecting 4, found " << val << std::endl;
    // two iterators that compare equal remain equal after being increased
    // (12)
    std::list<int>::iterator a1(b);
    std::list<int>::iterator a2(b);
    val = *a1;
    int val2 = *a2;
    std::cout << "Expecting -6, -6, found " << val << ", " << val2 << std::endl;
    eq = (a1 == a2);
    std::cout << "Equality test " << (eq ? "passed" : "failed") << std::endl;
    a1++;
    a2++;
    val = *a1;
    val2 = *a2;
    std::cout << "Expecting 2, 2, found " << val << ", " << val2 << std::endl;
    eq = (a1 == a2);
    std::cout << "Equality test " << (eq ? "passed" : "failed") << std::endl;
    // can be decremented (13)
    --i;
    val = *i;
    std::cout << "Expecting 3, found " << val << std::endl;
    // can be decremented (14)
    i--;
    val = *i;
    std::cout << "Expecting 2, found " << val << std::endl;
    // can be decremented (15)
    val = *i--;
    std::cout << "Expecting 2, found " << val << std::endl;
    val = *i;
    std::cout << "Expecting -6, found " << val << std::endl;
    // are swappable
    swap(b, e);
    int vals[10], ctr = 0;
    for (i=e; i != b; ++i)
        vals[ctr++] = *i;
    std::cout << "Expect -6, 2, 3, 4, 5, 6, 7, 8, 9, 10" << std::endl;
    for (int ii = 0; ii < 10; ++ii)
        std::cout << vals[ii] << ", ";
    std::cout << std::endl;
    // clean up
    delete my_list;
}

/**
 *  This is for reverse iterators
 */
void reverse_iterator_test_seq()
{
    std::cout << "Testing reverse iterators" << std::endl;

    // use the initializer list constructor
    my_list = new std::list<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});

    // test begin and end
    std::list<int>::reverse_iterator test_b = my_list->rbegin();
    std::list<int>::reverse_iterator test_e = my_list->rend();
    // a hack to prevent it being optimized out
    int tmp = 0;
    for (auto i = test_b; i != test_e; ++i)
        tmp += *i;
    std::cout << "computed " << tmp << " expected 55" << std::endl;

    // iterators should be default constructable (1)
    std::list<int>::reverse_iterator i;
    // iterators should be copy constructable(2)
    std::list<int>::reverse_iterator b(my_list->rbegin());
    // iterators should be copy assignable (3)
    std::list<int>::reverse_iterator e = my_list->rend();
    // note: above code also tested rbegin() and rend()

    // iterators should be destructable (4)
    std::list<int>::reverse_iterator* pe = new std::list<int>::reverse_iterator(e);
    delete pe;

    // iterators should be comparable with == (5) and != (6)
    bool eq, neq;
    eq = (e == my_list->rend());
    neq = (e != my_list->rbegin());
    std::cout << "Equality test " << (eq ? "passed" : "failed") << std::endl;
    std::cout << "Inequality test " << (neq ? "passed" : "failed") << std::endl;

    // iterators can be dereferenced as an rvalue if in a dereferenceable
    // state (7)
    int val = *b;
    std::cout << "Expecting 10, found " << val << std::endl;

    // iterators can be dereferenced as an lvalue (8)
    *b = -6;
    val = *my_list->rbegin();
    std::cout << "Expecting -6, found " << val << std::endl;

    // iterator access with ->
    std::list<Q> foo;
    foo.emplace(foo.begin());
    std::list<Q>::reverse_iterator x = foo.rbegin();
    std::cout << "-> test... Expecting 1, 2: " << x->a << ", " << x->b << std::endl;

    // iterators can be incremented (9)
    i = b;
    i++;
    val = *i;
    std::cout << "Expecting 9, found " << val << std::endl;
    // iterators can be incremented (10)
    ++i;
    val = *i;
    std::cout << "Expecting 8, found " << val << std::endl;
    // iterators can be incremented (11)
    val = *i++;
    std::cout << "Expecting 8, found " << val << std::endl;
    val = *i;
    std::cout << "Expecting 7, found " << val << std::endl;
    // two iterators that compare equal remain equal after being increased
    // (12)
    std::list<int>::reverse_iterator a1(b);
    std::list<int>::reverse_iterator a2(b);
    val = *a1;
    int val2 = *a2;
    std::cout << "Expecting -6, -6, found " << val << ", " << val2 << std::endl;
    eq = (a1 == a2);
    std::cout << "Equality test " << (eq ? "passed" : "failed") << std::endl;
    a1++;
    a2++;
    val = *a1;
    val2 = *a2;
    std::cout << "Expecting 9, 9, found " << val << ", " << val2 << std::endl;
    eq = (a1 == a2);
    std::cout << "Equality test " << (eq ? "passed" : "failed") << std::endl;
    // can be decremented (13)
    --i;
    val = *i;
    std::cout << "Expecting 8, found " << val << std::endl;
    // can be decremented (14)
    i--;
    val = *i;
    std::cout << "Expecting 9, found " << val << std::endl;
    // can be decremented (15)
    val = *i--;
    std::cout << "Expecting 9, found " << val << std::endl;
    val = *i;
    std::cout << "Expecting -6, found " << val << std::endl;
    // are swappable
    swap(b, e);
    int vals[10], ctr = 0;
    for (i=e; i != b; ++i)
        vals[ctr++] = *i;
    std::cout << "Expect -6, 9, 8, 7, 6, 5, 4, 3, 2, 1" << std::endl;
    for (int ii = 0; ii < 10; ++ii)
        std::cout << vals[ii] << ", ";
    std::cout << std::endl;
    delete my_list;
}

/**
 *  This is for legacy const iterators, where we use begin()
 */
void legacy_const_iterator_test_seq()
{
    std::cout << "Testing legacy const iterators" << std::endl;

    // use the initializer list constructor
    my_list = new std::list<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});

    // test begin and end
    const std::list<int>* l2 = my_list;
    std::list<int>::const_iterator test_b = l2->begin();
    std::list<int>::const_iterator test_e = l2->end();
    // a hack to prevent it being optimized out
    int tmp = 0;
    for (auto i = test_b; i != test_e; ++i)
        tmp += *i;
    std::cout << "computed " << tmp << " expected 55" << std::endl;

    // iterators should be default constructable (1)
    std::list<int>::const_iterator i;
    // iterators should be copy constructable(2)
    std::list<int>::const_iterator b(my_list->begin());
    // iterators should be copy assignable (3)
    std::list<int>::const_iterator e = my_list->end();
    // note: above code also tested begin() and end()

    // iterators should be destructable (4)
    std::list<int>::const_iterator* pe = new std::list<int>::const_iterator(e);
    delete pe;

    // iterators should be comparable with == (5) and != (6)
    bool eq, neq;
    eq = (e == my_list->end());
    neq = (e != my_list->begin());
    std::cout << "Equality test " << (eq ? "passed" : "failed") << std::endl;
    std::cout << "Inequality test " << (neq ? "passed" : "failed") << std::endl;

    // iterators can be dereferenced as an rvalue if in a dereferenceable
    // state (7)
    int val = *b;
    std::cout << "Expecting 1, found " << val << std::endl;

    // iterator access with ->
    std::list<Q> foo;
    foo.emplace(foo.begin());
    std::list<Q>::const_iterator x = foo.begin();
    std::cout << "Expecting 1, 2: " << x->a << ", " << x->b << std::endl;

    // iterators can be incremented (9)
    i = b;
    i++;
    val = *i;
    std::cout << "Expecting 2, found " << val << std::endl;
    // iterators can be incremented (10)
    ++i;
    val = *i;
    std::cout << "Expecting 3, found " << val << std::endl;
    // iterators can be incremented (11)
    val = *i++;
    std::cout << "Expecting 3, found " << val << std::endl;
    val = *i;
    std::cout << "Expecting 4, found " << val << std::endl;
    // two iterators that compare equal remain equal after being increased
    // (12)
    std::list<int>::const_iterator a1(b);
    std::list<int>::const_iterator a2(b);
    val = *a1;
    int val2 = *a2;
    std::cout << "Expecting 1, 1, found " << val << ", " << val2 << std::endl;
    eq = (a1 == a2);
    std::cout << "Equality test " << (eq ? "passed" : "failed") << std::endl;
    a1++;
    a2++;
    val = *a1;
    val2 = *a2;
    std::cout << "Expecting 2, 2, found " << val << ", " << val2 << std::endl;
    eq = (a1 == a2);
    std::cout << "Equality test " << (eq ? "passed" : "failed") << std::endl;
    // can be decremented (13)
    --i;
    val = *i;
    std::cout << "Expecting 3, found " << val << std::endl;
    // can be decremented (14)
    i--;
    val = *i;
    std::cout << "Expecting 2, found " << val << std::endl;
    // can be decremented (15)
    val = *i--;
    std::cout << "Expecting 2, found " << val << std::endl;
    val = *i;
    std::cout << "Expecting 1, found " << val << std::endl;
    // are swappable
    swap(b, e);
    int vals[10], ctr = 0;
    for (i=e; i != b; ++i)
        vals[ctr++] = *i;
    std::cout << "Expect 1, 2, 3, 4, 5, 6, 7, 8, 9, 10" << std::endl;
    for (int ii = 0; ii < 10; ++ii)
        std::cout << vals[ii] << ", ";
    std::cout << std::endl;
    // clean up
    delete my_list;
}

/**
 *  This is for legacy const reverse iterators, where we use rbegin()
 */
void legacy_const_reverse_iterator_test_seq()
{
    std::cout << "Testing legacy const reverse iterators" << std::endl;

    // use the initializer list constructor
    my_list = new std::list<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});

    // test begin and end
    const std::list<int>* l2 = my_list;
    std::list<int>::const_reverse_iterator test_b = l2->rbegin();
    std::list<int>::const_reverse_iterator test_e = l2->rend();
    // a hack to prevent it being optimized out
    int tmp = 0;
    for (auto i = test_b; i != test_e; ++i)
        tmp += *i;
    std::cout << "computed " << tmp << " expected 55" << std::endl;

    // iterators should be default constructable (1)
    std::list<int>::const_reverse_iterator i;
    // iterators should be copy constructable(2)
    std::list<int>::const_reverse_iterator b(my_list->rbegin());
    // iterators should be copy assignable (3)
    std::list<int>::const_reverse_iterator e = my_list->rend();
    // note: above code also tested rbegin() and rend()

    // iterators should be destructable (4)
    std::list<int>::const_reverse_iterator* pe = new std::list<int>::const_reverse_iterator(e);
    delete pe;

    // iterators should be comparable with == (5) and != (6)
    bool eq, neq;
    eq = (e == my_list->rend());
    neq = (e != my_list->rbegin());
    std::cout << "Equality test " << (eq ? "passed" : "failed") << std::endl;
    std::cout << "Inequality test " << (neq ? "passed" : "failed") << std::endl;

    // iterators can be dereferenced as an rvalue if in a dereferenceable
    // state (7)
    int val = *b;
    std::cout << "Expecting 10, found " << val << std::endl;

    // iterator access with ->
    std::list<Q> foo;
    foo.emplace(foo.begin());
    std::list<Q>::const_reverse_iterator x = foo.rbegin();
    std::cout << "-> test... Expecting 1, 2: " << x->a << ", " << x->b << std::endl;

    // iterators can be incremented (9)
    i = b;
    i++;
    val = *i;
    std::cout << "Expecting 9, found " << val << std::endl;
    // iterators can be incremented (10)
    ++i;
    val = *i;
    std::cout << "Expecting 8, found " << val << std::endl;
    // iterators can be incremented (11)
    val = *i++;
    std::cout << "Expecting 8, found " << val << std::endl;
    val = *i;
    std::cout << "Expecting 7, found " << val << std::endl;
    // two iterators that compare equal remain equal after being increased
    // (12)
    std::list<int>::const_reverse_iterator a1(b);
    std::list<int>::const_reverse_iterator a2(b);
    val = *a1;
    int val2 = *a2;
    std::cout << "Expecting 10, 10, found " << val << ", " << val2 << std::endl;
    eq = (a1 == a2);
    std::cout << "Equality test " << (eq ? "passed" : "failed") << std::endl;
    a1++;
    a2++;
    val = *a1;
    val2 = *a2;
    std::cout << "Expecting 9, 9, found " << val << ", " << val2 << std::endl;
    eq = (a1 == a2);
    std::cout << "Equality test " << (eq ? "passed" : "failed") << std::endl;
    // can be decremented (13)
    --i;
    val = *i;
    std::cout << "Expecting 8, found " << val << std::endl;
    // can be decremented (14)
    i--;
    val = *i;
    std::cout << "Expecting 9, found " << val << std::endl;
    // can be decremented (15)
    val = *i--;
    std::cout << "Expecting 9, found " << val << std::endl;
    val = *i;
    std::cout << "Expecting 10, found " << val << std::endl;
    // are swappable
    swap(b, e);
    int vals[10], ctr = 0;
    for (i=e; i != b; ++i)
        vals[ctr++] = *i;
    std::cout << "Expect 10, 9, 8, 7, 6, 5, 4, 3, 2, 1" << std::endl;
    for (int ii = 0; ii < 10; ++ii)
        std::cout << vals[ii] << ", ";
    std::cout << std::endl;
    delete my_list;
}

/**
 *  This is for c++ const iterators, where we use cbegin()
 */
void const_iterator_test_seq()
{
    std::cout << "Testing C++11 const iterators" << std::endl;

    // use the initializer list constructor
    my_list = new std::list<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});

    // iterators should be default constructable (1)
    std::list<int>::const_iterator i;
    // iterators should be copy constructable(2)
    std::list<int>::const_iterator b(my_list->cbegin());
    // iterators should be copy assignable (3)
    std::list<int>::const_iterator e = my_list->cend();
    // note: above code also tested cbegin() and cend()

    // iterators should be destructable (4)
    std::list<int>::const_iterator* pe = new std::list<int>::const_iterator(e);
    delete pe;

    // iterators should be comparable with == (5) and != (6)
    bool eq, neq;
    eq = (e == my_list->cend());
    neq = (e != my_list->cbegin());
    std::cout << "Equality test " << (eq ? "passed" : "failed") << std::endl;
    std::cout << "Inequality test " << (neq ? "passed" : "failed") << std::endl;

    // iterators can be dereferenced as an rvalue if in a dereferenceable
    // state (7)
    int val = *b;
    std::cout << "Expecting 1, found " << val << std::endl;

    // iterators can be incremented (9)
    i = b;
    i++;
    val = *i;
    std::cout << "Expecting 2, found " << val << std::endl;
    // iterators can be incremented (10)
    ++i;
    val = *i;
    std::cout << "Expecting 3, found " << val << std::endl;
    // iterators can be incremented (11)
    val = *i++;
    std::cout << "Expecting 3, found " << val << std::endl;
    val = *i;
    std::cout << "Expecting 4, found " << val << std::endl;
    // two iterators that compare equal remain equal after being increased
    // (12)
    std::list<int>::const_iterator a1(b);
    std::list<int>::const_iterator a2(b);
    val = *a1;
    int val2 = *a2;
    std::cout << "Expecting 1, 1, found " << val << ", " << val2 << std::endl;
    eq = (a1 == a2);
    std::cout << "Equality test " << (eq ? "passed" : "failed") << std::endl;
    a1++;
    a2++;
    val = *a1;
    val2 = *a2;
    std::cout << "Expecting 2, 2, found " << val << ", " << val2 << std::endl;
    eq = (a1 == a2);
    std::cout << "Equality test " << (eq ? "passed" : "failed") << std::endl;
    // can be decremented (13)
    --i;
    val = *i;
    std::cout << "Expecting 3, found " << val << std::endl;
    // can be decremented (14)
    i--;
    val = *i;
    std::cout << "Expecting 2, found " << val << std::endl;
    // can be decremented (15)
    val = *i--;
    std::cout << "Expecting 2, found " << val << std::endl;
    val = *i;
    std::cout << "Expecting 1, found " << val << std::endl;
    // are swappable
    swap(b, e);
    int vals[10], ctr = 0;
    for (i=e; i != b; ++i)
        vals[ctr++] = *i;
    std::cout << "Expect 1, 2, 3, 4, 5, 6, 7, 8, 9, 10" << std::endl;
    for (int ii = 0; ii < 10; ++ii)
        std::cout << vals[ii] << ", ";
    std::cout << std::endl;
    // clean up
    delete my_list;
}

/**
 *  This is for C++11 const reverse iterators, where we use crbegin()
 */
void const_reverse_iterator_test_seq()
{
    std::cout << "Testing C++11 const reverse iterators" << std::endl;

    // use the initializer list constructor
    my_list = new std::list<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});

    // iterators should be default constructable (1)
    std::list<int>::const_reverse_iterator i;
    // iterators should be copy constructable(2)
    std::list<int>::const_reverse_iterator b(my_list->crbegin());
    // iterators should be copy assignable (3)
    std::list<int>::const_reverse_iterator e = my_list->crend();
    // note: above code also tested crbegin() and crend()

    // iterators should be destructable (4)
    std::list<int>::const_reverse_iterator* pe = new std::list<int>::const_reverse_iterator(e);
    delete pe;

    // iterators should be comparable with == (5) and != (6)
    bool eq, neq;
    eq = (e == my_list->crend());
    neq = (e != my_list->crbegin());
    std::cout << "Equality test " << (eq ? "passed" : "failed") << std::endl;
    std::cout << "Inequality test " << (neq ? "passed" : "failed") << std::endl;

    // iterators can be dereferenced as an rvalue if in a dereferenceable
    // state (7)
    int val = *b;
    std::cout << "Expecting 10, found " << val << std::endl;

    // iterators can be incremented (9)
    i = b;
    i++;
    val = *i;
    std::cout << "Expecting 9, found " << val << std::endl;
    // iterators can be incremented (10)
    ++i;
    val = *i;
    std::cout << "Expecting 8, found " << val << std::endl;
    // iterators can be incremented (11)
    val = *i++;
    std::cout << "Expecting 8, found " << val << std::endl;
    val = *i;
    std::cout << "Expecting 7, found " << val << std::endl;
    // two iterators that compare equal remain equal after being increased
    // (12)
    std::list<int>::const_reverse_iterator a1(b);
    std::list<int>::const_reverse_iterator a2(b);
    val = *a1;
    int val2 = *a2;
    std::cout << "Expecting 10, 10, found " << val << ", " << val2 << std::endl;
    eq = (a1 == a2);
    std::cout << "Equality test " << (eq ? "passed" : "failed") << std::endl;
    a1++;
    a2++;
    val = *a1;
    val2 = *a2;
    std::cout << "Expecting 9, 9, found " << val << ", " << val2 << std::endl;
    eq = (a1 == a2);
    std::cout << "Equality test " << (eq ? "passed" : "failed") << std::endl;
    // can be decremented (13)
    --i;
    val = *i;
    std::cout << "Expecting 8, found " << val << std::endl;
    // can be decremented (14)
    i--;
    val = *i;
    std::cout << "Expecting 9, found " << val << std::endl;
    // can be decremented (15)
    val = *i--;
    std::cout << "Expecting 9, found " << val << std::endl;
    val = *i;
    std::cout << "Expecting 10, found " << val << std::endl;
    // are swappable
    swap(b, e);
    int vals[10], ctr = 0;
    for (i=e; i != b; ++i)
        vals[ctr++] = *i;
    std::cout << "Expect 10, 9, 8, 7, 6, 5, 4, 3, 2, 1" << std::endl;
    for (int ii = 0; ii < 10; ++ii)
        std::cout << vals[ii] << ", ";
    std::cout << std::endl;
    delete my_list;
}
