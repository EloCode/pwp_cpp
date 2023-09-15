#ifndef SUFFIXSET_H
#define SUFFIXSET_H

#include <gmpxx.h>
#include <utility>  // std::pair
#include <vector>
#include "SuffixClass.hpp"
#include "flat_hash_map-master/flat_hash_map.hpp"

using Integer = mpz_class;

/** Set containing suffix classes [r]_k with fast insert and lookup methods.
 */
class SuffixSet {
 public:
  /** Adds the suffix class [n]_k to the container
  @param k length of the suffix
  @param n exponent of one representative of the class
  */
  void insert(unsigned long n, unsigned long k);

  /** Adds the suffix class [n]_k to the container
  @param k length of the suffix
  @param n exponent of one representative of the class
  */
  void insert(const Integer& n, const Integer& k);

  /** Adds the suffix class [n]_k to the container
  @param k length of the suffix
  @param n exponent of one representative of the class
  @return true iff N is in any suffix class [r]_k that was previously added
  */
  [[nodiscard]] auto containsN(unsigned long n) -> bool;

  /** Test if this set contains n
  @param k length of the suffix
  @param n exponent of one representative of the class
  @return true iff n is in any suffix class [r]_k that was previously added
  */
  auto containsN(const Integer& n) -> bool;

  /**
  @return the maximal k of all classes [r]_k that are in this set
  */
  [[nodiscard]] auto getMaxK() const -> unsigned long { return maxK; };

  /**
  @return an equivalent representation of this set in form of a vector
  */
  [[nodiscard]] auto toVector() const -> std::vector<SuffixClass>;

 private:
  struct rk_hash {
    inline auto operator()(const std::pair<unsigned long, unsigned long>& p) const -> std::size_t {
      const auto SHIFT_FACTOR = 56;  // k in [1..256] = [1..2^8] -> 2^56 * k < 2^64
      return (std::size_t)(p.first ^ (p.second << SHIFT_FACTOR));
    }
  };

  ska::flat_hash_set<std::pair<unsigned long, unsigned long>, rk_hash> setRK;
  unsigned long maxK = 0;

  // Computes r = n mod cycleLen(k), takes care of wrapping and unwrapping
  static auto modCk(unsigned long n, unsigned long k) -> unsigned long;
};

#endif  // SUFFIXSET_H
