#include <iostream>
#include <ext/vstring.h>
#include <cassert>
#include "tests.h"

using string = __gnu_cxx::__sso_string;

/// The string we will use for our tests
string* elt_string = NULL;

/// clone the string to a local array represented by dsize, data[]
#define COPY_STRING                                 \
    dsize = 0;                                    \
    for (auto i : *elt_string)                     \
        data[dsize++] = i

/// destroy the string and set the pointer to NULL
#define CLEAR_STRING                                \
    delete elt_string;                             \
    elt_string = NULL

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

void element_tests(int id)
{
    // helper message
    if (id == 0) {
        std::cout << std::endl
                  << "Testing element access methods" << std::endl;
    }

    // test all accessors at once
    global_barrier->arrive(id);
    char f1, s1, t1, b1, f2, s2, t2, b2, f3, s3, t3, b3, f4, s4, t4, b4;
    BEGIN_TX;
    elt_string = new string("abcd");

    // start with a basic test of front and back
    f1 = elt_string->front();
    s1 = elt_string->at(1);
    t1 = (*elt_string)[2];
    b1 = elt_string->back();

    // now make sure we can get const references
    const string* z = elt_string;
    const char& cb = z->back();
    const char& cs = z->at(1);
    const char& ct = (*z)[2];
    const char& cf = z->front();
    f2 = cf;
    s2 = cs;
    t2 = ct;
    b2 = cb;

    // Ensure these don't compile:
    // cf += 11;
    // cb += 11;

    // now test that non-const references can be mutated
    char& rb = elt_string->back();
    char& rs = elt_string->at(1);
    char& rt = (*elt_string)[2];
    char& rf = elt_string->front();
    rb = 'z';
    rt = 'y';
    rs = 'x';
    rf = 'w';

    f3 = cf;
    s3 = cs;
    t3 = ct;
    b3 = cb;

    f4 = rf;
    s4 = rs;
    t4 = rt;
    b4 = rb;

    CLEAR_STRING;
    END_TX;

    bool ok = (f1 == 'a') && (s1 == 'b') && (t1 == 'c') && (b1 == 'd')
        && (f2 == 'a') && (s2 == 'b') && (t2 == 'c') && (b2 == 'd')
        && (f3 == 'w') && (s3 == 'x') && (t3 == 'y') && (b3 == 'z')
        && (f4 == 'w') && (s4 == 'x') && (t4 == 'y') && (b4 == 'z');
    if (!ok)
        std::cout << "["<<id<<"] errors with front and back" << std::endl;
    else if (id == 0)
        std::cout << " [OK] front and back" << std::endl;

    global_barrier->arrive(id);
}
