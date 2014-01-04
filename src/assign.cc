#include <iostream>
#include <string>
#include <list>
#include "sequential_tests.h"

std::list<int> z({1, 2, 3, 4, 5, 6});
std::list<int> y;

/**
 * Here is the sequential operator= test.
 */
void assign_test_seq()
{
  std::cout << "Testing operator=" << std::endl;
  // create a list with the initializer list operator
  std::list<int> tmp({1, 2, 3, 4, 5, 6});

  // test copy assignment (1)
  y = z;
  std::list<int> t2 = tmp;
  tmp.clear();
  my_list = &tmp;
  check("Expect empty");
  my_list = &t2;
  check("Expect 1, 2, 3, 4, 5, 6");

  // test move assignment (2)
  tmp = std::move(t2);
  my_list = &t2;
  check("Expect empty");
  my_list = &tmp;
  check("Expect 1, 2, 3, 4, 5, 6");

  // test initializer assignment (3)
  t2 = {8, 9, 11};
  my_list = &t2;
  check("Expect 8, 9, 11");

  // clean up...
  tmp.clear();
  t2.clear();
}
