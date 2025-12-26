#pragma once

#include <gtest/gtest.h>
#include "SuffixMath.hpp"

// NOLINTBEGIN because of intended magic numbers
using namespace SuffixMath;

TEST(SuffixMathTest, DefiningInteger) {
  Integer x = 4;
  EXPECT_EQ(x, 4);
}

TEST(SuffixMathTest, CycleStart_I) {
  EXPECT_EQ(cycleStart_I(4), 4_mpz);
}

TEST(SuffixMathTest, CycleLen_I) {
  EXPECT_EQ(cycleLen_I(4), 500_mpz);
}

TEST(SuffixMathTest, CycleEnd_I) {
  EXPECT_EQ(cycleEnd_I(2), 22_mpz);
}

TEST(SuffixMathTest, CycleLast_I) {
  EXPECT_EQ(cycleLast_I(3), 102_mpz);
}

TEST(SuffixMathTest, CycleStart) {
  EXPECT_EQ(cycleStart(4), 4);
}

TEST(SuffixMathTest, CycleLen) {
  EXPECT_EQ(cycleLen(4), 500);
}

TEST(SuffixMathTest, CycleEnd) {
  EXPECT_EQ(cycleEnd(2), 22);
}

TEST(SuffixMathTest, CycleLast) {
  EXPECT_EQ(cycleLast_I(3), 102);
}

TEST(SuffixMathTest, IntegerPersistence) {
  Integer x = 4;
  Integer y = x;
  x = 5;
  EXPECT_EQ(y, 4);
}

TEST(SuffixMathTest, PowContainsPows_2_8) {
  EXPECT_EQ(powContainsPows(2, 8), 3);  // 2⁸=256 -> k = 3
}

TEST(SuffixMathTest, PowContainsPows_2_16) {
  EXPECT_EQ(powContainsPows(2, 16), 0);  // 2^(16) = 65536 -> k = 0
}

TEST(SuffixMathTest, PowContainsPows_2_88) {
  EXPECT_EQ(powContainsPows(2, 88), 4);  // 2^(88) = 309,485,009,821,345,068,724,781,056 -> k = 4
}

TEST(SuffixMathTest, PowContains_Vector) {
  std::vector<int> v;
  v.push_back(3);
  v.push_back(5);
  EXPECT_EQ(powContains(v, 2, 10), 0);  // 2^(10) = 1024 -> k = 0
}

TEST(SuffixMathTest, MaxCompletedCycleK) {
  EXPECT_EQ(maxCompletedCycleK(1000), 4);
}

TEST(SuffixMathTest, MaxCompletedCycleN) {
  EXPECT_EQ(maxCompletedCycleN(1000), 503);
}

TEST(SuffixMathTest, NextCompletedCycleN) {
  EXPECT_EQ(nextCompletedCycleN(1000), 2504);
}
// NOLINTEND