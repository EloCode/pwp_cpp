#include "SuffixClass.hpp"

#include "config.hpp"

#include "SuffixMath.hpp"
#include <stdexcept>
#include <iostream>

using namespace std;

SuffixClass::SuffixClass(const Integer& k, const Integer& nORr, bool isN) {
    if(not k.fits_ulong_p())
        throw std::domain_error("Suffix of length k > 2^64 not supported");
    _k   = k;
    _min = SuffixMath::cycleStart_I(k.get_ui());
    _mod = SuffixMath::cycleLen_I(k.get_ui());
    _max = SuffixMath::cycleEnd_I(k.get_ui());

    if(isN) {
        _n   = nORr;
        _res = _n % _mod;
    } else {
        _res = nORr;
        _n = _res<_min ? _res+_mod : _res;
    }
    if (_n < _min) {
        throw std::domain_error("Suffix of length k has not started at exponent n");
    }
    #ifndef TEST
    if (_n >= _max) {
        throw std::domain_error("n is not the smallest representative of this class");
    }
    #endif
}

const Integer SuffixClass::getSuffixLen() {
    return _k; //copy _k
}

const Integer SuffixClass::getCycleLen() {
    return _mod; //copy _mod
}

const Integer SuffixClass::getRepresentativeN() {
    return _res; //copy _res
}

bool SuffixClass::contains(const Integer& n) {
    //Integer r = n % _mod;
    //cout << "n " << n.get_ui() << " === " << r.get_ui() << ", _res " << _res.get_ui() << " - Return " << (n >= _min and n % _mod == _res) << endl;
    return n >= _min and n % _mod == _res;
}

std::ostream& operator<<(std::ostream& os, const SuffixClass& obj) {
    os << obj._res << " (mod " << obj._mod << " )(k " << obj._k << " )";
    return os;
}

std::istream& operator>>(std::istream& is, SuffixClass& obj)
{
    string tmp;
    is >> tmp; // _res
    obj._res = Integer (tmp.c_str());
    is >> tmp; // (mod
    is >> tmp; // _mod
    obj._mod = Integer (tmp.c_str());
    is >> tmp; // )(k
    is >> tmp; // _k
    obj._k = Integer (tmp.c_str());
    is >> tmp; // )
    obj._min = SuffixMath::cycleStart(obj._k.get_ui());

    if ( (not obj._k.fits_ulong_p()) or (obj._res < obj._min) or (obj._mod != SuffixMath::cycleLen(obj._k.get_ui())) )
        is.setstate(std::istream::failbit);
    return is;
}
