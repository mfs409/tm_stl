// Driver to test the transactional version of the std::list interface
//
// according to http://www.cplusplus.com/reference/list/list/, the std::list
// interface consists of the following:
//
// Member functions
//  (constructor)
//  (destructor)
//  operator=
//
// Iterators:
//  begin
//  end
//  rbegin
//  rend
//  cbegin
//  cend
//  crbegin
//  crend
//
// Capacity:
//  empty
//  size
//  max_size
//
// Element access:
//  front
//  back
//
// Modifiers:
//  assign
//  emplace_front
//  push_front
//  pop_front
//  emplace_back
//  push_back
//  pop_back
//  emplace
//  insert
//  erase
//  swap
//  resize
//  clear
//
// Operations:
//  splice
//  remove
//  remove_if
//  unique
//  merge
//  sort
//  reverse
//
// Observers:
//  get_allocator
//
// Non-member function overloads
//  relational operators (forward_list)
//  swap (forward_list)

#include "list"
#include "cstdio"
#include "thread"

std::list<int>* global_list = NULL;

void listtest(int id)
{
    printf("Hello from thread %d\n", id);
    __transaction_atomic {
        std::list<int> l();
        std::list<int>* l2 = new std::list<int>();
    }

}


int main()
{
    std::thread threads[4];
    for (int i = 0; i < 4; ++i)
        threads[i] = std::thread(listtest, i);
    for (int i = 0; i < 4; ++i)
        threads[i].join();
}
