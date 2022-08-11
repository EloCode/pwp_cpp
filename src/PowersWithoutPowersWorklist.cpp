#include "PowersWithoutPowersWorklist.hpp"

#include <omp.h>
#include "SuffixMath.hpp"
#include <iostream>
#include "ExtraOperators.hpp"
#include <chrono>

void PowersWithoutPowersWorklist::resetAll() {
    for (auto& wl: worklist)
        wl.clear();
    unexpanded.clear();
    matches.clear();
    level = 1;
    #ifdef STATS
        worklist_size.fill(0);
        max_k.fill(0);
    #endif // STATS
}

void PowersWithoutPowersWorklist::setForbiddenDigits(const vector<int>& digits)
 {
     for (int i = 0; i < 10; i++)
        forbiddenDigits[i]=false;
     for (int i: digits)
        forbiddenDigits[i] = true;
 }
vector<int> PowersWithoutPowersWorklist::getForbiddenDigits() {
    vector<int> digits;;
    for (int i = 0; i < 10; i++) {
        if (forbiddenDigits[i])
            digits.push_back(i);
    }
    return digits;
}

vector<unsigned long> PowersWithoutPowersWorklist::run(unsigned max_level) {
    for(; level <= max_level; level++) {
        for(unsigned long n: getWL(level)) {
            const Integer forbiddenDigitIndex = SuffixMath::powContains(forbiddenDigits, 2, n);
            if(forbiddenDigitIndex.fits_ulong_p()) {
                unsigned long k_child = forbiddenDigitIndex.get_ui();
                #ifdef STATS
                if(k_child != 0 && k_child  > max_k[level-1]) {
                    max_k[level - 1] = k_child - 1;
                    max_k_number[level - 1] = n;
                }
                #endif // STATS
                if(k_child == 0) {
                    matches.push_back(n);
                    if(isInWLRange(n)) {
                        for(unsigned long k_parent = level; k_parent < n; k_parent++)
                            expand1(n, k_parent, getWL(k_parent+1));
                    } else {
                        unexpanded.push_back(n);
                    }
                } else if(isInWLRange(k_child)) {
                    for(unsigned long k_parent = level; k_parent < k_child; k_parent++)
                        expand1(n, k_parent, getWL(k_parent+1));
                } else {
                    unexpanded.push_back(n);
                }
            } else {
                unexpanded.push_back(n);
            }
        }
        #ifdef STATS
        worklist_size[level-1] += getWL(level).size();
        #endif // STATS
        getWL(level).clear();
    }
    return matches;
}

vector<unsigned long> PowersWithoutPowersWorklist::runDFS(unsigned max_level) {
    cout << "Initial worklist: " << worklist << endl;
    unsigned long n;
    while(true) {
        if(level == 0 or level > max_level) {
            break;
        }
        if(level < max_level and not (getWL(level + 1).empty())) {
            level++;
            continue;
        }
        if(getWL(level).empty()) {
            level--;
            continue;
        }

        n = getWL(level).back();
        cout << "N=" << n << endl;
        getWL(level).pop_back();
        #ifdef STATS
        worklist_size[level-1]++;
        #endif // STATS

        const Integer forbiddenDigitIndex = SuffixMath::powContainsPows(2,n);
        if(forbiddenDigitIndex.fits_ulong_p()) {
            unsigned long k_child = forbiddenDigitIndex.get_ui();
            #ifdef STATS
            if(k_child != 0 && k_child  > max_k[level-1]) {
                max_k[level - 1] = k_child - 1;
                max_k_number[level - 1] = n;
            }
            #endif // STATS
            if(k_child == 0) {
                matches.push_back(n);
                if(isInWLRange(n)) {
                    for(unsigned long k_parent = level; k_parent < n; k_parent++)
                        expand1(n, k_parent, getWL(k_parent+1));
                } else {
                    unexpanded.push_back(n);
                }
            } else if(isInWLRange(k_child)) {
                for(unsigned long k_parent = level; k_parent < k_child; k_parent++)
                    expand1(n, k_parent, getWL(k_parent+1));
            } else {
                unexpanded.push_back(n);
            }
        } else {
            unexpanded.push_back(n);
        }
    }
    cout << "Final worklist: " << worklist << endl;
    return matches;
}

