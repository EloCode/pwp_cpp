#pragma once

#include "config.hpp"
#include <gmpxx.h>

using Integer=mpz_class;
#include <iostream>
struct Stats   {
    static Integer maxK;
    static Integer maxLen;
    static Integer maxMod;

    inline static void updateK  (const Integer& k) {
        #ifdef STATS
        if (k > maxK)
            maxK = k;
        #endif
    };
    inline static void updateLen(const Integer& l) {
        #ifdef STATS
        if (l > maxLen)
            maxLen = l;
        #endif
    };
    inline static void updateMod(const Integer& m) {
        #ifdef STATS
        if (m > maxMod)
            maxMod = m;
        #endif
    };
};
