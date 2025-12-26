#pragma once

#include <gtest/gtest.h>
#include <gmpxx.h>
#include "ExtraOperators.hpp"
#include "PowersWithoutPowersWorklist.hpp"
#ifdef _OPENMP
#include <omp.h>
#endif

TEST(PowersWithoutPowersWorklistTest, Constructor) {
  PowersWithoutPowersWorklist pwp = PowersWithoutPowersWorklist();
  EXPECT_TRUE(true);  // Constructor succeeded
}

TEST(PowersWithoutPowersWorklistTest, Run1_Matches) {
  PowersWithoutPowersWorklist pwp = PowersWithoutPowersWorklist();
  auto m = pwp.run(1);
  EXPECT_TRUE(m.empty());
}

TEST(PowersWithoutPowersWorklistTest, Run1_Worklist) {
  PowersWithoutPowersWorklist pwp = PowersWithoutPowersWorklist();
  pwp.run(1);
  EXPECT_TRUE(pwp.worklist[0].empty());
  EXPECT_EQ(pwp.worklist[1], std::vector<unsigned long>({8, 12, 16, 20}));
  EXPECT_TRUE(pwp.worklist[2].empty());
  EXPECT_TRUE(pwp.worklist[3].empty());
}

TEST(PowersWithoutPowersWorklistTest, Run1_Unexpanded) {
  PowersWithoutPowersWorklist pwp = PowersWithoutPowersWorklist();
  pwp.run(1);
  EXPECT_TRUE(pwp.unexpanded.empty());
}

TEST(PowersWithoutPowersWorklistTest, Run2_Matches) {
  PowersWithoutPowersWorklist pwp = PowersWithoutPowersWorklist();
  auto m = pwp.run(2);
  EXPECT_EQ(m, std::vector<unsigned long>({16}));
}

TEST(PowersWithoutPowersWorklistTest, Run2_Worklist) {
  PowersWithoutPowersWorklist pwp = PowersWithoutPowersWorklist();
  pwp.run(2);
  EXPECT_TRUE(pwp.worklist[0].empty());
  EXPECT_TRUE(pwp.worklist[1].empty());
  EXPECT_EQ(pwp.worklist[2].size(), 16);
  EXPECT_EQ(pwp.worklist[3].size(), 12);
  EXPECT_EQ(pwp.worklist[4].size(), 4);
  EXPECT_EQ(pwp.worklist[5].size(), 4);
  EXPECT_EQ(pwp.worklist[6].size(), 4);
  EXPECT_EQ(pwp.worklist[7].size(), 4);
  EXPECT_EQ(pwp.worklist[8].size(), 4);
  EXPECT_EQ(pwp.worklist[9].size(), 4);
  EXPECT_EQ(pwp.worklist[10].size(), 4);
  EXPECT_EQ(pwp.worklist[11].size(), 4);
  EXPECT_EQ(pwp.worklist[12].size(), 4);
  EXPECT_EQ(pwp.worklist[13].size(), 4);
  EXPECT_EQ(pwp.worklist[14].size(), 4);
  EXPECT_EQ(pwp.worklist[15].size(), 4);
  EXPECT_TRUE(pwp.worklist[16].empty());
}

TEST(PowersWithoutPowersWorklistTest, Run2_Unexpanded) {
  PowersWithoutPowersWorklist pwp = PowersWithoutPowersWorklist();
  pwp.run(2);
  EXPECT_TRUE(pwp.unexpanded.empty());
}

#ifdef _OPENMP
TEST(PowersWithoutPowersWorklistTest, RunParallel) {
  PowersWithoutPowersWorklist pwp_serial;
  pwp_serial.run(10);

  for (int n : {1, 2, 4, 8, 12, 48}) {
    omp_set_num_threads(n);
    
    PowersWithoutPowersWorklist pwp2;
    auto m = pwp2.runParallel(10);
    
    // Check matches
    EXPECT_EQ(m, std::vector<unsigned long>({16})) << "Failed with " << n << " threads";
    
    // Check worklist sizes
    EXPECT_EQ(pwp_serial.worklist[10].size(), pwp2.worklist[10].size()) << "Failed with " << n << " threads";
    EXPECT_EQ(pwp_serial.worklist[11].size(), pwp2.worklist[11].size()) << "Failed with " << n << " threads";
    EXPECT_EQ(pwp_serial.worklist[12].size(), pwp2.worklist[12].size()) << "Failed with " << n << " threads";
    EXPECT_EQ(pwp_serial.worklist[13].size(), pwp2.worklist[13].size()) << "Failed with " << n << " threads";
    EXPECT_EQ(pwp_serial.worklist[14].size(), pwp2.worklist[14].size()) << "Failed with " << n << " threads";
    EXPECT_EQ(pwp_serial.worklist[15].size(), pwp2.worklist[15].size()) << "Failed with " << n << " threads";
    EXPECT_EQ(pwp_serial.worklist[16].size(), pwp2.worklist[16].size()) << "Failed with " << n << " threads";
    EXPECT_EQ(pwp_serial.worklist[17].size(), pwp2.worklist[17].size()) << "Failed with " << n << " threads";
    EXPECT_EQ(pwp_serial.worklist[18].size(), pwp2.worklist[18].size()) << "Failed with " << n << " threads";
    EXPECT_EQ(pwp_serial.worklist[19].size(), pwp2.worklist[19].size()) << "Failed with " << n << " threads";
    EXPECT_EQ(pwp_serial.worklist[20].size(), pwp2.worklist[20].size()) << "Failed with " << n << " threads";
    EXPECT_EQ(pwp_serial.worklist[21].size(), pwp2.worklist[21].size()) << "Failed with " << n << " threads";
    EXPECT_EQ(pwp_serial.worklist[22].size(), pwp2.worklist[22].size()) << "Failed with " << n << " threads";
    EXPECT_EQ(pwp_serial.worklist[23].size(), pwp2.worklist[23].size()) << "Failed with " << n << " threads";
    EXPECT_EQ(pwp_serial.worklist[24].size(), pwp2.worklist[24].size()) << "Failed with " << n << " threads";
    EXPECT_EQ(pwp_serial.worklist[25].size(), pwp2.worklist[25].size()) << "Failed with " << n << " threads";
    EXPECT_EQ(pwp_serial.worklist[26].size(), pwp2.worklist[26].size()) << "Failed with " << n << " threads";
    
    // Check unexpanded
    EXPECT_EQ(pwp_serial.unexpanded.size(), pwp2.unexpanded.size()) << "Failed with " << n << " threads";
  }
}
#endif  //_OPENMP
