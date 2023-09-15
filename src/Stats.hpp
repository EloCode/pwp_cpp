#pragma once

#include <gmpxx.h>
#include "config.hpp"

using Integer = mpz_class;
#include <iostream>
struct Stats {
#ifdef STATS
  static Integer maxK;
  static Integer maxLen;
  static Integer maxMod;
#endif

  inline static void updateK([[maybe_unused]] const Integer& k) {
#ifdef STATS
    if (k > maxK)
      maxK = k;
#endif
  };
  inline static void updateLen([[maybe_unused]] const Integer& l) {
#ifdef STATS
    if (l > maxLen)
      maxLen = l;
#endif
  };
  inline static void updateMod([[maybe_unused]] const Integer& m) {
#ifdef STATS
    if (m > maxMod)
      maxMod = m;
#endif
  };
};
