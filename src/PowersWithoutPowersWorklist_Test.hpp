#pragma once

#include <gmpxx.h>
#include "ExtraOperators.hpp"
#include "PowersWithoutPowersWorklist.hpp"

namespace PowersWithoutPowersWorklist_Test {
auto Test() -> bool {
  using namespace std;
  cout << " --- Testing PowersWithoutPowersWorklist --- " << "\n";
  bool success = true;
  bool test = true;

  cout << "Constructor: ";
  PowersWithoutPowersWorklist pwp = PowersWithoutPowersWorklist();
  test = true;
  success &= test;
  cout << ((test ? "success" : "failed")) << "\n";

  cout << "run (matches): ";
  auto m = pwp.run(1);
  test = (m.size() == 0);
  success &= test;
  cout << ((test ? "success" : "failed")) << "\n";

  cout << "run (worklist): ";
  test = (pwp.worklist[0].size() == 0);
  test &= (pwp.worklist[1] == vector<unsigned long>({8, 12, 16, 20}));
  test &= (pwp.worklist[2].size() == 0);
  test &= (pwp.worklist[3].size() == 0);
  success &= test;
  cout << ((test ? "success" : "failed")) << "\n";

  cout << "run (unexpanded): ";
  test = (pwp.unexpanded.size() == 0);
  success &= test;
  cout << ((test ? "success" : "failed")) << "\n";

  cout << "run2 (matches): ";
  m = pwp.run(2);
  test = (m == vector<unsigned long>({16}));
  success &= test;
  cout << ((test ? "success" : "failed")) << "\n";

  cout << "run2 (worklist): ";
  test = (pwp.worklist[0].size() == 0);
  test &= (pwp.worklist[1].size() == 0);
  test &= (pwp.worklist[2].size() == 16);
  test &= (pwp.worklist[3].size() == 12);
  test &= (pwp.worklist[4].size() == 4);
  test &= (pwp.worklist[5].size() == 4);
  test &= (pwp.worklist[6].size() == 4);
  test &= (pwp.worklist[7].size() == 4);
  test &= (pwp.worklist[8].size() == 4);
  test &= (pwp.worklist[9].size() == 4);
  test &= (pwp.worklist[10].size() == 4);
  test &= (pwp.worklist[11].size() == 4);
  test &= (pwp.worklist[12].size() == 4);
  test &= (pwp.worklist[13].size() == 4);
  test &= (pwp.worklist[14].size() == 4);
  test &= (pwp.worklist[15].size() == 4);
  test &= (pwp.worklist[16].size() == 0);
  success &= test;
  cout << ((test ? "success" : "failed")) << "\n";

  cout << "run2 (unexpanded): ";
  test = (pwp.unexpanded.size() == 0);
  success &= test;
  cout << ((test ? "success" : "failed")) << "\n";

  pwp.run(10);
#ifdef _OPENMP
  for (int n : {1, 2, 4, 8, 12, 48}) {
    omp_set_num_threads(n);
    cout << "OMP_NUM_THREADS=" << n << "\n" << "runParallel:";

    PowersWithoutPowersWorklist pwp2;
    m = pwp2.runParallel(10);
    test = true;
    success &= test;
    cout << ((test ? "success" : "failed")) << "\n";

    cout << "run == runParallel (matches): ";
    test = (m == vector<unsigned long>({16}));
    success &= test;
    cout << ((test ? "success" : "failed")) << "\n";

    cout << "run == runParallel (worklist): ";
    test = (pwp.worklist[10].size() == pwp2.worklist[10].size());
    test &= (pwp.worklist[11].size() == pwp2.worklist[11].size());
    test &= (pwp.worklist[12].size() == pwp2.worklist[12].size());
    test &= (pwp.worklist[13].size() == pwp2.worklist[13].size());
    test &= (pwp.worklist[14].size() == pwp2.worklist[14].size());
    test &= (pwp.worklist[15].size() == pwp2.worklist[15].size());
    test &= (pwp.worklist[16].size() == pwp2.worklist[16].size());
    test &= (pwp.worklist[17].size() == pwp2.worklist[17].size());
    test &= (pwp.worklist[18].size() == pwp2.worklist[18].size());
    test &= (pwp.worklist[19].size() == pwp2.worklist[19].size());
    test &= (pwp.worklist[20].size() == pwp2.worklist[20].size());
    test &= (pwp.worklist[21].size() == pwp2.worklist[21].size());
    test &= (pwp.worklist[22].size() == pwp2.worklist[22].size());
    test &= (pwp.worklist[23].size() == pwp2.worklist[23].size());
    test &= (pwp.worklist[24].size() == pwp2.worklist[24].size());
    test &= (pwp.worklist[25].size() == pwp2.worklist[25].size());
    test &= (pwp.worklist[26].size() == pwp2.worklist[26].size());
    success &= test;
    cout << ((test ? "success" : "failed")) << "\n";

    cout << "run == runParallel (unexpanded): ";
    test = (pwp.unexpanded.size() == pwp2.unexpanded.size());
    success &= test;
    cout << ((test ? "success" : "failed")) << "\n";
  }
#endif  //_OPENMP

  cout << " --- Overall " << (success ? "success --- " : "failed --- ") << "\n" << "\n";
  return success;
}
}  // namespace PowersWithoutPowersWorklist_Test
