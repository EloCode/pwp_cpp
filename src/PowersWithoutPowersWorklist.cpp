#include "PowersWithoutPowersWorklist.hpp"

#ifdef _OPENMP
#include <omp.h>
#endif
#include <chrono>
#include <iostream>
#include "ExtraOperators.hpp"
#include "SuffixMath.hpp"

using namespace std;

void PowersWithoutPowersWorklist::resetAll() {
  for (auto& worklistForCurrentCycle : worklist) {
    worklistForCurrentCycle.clear();
  }
  unexpanded.clear();
  matches.clear();
  level = 1;
#ifdef STATS
  worklist_size.fill(0);
  max_k.fill(0);
#endif  // STATS
}

void PowersWithoutPowersWorklist::setForbiddenDigits(const vector<int>& digits) {
  for (int digit = 0; digit < NUM_DIGITS_I; digit++) {
    forbiddenDigits[digit] = false;
  }
  for (int digit : digits) {
    forbiddenDigits[digit] = true;
  }
}
auto PowersWithoutPowersWorklist::getForbiddenDigits() -> vector<int> {
  vector<int> digits;
  ;
  for (int digit = 0; digit < NUM_DIGITS_I; digit++) {
    if (forbiddenDigits[digit]) {
      digits.push_back(digit);
    }
  }
  return digits;
}

auto PowersWithoutPowersWorklist::run(unsigned max_level) -> vector<unsigned long> {
  for (; level <= max_level; level++) {
    for (unsigned long n : getWL(level)) {
      const Integer forbiddenDigitIndex = SuffixMath::powContains(forbiddenDigits, 2, n);
      if (forbiddenDigitIndex.fits_ulong_p()) {
        unsigned long k_child = forbiddenDigitIndex.get_ui();
#ifdef STATS
        if (k_child != 0 && k_child > max_k[level - 1]) {
          max_k[level - 1] = k_child - 1;
          max_k_number[level - 1] = n;
        }
#endif  // STATS
        if (k_child == 0) {
          matches.push_back(n);
          if (isInWLRange(n)) {
            for (unsigned long k_parent = level; k_parent < n; k_parent++) {
              expand1(n, k_parent, getWL(k_parent + 1));
            }
          } else {
            unexpanded.push_back(n);
          }
        } else if (isInWLRange(k_child)) {
          for (unsigned long k_parent = level; k_parent < k_child; k_parent++) {
            expand1(n, k_parent, getWL(k_parent + 1));
          }
        } else {
          unexpanded.push_back(n);
        }
      } else {
        unexpanded.push_back(n);
      }
    }
#ifdef STATS
    worklist_size[level - 1] += getWL(level).size();
#endif  // STATS
    getWL(level).clear();
  }
  return matches;
}

auto PowersWithoutPowersWorklist::runDFS(unsigned max_level) -> vector<unsigned long> {
  cout << "Initial worklist: " << worklist << "\n";
  unsigned long n;
  while (true) {
    if (level == 0 or level > max_level) {
      break;
    }
    if (level < max_level and not(getWL(level + 1).empty())) {
      level++;
      continue;
    }
    if (getWL(level).empty()) {
      level--;
      continue;
    }

    n = getWL(level).back();
    cout << "N=" << n << "\n";
    getWL(level).pop_back();
#ifdef STATS
    worklist_size[level - 1]++;
#endif  // STATS

    const Integer forbiddenDigitIndex = SuffixMath::powContainsPows(2, n);
    if (forbiddenDigitIndex.fits_ulong_p()) {
      unsigned long k_child = forbiddenDigitIndex.get_ui();
#ifdef STATS
      if (k_child != 0 && k_child > max_k[level - 1]) {
        max_k[level - 1] = k_child - 1;
        max_k_number[level - 1] = n;
      }
#endif  // STATS
      if (k_child == 0) {
        matches.push_back(n);
        if (isInWLRange(n)) {
          for (unsigned long k_parent = level; k_parent < n; k_parent++) {
            expand1(n, k_parent, getWL(k_parent + 1));
          }
        } else {
          unexpanded.push_back(n);
        }
      } else if (isInWLRange(k_child)) {
        for (unsigned long k_parent = level; k_parent < k_child; k_parent++) {
          expand1(n, k_parent, getWL(k_parent + 1));
        }
      } else {
        unexpanded.push_back(n);
      }
    } else {
      unexpanded.push_back(n);
    }
  }
  cout << "Final worklist: " << worklist << "\n";
  return matches;
}

