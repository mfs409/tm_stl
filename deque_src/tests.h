#include <mutex>
#include "../common/tm.h"

/**
 * This header is just a convenience for listing all the different concurrent
 * tests that we might run.
 */

// member function tests, from member.cc: ctor tests, dtor tests, and operator= tests
void member_test(int id);

// iterator tests, from iter.cc
void iter_test(int id);
void legacy_const_iter_test(int id);

#if 0
// operator= tests, from assign.cc
void assign_test(int id);

// iterator tests, from iter.cc
void iterator_test(int id);
void reverse_iterator_test(int id);
void legacy_const_iterator_test(int id);
void legacy_const_reverse_iterator_test(int id);
void const_iterator_test(int id);
void const_reverse_iterator_test(int id);

// capacity tests, from cap.cc
void cap_test(int id);

// element access tests, from element.cc
void element_test(int id);

// modifier tests, from modifier.cc
void modifier_test(int id);

// operations tests, from operations.cc
void operations_test(int id);

// observers tests, from observers.cc
void observers_test(int id);

// test for relational operators
void relational_test(int id);

// test swap operation
void swap_test(int id);
#endif
