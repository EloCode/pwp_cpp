#include "SuffixSet.hpp"

#include "SuffixMath.hpp"
#include <algorithm>
#include <stdexcept>

SuffixSet::SuffixSet()
{
}
SuffixSet::~SuffixSet()
{
}

unsigned long SuffixSet::modCk(unsigned long n, unsigned long k) {
    if (SuffixMath::cycleLenOverflow(k))
        return n;
    else
        return n % SuffixMath::cycleLen(k);
}

void SuffixSet::insert(unsigned long n, unsigned long k) {
    setRK.insert(std::make_pair(modCk(n,k),k));
    if (k > maxK)
        maxK = k;
}
void SuffixSet::insert(Integer n, Integer k) {
    if(n.fits_ulong_p() && k.fits_ulong_p())
        insert(n.get_ui(),k.get_ui());
    else
        throw std::domain_error("Not yet implemented for n or k => 2^64");
}

bool SuffixSet::containsN(unsigned long n) {
    for(unsigned long k = 1; k <= std::min(maxK,n); k++) {
        unsigned long m = modCk(n,k);
        auto i = setRK.find(std::make_pair(m,k));
        if(i != setRK.end())
            return true;
    }
    return false;
}
bool SuffixSet::containsN(Integer n) {
    if(n.fits_ulong_p())
        return containsN(n.get_ui());
    else
        throw std::domain_error("Not yet implemented for n => 2^64");
}

std::vector<SuffixClass> SuffixSet::toVector() {
    Integer k_I, r_I;
    std::vector<SuffixClass> res;
    for(auto p: setRK) {
        r_I = std::get<0>(p);
        k_I = std::get<1>(p);
        res.push_back(SuffixClass(k_I,r_I,false));
    }
    return res;
}
