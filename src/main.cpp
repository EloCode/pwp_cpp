#include "config.hpp"

#include <iostream>
#include <cstdlib> 
#include <gmpxx.h>
#ifdef _OPENMP
   #include <omp.h>
#endif

#ifdef TEST
#include "SuffixMath_Test.hpp"
#include "SuffixClass_Test.hpp"
#include "SuffixSet_Test.hpp"
#include "PowersWithoutPowersFinder_Test.hpp"
#include "PowersWithoutPowersWorklist_Test.hpp"
#include "PowersWithoutPowersTasking_Test.hpp"
#endif // TEST
#ifdef BENCHMARK
#include <ctime>
#include "PowersWithoutPowersFinder.hpp"
#include "SuffixMath.hpp"
#include "Stats.hpp"
#endif // BENCHMARK
#include "PowersWithoutPowersWorklist.hpp"
#include "ExtraOperators.hpp"

void printWelcome() {
    cout << " --- Powers Without Powers --- \n"
         << "Version: " << PWP_VERSION_MAJOR << "." << PWP_VERSION_MINOR << "\n"
         << "Using gmp: " << gmp_version << "\n"
         << "OMP (threads): "
         #ifdef _OPENMP
         << "Yes (" << omp_get_max_threads() << ")\n"
         #else
         << "No"
         #endif //_OPENMP
         << "\n" << endl;
}

void printHelp() {
    #ifdef _OPENMP
    cout << "Usage: pwp (bench <min> <max>|run <max> [-s|-p])" << endl;
    #else
    cout << "Usage: pwp run <max>" << endl;
    #endif //_OPENMP
    
}

auto bench(std::vector<std::string>& args) -> void 
{
    #ifdef _OPENMP
    if(args.size() == 4) {
        printWelcome();
        PowersWithoutPowersWorklist::timeTableRunParallel(std::stol(args.at(3)));
        PowersWithoutPowersWorklist::speedUpTable(std::stol(args.at(2)), std::stol(args.at(3)));
    } else if(args.size() == 3) {
        printWelcome();
        PowersWithoutPowersWorklist::timeTableRunParallel();
        PowersWithoutPowersWorklist::speedUpTable(std::stol(args.at(2)));
    } if(args.size() == 2) {
        printWelcome();
        PowersWithoutPowersWorklist::timeTableRunParallel();
        PowersWithoutPowersWorklist::speedUpTable();
    } else {
        printHelp();
    }
    #else
    printHelp();
    #endif //_OPENMP
}

auto run(std::vector<std::string>& args) -> void 
{
    #ifdef _OPENMP
    auto runP = [args](){
        printWelcome();
        auto max_level=std::stol(args.at(2),nullptr,0);
        PowersWithoutPowersWorklist pwp;
        cout << "Run parallel, max_level=" << max_level << ", base=" << pwp.base << ", S=" << pwp.getForbiddenDigits()<< endl;
        pwp.runParallel(max_level);
        #ifdef STATS
        pwp.printStatistics(max_level);
        #endif // STATS
    };
    #endif //_OPENMP

    auto runS = [args](){
        printWelcome();
        auto max_level=std::stol(args.at(2),nullptr,0);
        PowersWithoutPowersWorklist pwp;
        cout << "Run serial, max_level=" << max_level << ", base=" << pwp.base << ", S=" << pwp.getForbiddenDigits()<< endl;
        pwp.run(max_level);
        #ifdef STATS
        pwp.printStatistics(max_level);
        #endif // STATS
    };

    if (args.size() == 3) {
        #ifdef _OPENMP
        runP();
        #else
        runS();
        #endif //_OPENMP
    }

    if (args.size() == 4) {
        #ifdef _OPENMP
        if(args.at(3) == "-p") {
            runP();
        } else if (args.at(3) == "-s") {
            runS();
        } else {
            printHelp();
        }
        #else
        printHelp();
        #endif //_OPENMP
    }
}

auto runPWS(std::vector<std::string>& args) -> void
{
    if (args.size() == 5) {
        printWelcome();
        auto max_level=std::stol(args.at(2),nullptr,0);
        PowersWithoutPowersWorklist pwp;
        pwp.base = std::stol(args.at(3),nullptr,0);;
        std::vector<int> forbidden;
        for (auto digit : args.at(4)) {
            forbidden.push_back(digit - '0');
        }
        pwp.setForbiddenDigits(forbidden);

        cout << "Run serial, max_level=" << max_level << ", base=" << pwp.base << ", S=" << pwp.getForbiddenDigits()<< endl;
        pwp.run(max_level);
        #ifdef STATS
        pwp.printStatistics(max_level);
        #endif // STATS
    } else {
        printHelp();
    }
}

auto main(int argc, char **argv) -> int
{
    std::vector<std::string> args(argv, argv + argc);
    if(args.size() > 1) {
        if(args.at(1) == "bench") {
            bench(args);
        } else if(args.at(1) == "run") {
            run(args);
        } else if(args.at(1) == "runPWS") {
            runPWS(args);
        } else {
            printHelp();
        }
    } else {
        printHelp();
    }





    #ifdef TEST
    //SuffixMath_Test::Test();
    //SuffixClass_Test::Test();
    //SuffixSet_Test::Test();
    //PowersWithoutPowersFinder_Test::Test();
    PowersWithoutPowersWorklist_Test::Test();
    //PowersWithoutPowersTasking_Test::Test();
    #endif // TEST
    #ifdef BENCHMARK
    //PowersWithoutPowersFinder::timeTable();
    //PowersWithoutPowersWorklist::timeTableRun();
    //PowersWithoutPowersWorklist::timeTableRunParallel();
    //PowersWithoutPowersWorklist::speedUpTable(12,15);
    #endif // BENCHMARK
    #ifdef RUN
    //PowersWithoutPowersWorklist pwp;
    //auto  v = pwp.runParallel(14);
    #ifdef STATS
    //pwp.printStatistics();
    #endif // STATS
    #endif // RUN
    return 0;
}
