#pragma once

#include "SuffixMath.hpp"
#include "SuffixSet.hpp"
#include "SuffixClass.hpp"
#include <vector>
#include <limits>

class PowersWithoutPowersFinder
{
    public:
        /** Default constructor */
        PowersWithoutPowersFinder();
        /** Default destructor */
        virtual ~PowersWithoutPowersFinder();

        /**
         * \return Returns a vector of all matches that could be found so far
         */
        std::vector<unsigned long> Getmatches() { return matches; };

        /**
         * \return Returns a vector of all forbidden suffix classes that could be found so far
         */
        std::vector<SuffixClass> GetforbiddenClasses();

        /**
         * \return Returns the biggest exponent that has been tested so far
         */
        unsigned long GetmaxTestedN() { return maxTestedN; };

        /** Finds all matches in the range [n_start, n_end)
         *  UNSAVE, ONLY CALL IF MATCHES AND FORBIDDENCLASSES SETUP PROPERLY.
         * \param n_start > 0! smallest  exponent to be tested
         * \param n_end biggest exponent to be tested is n_end - 1
         * \return Returns all matches in that range
         */
        std::vector<unsigned long> findInRange(unsigned long n_start, unsigned long n_end);
        //std::vector<Integer> findIn(Abstrakter Container oder Iterator); // Sketch

        /** Finds all matches in the range [1, n_end], starts searching at maxTestedN + 1
         * \param n_end biggest exponent to be tested is n_end-1
         * \return Returns all matches in that range
         */
        inline std::vector<unsigned long> findUntil(unsigned long n_end) { return findInRange(maxTestedN + 1, n_end); };

        /** Finds all matches in the range [1, C(k)], with smallest k such that C(k) > maxTestedN. Starts searching at maxTestedN + 1
         * \return Returns all matches in that range
         */
        inline std::vector<unsigned long> findUntilNextCycleEnd() { return findUntil(SuffixMath::nextCompletedCycleN(maxTestedN)); };

        /**
         * \return Returns true iff the finiteness can be proven with the computed information (forbiddenClasses)
         */
        bool finitenessProvable();

        /** Prints a small benchmark table
         * \param maxN upper bound for N for the benchmark
         */
        static void timeTable(unsigned long maxN=100000000);
    protected:
    private:
        std::vector<ulong> matches; //!< A match is an exponent n such that 2^n contains no 1,2,4 or 8 as digit
        SuffixSet forbiddenClasses;   //!< Member variable "forbiddenClasses"
        unsigned long maxTestedN = 0;         //!< Maximal exponent that has been tested, all matches and forbidden classes are up to date
};
