#include "ErrorCodes.hpp"
#include "config.hpp"

#include <gmpxx.h>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include "ExtraOperators.hpp"
#include "PowersWithoutPowersFinder.hpp"
#include "PowersWithoutPowersWorklist.hpp"
#include "Stats.hpp"
#include "SuffixMath.hpp"
#ifdef _OPENMP
#include <omp.h>
#endif

#ifdef TEST
#include "PowersWithoutPowersFinder_Test.hpp"
#include "PowersWithoutPowersWorklist_Test.hpp"
#include "SuffixClass_Test.hpp"
#include "SuffixMath_Test.hpp"
#include "SuffixSet_Test.hpp"
#endif  // TEST

using std::cout;

void printWelcome() {
  cout << " --- Powers Without Powers --- \n"
       << "Version: " << PWP_VERSION_MAJOR << "." << PWP_VERSION_MINOR << "\n"
       << "Using gmp: " << gmp_version << "\n"
       << "OMP (threads): "
#ifdef _OPENMP
       << "Yes (" << omp_get_max_threads() << ")\n"
#else
       << "No"
#endif  //_OPENMP
       << "\n"
       << "\n";
}

void printHelp() {
#ifdef _OPENMP
  cout << "Usage: pwp (bench <min> <max>|run <max> [-s|-p])"
       << "\n";
#else
  cout << "Usage: pwp run <max>"
       << "\n";
#endif  //_OPENMP
#pragma omp master
  std::exit(ERROR_INVALID_ARGUMENTS);
}

auto bench(std::vector<std::string>& args) -> void {
#ifdef _OPENMP
  if (args.size() == 4) {
    printWelcome();
    PowersWithoutPowersWorklist::timeTableRunParallel(std::stol(args.at(3)));
    PowersWithoutPowersWorklist::speedUpTable(std::stol(args.at(2)), std::stol(args.at(3)));
  } else if (args.size() == 3) {
    printWelcome();
    PowersWithoutPowersWorklist::timeTableRunParallel();
    PowersWithoutPowersWorklist::speedUpTable(std::stol(args.at(2)));
  } else if (args.size() == 2) {
    printWelcome();
    PowersWithoutPowersWorklist::timeTableRunParallel();
    PowersWithoutPowersWorklist::speedUpTable();
  } else {
    printHelp();
  }
#else
  printHelp();
#endif  //_OPENMP
}

auto run(std::vector<std::string>& args) -> void {
#ifdef _OPENMP
  auto runP = [args]() {
    printWelcome();
    auto max_level = std::stol(args.at(2), nullptr, 0);
    PowersWithoutPowersWorklist pwp;
    cout << "Run parallel, max_level=" << max_level << ", base=" << pwp.base << ", S=" << pwp.getForbiddenDigits() << "\n";
    pwp.runParallel(max_level);
    pwp.printStatistics(max_level);
  };
#endif  //_OPENMP

  auto runS = [args]() {
    printWelcome();
    auto max_level = std::stol(args.at(2), nullptr, 0);
    PowersWithoutPowersWorklist pwp;
    cout << "Run serial, max_level=" << max_level << ", base=" << pwp.base << ", S=" << pwp.getForbiddenDigits() << "\n";
    pwp.run(max_level);
    pwp.printStatistics(max_level);
  };

  if (args.size() == 3) {
#ifdef _OPENMP
    runP();
#else
    runS();
#endif  //_OPENMP
  }

  if (args.size() == 4) {
#ifdef _OPENMP
    if (args.at(3) == "-p") {
      runP();
    } else if (args.at(3) == "-s") {
      runS();
    } else {
      printHelp();
    }
#else
    printHelp();
#endif  //_OPENMP
  }
}

auto runPWS(std::vector<std::string>& args) -> void {
  if (args.size() == 5) {
    printWelcome();
    auto max_level = std::stol(args.at(2), nullptr, 0);
    PowersWithoutPowersWorklist pwp;
    pwp.base = std::stol(args.at(3), nullptr, 0);
    ;
    std::vector<int> forbidden;
    for (auto digit : args.at(4)) {
      forbidden.push_back(digit - '0');
    }
    pwp.setForbiddenDigits(forbidden);

    cout << "Run serial, max_level=" << max_level << ", base=" << pwp.base << ", S=" << pwp.getForbiddenDigits() << "\n";
    pwp.run(max_level);
    pwp.printStatistics(max_level);
  } else {
    printHelp();
  }
}

#ifdef TEST
auto main() -> int {
  auto success = SuffixMath_Test::Test();
  success &= SuffixClass_Test::Test();
  success &= SuffixSet_Test::Test();
  success &= PowersWithoutPowersFinder_Test::Test();
  success &= PowersWithoutPowersWorklist_Test::Test();

  cout << " === Testsuite " << (success ? "succeeded === " : "failed === ") << "\n"
       << "\n";
  return success ? SUCCESS : ERROR_PWP_TESTS_FAILED;
}
#else
auto main(int argc, char** argv) -> int {
  std::vector<std::string> args(argv, argv + argc);
  if (args.size() > 1) {
    if (args.at(1) == "bench") {
      bench(args);
    } else if (args.at(1) == "run") {
      run(args);
    } else if (args.at(1) == "runPWS") {
      runPWS(args);
    } else {
      printHelp();
    }
  } else {
    printHelp();
  }
  return SUCCESS;
}
#endif  // TEST