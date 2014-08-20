#include <iostream>
#include <deque>
#include "tests.h"
#include "verify.h"

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

/// The deques we will use for our tests
std::deque<int>* iter_deque_int = NULL;
std::deque<Q>*   iter_deque_q   = NULL;

/**
 * Ensure that all of the begin/end methods for getting an iterator from
 * std::deque work
 */
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
        verifier v;
        BEGIN_TX;
        iter_deque_int = new std::deque<int>();
        std::deque<int>::iterator b = iter_deque_int->begin();
        std::deque<int>::iterator e = iter_deque_int->end();
        ok = (b == e);
        delete(iter_deque_int);
        iter_deque_int = NULL;
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
        verifier v;
        BEGIN_TX;
        iter_deque_int = new std::deque<int>();
        const std::deque<int>* cd = iter_deque_int;
        std::deque<int>::const_iterator b = cd->begin();
        std::deque<int>::const_iterator e = cd->end();
        ok = (b == e);
        delete(iter_deque_int);
        iter_deque_int = NULL;
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
        verifier v;
        BEGIN_TX;
        iter_deque_int = new std::deque<int>();
        std::deque<int>::reverse_iterator b = iter_deque_int->rbegin();
        std::deque<int>::reverse_iterator e = iter_deque_int->rend();
        ok = (b == e);
        delete(iter_deque_int);
        iter_deque_int = NULL;
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
        verifier v;
        BEGIN_TX;
        iter_deque_int = new std::deque<int>();
        const std::deque<int>* cd = iter_deque_int;
        std::deque<int>::const_reverse_iterator b = cd->rbegin();
        std::deque<int>::const_reverse_iterator e = cd->rend();
        ok = (b == e);
        delete(iter_deque_int);
        iter_deque_int = NULL;
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
        verifier v;
        BEGIN_TX;
        iter_deque_int = new std::deque<int>();
        const std::deque<int>* cd = iter_deque_int;
        std::deque<int>::const_iterator b = cd->cbegin();
        std::deque<int>::const_iterator e = cd->cend();
        ok = (b == e);
        delete(iter_deque_int);
        iter_deque_int = NULL;
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
        verifier v;
        BEGIN_TX;
        iter_deque_int = new std::deque<int>();
        const std::deque<int>* cd = iter_deque_int;
        std::deque<int>::const_reverse_iterator b = cd->crbegin();
        std::deque<int>::const_reverse_iterator e = cd->crend();
        ok = (b == e);
        delete(iter_deque_int);
        iter_deque_int = NULL;
        END_TX;
        if (!ok)
            printf(" [%d] iterator crbegin and crend did not match for empty deque", id);
        else if (id == 0)
            printf(" [OK] %s\n", "c++11 crbegin and crend");
    }
}

/**
 *  Make sure that std::deque's random access iterator methods work
 *  (http://www.cplusplus.com/reference/iterator/RandomAccessIterator/)
 */
void iter_method_tests(int id)
{
    global_barrier->arrive(id);

    if (id == 0)
        printf("Testing basic methods of std::deque::iterator\n");

    // test copy-assignable, copy constructable, and destructable
    //
    // NB: the implementation has 3 ctors, this only manages to get two of them to run
    global_barrier->arrive(id);
    {
        verifier v;
        BEGIN_TX;
        iter_deque_int = new std::deque<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
        // iterators should be default constructable
        std::deque<int>::iterator i = std::deque<int>::iterator();
        // iterators should be copy constructable
        std::deque<int>::iterator b(iter_deque_int->begin());
        // iterators should be copy assignable
        std::deque<int>::iterator e = iter_deque_int->end();
        std::deque<int>::iterator* pe = new std::deque<int>::iterator(e);
        // manually copy the vector, so we can be sure to use i, b, and
        // pe... otherwise this might get optimized out
        for (i = b; i != *pe; ++i)
            v.insert(*i);
        // iterators should be destructable
        delete pe;
        END_TX;
        v.check("ctors, dtors, and assignment", id, 10, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    }

    // iterators should be swappable
    //
    // NB: not sure how to get this to TRACE
    global_barrier->arrive(id);
    {
        verifier v;
        BEGIN_TX;
        iter_deque_int = new std::deque<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
        std::deque<int>::iterator b = iter_deque_int->begin();
        std::deque<int>::iterator e = iter_deque_int->end();
        std::swap(b, e);
        // manually copy the vector, so we can be sure to use b, e
        for (std::deque<int>::iterator i = e; i != b; ++i)
            v.insert(*i);
        END_TX;
        v.check("swap iterators", id, 10, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    }
}

/**
 *  Test Iterator methods (operators)
 */
void iter_operator_tests(int id)
{
    global_barrier->arrive(id);
    if (id == 0)
        printf("Testing operator methods on std::deque::iterator\n");
}

/**
 * Test Iterator overloads (comparison operators)
 */
void iter_overload_tests(int id)
{
    global_barrier->arrive(id);
    if (id == 0)
        printf("Testing non-method comparator overloads on std::deque::iterator\n");

    // test comparability of iterators
    global_barrier->arrive(id);
    {
        bool ok = true;
        BEGIN_TX;
        iter_deque_int = new std::deque<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
        std::deque<int>::iterator b = iter_deque_int->begin();
        std::deque<int>::iterator e = iter_deque_int->end();
        std::deque<int>::const_iterator ce = iter_deque_int->cend();
        ok &= b != e;
        ok &= b != ce;
        ok &= !(b == e);
        ok &= !(b == ce);
        END_TX;
        if (!ok)
            printf(" [%d] error with operator== and operator!=\n", id);
        else if (id == 0)
            printf(" [OK] %s\n", "iterator comparability checks passed");
    }
}

/**
 * Test iterator functions
 */
void iter_function_tests(int id)
{
    global_barrier->arrive(id);
    if (id == 0)
        printf("Testing non-method functions on std::deque::iterator\n");
}

#if 0
void legacy_tests
{

    // iterators should be comparable with == (5) and != (6)
    global_barrier->arrive(id);
    std::vector<int>::iterator e;
    if (id == 0) {
        iter_vector = new std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    }
    global_barrier->arrive(id);
    e = iter_vector->end();
    bool eq, neq;
    BEGIN_TX;
    eq = (e == iter_vector->end());
    neq = (e != iter_vector->begin());
    END_TX;
    if (!eq || !neq)
        std::cout << "["<<id<<"] did not observe correct equality results" << std::endl;
    else if (id == 0)
        std::cout << " [OK] != and ==" << std::endl;

    // test operator* and operator->
    global_barrier->arrive(id);
    if (id == 0) {
        foo.emplace(foo.begin());
        CLEAR_VECTOR;
    }
    global_barrier->arrive(id);
    int val1, val2, val3, val4;
    BEGIN_TX;
    iter_vector = new std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    std::vector<int>::iterator b = iter_vector->begin();
    // iterators can be dereferenced as an rvalue if in a dereferenceable
    // state (7)
    val1 = *b;
    // iterators can be dereferenced as an lvalue (8)
    *b = -6;
    val2 = *iter_vector->begin();
    // iterator access with ->
    std::vector<Q>::iterator x = foo.begin();
    val3 = x->a;
    val4 = x->b;
    CLEAR_VECTOR;
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
    iter_vector = new std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    std::vector<int>::iterator i = iter_vector->begin();
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
    std::vector<int>::iterator a1(iter_vector->begin());
    std::vector<int>::iterator a2(iter_vector->begin());
    t1 = *a1;
    t2 = *a2;
    ok  &= (a1 == a2) && (t1 == 1) && (t2 == 1);
    a1++;
    a2++;
    t3 = *a1;
    t4 = *a2;
    ok &= (a1 == a2) && (t3 == 2) && (t4 == 2);
    CLEAR_VECTOR;
    END_TX;
    ok &= (val1 == 2) && (val2 == 3) && (val3 == 3) && (val4 == 4);
    if (!ok)
        std::cout << "["<<id<<"] errors on ++ tests" << std::endl;
    else if (id == 0)
        std::cout << " [OK] operator ++" << std::endl;

    // test operator --
    global_barrier->arrive(id);
    BEGIN_TX;
    iter_vector = new std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    std::vector<int>::iterator i = iter_vector->end();
    // can be decremented (13)
    --i;
    val1 = *i;
    // can be decremented (14)
    i--;
    val2 = *i;
    // can be decremented (15)
    val3 = *i--;
    val4 = *i;
    CLEAR_VECTOR;
    END_TX;
    ok = (val1 == 10) && (val2 == 9) && (val3 == 9) && (val4 == 8);
    if (!ok)
        std::cout << "["<<id<<"] errors on -- tests" << std::endl;
    else if (id == 0)
        std::cout << " [OK] operator --" << std::endl;


}
#endif

void const_iter_tests(int id)
{
}

void rev_iter_tests(int id)
{
}

void const_rev_iter_tests(int id)
{
}

/**
 * Here is the test of basic iteration.  we test non-const begin() and end(),
 * and then make sure the resulting iterator's functions all work.  Note that
 * the resulting iterator is a random access iterator.
 */
void iter_test(int id)
{
    global_barrier->arrive(id);
    // the first test will simply ensure that we can call begin() and end()
    // correctly
    if (id == 0)
        printf("Testing iterator functions: begin(1), end(1)\n");

    global_barrier->arrive(id);
    {
    bool ok = false;
    verifier v;
    BEGIN_TX;
    iter_deque_int = new std::deque<int>();
    std::deque<int>::iterator b = iter_deque_int->begin();
    std::deque<int>::iterator e = iter_deque_int->end();
    ok = (b == e);
    delete(iter_deque_int);
    iter_deque_int = NULL;
    END_TX;
    if (!ok)
        printf(" [%d] iterator begin and end did not match for empty deque", id);
    else if (id == 0)
        printf(" [OK] %s\n", "basic iterator begin and end");
    }

    // now test copy-assignable, copy constructable, and destructable
    global_barrier->arrive(id);
    verifier v;
    BEGIN_TX;
    iter_deque_int = new std::deque<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    // iterators should be default constructable
    std::deque<int>::iterator i;
    // iterators should be copy constructable
    std::deque<int>::iterator b(iter_deque_int->begin());
    // iterators should be copy assignable
    std::deque<int>::iterator e = iter_deque_int->end();
    std::deque<int>::iterator* pe = new std::deque<int>::iterator(e);
    // manually copy the vector, so we can be sure to use i, b, pe
    for (i = b; i != *pe; ++i)
        v.insert(*i);
    // iterators should be destructable
    delete pe;
    END_TX;
    v.check("ctors and dtors", id, 10, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10});

#if 0
    // iterators should be comparable with == (5) and != (6)
    global_barrier->arrive(id);
    std::vector<int>::iterator e;
    if (id == 0) {
        iter_vector = new std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    }
    global_barrier->arrive(id);
    e = iter_vector->end();
    bool eq, neq;
    BEGIN_TX;
    eq = (e == iter_vector->end());
    neq = (e != iter_vector->begin());
    END_TX;
    if (!eq || !neq)
        std::cout << "["<<id<<"] did not observe correct equality results" << std::endl;
    else if (id == 0)
        std::cout << " [OK] != and ==" << std::endl;

    // test operator* and operator->
    global_barrier->arrive(id);
    if (id == 0) {
        foo.emplace(foo.begin());
        CLEAR_VECTOR;
    }
    global_barrier->arrive(id);
    int val1, val2, val3, val4;
    BEGIN_TX;
    iter_vector = new std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    std::vector<int>::iterator b = iter_vector->begin();
    // iterators can be dereferenced as an rvalue if in a dereferenceable
    // state (7)
    val1 = *b;
    // iterators can be dereferenced as an lvalue (8)
    *b = -6;
    val2 = *iter_vector->begin();
    // iterator access with ->
    std::vector<Q>::iterator x = foo.begin();
    val3 = x->a;
    val4 = x->b;
    CLEAR_VECTOR;
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
    iter_vector = new std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    std::vector<int>::iterator i = iter_vector->begin();
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
    std::vector<int>::iterator a1(iter_vector->begin());
    std::vector<int>::iterator a2(iter_vector->begin());
    t1 = *a1;
    t2 = *a2;
    ok  &= (a1 == a2) && (t1 == 1) && (t2 == 1);
    a1++;
    a2++;
    t3 = *a1;
    t4 = *a2;
    ok &= (a1 == a2) && (t3 == 2) && (t4 == 2);
    CLEAR_VECTOR;
    END_TX;
    ok &= (val1 == 2) && (val2 == 3) && (val3 == 3) && (val4 == 4);
    if (!ok)
        std::cout << "["<<id<<"] errors on ++ tests" << std::endl;
    else if (id == 0)
        std::cout << " [OK] operator ++" << std::endl;

    // test operator --
    global_barrier->arrive(id);
    BEGIN_TX;
    iter_vector = new std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    std::vector<int>::iterator i = iter_vector->end();
    // can be decremented (13)
    --i;
    val1 = *i;
    // can be decremented (14)
    i--;
    val2 = *i;
    // can be decremented (15)
    val3 = *i--;
    val4 = *i;
    CLEAR_VECTOR;
    END_TX;
    ok = (val1 == 10) && (val2 == 9) && (val3 == 9) && (val4 == 8);
    if (!ok)
        std::cout << "["<<id<<"] errors on -- tests" << std::endl;
    else if (id == 0)
        std::cout << " [OK] operator --" << std::endl;

    // test swapping (16)
    global_barrier->arrive(id);
    BEGIN_TX;
    iter_vector = new std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    std::vector<int>::iterator b = iter_vector->begin();
    std::vector<int>::iterator e = iter_vector->end();
    swap(b, e);
    // manually copy the vector, so we can be sure to use i, b, e
    dsize = 0;
    for (std::vector<int>::iterator i = e; i != b; ++i) {
        data[dsize++] = *i;
    }
    CLEAR_VECTOR;
    END_TX;
    VERIFY("swap", 10, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10});

    global_barrier->arrive(id);

#endif
}

