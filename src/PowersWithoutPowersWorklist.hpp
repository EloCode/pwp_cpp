#pragma once

#include "SuffixMath.hpp"
#include "config.hpp"

#include <array>
#include <iostream>
#include <vector>
using std::array;
using std::vector;

class PowersWithoutPowersWorklist {
 public:
  unsigned long base = 2;  //!< The base B for the power series. Do NOT CHANGE. Not yet supported
  array<bool, NUM_DIGITS_ST> forbiddenDigits = {false, true,  true,  false, true,
                                                false, false, false, true,  false};  //!< The Set Ŝ of forbidden Digits
  unsigned int level = 1;                                  //!< The level (k) of the suffix class tree which we are
                                                           //!< currently processing
  array<vector<unsigned long>, K_MAX_FOR_ULONG> worklist;  //!< Each worklist belongs to one level
  vector<unsigned long> unexpanded;                        //!< n for which 2^n has legal suffix longer than 27 =>
                                                           //!< children > max<unsigned long>()
  vector<unsigned long> matches;                           //!< matches found during run
  PowersWithoutPowersWorklist() { worklist[0] = {1, 2, 3, 4}; };

  void resetAll();
  void setForbiddenDigits(const vector<int>& digits);
  auto getForbiddenDigits() -> vector<int>;

  /** Find all matches until tree level max_level
   * maximal exponent checked = (max_level - 1) + 4*5^(max_level - 1)
   * \param max_level 0 <= max_level <= 27, this implementation does not support
   * exploration beyond level 27
   */
  auto run(unsigned max_level) -> vector<unsigned long>;

  /** Find all matches until tree level max_level
   * maximal exponent checked = (max_level - 1) + 4*5^(max_level - 1)
   * processes tree with depth-first-search resulting in less memory consumption
   * \param max_level 0 <= max_level <= 27, this implementation does not support
   * exploration beyond level 27
   */
  auto runDFS(unsigned max_level) -> vector<unsigned long>;

#ifdef _OPENMP
  /** Find all matches until tree level max_level
   * maximal exponent checked = (max_level - 1) + 4*5^(max_level - 1)
   * uses OMP_NUM_THREADS parallel threads
   * \param max_level 0 <= max_level <= 27, this implementation does not support
   * exploration beyond level 27
   */
  auto runParallel(unsigned max_level) -> vector<unsigned long>;
#endif  //_OPENMP

  /**
   * \return true if unexpanded and matches are empty => last termination
   * happened due to empty worklist => series is finite and complete
   */
  auto isComplete() -> bool;

  static void timeTableRun(unsigned long maxK = 14);  // NOLINT
#ifdef _OPENMP
  static void timeTableRunParallel(unsigned long maxK = 14);                   // NOLINT
  static void speedUpTable(unsigned long minK = 13, unsigned long maxK = 16);  // NOLINT
#endif                                                                         //_OPENMP

#ifdef STATS
  array<unsigned, K_MAX_FOR_ULONG> max_k{};
  array<unsigned long, K_MAX_FOR_ULONG> max_k_number{};
  array<unsigned, K_MAX_FOR_ULONG> worklist_size{};
#endif  // STATS
  void printStatistics(unsigned long max_level = K_MAX_FOR_ULONG);

 private:
  inline auto getWL(unsigned long _level) -> vector<unsigned long>& { return worklist.at(_level - 1); }
  static inline auto isInWLRange(unsigned long _level) -> bool { return 0 < _level and _level <= K_MAX_FOR_ULONG; }

  /** Adds the first expansion of [r]_{k} to the end of the specified worklist
   * The nth expansion of [r]_{k} is [x | x in [r]_{k}, cycleStart (k+n) <= x <=
   * cycleLast (k+n)] \param r representative of [r]_{k}, must be r <= cycleLast
   * k \param k level of the parent \param k_child level of the child to be
   * expanded \param worklist where the children will be added
   */
  static inline void expand1(unsigned long r, unsigned long k, vector<unsigned long>& worklist);

  /** Adds the (k_child - k) expansion of [r]_{k} to the end of the specified
   * worklist The nth expansion of [r]_{k} is [x | x in [r]_{k}, cycleStart
   * (k+n) <= x <= cycleLast (k+n)] \param r representative of [r]_{k}, must be
   * r <= cycleLast k \param k level of the parent \param worklist where the
   * children will be added
   */
  static inline void expand(unsigned long r, unsigned long k, unsigned long k_child, vector<unsigned long>& worklist);
};
