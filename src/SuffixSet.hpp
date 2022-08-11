#ifndef SUFFIXSET_H
#define SUFFIXSET_H

#include <gmpxx.h>
#include "flat_hash_map-master/flat_hash_map.hpp"
#include <utility>       // std::pair
#include <vector>
#include "SuffixClass.hpp"

using Integer=mpz_class;

/** Set containing suffix classes [r]_k with fast insert and lookup methods.
*/
class SuffixSet
{
    public:
        SuffixSet();
        ~SuffixSet();

        /** Adds the suffix class [n]_k to the container
        @param k length of the suffix
        @param n exponent of one representative of the class
        */
        void insert(unsigned long n, unsigned long k);

        /** Adds the suffix class [n]_k to the container
        @param k length of the suffix
        @param n exponent of one representative of the class
        */
        void insert(Integer n, Integer k);

        /** Adds the suffix class [n]_k to the container
        @param k length of the suffix
        @param n exponent of one representative of the class
        @return true iff N is in any suffix class [r]_k that was previously added
        */
        bool containsN(unsigned long n);

        /** Test if this set contains n
        @param k length of the suffix
        @param n exponent of one representative of the class
        @return true iff n is in any suffix class [r]_k that was previously added
        */
        bool containsN(Integer n);

        /**
        @return the maximal k of all classes [r]_k that are in this set
        */
        unsigned long getMaxK() { return maxK; };

        /**
        @return an equivalent representation of this set in form of a vector
        */
        std::vector<SuffixClass> toVector();

    private:
        struct rk_hash {
            inline std::size_t operator()(const std::pair<unsigned long, unsigned long> & p) const {
                return (std::size_t) (p.first ^ (p.second << 56)); // k in [1..256] = [1..2^8] -> 2^56 * k < 2^64
            }
        };

        ska::flat_hash_set<std::pair<unsigned long, unsigned long>,rk_hash> setRK;
        unsigned long maxK = 0;
        // Preventing reallocation by providing
        mpz_t TWO; // constant base used for operations like powMod
        mpz_t op1; // buffers for the arguments of operations like powMod
        mpz_t op2; // buffers for the arguments of operations like powMod
        mpz_t rop; // buffers for the results of operations like powMod
        Integer i1; // buffer for arguments of operations like cycleLen
        Integer i2; // buffer for arguments of operations like cycleLen
        Integer i3; // buffer for arguments of operations like cycleLen

        // Computes r = n mod cycleLen(k), takes care of wrapping and unwrapping
        unsigned long modCk(unsigned long n, unsigned long r);
};

#endif // SUFFIXSET_H
