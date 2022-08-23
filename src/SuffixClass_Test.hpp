#pragma once

#include <iostream>
#include <sstream>
#include "SuffixClass.hpp"
namespace SuffixClass_Test {
bool Test() {
  using namespace std;
  cout << " --- Testing SuffixClass --- " << endl;
  bool success = true;
  bool test = true;

  cout << "Constructor: ";
  SuffixClass endsWithTwo = SuffixClass(1, 1);
  test = true;
  success &= test;
  cout << ((test ? "success" : "failed")) << endl;

  cout << "contains: ";
  test = endsWithTwo.contains(1);
  test &= endsWithTwo.contains(5);
  test &= endsWithTwo.contains(9);
  test &= endsWithTwo.contains(1001);
  test &= not endsWithTwo.contains(1002);
  test &= not endsWithTwo.contains(4);
  success &= test;
  cout << (test ? "success" : "failed") << endl;

  cout << "cetSuffixLen: ";
  test = (endsWithTwo.getSuffixLen() == 1);
  success &= test;
  cout << (test ? "success" : "failed") << endl;

  cout << "getCycleLen: ";
  test = (endsWithTwo.getCycleLen() == 4);
  success &= test;
  cout << (test ? "success" : "failed") << endl;

  cout << "getRepresentativeN: ";
  test = (endsWithTwo.getRepresentativeN() == 1);
  success &= test;
  cout << (test ? "success" : "failed") << endl;
  // TODO test if implemented getter
  //    cout << "Perisistence: ";
  //    test = false;
  //    success &= test;
  //    cout << (test? "success" : "failed") << endl;

  cout << "operator== ";
  test = (SuffixClass(4_mpz, 20_mpz) == SuffixClass(4_mpz, 20_mpz));
  test = (SuffixClass(4_mpz, 20_mpz) == SuffixClass(4_mpz, 520_mpz));
  success &= test;
  cout << (test ? "success" : "failed") << endl;

  cout << "operator!= ";
  test = (SuffixClass(4_mpz, 20_mpz) != SuffixClass(3_mpz, 20_mpz));
  test = (SuffixClass(4_mpz, 20_mpz) != SuffixClass(4_mpz, 521_mpz));
  success &= test;
  cout << (test ? "success" : "failed") << endl;

  cout << "operator<<: ";
  cout << SuffixClass(4_mpz, 20_mpz)
       << endl;  // should print "20 (mod 500 )(k 4 )"

  cout << "operator>>: ";
  SuffixClass cmp = SuffixClass(4_mpz, 20_mpz);
  SuffixClass read;
  stringstream sstr;
  sstr << "20 (mod 500 )(k 4 )";
  sstr >> read;
  test = (cmp == read);
  success &= test;
  cout << (test ? "success" : "failed") << endl;

  cout << " --- Overall " << (success ? "success --- " : "failed --- ") << endl
       << endl;
  return success;
}
}  // namespace SuffixClass_Test