void legacy_const_iter_test(int id)
{
    global_barrier->arrive(id);
    // the first test will simply ensure that we can call begin() and end()
    // correctly
    if (id == 0)
        printf("Testing legacy const iterator functions: begin(1), end(1)\n");
    global_barrier->arrive(id);
    {
    bool ok = false;
    verifier v;
    BEGIN_TX;
    iter_deque_int = new std::deque<int>();
    const std::deque<int>* cd = iter_deque_int;
    std::deque<int>::const_iterator b = cd->begin();
    std::deque<int>::const_iterator e = cd->end();
    ok = (b == e);
    delete(iter_deque_int);
    iter_deque_int = NULL;
    END_TX;
    if (!ok)
        printf(" [%d] iterator begin and end did not match for empty deque", id);
    else if (id == 0)
        printf(" [OK] %s\n", "basic iterator begin and end");
    }
}

void const_iter_test(int id)
{
}

void reverse_iter_test(int id)
{
}

void legacy_const_reverse_iter_test(int id)
{
}

void const_reverse_iter_test(int id)
{
}

#if 0
/**
 * Transactional test of basic (forward, mutable) iterators
 */
void iterator_test_seq()
{
    int id = 0;
    // invariant: the vector starts off null
    assert(iter_vector == NULL);

    // helper message
    if (id == 0) {
        // note that we cannot test copy_assignable, because
        // copy_assignable in the gcc vector is achieved via a default
        // method that is automatically generated by the compiler.
        //
        // std::swap is a TODO
        std::cout << std::endl
                  << "Testing iterators (note: no verification for copy "
                  << "assignable, dtor, lvalue_dereference, or swap)"
                  << std::endl;
    }

    // TODO: we should compare const_iterator and iterator using ==
    // and != operators... trace already in place in stl_vector.h

    // scratch space for holding data when computing whether the test
    // succeeded or failed.
    int data[32], dsize = 0;

    // test begin and end
    //barrier
    int tmp = 0;
    {
    // make a vector
    iter_vector = new std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    // use begin and end
    std::vector<int>::iterator test_b = iter_vector->begin();
    std::vector<int>::iterator test_e = iter_vector->end();
    // a hack to prevent it being optimized out
    for (std::vector<int>::iterator i = test_b; i != test_e; ++i) // using untested ++
        tmp += *i;
    CLEAR_VECTOR;
    // NB: implicit call to the implicit iterator dtor method
    }
    if (tmp != 55)
        std::cout << "["<<id<<"] did not compute 55" << std::endl;
    else if (id == 0)
        std::cout << " [OK] begin() and end()" << std::endl;

    // test ctors and dtors
    //barrier
    {
    iter_vector = new std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    // iterators should be default constructable (1)
    std::vector<int>::iterator i;
    // iterators should be copy constructable(2)
    std::vector<int>::iterator b(iter_vector->begin());
    // iterators should be copy assignable (3)
    std::vector<int>::iterator e = iter_vector->end();
    std::vector<int>::iterator* pe = new std::vector<int>::iterator(e);
    // manually copy the vector, so we can be sure to use i, b, pe
    dsize = 0;
    for (i = b; i != *pe; ++i) {
        data[dsize++] = *i;
    }
    // iterators should be destructable (4)
    delete pe;
    CLEAR_VECTOR;
    }
    VERIFY("ctors and dtors", 10, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10});

    // iterators should be comparable with == (5) and != (6)
    //barrier
    std::vector<int>::iterator e;
    if (id == 0) {
        iter_vector = new std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    }
    //barrier
    e = iter_vector->end();
    bool eq, neq;
    {
    eq = (e == iter_vector->end());
    neq = (e != iter_vector->begin());
    }
    if (!eq || !neq)
        std::cout << "["<<id<<"] did not observe correct equality results" << std::endl;
    else if (id == 0)
        std::cout << " [OK] != and ==" << std::endl;

    // test operator* and operator->
    //barrier
    if (id == 0) {
        foo.emplace(foo.begin());
        CLEAR_VECTOR;
    }
    //barrier
    int val1, val2, val3, val4;
    {
    iter_vector = new std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    std::vector<int>::iterator b = iter_vector->begin();
    // iterators can be dereferenced as an rvalue if in a dereferenceable
    // state (7)
    val1 = *b;
    // iterators can be dereferenced as an lvalue (8)
    *b = -6;
    val2 = *iter_vector->begin();
    // iterator access with ->
    std::vector<Q>::iterator x = foo.begin();
    val3 = x->a;
    val4 = x->b;
    CLEAR_VECTOR;
    }
    bool ok = (val1 == 1) && (val2 == -6) && (val3 == 1) && (val4 == 2);
    if (!ok)
        std::cout << "["<<id<<"] errors on * or ->" << std::endl;
    else if (id == 0)
        std::cout << " [OK] operators * and ->" << std::endl;
    //barrier
    if (id == 0) {
        foo.clear();
    }

    // test ++
    //barrier
    val1 = val2 = val3 = val4 = -8;
    int t1, t2, t3, t4;
    ok = true;
    {
    iter_vector = new std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    std::vector<int>::iterator i = iter_vector->begin();
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
    std::vector<int>::iterator a1(iter_vector->begin());
    std::vector<int>::iterator a2(iter_vector->begin());
    t1 = *a1;
    t2 = *a2;
    ok  &= (a1 == a2) && (t1 == 1) && (t2 == 1);
    a1++;
    a2++;
    t3 = *a1;
    t4 = *a2;
    ok &= (a1 == a2) && (t3 == 2) && (t4 == 2);
    CLEAR_VECTOR;
    }
    ok &= (val1 == 2) && (val2 == 3) && (val3 == 3) && (val4 == 4);
    if (!ok)
        std::cout << "["<<id<<"] errors on ++ tests" << std::endl;
    else if (id == 0)
        std::cout << " [OK] operator ++" << std::endl;

    // test operator --
    //barrier
    {
    iter_vector = new std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    std::vector<int>::iterator i = iter_vector->end();
    // can be decremented (13)
    --i;
    val1 = *i;
    // can be decremented (14)
    i--;
    val2 = *i;
    // can be decremented (15)
    val3 = *i--;
    val4 = *i;
    CLEAR_VECTOR;
    }
    ok = (val1 == 10) && (val2 == 9) && (val3 == 9) && (val4 == 8);
    if (!ok)
        std::cout << "["<<id<<"] errors on -- tests" << std::endl;
    else if (id == 0)
        std::cout << " [OK] operator --" << std::endl;

    // test swapping (16)
    //barrier
    {
    iter_vector = new std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    std::vector<int>::iterator b = iter_vector->begin();
    std::vector<int>::iterator e = iter_vector->end();
    swap(b, e);
    // manually copy the vector, so we can be sure to use i, b, e
    dsize = 0;
    for (std::vector<int>::iterator i = e; i != b; ++i) {
        data[dsize++] = *i;
    }
    CLEAR_VECTOR;
    }
    VERIFY("swap", 10, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10});

    //barrier
}

