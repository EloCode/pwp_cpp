#pragma once

#include <iostream>
#include <gmpxx.h>

using Integer=mpz_class;

/** Represents Suffix Class [r]_k
Container-like class containing all exponents n whose suffix of length k is identical. Powers are based 2, suffix = 2^n mod 10^k
*/
class SuffixClass {
private:
    Integer _k   = 0; // length of suffix
    Integer _min = 0; // start of the cycle of the suffix length k
    Integer _mod = 0; // length of the cycle of the suffix length k
    Integer _max = 0; // end of the cycle of the suffix length k
    Integer _res = 0; // residuum such that 2^n % _mod = _res
    Integer _n   = 0; // (not yet smallest) representative of the class
public:
    SuffixClass() {};

    /** Represents the Suffix [n]_k whith n in [n]_k
    @param k length of the suffix
    @param n exponent of one representative of the class [n]_k
    @param r residuum of the class [r]_k, r may not be in [r]_k
    @param isN true iff nORr is actually an n that is in [n]_k
    */
    SuffixClass(const Integer& k, const Integer& nORr, bool isN = true);

    const Integer getSuffixLen();
    const Integer getCycleLen();
    const Integer getRepresentativeN();

    bool contains(const Integer& n);

    friend bool operator==(const SuffixClass& lhs, const SuffixClass& rhs){ return lhs._k == rhs._k and lhs._res == rhs._res; };
    friend bool operator!=(const SuffixClass& lhs, const SuffixClass& rhs){ return not (lhs == rhs); };
    friend bool operator<=(const SuffixClass& lhs, const SuffixClass& rhs){ return lhs._k <= rhs._k or  lhs._res <= rhs._res; }; // Sorts for k first then for r
    friend bool operator<(const SuffixClass& lhs, const SuffixClass& rhs){ return lhs._k < rhs._k or  lhs._res < rhs._res; }; // Sorts for k first then for r
    friend std::ostream& operator<<(std::ostream& os, const SuffixClass& obj);
    friend std::istream& operator>>(std::istream& is, SuffixClass& obj);
};
