#pragma once

#include <gmpxx.h>
#include <iostream>

#ifdef _MSC_VER
#include <ciso646>
#endif

using Integer = mpz_class;

/** Represents Suffix Class [r]_k
Container-like class containing all exponents n whose suffix of length k is
identical. Powers are based 2, suffix = 2^n mod 10^k
*/
class SuffixClass {
 private:
  Integer _k = 0;    // length of suffix
  Integer _min = 0;  // start of the cycle of the suffix length k
  Integer _mod = 0;  // length of the cycle of the suffix length k
  Integer _max = 0;  // end of the cycle of the suffix length k
  Integer _res = 0;  // residuum such that 2^n % _mod = _res
  Integer _n = 0;    // (not yet smallest) representative of the class
 public:
  SuffixClass() = default;
  /** Represents the Suffix [n]_k whith n in [n]_k
  @param k length of the suffix
  @param n exponent of one representative of the class [n]_k
  @param r residuum of the class [r]_k, r may not be in [r]_k
  @param isN true iff nORr is actually an n that is in [n]_k
  */
  SuffixClass(const Integer& k, const Integer& nORr, bool isN = true);

  auto getSuffixLen() -> Integer {
    return _k;  // copy _k
  }

  auto getCycleLen() -> Integer {
    return _mod;  // copy _mod
  }

  auto getRepresentativeN() -> Integer {
    return _res;  // copy _res
  }

  auto contains(const Integer& n) -> bool;

  friend auto operator==(const SuffixClass& lhs, const SuffixClass& rhs) -> bool { return lhs._k == rhs._k and lhs._res == rhs._res; };
  friend auto operator!=(const SuffixClass& lhs, const SuffixClass& rhs) -> bool { return not(lhs == rhs); };
  friend auto operator<=(const SuffixClass& lhs, const SuffixClass& rhs) -> bool {
    return lhs._k <= rhs._k or lhs._res <= rhs._res;
  };  // Sorts for k first then for r
  friend auto operator<(const SuffixClass& lhs, const SuffixClass& rhs) -> bool {
    return lhs._k < rhs._k or lhs._res < rhs._res;
  };  // Sorts for k first then for r
  friend auto operator<<(std::ostream& os, const SuffixClass& obj) -> std::ostream&;
  friend auto operator>>(std::istream& is, SuffixClass& obj) -> std::istream&;
};