/**
 *  Transactional test of mutable reverse iterators
 */
void reverse_iterator_test_seq()
{
    int id = 0;
    // invariant: the vector starts off null
    assert(iter_vector == NULL);

    // helper message
    if (id == 0) {
        std::cout << std::endl
                  << "Testing reverse iterators" << std::endl;
    }

    // scratch space for holding data when computing whether the test
    // succeeded or failed.
    int data[32], dsize = 0;

    // test rbegin and rend
    //barrier
    int tmp = 0;
    {
    // make a vector
    iter_vector = new std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    // use rbegin and rend
    std::vector<int>::reverse_iterator test_b = iter_vector->rbegin();
    std::vector<int>::reverse_iterator test_e = iter_vector->rend();
    // a hack to prevent it being optimized out
    for (std::vector<int>::reverse_iterator i = test_b; i != test_e; ++i) // using untested ++
        tmp += *i;
    CLEAR_VECTOR;
    }
    if (tmp != 55)
        std::cout << "["<<id<<"] did not compute 55" << std::endl;
    else if (id == 0)
        std::cout << " [OK] rbegin() and rend()" << std::endl;

    // test ctors and dtors
    //barrier
    {
    iter_vector = new std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    // iterators should be default constructable (1)
    std::vector<int>::reverse_iterator i;
    // iterators should be copy constructable(2)
    std::vector<int>::reverse_iterator b(iter_vector->rbegin());
    // iterators should be copy assignable (3)
    std::vector<int>::reverse_iterator e = iter_vector->rend();
    std::vector<int>::reverse_iterator* pe = new std::vector<int>::reverse_iterator(e);
    // manually copy the vector, so we can be sure to use i, b, pe
    dsize = 0;
    for (i = b; i != *pe; ++i) {
        data[dsize++] = *i;
    }
    // iterators should be destructable (4)
    delete pe;
    CLEAR_VECTOR;
    }
    VERIFY("ctors and dtors", 10, {10, 9, 8, 7, 6, 5, 4, 3, 2, 1});

    // iterators should be comparable with == (5) and != (6)
    //barrier
    std::vector<int>::reverse_iterator e;
    if (id == 0) {
        iter_vector = new std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    }
    //barrier
    e = iter_vector->rend();
    bool eq, neq;
    {
    eq = (e == iter_vector->rend());
    neq = (e != iter_vector->rbegin());
    }
    if (!eq || !neq)
        std::cout << "["<<id<<"] did not observe correct equality results" << std::endl;
    else if (id == 0)
        std::cout << " [OK] != and ==" << std::endl;

    // test operator* and operator->
    //barrier
    if (id == 0) {
        foo.emplace(foo.begin());
        CLEAR_VECTOR;
    }
    //barrier
    int val1 = -9, val2 = -9, val3 = -9, val4 = -9;
    {
    iter_vector = new std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    std::vector<int>::reverse_iterator b = iter_vector->rbegin();
    // iterators can be dereferenced as an rvalue if in a dereferenceable
    // state (7)
    val1 = *b;
    // iterators can be dereferenced as an lvalue (8)
    *b = -6;
    val2 = *iter_vector->rbegin();
    // iterator access with ->
    std::vector<Q>::reverse_iterator x = foo.rbegin();
    val3 = x->a;
    val4 = x->b;
    CLEAR_VECTOR;
    }
    bool ok = (val1 == 10) && (val2 == -6) && (val3 == 1) && (val4 == 2);
    if (!ok)
        std::cout << "["<<id<<"] errors on * or ->" << std::endl;
    else if (id == 0)
        std::cout << " [OK] operators * and ->" << std::endl;
    //barrier
    if (id == 0) {
        foo.clear();
    }
    // test ++
    //barrier
    val1 = val2 = val3 = val4 = -8;
    int t1, t2, t3, t4;
    ok = true;
    {
    iter_vector = new std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    std::vector<int>::reverse_iterator i = iter_vector->rbegin();
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
    std::vector<int>::reverse_iterator a1(iter_vector->rbegin());
    std::vector<int>::reverse_iterator a2(iter_vector->rbegin());
    t1 = *a1;
    t2 = *a2;
    ok  &= (a1 == a2) && (t1 == 10) && (t2 == 10);
    a1++;
    a2++;
    t3 = *a1;
    t4 = *a2;
    ok &= (a1 == a2) && (t3 == 9) && (t4 == 9);
    CLEAR_VECTOR;
    }
    ok &= (val1 == 9) && (val2 == 8) && (val3 == 8) && (val4 == 7);
    if (!ok)
        std::cout << "["<<id<<"] errors on ++ tests" << std::endl;
    else if (id == 0)
        std::cout << " [OK] operator ++" << std::endl;

    // test operator --
    //barrier
    {
    iter_vector = new std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    std::vector<int>::reverse_iterator i = iter_vector->rbegin();
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
    CLEAR_VECTOR;
    }
    ok = (val1 == 8) && (val2 == 9) && (val3 == 9) && (val4 == 10);
    if (!ok)
        std::cout << "["<<id<<"] errors on -- tests" << std::endl;
    else if (id == 0)
        std::cout << " [OK] operator --" << std::endl;
    //barrier
    if (id == 0)
        delete iter_vector;

    // test swapping (16)
    //barrier
    {
    iter_vector = new std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    std::vector<int>::reverse_iterator b = iter_vector->rbegin();
    std::vector<int>::reverse_iterator e = iter_vector->rend();
    swap(b, e);
    // manually copy the vector, so we can be sure to use i, b, e
    dsize = 0;
    for (std::vector<int>::reverse_iterator i = e; i != b; ++i) {
        data[dsize++] = *i;
    }
    CLEAR_VECTOR;
    }
    VERIFY("swap", 10, {10, 9, 8, 7, 6, 5, 4, 3, 2, 1});

    //barrier
}

/**
 * Transactional test of legacy const forward iterators
 */
void legacy_const_iterator_test_seq()
{
    int id = 0;
    // invariant: the vector starts off null
    assert(iter_vector == NULL);

    // helper message
    if (id == 0) {
        std::cout << std::endl
                  << "Testing legacy const iterators" << std::endl;
    }

    // scratch space for holding data when computing whether the test
    // succeeded or failed.
    int data[32], dsize = 0;

    // test begin and end
    //barrier
    int tmp = 0;
    {
    // make a vector
    iter_vector = new std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    // use begin and end
    const std::vector<int>* l2 = iter_vector;
    std::vector<int>::const_iterator test_b = l2->begin();
    std::vector<int>::const_iterator test_e = l2->end();
    // a hack to prevent it being optimized out
    for (std::vector<int>::const_iterator i = test_b; i != test_e; ++i) // using untested ++
        tmp += *i;
    CLEAR_VECTOR;
    }
    if (tmp != 55)
        std::cout << "["<<id<<"] did not compute 55" << std::endl;
    else if (id == 0)
        std::cout << " [OK] begin() and end()" << std::endl;

    // test ctors and dtors
    //barrier
    {
    iter_vector = new std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    // iterators should be default constructable (1)
    std::vector<int>::const_iterator i;
    // iterators should be copy constructable(2)
    std::vector<int>::const_iterator b(iter_vector->begin());
    // iterators should be copy assignable (3)
    std::vector<int>::const_iterator e = iter_vector->end();
    std::vector<int>::const_iterator* pe = new std::vector<int>::const_iterator(e);
    // manually copy the vector, so we can be sure to use i, b, pe
    dsize = 0;
    for (i = b; i != *pe; ++i) {
        data[dsize++] = *i;
    }
    // iterators should be destructable (4)
    delete pe;
    CLEAR_VECTOR;
    }
    VERIFY("ctors and dtors", 10, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10});

    // iterators should be comparable with == (5) and != (6)
    //barrier
    std::vector<int>::const_iterator e;
    if (id == 0) {
        iter_vector = new std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    }
    //barrier
    e = iter_vector->end();
    bool eq, neq;
    {
    eq = (e == iter_vector->end());
    neq = (e != iter_vector->begin());
    }
    if (!eq || !neq)
        std::cout << "["<<id<<"] did not observe correct equality results" << std::endl;
    else if (id == 0)
        std::cout << " [OK] != and ==" << std::endl;

    // test operator* and operator->
    //barrier
    if (id == 0) {
        foo.emplace(foo.begin());
        CLEAR_VECTOR;
    }
    //barrier
    int val1, val2, val3, val4;
    {
    iter_vector = new std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    std::vector<int>::const_iterator b = iter_vector->begin();
    // iterators can be dereferenced as an rvalue if in a dereferenceable
    // state (7)
    val1 = *b;
    // iterator access with ->
    std::vector<Q>::const_iterator x = foo.begin();
    val3 = x->a;
    val4 = x->b;
    CLEAR_VECTOR;
    }
    bool ok = (val1 == 1) && (val3 == 1) && (val4 == 2);
    if (!ok)
        std::cout << "["<<id<<"] errors on * or ->" << std::endl;
    else if (id == 0)
        std::cout << " [OK] operators * and ->" << std::endl;
    //barrier
    if (id == 0) {
        foo.clear();
    }

    // test ++
    //barrier
    val1 = val2 = val3 = val4 = -8;
    int t1, t2, t3, t4;
    ok = true;
    {
    iter_vector = new std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    std::vector<int>::const_iterator i = iter_vector->begin();
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
    std::vector<int>::const_iterator a1(iter_vector->begin());
    std::vector<int>::const_iterator a2(iter_vector->begin());
    t1 = *a1;
    t2 = *a2;
    ok  &= (a1 == a2) && (t1 == 1) && (t2 == 1);
    a1++;
    a2++;
    t3 = *a1;
    t4 = *a2;
    ok &= (a1 == a2) && (t3 == 2) && (t4 == 2);
    CLEAR_VECTOR;
    }
    ok &= (val1 == 2) && (val2 == 3) && (val3 == 3) && (val4 == 4);
    if (!ok)
        std::cout << "["<<id<<"] errors on ++ tests" << std::endl;
    else if (id == 0)
        std::cout << " [OK] operator ++" << std::endl;

    // test operator --
    //barrier
    {
    iter_vector = new std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    std::vector<int>::const_iterator i = iter_vector->end();
    // can be decremented (13)
    --i;
    val1 = *i;
    // can be decremented (14)
    i--;
    val2 = *i;
    // can be decremented (15)
    val3 = *i--;
    val4 = *i;
    CLEAR_VECTOR;
    }
    ok = (val1 == 10) && (val2 == 9) && (val3 == 9) && (val4 == 8);
    if (!ok)
        std::cout << "["<<id<<"] errors on -- tests" << std::endl;
    else if (id == 0)
        std::cout << " [OK] operator --" << std::endl;

    // test swapping (16)
    //barrier
    {
    iter_vector = new std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    std::vector<int>::const_iterator b = iter_vector->begin();
    std::vector<int>::const_iterator e = iter_vector->end();
    swap(b, e);
    // manually copy the vector, so we can be sure to use i, b, e
    dsize = 0;
    for (std::vector<int>::const_iterator i = e; i != b; ++i) {
        data[dsize++] = *i;
    }
    CLEAR_VECTOR;
    }
    VERIFY("swap", 10, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10});

    //barrier
}

