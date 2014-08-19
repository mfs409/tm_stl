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
void swap_tests(int id);

// non-member function overload tests, from overload.cc
void overload_tests(int id);

// non-member class specialization tests, from specialize.cc
void specialize_tests(int id);

// non-member function tests, from function.cc
void function_tests(int id);
