#pragma once

#include "config.hpp"
#include "SuffixMath.hpp"

#include <vector>
#include <array>
#include <iostream>
using std::vector;
using std::array;


class PowersWithoutPowersWorklist
{
    public:
        unsigned long base = 2;                                //!< The base B for the power series. Do NOT CHANGE. Not yet supported
        array<bool, 10> forbiddenDigits = {false,true,true,false,true,false,false,false,true,false};    //!< The Set Ŝ of forbidden Digits

        int level = 1;                                         //!< The level (k) of the suffix class tree which we are currently processing
        array<vector<unsigned long>,K_MAX_FOR_ULONG> worklist; //!< Each worklist belongs to one level
        vector<unsigned long> unexpanded;                      //!< n for which 2^n has legal suffix longer than 27 => children > max<ulong>()
        vector<unsigned long> matches;                         //!< matches found during run
        PowersWithoutPowersWorklist() { worklist[0] = {1,2,3,4}; };

        void resetAll();
        void setForbiddenDigits(const vector<int>& digits);
        vector<int> getForbiddenDigits();

        /** Find all matches until tree level max_level
         * maximal exponent checked = (max_level - 1) + 4*5^(max_level - 1)
         * \param max_level 0 <= max_level <= 27, this implementation does not support exploration beyond level 27
         */
        vector<unsigned long> run(unsigned max_level);

        /** Find all matches until tree level max_level
         * maximal exponent checked = (max_level - 1) + 4*5^(max_level - 1)
         * processes tree with depth-first-search resulting in less memory consumption
         * \param max_level 0 <= max_level <= 27, this implementation does not support exploration beyond level 27
         */
        vector<unsigned long> runDFS(unsigned max_level);

        #ifdef _OPENMP
        /** Find all matches until tree level max_level
         * maximal exponent checked = (max_level - 1) + 4*5^(max_level - 1)
         * uses OMP_NUM_THREADS parallel threads
         * \param max_level 0 <= max_level <= 27, this implementation does not support exploration beyond level 27
         */
        vector<unsigned long> runParallel(unsigned max_level);
        #endif //_OPENMP

        /**
         * \return true if unexpanded and matches are empty => last termination happened due to empty worklist => series is finite and complete
         */
        bool isComplete();

        static void timeTableRun(unsigned long maxK=14);
        #ifdef _OPENMP
        static void timeTableRunParallel(unsigned long maxK=14);
        static void speedUpTable(unsigned long minK = 13, unsigned long maxK=16);
        #endif //_OPENMP

        #ifdef STATS
        array<unsigned, K_MAX_FOR_ULONG> max_k{};
        array<unsigned long, K_MAX_FOR_ULONG> max_k_number{};
        array<unsigned, K_MAX_FOR_ULONG> worklist_size{};
        void printStatistics(unsigned long  max_level=K_MAX_FOR_ULONG);
        #endif // STATS
    private:
        inline vector<unsigned long>& getWL(unsigned long _level) { return worklist.at(_level-1); }
        inline bool isInWLRange(unsigned long _level) { return 0 < _level and _level <= K_MAX_FOR_ULONG; }

        /** Adds the first expansion of [r]_{k} to the end of the specified worklist
         * The nth expansion of [r]_{k} is [x | x in [r]_{k}, cycleStart (k+n) <= x <= cycleLast (k+n)]
         * \param r representative of [r]_{k}, must be r <= cycleLast k
         * \param k level of the parent
         * \param k_child level of the child to be expanded
         * \param worklist where the children will be added
         */
        inline void expand1(unsigned long r, unsigned long k, vector<unsigned long> & worklist);

        /** Adds the (k_child - k) expansion of [r]_{k} to the end of the specified worklist
         * The nth expansion of [r]_{k} is [x | x in [r]_{k}, cycleStart (k+n) <= x <= cycleLast (k+n)]
         * \param r representative of [r]_{k}, must be r <= cycleLast k
         * \param k level of the parent
         * \param worklist where the children will be added
         */
        inline void expand(unsigned long r, unsigned long k, unsigned long k_child , vector<unsigned long> & worklist);
};


