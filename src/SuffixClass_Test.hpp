#pragma once

#include <gtest/gtest.h>
#include <sstream>
#include "SuffixClass.hpp"

TEST(SuffixClassTest, Constructor) {
  SuffixClass endsWithTwo = SuffixClass(1, 1);
  EXPECT_TRUE(true);  // Constructor succeeded
}

TEST(SuffixClassTest, Contains) {
  SuffixClass endsWithTwo = SuffixClass(1, 1);
  EXPECT_TRUE(endsWithTwo.contains(1));
  EXPECT_TRUE(endsWithTwo.contains(5));
  EXPECT_TRUE(endsWithTwo.contains(9));
  EXPECT_TRUE(endsWithTwo.contains(1001));
  EXPECT_FALSE(endsWithTwo.contains(1002));
  EXPECT_FALSE(endsWithTwo.contains(4));
}

TEST(SuffixClassTest, GetSuffixLen) {
  SuffixClass endsWithTwo = SuffixClass(1, 1);
  EXPECT_EQ(endsWithTwo.getSuffixLen(), 1);
}

TEST(SuffixClassTest, GetCycleLen) {
  SuffixClass endsWithTwo = SuffixClass(1, 1);
  EXPECT_EQ(endsWithTwo.getCycleLen(), 4);
}

TEST(SuffixClassTest, GetRepresentativeN) {
  SuffixClass endsWithTwo = SuffixClass(1, 1);
  EXPECT_EQ(endsWithTwo.getRepresentativeN(), 1);
}

TEST(SuffixClassTest, OperatorEqual) {
  EXPECT_EQ(SuffixClass(4_mpz, 20_mpz), SuffixClass(4_mpz, 20_mpz));
}

TEST(SuffixClassTest, OperatorNotEqual) {
  EXPECT_NE(SuffixClass(4_mpz, 20_mpz), SuffixClass(3_mpz, 20_mpz));
  // For k=4: cycleLen=500, cycleEnd=504, so 21 mod 500 = 21, which is != 20 mod 500
  EXPECT_NE(SuffixClass(4_mpz, 20_mpz), SuffixClass(4_mpz, 21_mpz));
}

TEST(SuffixClassTest, StreamOperators) {
  SuffixClass cmp = SuffixClass(4_mpz, 20_mpz);
  SuffixClass read;
  std::stringstream sstr;
  sstr << "20 (mod 500 )(k 4 )";
  sstr >> read;
  EXPECT_EQ(cmp, read);
}
