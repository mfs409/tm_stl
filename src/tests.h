/**
 * This header is just a convenience for listing all the different tests that
 * we might run.
 */

// constructor/destructor tests, from ctor.cc
void ctor_test();

// operator= tests, from assign.cc
void assign_test();

// iterator tests, from iter.cc
void iterator_test();
void reverse_iterator_test();
void legacy_const_iterator_test();
void legacy_const_reverse_iterator_test();
void const_iterator_test();
void const_reverse_iterator_test();

// capacity tests, from cap.cc
void cap_test();

// element access tests, from element.cc
void element_test();

// modifier tests, from modifier.cc
void modifier_test();

// operations tests, from operations.cc
void operations_test();

// observers tests, from observers.cc
void observers_test();

// test for relational operators
void relational_test();

// a short-term helper function for outputting expected and observed results
void check(std::string s);

// a short-term helper... make the list a global so that we know transactions
// won't optimize it out.
extern std::list<int>* my_list;
