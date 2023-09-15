#pragma once

#include <gmp.h>
#include <gmpxx.h>
#include <array>
#include <vector>

using Integer = mpz_class;

const int NUM_DIGITS_I = 10;
const std::size_t NUM_DIGITS_ST = 10;

const unsigned long K_MAX_FOR_ULONG = 27;

namespace SuffixMath {

const std::array<const unsigned long, K_MAX_FOR_ULONG> Len_ulong = {4,
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

inline auto cycleStart_I(unsigned long k) -> Integer {
  return {k};
}
auto cycleLen_I(unsigned long k) -> Integer;
inline auto cycleEnd_I(unsigned long k) -> Integer {
  return cycleStart_I(k) + cycleLen_I(k);
}
inline auto cycleLast_I(unsigned long k) -> Integer {
  return cycleStart_I(k) + cycleLen_I(k) - 1;
}

inline auto cycleStart(unsigned long k) -> unsigned long {
  return k;
}
inline auto cycleLen(unsigned long k) -> unsigned long {
  return Len_ulong[k - 1];
}
inline auto cycleEnd(unsigned long k) -> unsigned long {
  return cycleStart(k) + cycleLen(k);
}
inline auto cycleLast(unsigned long k) -> unsigned long {
  return cycleStart(k) + cycleLen(k) - 1;
}

inline auto cycleStartOverflow(unsigned long /*unused*/) -> bool {
  return false;
}
inline auto cycleLenOverflow(unsigned long k) -> bool {
  return k > K_MAX_FOR_ULONG;
}
inline auto cycleEndOverflow(unsigned long k) -> bool {
  return k > K_MAX_FOR_ULONG;
}
inline auto cycleLastOverflow(unsigned long k) -> bool {
  return k > K_MAX_FOR_ULONG;
}

/**
@return biggest k such that cycleLast(k) <= n
*/
auto maxCompletedCycleK(unsigned long n) -> unsigned long;

/**
@return biggest integer n_k <= n where n_k is the end of a cycle
*/
auto maxCompletedCycleN(unsigned long n) -> unsigned long;

/**
@return smallest integer n_k > n where n_k is the end of a cycle
*/
auto nextCompletedCycleN(unsigned long n) -> unsigned long;

/**
@param digits The list of forbidden digits
@param b base
@param n exponent
@return the smallest index k where a forbidden digit was found, 0 if b^n does
not contain any forbidden digits
*/
auto powContains(const std::vector<int>& digits, const Integer& b, const Integer& n) -> Integer;

/**
@param forbidden mask for forbidden digits s.t. forbidden[i] == true means i is
a forbidden digit
@param b base
@param n exponent
@return the smallest index k where a forbidden digit was found, 0 if b^n does
not contain any forbidden digits
*/
auto powContains(const std::array<bool, NUM_DIGITS_ST>& forbidden, const Integer& b, const Integer& n) -> Integer;

/** forbidden digits are all powers of b that are smaller than 10
@param b base
@param n exponent
@return the smallest index k where a forbidden digit was found, 0 if b^n does
not contain any forbidden digits
*/
auto powContainsPows(const Integer& b, const Integer& n) -> Integer;

}  // namespace SuffixMath
