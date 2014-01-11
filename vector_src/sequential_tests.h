/**
 * This header is just a convenience for listing all the different sequential
 * tests that we might run.
 */

// constructor/destructor tests, from ctor.cc
void ctor_test_seq();

// operator= tests, from assign.cc
void assign_test_seq();

// iterator tests, from iter.cc
void iterator_test_seq();
void reverse_iterator_test_seq();
void legacy_const_iterator_test_seq();
void legacy_const_reverse_iterator_test_seq();
void const_iterator_test_seq();
void const_reverse_iterator_test_seq();

// capacity tests, from cap.cc
void cap_test_seq();

// element access tests, from element.cc
void element_test_seq();

// modifier tests, from modifier.cc
void modifier_test_seq();

// operations tests, from operations.cc
void operations_test_seq();

// observers tests, from observers.cc
void observers_test_seq();

// test for relational operators
void relational_test_seq();

// test swap operation
void swap_test_seq() ;

// a short-term helper function for outputting expected and observed results
void check(std::string s);

// a short-term helper... make the vector a global so that we know transactions
// won't optimize it out.
extern std::vector<int>* my_vector;
