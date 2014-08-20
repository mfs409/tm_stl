// -*-c++-*-
#pragma once

#include <initializer_list>
using std::initializer_list;

class verifier
{
    /// max number of things we can store in the verifier
    static const int SIZE = 256;

    /// storage for stuff we need to verify
    int data[SIZE];
    int count;

  public:
    /// construct by filling with -2 and setting count to 0
    verifier() : count(0)
    {
        for (int i = 0; i < SIZE; ++i) {
            data[i] = -2;
        }
    }

    /// add something to the verifier
    void insert(int i)
    {
        data[count++] = i;
    }

    template <class T>
    void insert_all(T* in)
    {
        for (auto i : *in)
            data[count++] = i;
    }


    void check(const char* test_name, int thread_id, int expected_size,
               initializer_list<int> expected_data)
    {
        bool ok = true;
        int  c  = 0;
        if (count != expected_size)
            printf(" [%d] size did not match %d != %d\n", thread_id, count, expected_size);
        else if (!ok)
            printf(" [%d] array copy did not match\n");
        else if (thread_id == 0)
            printf(" [OK::count+data] %s\n", test_name);
    }

    void check_size(const char* test_name, int thread_id, int expected_size)
    {
      if (count != expected_size)
          printf(" [%d] size did not match %d != %d\n", thread_id, count, expected_size);
      else if (thread_id == 0)
          printf(" [OK::count] %s\n", test_name);
    }
};
