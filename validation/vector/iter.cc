#include <iostream>
#include <vector>
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

/// The vector we will use for our tests
std::vector<int>* iter_vec_int = NULL;
std::vector<Q>*   iter_vec_q   = NULL;

/**
 * Ensure that all of the begin/end methods for getting an iterator from
 * std::vector work
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
        iter_vec_int = new std::vector<int>();
        std::vector<int>::iterator b = iter_vec_int->begin();
        std::vector<int>::iterator e = iter_vec_int->end();
        ok = (b == e);
        delete(iter_vec_int);
        iter_vec_int = NULL;
        END_TX;
        if (!ok)
            printf(" [%d] iterator begin and end did not match for empty vector", id);
        else if (id == 0)
            printf(" [OK] %s\n", "basic iterator begin and end");
    }

    // now test the legacy const begin() and end() calls
    global_barrier->arrive(id);
    {
        bool ok = false;
        verifier v;
        BEGIN_TX;
        iter_vec_int = new std::vector<int>();
        const std::vector<int>* cd = iter_vec_int;
        std::vector<int>::const_iterator b = cd->begin();
        std::vector<int>::const_iterator e = cd->end();
        ok = (b == e);
        delete(iter_vec_int);
        iter_vec_int = NULL;
        END_TX;
        if (!ok)
            printf(" [%d] iterator begin and end did not match for empty vector", id);
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
        iter_vec_int = new std::vector<int>();
        std::vector<int>::reverse_iterator b = iter_vec_int->rbegin();
        std::vector<int>::reverse_iterator e = iter_vec_int->rend();
        ok = (b == e);
        delete(iter_vec_int);
        iter_vec_int = NULL;
        END_TX;
        if (!ok)
            printf(" [%d] iterator rbegin and rend did not match for empty vector", id);
        else if (id == 0)
            printf(" [OK] %s\n", "basic reverse iterator rbegin and rend");
    }
    // now test the legacy const rbegin() and rend() calls
    global_barrier->arrive(id);
    {
        bool ok = false;
        verifier v;
        BEGIN_TX;
        iter_vec_int = new std::vector<int>();
        const std::vector<int>* cd = iter_vec_int;
        std::vector<int>::const_reverse_iterator b = cd->rbegin();
        std::vector<int>::const_reverse_iterator e = cd->rend();
        ok = (b == e);
        delete(iter_vec_int);
        iter_vec_int = NULL;
        END_TX;
        if (!ok)
            printf(" [%d] iterator rbegin and rend did not match for empty vector", id);
        else if (id == 0)
            printf(" [OK] %s\n", "legacy const reverse iterator rbegin and rend");
    }

    // now test the c++11 const begin() and end() calls
    global_barrier->arrive(id);
    {
        bool ok = false;
        verifier v;
        BEGIN_TX;
        iter_vec_int = new std::vector<int>();
        const std::vector<int>* cd = iter_vec_int;
        std::vector<int>::const_iterator b = cd->cbegin();
        std::vector<int>::const_iterator e = cd->cend();
        ok = (b == e);
        delete(iter_vec_int);
        iter_vec_int = NULL;
        END_TX;
        if (!ok)
            printf(" [%d] iterator cbegin and cend did not match for empty vector", id);
        else if (id == 0)
            printf(" [OK] %s\n", "c++11 cbegin and cend");
    }

     // now test the c++11 crbegin() and crend() calls
    global_barrier->arrive(id);
    {
        bool ok = false;
        verifier v;
        BEGIN_TX;
        iter_vec_int = new std::vector<int>();
        const std::vector<int>* cd = iter_vec_int;
        std::vector<int>::const_reverse_iterator b = cd->crbegin();
        std::vector<int>::const_reverse_iterator e = cd->crend();
        ok = (b == e);
        delete(iter_vec_int);
        iter_vec_int = NULL;
        END_TX;
        if (!ok)
            printf(" [%d] iterator crbegin and crend did not match for empty vector", id);
        else if (id == 0)
            printf(" [OK] %s\n", "c++11 crbegin and crend");
    }


}

void iter_method_tests(int id)
{
    global_barrier->arrive(id);

    if (id == 0)
        printf("Testing basic methods of std::vector::iterator\n");
    global_barrier->arrive(id);
    {
        verifier v;
        BEGIN_TX;
        iter_vec_int = new std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
        // iterators should be default constructable
        std::vector<int>::iterator i = std::vector<int>::iterator();
        // iterators should be copy constructable
        std::vector<int>::iterator b(iter_vec_int->begin());
        // iterators should be copy assignable
        std::vector<int>::iterator e = iter_vec_int->end();
        std::vector<int>::iterator* pe = new std::vector<int>::iterator(e);
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
        iter_vec_int = new std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
        std::vector<int>::iterator b = iter_vec_int->begin();
        std::vector<int>::iterator e = iter_vec_int->end();
        std::swap(b, e);
        // manually copy the vector, so we can be sure to use b, e
        for (std::vector<int>::iterator i = e; i != b; ++i)
            v.insert(*i);
        END_TX;
        v.check("swap iterators", id, 10, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    }
}


void iter_operator_tests(int id)
{
    global_barrier->arrive(id);
    if (id == 0)
        printf("Testing operator methods on std::vector::iterator\n");
}

void iter_overload_tests(int id)
{
    global_barrier->arrive(id);
    if (id == 0)
        printf("Testing non-method comparator overloads on std::vector::iterator\n");
     // test comparability of iterators
    global_barrier->arrive(id);
    {
        bool ok = true;
        BEGIN_TX;
        iter_vec_int = new std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
        std::vector<int>::iterator b = iter_vec_int->begin();
        std::vector<int>::iterator e = iter_vec_int->end();
        std::vector<int>::const_iterator ce = iter_vec_int->cend();
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

void iter_function_tests(int id)
{
     global_barrier->arrive(id);
    if (id == 0)
        printf("Testing non-method functions on std::vector::iterator\n");
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
    iter_vec_int = new std::vector<int>();
    std::vector<int>::iterator b = iter_vec_int->begin();
    std::vector<int>::iterator e = iter_vec_int->end();
    ok = (b == e);
    delete(iter_vec_int);
    iter_vec_int = NULL;
    END_TX;
    if (!ok)
        printf(" [%d] iterator begin and end did not match for empty vector", id);
    else if (id == 0)
        printf(" [OK] %s\n", "basic iterator begin and end");
    }

    // now test copy-assignable, copy constructable, and destructable
    global_barrier->arrive(id);
    verifier v;
    BEGIN_TX;
    iter_vec_int = new std::vector<int>({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    // iterators should be default constructable
    std::vector<int>::iterator i;
    // iterators should be copy constructable
    std::vector<int>::iterator b(iter_vec_int->begin());
    // iterators should be copy assignable
    std::vector<int>::iterator e = iter_vec_int->end();
    std::vector<int>::iterator* pe = new std::vector<int>::iterator(e);
    // manually copy the vector, so we can be sure to use i, b, pe
    for (i = b; i != *pe; ++i)
        v.insert(*i);
    // iterators should be destructable
    delete pe;
    END_TX;
    v.check("ctors and dtors", id, 10, {1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
   
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
    iter_vec_int = new std::vector<int>();
    const std::vector<int>* cd = iter_vec_int;
    std::vector<int>::const_iterator b = cd->begin();
    std::vector<int>::const_iterator e = cd->end();
    ok = (b == e);
     delete(iter_vec_int);
    iter_vec_int = NULL;
    END_TX;
    if (!ok)
        printf(" [%d] iterator begin and end did not match for empty vector", id);
    else if (id == 0)
        printf(" [OK] %s\n", "basic iterator begin and end");
    }
}