/**
 * Transactional test of legacy const reverse iterators
 */
void legacy_const_reverse_iterator_test_seq()
{
    int id = 0;
    // invariant: the vector starts off null
    assert(iter_vector == NULL);

    // helper message
    if (id == 0) {
        std::cout << std::endl
                  << "Testing legacy const reverse iterators" << std::endl;
    }

    // scratch space for holding data when computing whether the test
    // succeeded or failed.
    int data[32], dsize = 0;

    // test begin and end
    //barrier
    int tmp = 0;
    {
    // make a vector
    iter_vector = new std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    // use begin and end
    const std::vector<int>* l2 = iter_vector;
    std::vector<int>::const_reverse_iterator test_b = l2->rbegin();
    std::vector<int>::const_reverse_iterator test_e = l2->rend();
    // a hack to prevent it being optimized out
    for (std::vector<int>::const_reverse_iterator i = test_b; i != test_e; ++i) // using untested ++
        tmp += *i;
    CLEAR_VECTOR;
    }
    if (tmp != 55)
        std::cout << "["<<id<<"] did not compute 55" << std::endl;
    else if (id == 0)
        std::cout << " [OK] begin() and end()" << std::endl;

    // test ctors and dtors
    //barrier
    {
    iter_vector = new std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    // iterators should be default constructable (1)
    std::vector<int>::const_reverse_iterator i;
    // iterators should be copy constructable(2)
    std::vector<int>::const_reverse_iterator b(iter_vector->rbegin());
    // iterators should be copy assignable (3)
    std::vector<int>::const_reverse_iterator e = iter_vector->rend();
    std::vector<int>::const_reverse_iterator* pe = new std::vector<int>::const_reverse_iterator(e);
    // manually copy the vector, so we can be sure to use i, b, pe
    dsize = 0;
    for (i = b; i != *pe; ++i) {
        data[dsize++] = *i;
    }
    // iterators should be destructable (4)
    delete pe;
    CLEAR_VECTOR;
    }
    VERIFY("ctors and dtors", 10, {10, 9, 8, 7, 6, 5, 4, 3, 2, 1});

    // iterators should be comparable with == (5) and != (6)
    //barrier
    std::vector<int>::const_reverse_iterator e;
    if (id == 0) {
        iter_vector = new std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    }
    //barrier
    e = iter_vector->rend();
    bool eq, neq;
    {
    eq = (e == iter_vector->rend());
    neq = (e != iter_vector->rbegin());
    }
    if (!eq || !neq)
        std::cout << "["<<id<<"] did not observe correct equality results" << std::endl;
    else if (id == 0)
        std::cout << " [OK] != and ==" << std::endl;

    // test operator* and operator->
    //barrier
    if (id == 0) {
        foo.emplace(foo.begin());
        CLEAR_VECTOR;
    }
    //barrier
    int val1, val2, val3, val4;
    {
    iter_vector = new std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    std::vector<int>::const_reverse_iterator b = iter_vector->rbegin();
    // iterators can be dereferenced as an rvalue if in a dereferenceable
    // state (7)
    val1 = *b;
    // iterator access with ->
    std::vector<Q>::const_reverse_iterator x = foo.rbegin();
    val3 = x->a;
    val4 = x->b;
    CLEAR_VECTOR;
    }
    bool ok = (val1 == 10) && (val3 == 1) && (val4 == 2);
    if (!ok)
        std::cout << "["<<id<<"] errors on * or ->" << std::endl;
    else if (id == 0)
        std::cout << " [OK] operators * and ->" << std::endl;
    //barrier
    if (id == 0) {
        foo.clear();
    }

    // test ++
    //barrier
    val1 = val2 = val3 = val4 = -8;
    int t1, t2, t3, t4;
    ok = true;
    {
    iter_vector = new std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    std::vector<int>::const_reverse_iterator i = iter_vector->rbegin();
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
    std::vector<int>::const_reverse_iterator a1(iter_vector->rbegin());
    std::vector<int>::const_reverse_iterator a2(iter_vector->rbegin());
    t1 = *a1;
    t2 = *a2;
    ok  &= (a1 == a2) && (t1 == 10) && (t2 == 10);
    a1++;
    a2++;
    t3 = *a1;
    t4 = *a2;
    ok &= (a1 == a2) && (t3 == 9) && (t4 == 9);
    CLEAR_VECTOR;
    }
    ok &= (val1 == 9) && (val2 == 8) && (val3 == 8) && (val4 == 7);
    if (!ok)
        std::cout << "["<<id<<"] errors on ++ tests" << std::endl;
    else if (id == 0)
        std::cout << " [OK] operator ++" << std::endl;

    // test operator --
    //barrier
    {
    iter_vector = new std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    std::vector<int>::const_reverse_iterator i = iter_vector->rbegin();
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
    CLEAR_VECTOR;
    }
    ok = (val1 == 8) && (val2 == 9) && (val3 == 9) && (val4 == 10);
    if (!ok)
        std::cout << "["<<id<<"] errors on -- tests" << std::endl;
    else if (id == 0)
        std::cout << " [OK] operator --" << std::endl;

    // test swapping (16)
    //barrier
    {
    iter_vector = new std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    std::vector<int>::const_reverse_iterator b = iter_vector->rbegin();
    std::vector<int>::const_reverse_iterator e = iter_vector->rend();
    swap(b, e);
    // manually copy the vector, so we can be sure to use i, b, e
    dsize = 0;
    for (std::vector<int>::const_reverse_iterator i = e; i != b; ++i) {
        data[dsize++] = *i;
    }
    CLEAR_VECTOR;
    }
    VERIFY("swap", 10, {10, 9, 8, 7, 6, 5, 4, 3, 2, 1});

    //barrier
}

/**
 * Transactional test of C++ const forward iterators with cbegin
 */
void const_iterator_test_seq()
{
    int id = 0;
    // invariant: the vector starts off null
    assert(iter_vector == NULL);

    // helper message
    if (id == 0) {
        std::cout << std::endl
                  << "Testing C++11 const iterators" << std::endl;
    }

    // scratch space for holding data when computing whether the test
    // succeeded or failed.
    int data[32], dsize = 0;

    // test begin and end
    //barrier
    int tmp = 0;
    {
    // make a vector
    iter_vector = new std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    // use begin and end
    const std::vector<int>* l2 = iter_vector;
    std::vector<int>::const_iterator test_b = l2->cbegin();
    std::vector<int>::const_iterator test_e = l2->cend();
    // a hack to prevent it being optimized out
    for (std::vector<int>::const_iterator i = test_b; i != test_e; ++i) // using untested ++
        tmp += *i;
    CLEAR_VECTOR;
    }
    if (tmp != 55)
        std::cout << "["<<id<<"] did not compute 55" << std::endl;
    else if (id == 0)
        std::cout << " [OK] cbegin() and cend()" << std::endl;

    // test that const and non-const iterators can be compared
    //barrier
    bool ok = true;
    {
    // make a vector
    iter_vector = new std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    // get const iterators
    std::vector<int>::const_iterator test_b = iter_vector->cbegin();
    std::vector<int>::const_iterator test_e = iter_vector->cend();
    std::vector<int>::iterator other_b = iter_vector->begin();
    std::vector<int>::iterator other_e = iter_vector->end();
    ok = ok & (test_b == other_b) && (other_e == test_e) && (test_b != other_e) && (other_b != test_e);
    }
    if (!ok)
        std::cout << "["<<id<<"] failed const vs. non-const equality tests" << std::endl;
    else if (id == 0)
        std::cout << " [OK] const vs. non-const equality tests" << std::endl;

    // test ctors and dtors
    //barrier
    {
    iter_vector = new std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    // iterators should be default constructable (1)
    std::vector<int>::const_iterator i;
    // iterators should be copy constructable(2)
    std::vector<int>::const_iterator b(iter_vector->cbegin());
    // iterators should be copy assignable (3)
    std::vector<int>::const_iterator e = iter_vector->cend();
    std::vector<int>::const_iterator* pe = new std::vector<int>::const_iterator(e);
    // manually copy the vector, so we can be sure to use i, b, pe
    dsize = 0;
    for (i = b; i != *pe; ++i) {
        data[dsize++] = *i;
    }
    // iterators should be destructable (4)
    delete pe;
    CLEAR_VECTOR;
    }
    VERIFY("ctors and dtors", 10, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10});

    // iterators should be comparable with == (5) and != (6)
    //barrier
    std::vector<int>::const_iterator e;
    if (id == 0) {
        iter_vector = new std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    }
    //barrier
    e = iter_vector->cend();
    bool eq, neq;
    {
    eq = (e == iter_vector->cend());
    neq = (e != iter_vector->cbegin());
    }
    if (!eq || !neq)
        std::cout << "["<<id<<"] did not observe correct equality results" << std::endl;
    else if (id == 0)
        std::cout << " [OK] != and ==" << std::endl;

    // test operator* and operator->
    //barrier
    if (id == 0) {
        foo.emplace(foo.cbegin());
        CLEAR_VECTOR;
    }
    //barrier
    int val1, val2, val3, val4;
    {
    iter_vector = new std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    std::vector<int>::const_iterator b = iter_vector->cbegin();
    // iterators can be dereferenced as an rvalue if in a dereferenceable
    // state (7)
    val1 = *b;
    // iterator access with ->
    std::vector<Q>::const_iterator x = foo.cbegin();
    val3 = x->a;
    val4 = x->b;
    CLEAR_VECTOR;
    }
    ok = (val1 == 1) && (val3 == 1) && (val4 == 2);
    if (!ok)
        std::cout << "["<<id<<"] errors on * or ->" << std::endl;
    else if (id == 0)
        std::cout << " [OK] operators * and ->" << std::endl;
    //barrier
    if (id == 0) {
        foo.clear();
    }

    // test ++
    //barrier
    val1 = val2 = val3 = val4 = -8;
    int t1, t2, t3, t4;
    ok = true;
    {
    iter_vector = new std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    std::vector<int>::const_iterator i = iter_vector->cbegin();
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
    std::vector<int>::const_iterator a1(iter_vector->cbegin());
    std::vector<int>::const_iterator a2(iter_vector->cbegin());
    t1 = *a1;
    t2 = *a2;
    ok  &= (a1 == a2) && (t1 == 1) && (t2 == 1);
    a1++;
    a2++;
    t3 = *a1;
    t4 = *a2;
    ok &= (a1 == a2) && (t3 == 2) && (t4 == 2);
    CLEAR_VECTOR;
    }
    ok &= (val1 == 2) && (val2 == 3) && (val3 == 3) && (val4 == 4);
    if (!ok)
        std::cout << "["<<id<<"] errors on ++ tests" << std::endl;
    else if (id == 0)
        std::cout << " [OK] operator ++" << std::endl;

    // test operator --
    //barrier
    {
    iter_vector = new std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    std::vector<int>::const_iterator i = iter_vector->cend();
    // can be decremented (13)
    --i;
    val1 = *i;
    // can be decremented (14)
    i--;
    val2 = *i;
    // can be decremented (15)
    val3 = *i--;
    val4 = *i;
    CLEAR_VECTOR;
    }
    ok = (val1 == 10) && (val2 == 9) && (val3 == 9) && (val4 == 8);
    if (!ok)
        std::cout << "["<<id<<"] errors on -- tests" << std::endl;
    else if (id == 0)
        std::cout << " [OK] operator --" << std::endl;

    // test swapping (16)
    //barrier
    {
    iter_vector = new std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    std::vector<int>::const_iterator b = iter_vector->cbegin();
    std::vector<int>::const_iterator e = iter_vector->cend();
    swap(b, e);
    // manually copy the vector, so we can be sure to use i, b, e
    dsize = 0;
    for (std::vector<int>::const_iterator i = e; i != b; ++i) {
        data[dsize++] = *i;
    }
    CLEAR_VECTOR;
    }
    VERIFY("swap", 10, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10});

    //barrier
}

