/* -*- C++ -*-*/

#include <mutex>
#include "../common/tm.h"

#pragma once

/**
 * This header is just a convenience for listing all the different concurrent
 * tests that we might run.
 */

// member function tests, from member.cc: ctor tests, dtor tests, and operator= tests
void ctor_dtor_tests(int id);
void op_eq_tests(int id);

// iterator tests, from iter.cc
void iter_create_tests(int id);
void basic_iter_tests(int id);
void const_iter_tests(int id);
void rev_iter_tests(int id);
void const_rev_iter_tests(int id);

// capacity tests, from cap.cc
void cap_tests(int id);

// element access tests, from element.cc
void element_tests(int id);

// modifier tests, from modifier.cc
void modifier_tests(int id);

// observers tests, from observers.cc
void observers_test(int id);

// test for operations, from operations.cc
void operations_tests(int id);

// allocator tests, from allocator.cc
void allocator_tests(int id);
