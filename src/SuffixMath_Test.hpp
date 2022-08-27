#pragma once

#include <iostream>
#include "SuffixMath.hpp"
namespace SuffixMath_Test {
bool Test() {
  using namespace std;
  using namespace SuffixMath;
  cout << " --- Testing SuffixMath --- " << endl;
  bool success = true;
  bool test = true;

  cout << "Defining Integer: ";
  Integer x = 4;
  test = true;
  success &= test;
  cout << ((test ? "success" : "failed")) << endl;

  cout << "cycleStart_I: ";
  test = (cycleStart_I(4) == 4_mpz);
  success &= test;
  cout << (test ? "success" : "failed") << endl;

  cout << "cycleLen_I: ";
  test = (cycleLen_I(4) == 500_mpz);
  success &= test;
  cout << (test ? "success" : "failed") << endl;

  cout << "cycleEnd_I: ";
  test = (cycleEnd_I(2) == 22_mpz);
  success &= test;
  cout << (test ? "success" : "failed") << endl;

  cout << "cycleLast_I: ";
  test = (cycleLast_I(3) == 102_mpz);
  success &= test;
  cout << (test ? "success" : "failed") << endl;

  cout << "cycleStart: ";
  test = (cycleStart(4) == 4);
  success &= test;
  cout << (test ? "success" : "failed") << endl;

  cout << "cycleLen: ";
  test = (cycleLen(4) == 500);
  success &= test;
  cout << (test ? "success" : "failed") << endl;

  cout << "cycleEnd: ";
  test = (cycleEnd(2) == 22);
  success &= test;
  cout << (test ? "success" : "failed") << endl;

  cout << "cycleLast_I: ";
  test = (cycleLast_I(3) == 102);
  success &= test;
  cout << (test ? "success" : "failed") << endl;

  cout << "Integer perisistence: ";
  Integer y = x;
  x = 5;
  test = (y == 4);
  success &= test;
  cout << (test ? "success" : "failed") << endl;

  cout << "powContainsPows(2, 8): ";
  test = (powContainsPows(2, 8) == 3);  // 2⁸=256 -> k = 3
  success &= test;
  cout << (test ? "success" : "failed") << endl;

  cout << "powContainsPows(2, 16): ";
  test = (powContainsPows(2, 16) == 0);  // 2^(16) = 65536 -> k = 0
  success &= test;
  cout << (test ? "success" : "failed") << endl;

  cout << "powContainsPows(2, 88): ";
  test = (powContainsPows(2, 88) == 4);  // 2^(88) = 309,485,009,821,345,068,724,781,056 -> k = 4
  success &= test;
  cout << (test ? "success" : "failed") << endl;

  cout << "powContains([3, 5], 2, 10): ";
  std::vector<int> v;
  v.push_back(3);
  v.push_back(5);
  test = (powContains(v, 2, 10) == 0);  // 2^(10) = 1024 -> k = 0
  success &= test;
  cout << (test ? "success" : "failed") << endl;

  cout << "maxCompletedCycleK: ";
  test = (maxCompletedCycleK(1000) == 4);
  success &= test;
  cout << ((test ? "success" : "failed")) << endl;

  cout << "maxCompletedCycleN: ";
  test = (maxCompletedCycleN(1000) == 503);
  success &= test;
  cout << ((test ? "success" : "failed")) << endl;

  cout << "nextCompletedCycleN: ";
  test = (nextCompletedCycleN(1000) == 2504);
  success &= test;
  cout << ((test ? "success" : "failed")) << endl;

  cout << " --- Overall " << (success ? "success --- " : "failed --- ") << endl << endl;
  return success;
}
}  // namespace SuffixMath_Test
