// Driver to test the transactional version of the std::list interface
//
// according to http://www.cplusplus.com/reference/list/list/, the std::list
// interface consists of the following:
//
// Member functions
//  (constructor), (destructor), and operator=
//
// Iterators:
//  begin, end
//  rbegin, rend
//  cbegin, cend
//  crbegin, crend
//
// Capacity:
//  empty, size, max_size
//
// Element access:
//  front, back
//
// Modifiers:
//  assign,
//  emplace, emplace_front, emplace_back,
//  push_front, pop_front
//  push_back, pop_back
//  insert, erase, swap, resize, clear
//
// Operations:
//  splice, remove, remove_if, unique, merge, sort, reverse
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
#include "atomic"
#include "mutex"

std::list<int>* global_list_ptr = NULL;
std::list<int>  global_list;

std::atomic<int> ready(0);

#ifdef NO_TM
std::mutex global_mutex;
#define BEGIN_TX {std::lock_guard<std::mutex> _g(global_mutex);
#define END_TX   }
#else
#define BEGIN_TX __transaction_atomic {
#define END_TX   }
#endif

void listtest(int id)
{
    // notify that we've started, then wait for all threads to be ready
    printf("Hello from thread %d\n", id);
    while (!ready);

    // test construction and operator=
    BEGIN_TX
        if (global_list_ptr == NULL) {
            global_list = std::list<int>();
            global_list_ptr = new std::list<int>();
        }
    END_TX

}


int main()
{
    std::thread threads[4];
    for (int i = 0; i < 4; ++i)
        threads[i] = std::thread(listtest, i);
    ready = true;
    for (int i = 0; i < 4; ++i)
        threads[i].join();
}
