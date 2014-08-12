// -*-c++-*-
#pragma once

#include <initializer_list>
using std::initializer_list;

namespace
{
  /// helper function for resetting the vars we use for doing data structure
  /// checks
  void RESET_LOCAL(int val, int* data, int& dsize) {
      for (int i = 0; i < 256; ++i)
          data[i] = val;
      dsize = 0;
  }

  /// helper function for making sure that the data we copied within the
  /// transaction matches what we expect
  void CHECK(const char* test_name, int size, int dsize, int* data, int id,
             initializer_list<int> list) {
      bool ok = true;
      int  c  = 0;
      for (auto i : list)
          ok &= (i == data[c++]);
      if (dsize != size)
          printf(" [%d] size did not match %d != %d\n", id, dsize, size);
      else if (!ok)
          printf(" [%d] array copy did not match\n");
      else if (id == 0)
          printf(" [OK::count+data] %s\n", test_name);
  }

  /// clone the list to a local array represented by dsize, data[]
  void COPY_DEQUE(std::deque<int>* source, int* data, int& dsize) {
      for (auto i : *source)
          data[dsize++] = i;
  }

  ///
  void CHECK_SIZE(const char* test_name, int size, int dsize, int id) {
      if (dsize != size)
        printf(" [%d] size did not match %d != %d\n", id, dsize, size);
    else if (id == 0)
        printf(" [OK::count] %s\n", test_name);
  }
}
