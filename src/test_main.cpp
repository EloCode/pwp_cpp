#include <gtest/gtest.h>

// Include all test files
#include "SuffixMath_Test.hpp"
#include "SuffixClass_Test.hpp"
#include "SuffixSet_Test.hpp"
#include "PowersWithoutPowersFinder_Test.hpp"
#include "PowersWithoutPowersWorklist_Test.hpp"

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