#ifdef _OPENMP
auto PowersWithoutPowersWorklist::runParallel(unsigned max_level) -> vector<unsigned long> {
  size_t N = omp_get_max_threads();
  while (level <= max_level and getWL(level).size() < N * 10) {
    run(level + 1);
  }
  if (level == max_level) {
    return matches;
  }
#pragma omp parallel default(shared)
  {
    PowersWithoutPowersWorklist pwp_thread;
    for (size_t wl = level; wl <= max_level; wl++) {
#pragma omp for schedule(dynamic, 1) nowait
      for (auto it = getWL(wl).begin(); it < getWL(wl).end(); it++) {
        pwp_thread.getWL(wl).push_back(*it);
        pwp_thread.level = wl;
        pwp_thread.run(max_level);
      }
    }

#pragma omp critical(combineUnexpanded)
    unexpanded.insert(unexpanded.end(), pwp_thread.unexpanded.begin(), pwp_thread.unexpanded.end());
#pragma omp critical(combineMatches)
    matches.insert(matches.end(), pwp_thread.matches.begin(), pwp_thread.matches.end());

#pragma omp barrier
#pragma omp single
    {
      for (size_t wl = level; wl <= max_level; wl++) {
        getWL(wl).clear();
      }
    }
    for (size_t wl = level + 1; wl <= K_MAX_FOR_ULONG; wl++) {
#pragma omp critical(combineWorklists)
      getWL(wl).insert(getWL(wl).end(), pwp_thread.getWL(wl).begin(), pwp_thread.getWL(wl).end());
    }
#ifdef STATS
#pragma omp critical(combineStats)
    {
      for (size_t i = 0; i < K_MAX_FOR_ULONG; i++) {
        if (pwp_thread.max_k[i] > max_k[i]) {
          max_k[i] = pwp_thread.max_k[i];
          max_k_number[i] = pwp_thread.max_k_number[i];
        }
        worklist_size[i] += pwp_thread.worklist_size[i];
      }
    }
#endif  // STATS
  }
  level = max_level + 1;
  return matches;
}
#endif  //_OPENMP

auto PowersWithoutPowersWorklist::isComplete() -> bool {
  if (not unexpanded.empty()) {
    return false;
  }
  for (level = 1; level <= K_MAX_FOR_ULONG; level++) {
    if (not getWL(level).empty()) {
      return false;
    }
  }
  return true;
}

void PowersWithoutPowersWorklist::expand1(unsigned long r, unsigned long k, vector<unsigned long>& worklist) {
  unsigned long m = SuffixMath::cycleLen(k);
  for (; r < SuffixMath::cycleEnd(k); r += m) {
  }
  for (; r < SuffixMath::cycleStart(k + 1); r += m) {
  }
  for (int i = 0; i < 4; i++, r += m) {
    worklist.push_back(r);
  }
}

void PowersWithoutPowersWorklist::expand(unsigned long r, unsigned long k, unsigned long k_child, vector<unsigned long>& worklist) {
  unsigned long m = SuffixMath::cycleLen(k);
  for (; r < SuffixMath::cycleEnd(k); r += m) {
  }
  for (; r < SuffixMath::cycleStart(k_child); r += m) {
  }
  for (; r < SuffixMath::cycleEnd(k_child); r += m) {
    worklist.push_back(r);
  }
}

void PowersWithoutPowersWorklist::timeTableRun(unsigned long maxK) {
  PowersWithoutPowersWorklist pwp = PowersWithoutPowersWorklist();
  cout << "Worklist"
       << "\n";
  printf("|%10s|%8s|%10s|%10s|\n", "N", "HH:MM:SS", "Matches", "Unexpanded");
  auto t_start = time(nullptr);
  for (unsigned max_level = 1; max_level <= maxK; max_level++) {
    pwp.run(max_level);
    auto t_end = time(nullptr);

    auto s = (int)(t_end - t_start);
    auto h = s / 3600;
    s %= 3600;
    auto m = s / 60;
    s %= 60;
    auto numMatches = pwp.matches.size();
    auto numUnexpanded = pwp.unexpanded.size();
    printf("|%10lu|%2i:%2i:%2i|%10lu|%10lu|\n", SuffixMath::cycleLast(max_level), h, m, s, numMatches, numUnexpanded);
  }
  cout << "\n";
}

