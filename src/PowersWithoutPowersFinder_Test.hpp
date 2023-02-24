#pragma once

#include <gmpxx.h>
#include "ExtraOperators.hpp"
#include "PowersWithoutPowersFinder.hpp"

namespace PowersWithoutPowersFinder_Test {
auto Test() -> bool {
  using namespace std;
  cout << " --- Testing PowersWithoutPowersFinder --- " << "\n";
  bool success = true;
  bool test = true;

  cout << "Constructor: ";
  PowersWithoutPowersFinder pwp = PowersWithoutPowersFinder();
  test = true;
  success &= test;
  cout << ((test ? "success" : "failed")) << "\n";

  cout << "find...(1, 6): ";
  pwp.findInRange(1, 6);
  pwp.findUntilNextCycleEnd();
  std::vector<unsigned long> matches = pwp.Getmatches();
  std::vector<SuffixClass> forbidden = pwp.GetforbiddenClasses();
  std::sort(forbidden.begin(), forbidden.end());
  std::vector<unsigned long> m = {16};
  std::vector<SuffixClass> f = {SuffixClass(1_mpz, 1_mpz), SuffixClass(1_mpz, 2_mpz),  SuffixClass(1_mpz, 3_mpz), SuffixClass(2_mpz, 4_mpz),
                                SuffixClass(3_mpz, 8_mpz), SuffixClass(4_mpz, 12_mpz), SuffixClass(4_mpz, 20_mpz)};
  test &= (matches == m);
  test &= (forbidden == f);
  success &= test;
  cout << (test ? "success" : "failed") << "\n";
  cout << "Matches " << matches << "\n";
  cout << "Forbidden " << forbidden << "\n";

  cout << "finitenessProvable: ";
  test = not pwp.finitenessProvable();
  success &= test;
  cout << ((test ? "success" : "failed")) << "\n";

  cout << " --- Overall " << (success ? "success --- " : "failed --- ") << "\n" << "\n";
  return success;
}
}  // namespace PowersWithoutPowersFinder_Test
