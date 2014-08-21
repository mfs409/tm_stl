#include <mutex>
#include "../common/tm.h"

#pragma once

/**
 * This header is just a convenience for listing all the different
 * tests that we might run.
 */

// member function tests, from member.cc: ctor tests, dtor tests, and operator= tests
void ctor_dtor_tests(int id);
void op_eq_tests(int id);

// iterator tests, from iter.cc
void iter_create_tests(int id);
void iter_method_tests(int id);
void iter_operator_tests(int id);
void iter_overload_tests(int id);
void iter_function_tests(int id);

// TODO: what about reverse iterators?  const iterators?  const reverse
// iterators?

// capacity tests, from cap.cc
void cap_tests(int id);

// element access tests, from element.cc
void element_tests(int id);

// modifier tests, from modifier.cc
void modifier_tests(int id);

// observer tests, from observers.cc
void observer_tests(int id);

// tests of map operations, from operations.cc
void operation_tests(int id);

// tests of non-member function overloads, from overloads.cc
void overload_tests(int id);
