#include "config.hpp"

#include <iostream>
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


using namespace std;
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

int main(int argc, char **argv)
{
    if(argc > 1) {
        if(strcmp(argv[1],"bench") == 0) {
            #ifdef _OPENMP
            if(argc == 4) {
                printWelcome();
                PowersWithoutPowersWorklist::timeTableRunParallel((unsigned long)atoi(argv[3]));
                PowersWithoutPowersWorklist::speedUpTable((unsigned long)atoi(argv[2]), (unsigned long)atoi(argv[3]));
            } else if(argc == 3) {
                printWelcome();
                PowersWithoutPowersWorklist::timeTableRunParallel();
                PowersWithoutPowersWorklist::speedUpTable((unsigned long)atoi(argv[2]));
            } if(argc == 2) {
                printWelcome();
                PowersWithoutPowersWorklist::timeTableRunParallel();
                PowersWithoutPowersWorklist::speedUpTable();
            } else {
                printHelp();
            }
            #else
            printHelp();
            #endif //_OPENMP
        } else if(strcmp(argv[1],"run") == 0) {

            #ifdef _OPENMP
            auto runP = [argv](){
                printWelcome();
                unsigned long max_level=(unsigned long)atoi(argv[2]);
                PowersWithoutPowersWorklist pwp;
                cout << "Run parallel, max_level=" << max_level << ", base=" << pwp.base << ", S=" << pwp.getForbiddenDigits()<< endl;
                pwp.runParallel(max_level);
                #ifdef STATS
                pwp.printStatistics(max_level);
                #endif // STATS
            };
            #endif //_OPENMP

            auto runS = [argv](){
                printWelcome();
                unsigned long max_level=(unsigned long)atoi(argv[2]);
                PowersWithoutPowersWorklist pwp;
                cout << "Run serial, max_level=" << max_level << ", base=" << pwp.base << ", S=" << pwp.getForbiddenDigits()<< endl;
                pwp.run(max_level);
                #ifdef STATS
                pwp.printStatistics(max_level);
                #endif // STATS
            };

            if (argc == 3) {
                #ifdef _OPENMP
                runP();
                #else
                runS();
                #endif //_OPENMP
            }

            if (argc == 4) {
                #ifdef _OPENMP
                if(strcmp(argv[3],"-p") == 0) {
                    runP();
                } else if (strcmp(argv[3],"-s") == 0) {
                    runS();
                } else {
                    printHelp();
                }
                #else
                printHelp();
                #endif //_OPENMP
            }
        } else if(strcmp(argv[1],"runPWS") == 0) {
            if (argc >= 2) {
                printWelcome();
                unsigned long max_level=(unsigned long)atoi(argv[2]);
                PowersWithoutPowersWorklist pwp;
                pwp.base = atoi(argv[3]);
                vector<int> forbidden;
                char * d = argv[4];
                while(*d != 0) {
                    forbidden.push_back((int)(*d -'0'));
                    d++;
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
