#pragma once

#include <gtest/gtest.h>
#include <gmpxx.h>
#include "ExtraOperators.hpp"
#include "PowersWithoutPowersFinder.hpp"

TEST(PowersWithoutPowersFinderTest, Constructor) {
  PowersWithoutPowersFinder pwp = PowersWithoutPowersFinder();
  EXPECT_TRUE(true);  // Constructor succeeded
}

TEST(PowersWithoutPowersFinderTest, FindInRangeAndUntilNextCycleEnd) {
  PowersWithoutPowersFinder pwp = PowersWithoutPowersFinder();
  pwp.findInRange(1, 6);
  pwp.findUntilNextCycleEnd();
  std::vector<unsigned long> matches = pwp.Getmatches();
  std::vector<SuffixClass> forbidden = pwp.GetforbiddenClasses();
  std::sort(forbidden.begin(), forbidden.end());
  
  std::vector<unsigned long> matchesCorrect = {16};
  std::vector<SuffixClass> forbiddenCorrect = {SuffixClass(1_mpz, 1_mpz), SuffixClass(1_mpz, 2_mpz), SuffixClass(1_mpz, 3_mpz),
                                               SuffixClass(2_mpz, 4_mpz), SuffixClass(3_mpz, 8_mpz), SuffixClass(4_mpz, 12_mpz),
                                               SuffixClass(4_mpz, 20_mpz)};
  
  EXPECT_EQ(matches, matchesCorrect);
  EXPECT_EQ(forbidden, forbiddenCorrect);
}

TEST(PowersWithoutPowersFinderTest, FinitenessProvable) {
  PowersWithoutPowersFinder pwp = PowersWithoutPowersFinder();
  pwp.findInRange(1, 6);
  pwp.findUntilNextCycleEnd();
  EXPECT_FALSE(pwp.finitenessProvable());
}