/**
 * Transactional test of C++11 const reverse iterators with crbegin
 */
void const_reverse_iterator_test_seq()
{
    int id = 0;
    // invariant: the vector starts off null
    assert(iter_vector == NULL);

    // helper message
    if (id == 0) {
        std::cout << std::endl
                  << "Testing legacy const reverse iterators" << std::endl;
    }

    // scratch space for holding data when computing whether the test
    // succeeded or failed.
    int data[32], dsize = 0;

    // test begin and end
    //barrier
    int tmp = 0;
    {
    // make a vector
    iter_vector = new std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    // use begin and end
    const std::vector<int>* l2 = iter_vector;
    std::vector<int>::const_reverse_iterator test_b = l2->crbegin();
    std::vector<int>::const_reverse_iterator test_e = l2->crend();
    // a hack to prevent it being optimized out
    for (std::vector<int>::const_reverse_iterator i = test_b; i != test_e; ++i) // using untested ++
        tmp += *i;
    CLEAR_VECTOR;
    }
    if (tmp != 55)
        std::cout << "["<<id<<"] did not compute 55" << std::endl;
    else if (id == 0)
        std::cout << " [OK] crbegin() and crend()" << std::endl;

    // test ctors and dtors
    //barrier
    {
    iter_vector = new std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    // iterators should be default constructable (1)
    std::vector<int>::const_reverse_iterator i;
    // iterators should be copy constructable(2)
    std::vector<int>::const_reverse_iterator b(iter_vector->crbegin());
    // iterators should be copy assignable (3)
    std::vector<int>::const_reverse_iterator e = iter_vector->crend();
    std::vector<int>::const_reverse_iterator* pe = new std::vector<int>::const_reverse_iterator(e);
    // manually copy the vector, so we can be sure to use i, b, pe
    dsize = 0;
    for (i = b; i != *pe; ++i) {
        data[dsize++] = *i;
    }
    // iterators should be destructable (4)
    delete pe;
    CLEAR_VECTOR;
    }
    VERIFY("ctors and dtors", 10, {10, 9, 8, 7, 6, 5, 4, 3, 2, 1});

    // iterators should be comparable with == (5) and != (6)
    //barrier
    std::vector<int>::const_reverse_iterator e;
    if (id == 0) {
        iter_vector = new std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    }
    //barrier
    e = iter_vector->crend();
    bool eq, neq;
    {
    eq = (e == iter_vector->crend());
    neq = (e != iter_vector->crbegin());
    }
    if (!eq || !neq)
        std::cout << "["<<id<<"] did not observe correct equality results" << std::endl;
    else if (id == 0)
        std::cout << " [OK] != and ==" << std::endl;

    // test operator* and operator->
    //barrier
    if (id == 0) {
        foo.emplace(foo.begin());
        CLEAR_VECTOR;
    }
    //barrier
    int val1, val2, val3, val4;
    {
    iter_vector = new std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    std::vector<int>::const_reverse_iterator b = iter_vector->crbegin();
    // iterators can be dereferenced as an rvalue if in a dereferenceable
    // state (7)
    val1 = *b;
    // iterator access with ->
    std::vector<Q>::const_reverse_iterator x = foo.crbegin();
    val3 = x->a;
    val4 = x->b;
    CLEAR_VECTOR;
    }
    bool ok = (val1 == 10) && (val3 == 1) && (val4 == 2);
    if (!ok)
        std::cout << "["<<id<<"] errors on * or ->" << std::endl;
    else if (id == 0)
        std::cout << " [OK] operators * and ->" << std::endl;
    //barrier
    if (id == 0) {
        foo.clear();
    }

    // test ++
    //barrier
    val1 = val2 = val3 = val4 = -8;
    int t1, t2, t3, t4;
    ok = true;
    {
    iter_vector = new std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    std::vector<int>::const_reverse_iterator i = iter_vector->crbegin();
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
    std::vector<int>::const_reverse_iterator a1(iter_vector->crbegin());
    std::vector<int>::const_reverse_iterator a2(iter_vector->crbegin());
    t1 = *a1;
    t2 = *a2;
    ok  &= (a1 == a2) && (t1 == 10) && (t2 == 10);
    a1++;
    a2++;
    t3 = *a1;
    t4 = *a2;
    ok &= (a1 == a2) && (t3 == 9) && (t4 == 9);
    CLEAR_VECTOR;
    }
    ok &= (val1 == 9) && (val2 == 8) && (val3 == 8) && (val4 == 7);
    if (!ok)
        std::cout << "["<<id<<"] errors on ++ tests" << std::endl;
    else if (id == 0)
        std::cout << " [OK] operator ++" << std::endl;

    // test operator --
    //barrier
    {
    iter_vector = new std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    std::vector<int>::const_reverse_iterator i = iter_vector->crbegin();
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
    CLEAR_VECTOR;
    }
    ok = (val1 == 8) && (val2 == 9) && (val3 == 9) && (val4 == 10);
    if (!ok)
        std::cout << "["<<id<<"] errors on -- tests" << std::endl;
    else if (id == 0)
        std::cout << " [OK] operator --" << std::endl;

    // test swapping (16)
    //barrier
    {
    iter_vector = new std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    std::vector<int>::const_reverse_iterator b = iter_vector->crbegin();
    std::vector<int>::const_reverse_iterator e = iter_vector->crend();
    swap(b, e);
    // manually copy the vector, so we can be sure to use i, b, e
    dsize = 0;
    for (std::vector<int>::const_reverse_iterator i = e; i != b; ++i) {
        data[dsize++] = *i;
    }
    CLEAR_VECTOR;
    }
    VERIFY("swap", 10, {10, 9, 8, 7, 6, 5, 4, 3, 2, 1});

    //barrier
}




///~~~~~~~~~~~~~~~~





/**
 * Transactional test of basic (forward, mutable) iterators
 */