#ifdef _OPENMP
void PowersWithoutPowersWorklist::timeTableRunParallel(unsigned long maxK) {
  PowersWithoutPowersWorklist pwp = PowersWithoutPowersWorklist();
  cout << "Worklist parallel  (x" << omp_get_max_threads() << ")"
       << "\n";
  printf("|%10s|%8s|%10s|%10s|\n", "N", "HH:MM:SS", "Matches", "Unexpanded");
  auto t_start = time(nullptr);
  for (unsigned max_level = 1; max_level <= maxK; max_level++) {
    pwp.runParallel(max_level);
    auto t_end = time(nullptr);

    auto s = (int)(t_end - t_start);
    auto h = s / 3600;
    s %= 3600;
    auto m = s / 60;
    s %= 60;
    auto numMatches = pwp.matches.size();
    auto numUnexpanded = pwp.unexpanded.size();
    printf("|%10lu|%2i:%2i:%2i|%10lu|%10lu|\n", SuffixMath::cycleLast(max_level), h, m, s, numMatches, numUnexpanded);
  }
  cout << "\n";
}

void PowersWithoutPowersWorklist::speedUpTable(unsigned long minK, unsigned long maxK) {
  chrono::time_point<chrono::steady_clock> t_start;
  chrono::time_point<chrono::steady_clock> t_end;

  cout << "Worklist speedUp"
       << "\n";
  vector<int> parLevelNumThreads = {0, 1, 2, 4, 8, 16, 18, 32, 64, 128, 144};
  vector<const char*> parLevel = {"No OMP", "x1", "x2", "x4", "x8", "x16", "x18", "x32", "x64", "x128", "x144"};
  vector<vector<unsigned>> t(parLevel.size());

  // ---Timetable---
  cout << "Runtime [ms]"
       << "\n";
  // Header Line
  printf("|%14s|", "N");
  for (const char* s : parLevel)
    printf("%6s|", s);
  printf("\n");
  for (unsigned max_level = minK; max_level <= maxK; max_level++) {
    printf("|%14lu|", SuffixMath::cycleLast(max_level));
    for (size_t xFac = 0; xFac < parLevel.size(); xFac++) {
      omp_set_num_threads(parLevelNumThreads.at(xFac));
      PowersWithoutPowersWorklist pwp = PowersWithoutPowersWorklist();

      if (parLevelNumThreads.at(xFac) < 1) {
        t_start = chrono::steady_clock::now();
        pwp.run(max_level);
        t_end = chrono::steady_clock::now();
      } else {
        t_start = chrono::steady_clock::now();
        pwp.runParallel(max_level);
        t_end = chrono::steady_clock::now();
      }

      t[max_level - minK].push_back(chrono::duration_cast<chrono::microseconds>(t_end - t_start).count());

      printf("%6u|", t.at(max_level - minK).at(xFac) / 1000);
    }
    printf("\n");
  }
  cout << "\n";

  // ---Speedup  table---
  cout << "Speedup"
       << "\n";
  // Header Line
  printf("|%14s|", "N");
  for (const char* s : parLevel)
    printf("%6s|", s);
  printf("\n");
  // Line
  for (unsigned max_level = minK; max_level <= maxK; max_level++) {
    double t_0 = t.at(max_level - minK).at(0);
    // N
    printf("|%14lu|", SuffixMath::cycleLast(max_level));
    for (unsigned t_i : t.at(max_level - minK)) {
      // Speedup
      printf("%6.2f|", t_0 / (double)t_i);
    }
    printf("\n");
  }
  cout << "\n";
}
#endif  //_OPENMP

void PowersWithoutPowersWorklist::printStatistics([[maybe_unused]] unsigned long max_level) {
#ifdef STATS
  printf("%5s|%5s|%14s|%14s\n", "Level", "Max K", "Max K Exponent", "worklist_size");
  for (unsigned i = 0; i < max_level; i++) {
    printf("%5u|%5u|%14lu|%14u\n", i + 1, max_k[i], max_k_number[i], worklist_size[i]);
  }
  cout << "\n"
       << "Complete: " << (isComplete() ? "Yes" : "No") << "\n"
       << "Matches: " << matches << "\n";
#endif  // STATS
}
