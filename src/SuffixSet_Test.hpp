#pragma once

#include <array>
#include <iostream>
#include "SuffixSet.hpp"
namespace SuffixSet_Test {
bool Test() {
  using namespace std;
  cout << " --- Testing SuffixSet --- " << endl;
  bool success = true;
  bool test = true;

  cout << "Constructor: ";
  SuffixSet s = SuffixSet();
  test = true;
  success &= test;
  cout << ((test ? "success" : "failed")) << endl;

  cout << "inserting 3 elements: ";
  s.insert(1, 1);
  s.insert(2, 2);
  s.insert(3, 3);
  success &= test;
  cout << (test ? "success" : "failed") << endl;

  cout << "contains [0..30]: ";
  const std::array<bool, 30> member = {false, true,  true,  true,  false, true,  false, false, false, true,
                                       false, false, false, true,  false, false, false, true,  false, false,
                                       false, true,  true,  false, false, true,  false, false, false, true};
  test = true;
  for (unsigned long n = 0; n < 30; n++) {
    test &= (s.containsN(n) == member[n]);
  }
  success &= test;
  cout << (test ? "success" : "failed") << endl;

  cout << " --- Overall " << (success ? "success --- " : "failed --- ") << endl << endl;
  return success;
}
}  // namespace SuffixSet_Test