void iterator_test_concurrent(int id)
{
    // invariant: the vector starts off null
    assert(iter_vector == NULL);

    // helper message
    if (id == 0) {
        // note that we cannot test copy_assignable, because
        // copy_assignable in the gcc vector is achieved via a default
        // method that is automatically generated by the compiler.
        //
        // std::swap is a TODO
        std::cout << std::endl
                  << "Testing iterators (note: no verification for copy "
                  << "assignable, dtor, lvalue_dereference, or swap)"
                  << std::endl;
    }

    // TODO: we should compare const_iterator and iterator using ==
    // and != operators... trace already in place in stl_vector.h

    // scratch space for holding data when computing whether the test
    // succeeded or failed.
    int data[32], dsize = 0;

    // test begin and end
    global_barrier->arrive(id);
    int tmp = 0;
    BEGIN_TX;
    // make a vector
    iter_vector = new std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    // use begin and end
    std::vector<int>::iterator test_b = iter_vector->begin();
    std::vector<int>::iterator test_e = iter_vector->end();
    // a hack to prevent it being optimized out
    for (std::vector<int>::iterator i = test_b; i != test_e; ++i) // using untested ++
        tmp += *i;
    CLEAR_VECTOR;
    // NB: implicit call to the implicit iterator dtor method
    END_TX;
    if (tmp != 55)
        std::cout << "["<<id<<"] did not compute 55" << std::endl;
    else if (id == 0)
        std::cout << " [OK] begin() and end()" << std::endl;

    // test ctors and dtors
    global_barrier->arrive(id);
    BEGIN_TX;
    iter_vector = new std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    // iterators should be default constructable (1)
    std::vector<int>::iterator i;
    // iterators should be copy constructable(2)
    std::vector<int>::iterator b(iter_vector->begin());
    // iterators should be copy assignable (3)
    std::vector<int>::iterator e = iter_vector->end();
    std::vector<int>::iterator* pe = new std::vector<int>::iterator(e);
    // manually copy the vector, so we can be sure to use i, b, pe
    dsize = 0;
    for (i = b; i != *pe; ++i) {
        data[dsize++] = *i;
    }
    // iterators should be destructable (4)
    delete pe;
    CLEAR_VECTOR;
    END_TX;
    VERIFY("ctors and dtors", 10, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10});

    // iterators should be comparable with == (5) and != (6)
    global_barrier->arrive(id);
    std::vector<int>::iterator e;
    if (id == 0) {
        iter_vector = new std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    }
    global_barrier->arrive(id);
    e = iter_vector->end();
    bool eq, neq;
    BEGIN_TX;
    eq = (e == iter_vector->end());
    neq = (e != iter_vector->begin());
    END_TX;
    if (!eq || !neq)
        std::cout << "["<<id<<"] did not observe correct equality results" << std::endl;
    else if (id == 0)
        std::cout << " [OK] != and ==" << std::endl;

    // test operator* and operator->
    global_barrier->arrive(id);
    if (id == 0) {
        foo.emplace(foo.begin());
        CLEAR_VECTOR;
    }
    global_barrier->arrive(id);
    int val1, val2, val3, val4;
    BEGIN_TX;
    iter_vector = new std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    std::vector<int>::iterator b = iter_vector->begin();
    // iterators can be dereferenced as an rvalue if in a dereferenceable
    // state (7)
    val1 = *b;
    // iterators can be dereferenced as an lvalue (8)
    *b = -6;
    val2 = *iter_vector->begin();
    // iterator access with ->
    std::vector<Q>::iterator x = foo.begin();
    val3 = x->a;
    val4 = x->b;
    CLEAR_VECTOR;
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
    iter_vector = new std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    std::vector<int>::iterator i = iter_vector->begin();
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
    std::vector<int>::iterator a1(iter_vector->begin());
    std::vector<int>::iterator a2(iter_vector->begin());
    t1 = *a1;
    t2 = *a2;
    ok  &= (a1 == a2) && (t1 == 1) && (t2 == 1);
    a1++;
    a2++;
    t3 = *a1;
    t4 = *a2;
    ok &= (a1 == a2) && (t3 == 2) && (t4 == 2);
    CLEAR_VECTOR;
    END_TX;
    ok &= (val1 == 2) && (val2 == 3) && (val3 == 3) && (val4 == 4);
    if (!ok)
        std::cout << "["<<id<<"] errors on ++ tests" << std::endl;
    else if (id == 0)
        std::cout << " [OK] operator ++" << std::endl;

    // test operator --
    global_barrier->arrive(id);
    BEGIN_TX;
    iter_vector = new std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    std::vector<int>::iterator i = iter_vector->end();
    // can be decremented (13)
    --i;
    val1 = *i;
    // can be decremented (14)
    i--;
    val2 = *i;
    // can be decremented (15)
    val3 = *i--;
    val4 = *i;
    CLEAR_VECTOR;
    END_TX;
    ok = (val1 == 10) && (val2 == 9) && (val3 == 9) && (val4 == 8);
    if (!ok)
        std::cout << "["<<id<<"] errors on -- tests" << std::endl;
    else if (id == 0)
        std::cout << " [OK] operator --" << std::endl;

    // test swapping (16)
    global_barrier->arrive(id);
    BEGIN_TX;
    iter_vector = new std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    std::vector<int>::iterator b = iter_vector->begin();
    std::vector<int>::iterator e = iter_vector->end();
    swap(b, e);
    // manually copy the vector, so we can be sure to use i, b, e
    dsize = 0;
    for (std::vector<int>::iterator i = e; i != b; ++i) {
        data[dsize++] = *i;
    }
    CLEAR_VECTOR;
    END_TX;
    VERIFY("swap", 10, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10});

    global_barrier->arrive(id);
}

/**
 *  Transactional test of mutable reverse iterators
 */
void reverse_iterator_test_concurrent(int id)
{
    // invariant: the vector starts off null
    assert(iter_vector == NULL);

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
    // make a vector
    iter_vector = new std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    // use rbegin and rend
    std::vector<int>::reverse_iterator test_b = iter_vector->rbegin();
    std::vector<int>::reverse_iterator test_e = iter_vector->rend();
    // a hack to prevent it being optimized out
    for (std::vector<int>::reverse_iterator i = test_b; i != test_e; ++i) // using untested ++
        tmp += *i;
    CLEAR_VECTOR;
    END_TX;
    if (tmp != 55)
        std::cout << "["<<id<<"] did not compute 55" << std::endl;
    else if (id == 0)
        std::cout << " [OK] rbegin() and rend()" << std::endl;

    // test ctors and dtors
    global_barrier->arrive(id);
    BEGIN_TX;
    iter_vector = new std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    // iterators should be default constructable (1)
    std::vector<int>::reverse_iterator i;
    // iterators should be copy constructable(2)
    std::vector<int>::reverse_iterator b(iter_vector->rbegin());
    // iterators should be copy assignable (3)
    std::vector<int>::reverse_iterator e = iter_vector->rend();
    std::vector<int>::reverse_iterator* pe = new std::vector<int>::reverse_iterator(e);
    // manually copy the vector, so we can be sure to use i, b, pe
    dsize = 0;
    for (i = b; i != *pe; ++i) {
        data[dsize++] = *i;
    }
    // iterators should be destructable (4)
    delete pe;
    CLEAR_VECTOR;
    END_TX;
    VERIFY("ctors and dtors", 10, {10, 9, 8, 7, 6, 5, 4, 3, 2, 1});

    // iterators should be comparable with == (5) and != (6)
    global_barrier->arrive(id);
    std::vector<int>::reverse_iterator e;
    if (id == 0) {
        iter_vector = new std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    }
    global_barrier->arrive(id);
    e = iter_vector->rend();
    bool eq, neq;
    BEGIN_TX;
    eq = (e == iter_vector->rend());
    neq = (e != iter_vector->rbegin());
    END_TX;
    if (!eq || !neq)
        std::cout << "["<<id<<"] did not observe correct equality results" << std::endl;
    else if (id == 0)
        std::cout << " [OK] != and ==" << std::endl;

    // test operator* and operator->
    global_barrier->arrive(id);
    if (id == 0) {
        foo.emplace(foo.begin());
        CLEAR_VECTOR;
    }
    global_barrier->arrive(id);
    int val1 = -9, val2 = -9, val3 = -9, val4 = -9;
    BEGIN_TX;
    iter_vector = new std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    std::vector<int>::reverse_iterator b = iter_vector->rbegin();
    // iterators can be dereferenced as an rvalue if in a dereferenceable
    // state (7)
    val1 = *b;
    // iterators can be dereferenced as an lvalue (8)
    *b = -6;
    val2 = *iter_vector->rbegin();
    // iterator access with ->
    std::vector<Q>::reverse_iterator x = foo.rbegin();
    val3 = x->a;
    val4 = x->b;
    CLEAR_VECTOR;
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
    iter_vector = new std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    std::vector<int>::reverse_iterator i = iter_vector->rbegin();
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
    std::vector<int>::reverse_iterator a1(iter_vector->rbegin());
    std::vector<int>::reverse_iterator a2(iter_vector->rbegin());
    t1 = *a1;
    t2 = *a2;
    ok  &= (a1 == a2) && (t1 == 10) && (t2 == 10);
    a1++;
    a2++;
    t3 = *a1;
    t4 = *a2;
    ok &= (a1 == a2) && (t3 == 9) && (t4 == 9);
    CLEAR_VECTOR;
    END_TX;
    ok &= (val1 == 9) && (val2 == 8) && (val3 == 8) && (val4 == 7);
    if (!ok)
        std::cout << "["<<id<<"] errors on ++ tests" << std::endl;
    else if (id == 0)
        std::cout << " [OK] operator ++" << std::endl;

    // test operator --
    global_barrier->arrive(id);
    BEGIN_TX;
    iter_vector = new std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    std::vector<int>::reverse_iterator i = iter_vector->rbegin();
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
    CLEAR_VECTOR;
    END_TX;
    ok = (val1 == 8) && (val2 == 9) && (val3 == 9) && (val4 == 10);
    if (!ok)
        std::cout << "["<<id<<"] errors on -- tests" << std::endl;
    else if (id == 0)
        std::cout << " [OK] operator --" << std::endl;
    global_barrier->arrive(id);
    if (id == 0)
        delete iter_vector;

    // test swapping (16)
    global_barrier->arrive(id);
    BEGIN_TX;
    iter_vector = new std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    std::vector<int>::reverse_iterator b = iter_vector->rbegin();
    std::vector<int>::reverse_iterator e = iter_vector->rend();
    swap(b, e);
    // manually copy the vector, so we can be sure to use i, b, e
    dsize = 0;
    for (std::vector<int>::reverse_iterator i = e; i != b; ++i) {
        data[dsize++] = *i;
    }
    CLEAR_VECTOR;
    END_TX;
    VERIFY("swap", 10, {10, 9, 8, 7, 6, 5, 4, 3, 2, 1});

    global_barrier->arrive(id);
}

/**
 * Transactional test of legacy const forward iterators
 */
