#pragma once

#include <gtest/gtest.h>
#include <array>
#include "SuffixSet.hpp"

TEST(SuffixSetTest, Constructor) {
  SuffixSet s = SuffixSet();
  EXPECT_TRUE(true);  // Constructor succeeded
}

TEST(SuffixSetTest, InsertAndContains) {
  SuffixSet s = SuffixSet();
  s.insert(1, 1);
  s.insert(2, 2);
  s.insert(3, 3);

  const unsigned long TEST_N_MAX = 30;
  const std::array<bool, TEST_N_MAX> member = {false, true,  true,  true,  false, true,  false, false, false, true,
                                               false, false, false, true,  false, false, false, true,  false, false,
                                               false, true,  true,  false, false, true,  false, false, false, true};
  
  for (unsigned long n = 0; n < TEST_N_MAX; n++) {
    EXPECT_EQ(s.containsN(n), member[n]) << "Failed at n=" << n;
  }
}
