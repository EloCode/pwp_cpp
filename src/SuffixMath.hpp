#pragma once

#include <gmp.h>
#include <gmpxx.h>
#include <array>
#include <vector>

using Integer = mpz_class;
#define K_MAX_FOR_ULONG 27

namespace SuffixMath {
const Integer ZERO = 0;
const Integer ONE = 1;
const Integer TWO = 2;
const Integer FOUR = 4;
const Integer FIVE = 5;

const std::array<const unsigned long, K_MAX_FOR_ULONG> Len_ulong = {
    4,
    20,
    100,
    500,
    2500,
    12500,
    62500,
    312500,
    1562500,
    7812500,
    39062500,
    195312500,
    976562500,
    4882812500,
    24414062500,
    122070312500,
    610351562500,
    3051757812500,
    15258789062500,
    76293945312500,
    381469726562500,
    1907348632812500,
    9536743164062500,
    47683715820312500,
    238418579101562500,
    1192092895507812500,
    5960464477539062500};

inline const Integer cycleStart_I(unsigned long k) {
  return Integer(k);
};
const Integer cycleLen_I(unsigned long k);
inline const Integer cycleEnd_I(unsigned long k) {
  return cycleStart_I(k) + cycleLen_I(k);
};
inline const Integer cycleLast_I(unsigned long k) {
  return cycleStart_I(k) + cycleLen_I(k) - 1;
};

inline unsigned long cycleStart(unsigned long k) {
  return k;
};
inline unsigned long cycleLen(unsigned long k) {
  return Len_ulong[k - 1];
};
inline unsigned long cycleEnd(unsigned long k) {
  return cycleStart(k) + cycleLen(k);
};
inline unsigned long cycleLast(unsigned long k) {
  return cycleStart(k) + cycleLen(k) - 1;
};

inline bool cycleStartOverflow(unsigned long k) {
  return false;
};
inline bool cycleLenOverflow(unsigned long k) {
  return k > K_MAX_FOR_ULONG;
};
inline bool cycleEndOverflow(unsigned long k) {
  return k > K_MAX_FOR_ULONG;
};
inline bool cycleLastOverflow(unsigned long k) {
  return k > K_MAX_FOR_ULONG;
};

unsigned long maxCompletedCycleK(
    unsigned long n);  //!< biggest k such that CycleLast(k) <= maxTestedN
unsigned long maxCompletedCycleN(
    unsigned long n);  //!< biggest CycleLast <= maxTestedN. maxCompletedCycleN
                       //!< = CycleLast(maxCompletedCycleK)
unsigned long nextCompletedCycleN(
    unsigned long
        n);  //!< smallest CycleLast > maxTestedN iff nextCycleFitsULong() ==
             //!< true, else undefined. nextCompletedCycleN =
             //!< CycleLast(maxCompletedCycleK+1)

/**
@param digits The list of forbidden digits
@param b base
@param n exponent
@return the smallest index k where a forbidden digit was found, 0 if b^n does
not contain any forbidden digits
*/
const Integer powContains(const std::vector<int>& digits,
                          const Integer& b,
                          const Integer& n);

/**
@param forbidden mask for forbidden digits s.t. forbidden[i] == true means i is
a forbidden digit
@param b base
@param n exponent
@return the smallest index k where a forbidden digit was found, 0 if b^n does
not contain any forbidden digits
*/
const Integer powContains(const std::array<bool, 10>& forbidden,
                          const Integer& b,
                          const Integer& n);

/** forbidden digits are all powers of b that are smaller than 10
@param b base
@param n exponent
@return the smallest index k where a forbidden digit was found, 0 if b^n does
not contain any forbidden digits
*/
const Integer powContainsPows(const Integer& b, const Integer& n);
}  // namespace SuffixMath
