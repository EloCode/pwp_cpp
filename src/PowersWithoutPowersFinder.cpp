#include "PowersWithoutPowersFinder.hpp"

#include <algorithm>
#include <iostream>
#include <limits>
#include "Stats.hpp"

using namespace std;

auto PowersWithoutPowersFinder::findInRange(unsigned long n_start, unsigned long n_end) -> vector<unsigned long> {
  Integer n_I, k_I;
  unsigned long k;
  if (n_start % 4 != 0)
    n_start += 4 - n_start % 4;
  for (unsigned long n = n_start; n < n_end; n += 4) {
    if (forbiddenClasses.containsN(n))
      continue;
    n_I = n;
    k_I = SuffixMath::powContainsPows(2, n_I);
    Stats::updateK(k_I);
    // cout << Stats::maxK.get_str() << "\n";
    if (k_I == 0)
      matches.push_back(n);
    else {
      if (k_I.fits_ulong_p()) {
        k = k_I.get_ui();
        forbiddenClasses.insert(n, k);
      }  // else Insert n_I,k_I not yet implemented
    }
  }

  maxTestedN = n_end - 1;

  return Getmatches();
}

auto PowersWithoutPowersFinder::GetforbiddenClasses() -> vector<SuffixClass> {
  vector<SuffixClass> v = forbiddenClasses.toVector();
  v.push_back(SuffixClass(1, 1));
  v.push_back(SuffixClass(1, 2));
  v.push_back(SuffixClass(1, 3));
  return v;
}

auto PowersWithoutPowersFinder::finitenessProvable() -> bool {
  vector<unsigned long>::iterator matchIt;
  matchIt = max_element(matches.begin(), matches.end());
  Integer maxMatch = matches[distance(matches.begin(), matchIt)];
  return maxMatch < SuffixMath::cycleStart(SuffixMath::maxCompletedCycleK(maxTestedN)) and
         forbiddenClasses.getMaxK() <= SuffixMath::maxCompletedCycleK(maxTestedN);
}

void PowersWithoutPowersFinder::timeTable(unsigned long maxN) {
  PowersWithoutPowersFinder pwp = PowersWithoutPowersFinder();
  cout << "\n" << "Forbidden Classes" << "\n";
  printf("|%10s|%8s|%10s|%10s|\n", "N", "HH:MM:SS", "Matches", "Classes");
  auto t_start = time(nullptr);
  for (unsigned long n_start = 1, n_stop = 512000; n_stop < maxN; n_start = n_stop, n_stop *= 2) {
    pwp.findInRange(n_start, n_stop);
    auto t_end = time(nullptr);
    auto s = (int)(t_end - t_start);
    auto h = s / 3600;
    s %= 3600;
    auto m = s / 60;
    s %= 60;
    auto numMatches = pwp.Getmatches().size();
    auto numClasses = pwp.GetforbiddenClasses().size();
    printf("|%10lu|%2i:%2i:%2i|%10lu|%10lu|\n", n_stop, h, m, s, numMatches, numClasses);
  }
  cout << "\n";
}
