#include <iostream>
#include <string>
#include <list>
#include "sequential_tests.h"
#include "concurrent_tests.h"
#include <cassert>

/**
 * Here is the sequential ctor test.  It also happens to be the sequential
 * dtor test, since there is only a basic dtor.
 */
void ctor_test_seq()
{
    std::cout << "Testing ctor and dtor" << std::endl;
    std::cout << "Note: we aren't testing the allocator parameter "
              << "of the constructors" << std::endl;

    // test basic constructor (1)
    my_list = new std::list<int>();
    check("Expect an empty list");
    delete my_list;

    // test sized constructor (2)
    my_list = new std::list<int>(5);
    check("Expect 5 default entries");
    delete my_list;

    // test sized constructor with defaults (2)
    my_list = new std::list<int>(5, 1);
    check("Expect 5 ones");
    delete my_list;

    // test range constructor (3)
    int a[] = {1, 2, 3};
    my_list = new std::list<int>(a, a+3);
    check("Expect 1,2,3");

    // test the copy constructor (4)
    std::list<int>* old = my_list;
    my_list = new std::list<int>(*old);
    delete old;
    check("Expect 1,2,3");

    // test the move constructor (5)
    old = my_list;
    my_list = new std::list<int>(std::move(*old));
    std::list<int>* swapper = my_list;
    my_list = old;
    check("Expect empty");
    my_list = swapper;
    check("Expect 1, 2, 3");
    delete old;
    delete swapper;

    // test the initializer list constructor (6)
    my_list = new std::list<int>({1, 2, 3, 4, 5, 6});
    check("Expect 1, 2, 3, 4, 5, 6");
    delete my_list;
}

/**
 * Here is the concurrent ctor test.  It also happens to be the concurrent
 * dtor test, since there is only a basic dtor.
 */
void ctor_test_concurrent(int id)
{
    if (id == 0) {
        std::cout << "Testing ctor and dtor" << std::endl;
        std::cout << "Note: we aren't testing the allocator parameter "
                  << "of the constructors" << std::endl;
    }

    // scratch space for holding data when computing whether the test
    // succeeded or failed.
    int data[256], dsize = 0;

    // test basic constructor (1)
    global_barrier->arrive(id);
    BEGIN_TX;
    // ensure the list is not constructed
    if (my_list != NULL) {
        dsize = 256; // not 0, so we'll report an error
    }
    else {
        // call the basic constructor, then iterate through the list to
        // ensure that it is empty
        my_list = new std::list<int>();
        dsize = 0;
        for (auto i : *my_list)
            data[dsize++] = i;
        delete my_list;
        my_list = NULL;
    }
    END_TX;
    std::cout << "Basic ctor (1) " << (dsize == 0 ? "passed" : "failed") << std::endl;

    // test sized constructor (2)
    global_barrier->arrive(id);
    // make sure we'll detect errors in data[]
    for (int i = 0; i < 16; ++i)
        data[i] = 17;
    dsize = 0;
    BEGIN_TX;
    // ensure the list is not constructed
    if (my_list != NULL) {
        dsize = 256; // not 0, so we'll report an error
    }
    else {
        // call the sized constructor, then iterate through the list to
        // ensure that it has 5 zeroes
        my_list = new std::list<int>(5);
        dsize = 0;
        for (auto i : *my_list)
            data[dsize++] = i;
        delete my_list;
        my_list = NULL;
    }
    END_TX;
    bool good = dsize == 5;
    for (int i = 0; i < 5; ++i)
        good &= (data[i] == 0);
    for (int i = 5; i < 16; ++i)
        good &= (data[i] == 17);
    std::cout << "Sized ctor (2) " << (good ? "passed" : "failed") << std::endl;



    global_barrier->arrive(id);



    return;

    // test range constructor (3)
    int a[] = {1, 2, 3};
    my_list = new std::list<int>(a, a+3);
    check("Expect 1,2,3");

    // test the copy constructor (4)
    std::list<int>* old = my_list;
    my_list = new std::list<int>(*old);
    delete old;
    check("Expect 1,2,3");

    // test the move constructor (5)
    old = my_list;
    my_list = new std::list<int>(std::move(*old));
    std::list<int>* swapper = my_list;
    my_list = old;
    check("Expect empty");
    my_list = swapper;
    check("Expect 1, 2, 3");
    delete old;
    delete swapper;

    // test the initializer list constructor (6)
    my_list = new std::list<int>({1, 2, 3, 4, 5, 6});
    check("Expect 1, 2, 3, 4, 5, 6");
    delete my_list;
}