void legacy_const_iterator_test_concurrent(int id)
{
    // invariant: the vector starts off null
    assert(iter_vector == NULL);

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
    // make a vector
    iter_vector = new std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    // use begin and end
    const std::vector<int>* l2 = iter_vector;
    std::vector<int>::const_iterator test_b = l2->begin();
    std::vector<int>::const_iterator test_e = l2->end();
    // a hack to prevent it being optimized out
    for (std::vector<int>::const_iterator i = test_b; i != test_e; ++i) // using untested ++
        tmp += *i;
    CLEAR_VECTOR;
    END_TX;
    if (tmp != 55)
        std::cout << "["<<id<<"] did not compute 55" << std::endl;
    else if (id == 0)
        std::cout << " [OK] begin() and end()" << std::endl;

    // test ctors and dtors
    global_barrier->arrive(id);
    BEGIN_TX;
    iter_vector = new std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    // iterators should be default constructable (1)
    std::vector<int>::const_iterator i;
    // iterators should be copy constructable(2)
    std::vector<int>::const_iterator b(iter_vector->begin());
    // iterators should be copy assignable (3)
    std::vector<int>::const_iterator e = iter_vector->end();
    std::vector<int>::const_iterator* pe = new std::vector<int>::const_iterator(e);
    // manually copy the vector, so we can be sure to use i, b, pe
    dsize = 0;
    for (i = b; i != *pe; ++i) {
        data[dsize++] = *i;
    }
    // iterators should be destructable (4)
    delete pe;
    CLEAR_VECTOR;
    END_TX;
    VERIFY("ctors and dtors", 10, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10});

    // iterators should be comparable with == (5) and != (6)
    global_barrier->arrive(id);
    std::vector<int>::const_iterator e;
    if (id == 0) {
        iter_vector = new std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    }
    global_barrier->arrive(id);
    e = iter_vector->end();
    bool eq, neq;
    BEGIN_TX;
    eq = (e == iter_vector->end());
    neq = (e != iter_vector->begin());
    END_TX;
    if (!eq || !neq)
        std::cout << "["<<id<<"] did not observe correct equality results" << std::endl;
    else if (id == 0)
        std::cout << " [OK] != and ==" << std::endl;

    // test operator* and operator->
    global_barrier->arrive(id);
    if (id == 0) {
        foo.emplace(foo.begin());
        CLEAR_VECTOR;
    }
    global_barrier->arrive(id);
    int val1, val2, val3, val4;
    BEGIN_TX;
    iter_vector = new std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    std::vector<int>::const_iterator b = iter_vector->begin();
    // iterators can be dereferenced as an rvalue if in a dereferenceable
    // state (7)
    val1 = *b;
    // iterator access with ->
    std::vector<Q>::const_iterator x = foo.begin();
    val3 = x->a;
    val4 = x->b;
    CLEAR_VECTOR;
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
    iter_vector = new std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    std::vector<int>::const_iterator i = iter_vector->begin();
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
    std::vector<int>::const_iterator a1(iter_vector->begin());
    std::vector<int>::const_iterator a2(iter_vector->begin());
    t1 = *a1;
    t2 = *a2;
    ok  &= (a1 == a2) && (t1 == 1) && (t2 == 1);
    a1++;
    a2++;
    t3 = *a1;
    t4 = *a2;
    ok &= (a1 == a2) && (t3 == 2) && (t4 == 2);
    CLEAR_VECTOR;
    END_TX;
    ok &= (val1 == 2) && (val2 == 3) && (val3 == 3) && (val4 == 4);
    if (!ok)
        std::cout << "["<<id<<"] errors on ++ tests" << std::endl;
    else if (id == 0)
        std::cout << " [OK] operator ++" << std::endl;

    // test operator --
    global_barrier->arrive(id);
    BEGIN_TX;
    iter_vector = new std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    std::vector<int>::const_iterator i = iter_vector->end();
    // can be decremented (13)
    --i;
    val1 = *i;
    // can be decremented (14)
    i--;
    val2 = *i;
    // can be decremented (15)
    val3 = *i--;
    val4 = *i;
    CLEAR_VECTOR;
    END_TX;
    ok = (val1 == 10) && (val2 == 9) && (val3 == 9) && (val4 == 8);
    if (!ok)
        std::cout << "["<<id<<"] errors on -- tests" << std::endl;
    else if (id == 0)
        std::cout << " [OK] operator --" << std::endl;

    // test swapping (16)
    global_barrier->arrive(id);
    BEGIN_TX;
    iter_vector = new std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    std::vector<int>::const_iterator b = iter_vector->begin();
    std::vector<int>::const_iterator e = iter_vector->end();
    swap(b, e);
    // manually copy the vector, so we can be sure to use i, b, e
    dsize = 0;
    for (std::vector<int>::const_iterator i = e; i != b; ++i) {
        data[dsize++] = *i;
    }
    CLEAR_VECTOR;
    END_TX;
    VERIFY("swap", 10, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10});

    global_barrier->arrive(id);
}

/**
 * Transactional test of legacy const reverse iterators
 */
void legacy_const_reverse_iterator_test_concurrent(int id)
{
    // invariant: the vector starts off null
    assert(iter_vector == NULL);

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
    // make a vector
    iter_vector = new std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    // use begin and end
    const std::vector<int>* l2 = iter_vector;
    std::vector<int>::const_reverse_iterator test_b = l2->rbegin();
    std::vector<int>::const_reverse_iterator test_e = l2->rend();
    // a hack to prevent it being optimized out
    for (std::vector<int>::const_reverse_iterator i = test_b; i != test_e; ++i) // using untested ++
        tmp += *i;
    CLEAR_VECTOR;
    END_TX;
    if (tmp != 55)
        std::cout << "["<<id<<"] did not compute 55" << std::endl;
    else if (id == 0)
        std::cout << " [OK] begin() and end()" << std::endl;

    // test ctors and dtors
    global_barrier->arrive(id);
    BEGIN_TX;
    iter_vector = new std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    // iterators should be default constructable (1)
    std::vector<int>::const_reverse_iterator i;
    // iterators should be copy constructable(2)
    std::vector<int>::const_reverse_iterator b(iter_vector->rbegin());
    // iterators should be copy assignable (3)
    std::vector<int>::const_reverse_iterator e = iter_vector->rend();
    std::vector<int>::const_reverse_iterator* pe = new std::vector<int>::const_reverse_iterator(e);
    // manually copy the vector, so we can be sure to use i, b, pe
    dsize = 0;
    for (i = b; i != *pe; ++i) {
        data[dsize++] = *i;
    }
    // iterators should be destructable (4)
    delete pe;
    CLEAR_VECTOR;
    END_TX;
    VERIFY("ctors and dtors", 10, {10, 9, 8, 7, 6, 5, 4, 3, 2, 1});

    // iterators should be comparable with == (5) and != (6)
    global_barrier->arrive(id);
    std::vector<int>::const_reverse_iterator e;
    if (id == 0) {
        iter_vector = new std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    }
    global_barrier->arrive(id);
    e = iter_vector->rend();
    bool eq, neq;
    BEGIN_TX;
    eq = (e == iter_vector->rend());
    neq = (e != iter_vector->rbegin());
    END_TX;
    if (!eq || !neq)
        std::cout << "["<<id<<"] did not observe correct equality results" << std::endl;
    else if (id == 0)
        std::cout << " [OK] != and ==" << std::endl;

    // test operator* and operator->
    global_barrier->arrive(id);
    if (id == 0) {
        foo.emplace(foo.begin());
        CLEAR_VECTOR;
    }
    global_barrier->arrive(id);
    int val1, val2, val3, val4;
    BEGIN_TX;
    iter_vector = new std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    std::vector<int>::const_reverse_iterator b = iter_vector->rbegin();
    // iterators can be dereferenced as an rvalue if in a dereferenceable
    // state (7)
    val1 = *b;
    // iterator access with ->
    std::vector<Q>::const_reverse_iterator x = foo.rbegin();
    val3 = x->a;
    val4 = x->b;
    CLEAR_VECTOR;
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
    iter_vector = new std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    std::vector<int>::const_reverse_iterator i = iter_vector->rbegin();
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
    std::vector<int>::const_reverse_iterator a1(iter_vector->rbegin());
    std::vector<int>::const_reverse_iterator a2(iter_vector->rbegin());
    t1 = *a1;
    t2 = *a2;
    ok  &= (a1 == a2) && (t1 == 10) && (t2 == 10);
    a1++;
    a2++;
    t3 = *a1;
    t4 = *a2;
    ok &= (a1 == a2) && (t3 == 9) && (t4 == 9);
    CLEAR_VECTOR;
    END_TX;
    ok &= (val1 == 9) && (val2 == 8) && (val3 == 8) && (val4 == 7);
    if (!ok)
        std::cout << "["<<id<<"] errors on ++ tests" << std::endl;
    else if (id == 0)
        std::cout << " [OK] operator ++" << std::endl;

    // test operator --
    global_barrier->arrive(id);
    BEGIN_TX;
    iter_vector = new std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    std::vector<int>::const_reverse_iterator i = iter_vector->rbegin();
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
    CLEAR_VECTOR;
    END_TX;
    ok = (val1 == 8) && (val2 == 9) && (val3 == 9) && (val4 == 10);
    if (!ok)
        std::cout << "["<<id<<"] errors on -- tests" << std::endl;
    else if (id == 0)
        std::cout << " [OK] operator --" << std::endl;

    // test swapping (16)
    global_barrier->arrive(id);
    BEGIN_TX;
    iter_vector = new std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    std::vector<int>::const_reverse_iterator b = iter_vector->rbegin();
    std::vector<int>::const_reverse_iterator e = iter_vector->rend();
    swap(b, e);
    // manually copy the vector, so we can be sure to use i, b, e
    dsize = 0;
    for (std::vector<int>::const_reverse_iterator i = e; i != b; ++i) {
        data[dsize++] = *i;
    }
    CLEAR_VECTOR;
    END_TX;
    VERIFY("swap", 10, {10, 9, 8, 7, 6, 5, 4, 3, 2, 1});

    global_barrier->arrive(id);
}

/**
 * Transactional test of C++ const forward iterators with cbegin
 */
