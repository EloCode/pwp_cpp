#pragma once

#include <limits>
#include <vector>
#include "SuffixClass.hpp"
#include "SuffixMath.hpp"
#include "SuffixSet.hpp"

class PowersWithoutPowersFinder {
 public:
  /**
   * \return Returns a vector of all matches that could be found so far
   */
  auto Getmatches() -> std::vector<unsigned long> { return matches; };

  /**
   * \return Returns a vector of all forbidden suffix classes that could be
   * found so far
   */
  auto GetforbiddenClasses() -> std::vector<SuffixClass>;

  /**
   * \return Returns the biggest exponent that has been tested so far
   */
  auto GetmaxTestedN() -> unsigned long { return maxTestedN; };

  /** Finds all matches in the range [n_start, n_end)
   *  UNSAVE, ONLY CALL IF MATCHES AND FORBIDDENCLASSES SETUP PROPERLY.
   * \param n_start > 0! smallest  exponent to be tested
   * \param n_end biggest exponent to be tested is n_end - 1
   * \return Returns all matches in that range
   */
  auto findInRange(unsigned long n_start, unsigned long n_end)
      -> std::vector<unsigned long>;
  // std::vector<Integer> findIn(Abstrakter Container oder Iterator); // Sketch

  /** Finds all matches in the range [1, n_end], starts searching at maxTestedN
   * + 1 \param n_end biggest exponent to be tested is n_end-1 \return Returns
   * all matches in that range
   */
  inline auto findUntil(unsigned long n_end) -> std::vector<unsigned long> {
    return findInRange(maxTestedN + 1, n_end);
  };

  /** Finds all matches in the range [1, C(k)], with smallest k such that C(k) >
   * maxTestedN. Starts searching at maxTestedN + 1 \return Returns all matches
   * in that range
   */
  inline auto findUntilNextCycleEnd() -> std::vector<unsigned long> {
    return findUntil(SuffixMath::nextCompletedCycleN(maxTestedN));
  };

  /**
   * \return Returns true iff the finiteness can be proven with the computed
   * information (forbiddenClasses)
   */
  auto finitenessProvable() -> bool;

  /** Prints a small benchmark table
   * \param maxN upper bound for N for the benchmark
   */
  static void timeTable(unsigned long maxN = 100000000);

 protected:
 private:
  std::vector<ulong> matches;  //!< A match is an exponent n such that 2^n
                               //!< contains no 1,2,4 or 8 as digit
  SuffixSet forbiddenClasses;  //!< Member variable "forbiddenClasses"
  unsigned long maxTestedN =
      0;  //!< Maximal exponent that has been tested, all matches and forbidden
          //!< classes are up to date
};
