#include "SuffixSet.hpp"

#include <algorithm>
#include <stdexcept>
#include "SuffixMath.hpp"

auto SuffixSet::modCk(unsigned long n, unsigned long k) -> unsigned long {
  if (SuffixMath::cycleLenOverflow(k)) {
    return n;
  }
  return n % SuffixMath::cycleLen(k);
}

void SuffixSet::insert(unsigned long n, unsigned long k) {
  setRK.insert(std::make_pair(modCk(n, k), k));
  if (k > maxK) {
    maxK = k;
  }
}
void SuffixSet::insert(const Integer& n, const Integer& k) {
  if (n.fits_ulong_p() && k.fits_ulong_p()) {
    insert(n.get_ui(), k.get_ui());
  } else {
    throw std::domain_error("Not yet implemented for n or k => 2^64");
  }
}

auto SuffixSet::containsN(unsigned long n) -> bool {
  for (unsigned long k = 1; k <= std::min(maxK, n); k++) {
    unsigned long m = modCk(n, k);
    auto i = setRK.find(std::make_pair(m, k));
    if (i != setRK.end()) {
      return true;
    }
  }
  return false;
}
auto SuffixSet::containsN(const Integer& n) -> bool {
  if (n.fits_ulong_p()) {
    return containsN(n.get_ui());
  }
  throw std::domain_error("Not yet implemented for n => 2^64");
}

auto SuffixSet::toVector() const -> std::vector<SuffixClass> {
  Integer k_I;
  Integer r_I;
  std::vector<SuffixClass> res;
  for (auto p : setRK) {
    r_I = std::get<0>(p);
    k_I = std::get<1>(p);
    res.emplace_back(k_I, r_I, false);
  }
  return res;
}