void const_iterator_test_concurrent(int id)
{
    // invariant: the vector starts off null
    assert(iter_vector == NULL);

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
    // make a vector
    iter_vector = new std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    // use begin and end
    const std::vector<int>* l2 = iter_vector;
    std::vector<int>::const_iterator test_b = l2->cbegin();
    std::vector<int>::const_iterator test_e = l2->cend();
    // a hack to prevent it being optimized out
    for (std::vector<int>::const_iterator i = test_b; i != test_e; ++i) // using untested ++
        tmp += *i;
    CLEAR_VECTOR;
    END_TX;
    if (tmp != 55)
        std::cout << "["<<id<<"] did not compute 55" << std::endl;
    else if (id == 0)
        std::cout << " [OK] cbegin() and cend()" << std::endl;

    // test that const and non-const iterators can be compared
    global_barrier->arrive(id);
    bool ok = true;
    BEGIN_TX;
    // make a vector
    iter_vector = new std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    // get const iterators
    std::vector<int>::const_iterator test_b = iter_vector->cbegin();
    std::vector<int>::const_iterator test_e = iter_vector->cend();
    std::vector<int>::iterator other_b = iter_vector->begin();
    std::vector<int>::iterator other_e = iter_vector->end();
    ok = ok & (test_b == other_b) && (other_e == test_e) && (test_b != other_e) && (other_b != test_e);
    END_TX;
    if (!ok)
        std::cout << "["<<id<<"] failed const vs. non-const equality tests" << std::endl;
    else if (id == 0)
        std::cout << " [OK] const vs. non-const equality tests" << std::endl;

    // test ctors and dtors
    global_barrier->arrive(id);
    BEGIN_TX;
    iter_vector = new std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    // iterators should be default constructable (1)
    std::vector<int>::const_iterator i;
    // iterators should be copy constructable(2)
    std::vector<int>::const_iterator b(iter_vector->cbegin());
    // iterators should be copy assignable (3)
    std::vector<int>::const_iterator e = iter_vector->cend();
    std::vector<int>::const_iterator* pe = new std::vector<int>::const_iterator(e);
    // manually copy the vector, so we can be sure to use i, b, pe
    dsize = 0;
    for (i = b; i != *pe; ++i) {
        data[dsize++] = *i;
    }
    // iterators should be destructable (4)
    delete pe;
    CLEAR_VECTOR;
    END_TX;
    VERIFY("ctors and dtors", 10, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10});

    // iterators should be comparable with == (5) and != (6)
    global_barrier->arrive(id);
    std::vector<int>::const_iterator e;
    if (id == 0) {
        iter_vector = new std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    }
    global_barrier->arrive(id);
    e = iter_vector->cend();
    bool eq, neq;
    BEGIN_TX;
    eq = (e == iter_vector->cend());
    neq = (e != iter_vector->cbegin());
    END_TX;
    if (!eq || !neq)
        std::cout << "["<<id<<"] did not observe correct equality results" << std::endl;
    else if (id == 0)
        std::cout << " [OK] != and ==" << std::endl;

    // test operator* and operator->
    global_barrier->arrive(id);
    if (id == 0) {
        foo.emplace(foo.cbegin());
        CLEAR_VECTOR;
    }
    global_barrier->arrive(id);
    int val1, val2, val3, val4;
    BEGIN_TX;
    iter_vector = new std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    std::vector<int>::const_iterator b = iter_vector->cbegin();
    // iterators can be dereferenced as an rvalue if in a dereferenceable
    // state (7)
    val1 = *b;
    // iterator access with ->
    std::vector<Q>::const_iterator x = foo.cbegin();
    val3 = x->a;
    val4 = x->b;
    CLEAR_VECTOR;
    END_TX;
    ok = (val1 == 1) && (val3 == 1) && (val4 == 2);
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
    iter_vector = new std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    std::vector<int>::const_iterator i = iter_vector->cbegin();
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
    std::vector<int>::const_iterator a1(iter_vector->cbegin());
    std::vector<int>::const_iterator a2(iter_vector->cbegin());
    t1 = *a1;
    t2 = *a2;
    ok  &= (a1 == a2) && (t1 == 1) && (t2 == 1);
    a1++;
    a2++;
    t3 = *a1;
    t4 = *a2;
    ok &= (a1 == a2) && (t3 == 2) && (t4 == 2);
    CLEAR_VECTOR;
    END_TX;
    ok &= (val1 == 2) && (val2 == 3) && (val3 == 3) && (val4 == 4);
    if (!ok)
        std::cout << "["<<id<<"] errors on ++ tests" << std::endl;
    else if (id == 0)
        std::cout << " [OK] operator ++" << std::endl;

    // test operator --
    global_barrier->arrive(id);
    BEGIN_TX;
    iter_vector = new std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    std::vector<int>::const_iterator i = iter_vector->cend();
    // can be decremented (13)
    --i;
    val1 = *i;
    // can be decremented (14)
    i--;
    val2 = *i;
    // can be decremented (15)
    val3 = *i--;
    val4 = *i;
    CLEAR_VECTOR;
    END_TX;
    ok = (val1 == 10) && (val2 == 9) && (val3 == 9) && (val4 == 8);
    if (!ok)
        std::cout << "["<<id<<"] errors on -- tests" << std::endl;
    else if (id == 0)
        std::cout << " [OK] operator --" << std::endl;

    // test swapping (16)
    global_barrier->arrive(id);
    BEGIN_TX;
    iter_vector = new std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    std::vector<int>::const_iterator b = iter_vector->cbegin();
    std::vector<int>::const_iterator e = iter_vector->cend();
    swap(b, e);
    // manually copy the vector, so we can be sure to use i, b, e
    dsize = 0;
    for (std::vector<int>::const_iterator i = e; i != b; ++i) {
        data[dsize++] = *i;
    }
    CLEAR_VECTOR;
    END_TX;
    VERIFY("swap", 10, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10});

    global_barrier->arrive(id);
}

/**
 * Transactional test of C++11 const reverse iterators with crbegin
 */
void const_reverse_iterator_test_concurrent(int id)
{
    // invariant: the vector starts off null
    assert(iter_vector == NULL);

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
    // make a vector
    iter_vector = new std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    // use begin and end
    const std::vector<int>* l2 = iter_vector;
    std::vector<int>::const_reverse_iterator test_b = l2->crbegin();
    std::vector<int>::const_reverse_iterator test_e = l2->crend();
    // a hack to prevent it being optimized out
    for (std::vector<int>::const_reverse_iterator i = test_b; i != test_e; ++i) // using untested ++
        tmp += *i;
    CLEAR_VECTOR;
    END_TX;
    if (tmp != 55)
        std::cout << "["<<id<<"] did not compute 55" << std::endl;
    else if (id == 0)
        std::cout << " [OK] crbegin() and crend()" << std::endl;

    // test ctors and dtors
    global_barrier->arrive(id);
    BEGIN_TX;
    iter_vector = new std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    // iterators should be default constructable (1)
    std::vector<int>::const_reverse_iterator i;
    // iterators should be copy constructable(2)
    std::vector<int>::const_reverse_iterator b(iter_vector->crbegin());
    // iterators should be copy assignable (3)
    std::vector<int>::const_reverse_iterator e = iter_vector->crend();
    std::vector<int>::const_reverse_iterator* pe = new std::vector<int>::const_reverse_iterator(e);
    // manually copy the vector, so we can be sure to use i, b, pe
    dsize = 0;
    for (i = b; i != *pe; ++i) {
        data[dsize++] = *i;
    }
    // iterators should be destructable (4)
    delete pe;
    CLEAR_VECTOR;
    END_TX;
    VERIFY("ctors and dtors", 10, {10, 9, 8, 7, 6, 5, 4, 3, 2, 1});

    // iterators should be comparable with == (5) and != (6)
    global_barrier->arrive(id);
    std::vector<int>::const_reverse_iterator e;
    if (id == 0) {
        iter_vector = new std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    }
    global_barrier->arrive(id);
    e = iter_vector->crend();
    bool eq, neq;
    BEGIN_TX;
    eq = (e == iter_vector->crend());
    neq = (e != iter_vector->crbegin());
    END_TX;
    if (!eq || !neq)
        std::cout << "["<<id<<"] did not observe correct equality results" << std::endl;
    else if (id == 0)
        std::cout << " [OK] != and ==" << std::endl;

    // test operator* and operator->
    global_barrier->arrive(id);
    if (id == 0) {
        foo.emplace(foo.begin());
        CLEAR_VECTOR;
    }
    global_barrier->arrive(id);
    int val1, val2, val3, val4;
    BEGIN_TX;
    iter_vector = new std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    std::vector<int>::const_reverse_iterator b = iter_vector->crbegin();
    // iterators can be dereferenced as an rvalue if in a dereferenceable
    // state (7)
    val1 = *b;
    // iterator access with ->
    std::vector<Q>::const_reverse_iterator x = foo.crbegin();
    val3 = x->a;
    val4 = x->b;
    CLEAR_VECTOR;
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
    iter_vector = new std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    std::vector<int>::const_reverse_iterator i = iter_vector->crbegin();
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
    std::vector<int>::const_reverse_iterator a1(iter_vector->crbegin());
    std::vector<int>::const_reverse_iterator a2(iter_vector->crbegin());
    t1 = *a1;
    t2 = *a2;
    ok  &= (a1 == a2) && (t1 == 10) && (t2 == 10);
    a1++;
    a2++;
    t3 = *a1;
    t4 = *a2;
    ok &= (a1 == a2) && (t3 == 9) && (t4 == 9);
    CLEAR_VECTOR;
    END_TX;
    ok &= (val1 == 9) && (val2 == 8) && (val3 == 8) && (val4 == 7);
    if (!ok)
        std::cout << "["<<id<<"] errors on ++ tests" << std::endl;
    else if (id == 0)
        std::cout << " [OK] operator ++" << std::endl;

    // test operator --
    global_barrier->arrive(id);
    BEGIN_TX;
    iter_vector = new std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    std::vector<int>::const_reverse_iterator i = iter_vector->crbegin();
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
    CLEAR_VECTOR;
    END_TX;
    ok = (val1 == 8) && (val2 == 9) && (val3 == 9) && (val4 == 10);
    if (!ok)
        std::cout << "["<<id<<"] errors on -- tests" << std::endl;
    else if (id == 0)
        std::cout << " [OK] operator --" << std::endl;

    // test swapping (16)
    global_barrier->arrive(id);
    BEGIN_TX;
    iter_vector = new std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    std::vector<int>::const_reverse_iterator b = iter_vector->crbegin();
    std::vector<int>::const_reverse_iterator e = iter_vector->crend();
    swap(b, e);
    // manually copy the vector, so we can be sure to use i, b, e
    dsize = 0;
    for (std::vector<int>::const_reverse_iterator i = e; i != b; ++i) {
        data[dsize++] = *i;
    }
    CLEAR_VECTOR;
    END_TX;
    VERIFY("swap", 10, {10, 9, 8, 7, 6, 5, 4, 3, 2, 1});

    global_barrier->arrive(id);
}
#endif
