#pragma once

#include <array>
#include <iostream>
#include "SuffixSet.hpp"
namespace SuffixSet_Test {
inline auto Test() -> bool {
  using namespace std;
  cout << " --- Testing SuffixSet --- "
       << "\n";
  bool success = true;
  bool test = true;

  cout << "Constructor: ";
  SuffixSet s = SuffixSet();
  test = true;
  success &= test;
  cout << ((test ? "success" : "failed")) << "\n";

  cout << "inserting 3 elements: ";
  s.insert(1, 1);
  s.insert(2, 2);
  s.insert(3, 3);
  success &= test;
  cout << (test ? "success" : "failed") << "\n";

  cout << "contains [0..30]: ";
  const unsigned long TEST_N_MAX = 30;
  const std::array<bool, TEST_N_MAX> member = {false, true,  true,  true,  false, true,  false, false, false, true,
                                               false, false, false, true,  false, false, false, true,  false, false,
                                               false, true,  true,  false, false, true,  false, false, false, true};
  test = true;
  for (unsigned long n = 0; n < TEST_N_MAX; n++) {
    test &= (s.containsN(n) == member[n]);
  }
  success &= test;
  cout << (test ? "success" : "failed") << "\n";

  cout << " --- Overall " << (success ? "success --- " : "failed --- ") << "\n"
       << "\n";
  return success;
}
}  // namespace SuffixSet_Test