vector<unsigned long> PowersWithoutPowersWorklist::runParallel(unsigned max_level) {
    int N = omp_get_max_threads();
    while(level <= max_level and getWL(level).size() < N * 10)
        run(level + 1);
    if(level == max_level)
        return matches;
    #pragma omp parallel default(shared)
    {
        PowersWithoutPowersWorklist pwp_thread;
        for(size_t wl = level; wl <= max_level; wl++) {
            #pragma omp for schedule(dynamic,1) nowait
            for(auto it = getWL(wl).begin(); it < getWL(wl).end(); it++) {
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
            for(size_t wl = level; wl <= max_level; wl++)
                getWL(wl).clear();
        }
        for(size_t wl = level+1; wl <= K_MAX_FOR_ULONG; wl++) {
            #pragma omp critical(combineWorklists)
            getWL(wl).insert(getWL(wl).end(), pwp_thread.getWL(wl).begin(), pwp_thread.getWL(wl).end());
        }
        #ifdef STATS
        #pragma omp critical(combineStats)
        {
            for(size_t i = 0; i < K_MAX_FOR_ULONG; i++)  {
                if(pwp_thread.max_k[i] > max_k[i]) {
                    max_k[i] = pwp_thread.max_k[i];
                    max_k_number[i] = pwp_thread.max_k_number[i];
                }
                worklist_size[i] += pwp_thread.worklist_size[i];
            }
        }
        #endif // STATS
    }
    level = max_level + 1;
    return matches;
}

bool PowersWithoutPowersWorklist::isComplete() {
    if(not  unexpanded.empty())
        return false;
    for(level = 1; level <= K_MAX_FOR_ULONG; level++) {
        if(not getWL(level).empty())
            return false;
    }
    return true;
}

void PowersWithoutPowersWorklist::expand1(unsigned long r, unsigned long k , vector<unsigned long> & worklist) {
    unsigned long m = SuffixMath::cycleLen(k);
    for(;r < SuffixMath::cycleEnd(k)    ;r += m);
    for(;r < SuffixMath::cycleStart(k+1);r += m);
    for(int i = 0; i < 4 ;i++, r += m)
        worklist.push_back(r);
}

void PowersWithoutPowersWorklist::expand(unsigned long r, unsigned long k, unsigned long k_child , vector<unsigned long> & worklist) {
    unsigned long m = SuffixMath::cycleLen(k);
    for(;r < SuffixMath::cycleEnd(k)        ;r += m);
    for(;r < SuffixMath::cycleStart(k_child);r += m);
    for(;r < SuffixMath::cycleEnd  (k_child);r += m)
        worklist.push_back(r);
}

void PowersWithoutPowersWorklist::timeTableRun(unsigned long maxK) {
    using namespace std;
    time_t t_start, t_end;
    int h,m,s; // hours,minutes,seconds
    int numMatches, numClasses, numUnexpanded;

    PowersWithoutPowersWorklist pwp = PowersWithoutPowersWorklist();
    cout << "Worklist" << endl;
    printf("|%10s|%8s|%10s|%10s|\n", "N", "HH:MM:SS", "Matches", "Unexpanded");
    t_start = time(nullptr);
    for(unsigned max_level = 1; max_level <= maxK; max_level++) {
        pwp.run(max_level);
        t_end = time(nullptr);

        s = (int)(t_end - t_start);
        h = s/3600;
        s %= 3600;
        m = s/60;
        s %= 60;
        numMatches = pwp.matches.size();
        numUnexpanded = pwp.unexpanded.size();
        printf("|%10lu|%2i:%2i:%2i|%10i|%10i|\n", SuffixMath::cycleLast(max_level), h, m, s, numMatches, numUnexpanded);
    }
    cout << endl;
}
void PowersWithoutPowersWorklist::timeTableRunParallel(unsigned long maxK) {
    using namespace std;
    time_t t_start, t_end;
    int h,m,s; // hours,minutes,seconds
    int numMatches, numClasses, numUnexpanded;

    PowersWithoutPowersWorklist pwp = PowersWithoutPowersWorklist();
    cout << "Worklist parallel  (x" << omp_get_max_threads() << ")" << endl;
    printf("|%10s|%8s|%10s|%10s|\n", "N", "HH:MM:SS", "Matches", "Unexpanded");
    t_start = time(nullptr);
    for(unsigned max_level = 1; max_level <= maxK; max_level++) {
        pwp.runParallel(max_level);
        t_end = time(nullptr);

        s = (int)(t_end - t_start);
        h = s/3600;
        s %= 3600;
        m = s/60;
        s %= 60;
        numMatches = pwp.matches.size();
        numUnexpanded = pwp.unexpanded.size();
        printf("|%10lu|%2i:%2i:%2i|%10i|%10i|\n", SuffixMath::cycleLast(max_level), h, m, s, numMatches, numUnexpanded);
    }
    cout << endl;
}
void PowersWithoutPowersWorklist::speedUpTable(unsigned long minK,  unsigned long maxK) {
    using namespace std;
    chrono::time_point<chrono::steady_clock> t_start, t_end;

    cout << "Worklist speedUp" << endl;
    vector<int> parLevelNumThreads = {       0,    1,    2,    4,    8,    16,    18,    32,    64,    128,    144};
    vector<const char *> parLevel  = {"No OMP", "x1", "x2", "x4", "x8", "x16", "x18", "x32", "x64", "x128", "x144"};
    vector<vector<unsigned>> t(parLevel.size());

    // ---Timetable---
    cout << "Runtime [ms]" << endl;
    // Header Line
    printf("|%14s|", "N");
        for(const char *s: parLevel)
        printf("%6s|", s);
    printf("\n");
    for(unsigned max_level = minK; max_level <= maxK; max_level++) {
        printf("|%14lu|", SuffixMath::cycleLast(max_level));
         for(size_t xFac = 0; xFac < parLevel.size(); xFac++) {
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

            t[max_level  - minK].push_back(std::chrono::duration_cast<std::chrono::microseconds>(t_end - t_start).count());

            printf("%6u|", t.at(max_level - minK).at(xFac) / 1000);
        }
        printf("\n");
    }
    cout << endl;

    // ---Speedup  table---
    cout << "Speedup" << endl;
    // Header Line
    printf("|%14s|", "N");
    for(const char *s: parLevel)
        printf("%6s|", s);
    printf("\n");
    // Line
    for(unsigned max_level = minK; max_level <= maxK; max_level++) {
        double t_0 = t.at(max_level - minK).at(0);
        // N
        printf("|%14lu|", SuffixMath::cycleLast(max_level));
        for(unsigned t_i: t.at(max_level - minK))  {
            // Speedup
            printf("%6.2f|", t_0 / (double) t_i);
        }
        printf("\n");
    }
    cout << endl;
}
#define STATS
#ifdef STATS
void PowersWithoutPowersWorklist::printStatistics(unsigned long max_level) {
    using namespace std;
    printf("%5s|%5s|%14s|%14s\n", "Level", "Max K", "Max K Exponent", "worklist_size");
    for(unsigned i = 0; i < max_level; i++)  {
        printf("%5u|%5u|%14lu|%14u\n", i+1, max_k[i], max_k_number[i], worklist_size[i]);
    }
    cout << endl
         << "Complete: " << (isComplete()?"Yes":"No") << endl
         << "Matches: "  << matches << endl;
}
#endif //STATS
