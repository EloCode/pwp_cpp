#include "SuffixMath.hpp"

#include <gmp.h>

#include <algorithm>
#include <array>
#include <limits>
#include <stdexcept>
#include <iostream>

#ifdef _MSC_VER
#include <ciso646>
#endif

using namespace std;

const unsigned long NUM_HARDCODED = 50;
const std::array<const Integer, NUM_HARDCODED> Len_I = {4_mpz,
                                                        20_mpz,
                                                        100_mpz,
                                                        500_mpz,
                                                        2500_mpz,
                                                        12500_mpz,
                                                        62500_mpz,
                                                        312500_mpz,
                                                        1562500_mpz,
                                                        7812500_mpz,
                                                        39062500_mpz,
                                                        195312500_mpz,
                                                        976562500_mpz,
                                                        4882812500_mpz,
                                                        24414062500_mpz,
                                                        122070312500_mpz,
                                                        610351562500_mpz,
                                                        3051757812500_mpz,
                                                        15258789062500_mpz,
                                                        76293945312500_mpz,
                                                        381469726562500_mpz,
                                                        1907348632812500_mpz,
                                                        9536743164062500_mpz,
                                                        47683715820312500_mpz,
                                                        238418579101562500_mpz,
                                                        1192092895507812500_mpz,
                                                        5960464477539062500_mpz,
                                                        29802322387695312500_mpz,
                                                        149011611938476562500_mpz,
                                                        745058059692382812500_mpz,
                                                        3725290298461914062500_mpz,
                                                        18626451492309570312500_mpz,
                                                        93132257461547851562500_mpz,
                                                        465661287307739257812500_mpz,
                                                        2328306436538696289062500_mpz,
                                                        11641532182693481445312500_mpz,
                                                        58207660913467407226562500_mpz,
                                                        291038304567337036132812500_mpz,
                                                        1455191522836685180664062500_mpz,
                                                        7275957614183425903320312500_mpz,
                                                        36379788070917129516601562500_mpz,
                                                        181898940354585647583007812500_mpz,
                                                        909494701772928237915039062500_mpz,
                                                        4547473508864641189575195312500_mpz,
                                                        22737367544323205947875976562500_mpz,
                                                        113686837721616029739379882812500_mpz,
                                                        568434188608080148696899414062500_mpz,
                                                        2842170943040400743484497070312500_mpz,
                                                        14210854715202003717422485351562500_mpz,
                                                        71054273576010018587112426757812500_mpz

};
auto SuffixMath::cycleLen_I(unsigned long k) -> Integer {
  if (k <= NUM_HARDCODED) {
    return Len_I[k - 1];
  }
  throw std::overflow_error("SuffixMath::cycleLen_I called with k > NUM_HARDCODED");
}

auto SuffixMath::maxCompletedCycleK(unsigned long n) -> unsigned long {
  unsigned long k = 0;
  while (not cycleLastOverflow(k + 1) and cycleLast(k + 1) <= n) {
    k++;
  }
  return k;
}
auto SuffixMath::maxCompletedCycleN(unsigned long n) -> unsigned long {
  return SuffixMath::cycleLast(maxCompletedCycleK(n));
}
auto SuffixMath::nextCompletedCycleN(unsigned long n) -> unsigned long {
  return SuffixMath::cycleLast(maxCompletedCycleK(n) + 1);
}

auto SuffixMath::powContainsPows(const Integer& b, const Integer& n) -> Integer {
  array<bool, NUM_DIGITS_ST> powers = {false, false, false, false, false, false, false, false, false, false};
  for (size_t p = 1; p < NUM_DIGITS_ST; p *= b.get_ui()) {
    powers[p] = true;
  }
  return powContains(powers, b, n);
}

auto SuffixMath::powContains(const vector<int>& digits, const Integer& b, const Integer& n) -> Integer {
  array<bool, NUM_DIGITS_ST> forbidden = {false, false, false, false, false, false, false, false, false, false};
  for (int i : digits) {
    forbidden[i] = true;
  }
  return powContains(forbidden, b, n);
}

// NOLINTBEGIN since here comes a lot of interaction with C Library
auto SuffixMath::powContains(const array<bool, NUM_DIGITS_ST>& forbidden, const Integer& b, const Integer& n) -> Integer {
  // cout << "powContains(forbidden = [";
  // for (auto i: forbidden)
  // cout << i << " ";
  // cout << "], b = " << b << ", n = " << n << ")" << "\n";
  Integer max_num_digits = 1 + n * 30103 / 100000;  // 30103 / 100000 >~ log_10(2)
  bool max_num_digits_fits_ui = max_num_digits.fits_uint_p();

  // cout << "max_num_digits " << max_num_digits << "\n";

  unsigned long m_start = 1,
                m_stop = 10;  // mpz_pow only supports unsigned long as exponent
  while (true) {
    mpz_t suffix, pow10m, digit;
    mpz_inits(suffix, pow10m, digit, NULL);
    mpz_ui_pow_ui(pow10m, 10, m_stop - 1);                   // Index starts with 1, exponents with 0 => -1
    mpz_powm(suffix, b.get_mpz_t(), n.get_mpz_t(), pow10m);  // suffix = b^n % 10^m_stop
    mpz_ui_pow_ui(pow10m, 10, m_start - 1);                  // Index starts with 1, exponents with 0 => -1
    mpz_fdiv_q(suffix, suffix, pow10m);                      // suffix = suffix / 10^m_start
    int d;
    for (unsigned long m = m_start; m < m_stop; m++) {
      d = (int)mpz_tdiv_qr_ui(suffix, digit, suffix,
                              10);  // suffix = new_suffix * 10 + digit, also returns digit as unsigned long
      if (forbidden[d]) {
        mpz_clears(suffix, pow10m, digit, NULL);
        return m;  // Return index of forbidden digit
      }
      if (max_num_digits_fits_ui and m >= max_num_digits.get_ui()) {  // Checked all digits from b^n
        mpz_clears(suffix, pow10m, digit, NULL);
        return 0;  // Return 0 for signaling a match
      }
    }
    if (m_stop > std::numeric_limits<unsigned long>::max() / 10) {  // b^n has more digits but we cannot check since
                                                                    // m_stop is bound to unsigned long
      mpz_clears(suffix, pow10m, digit, NULL);
      throw std::overflow_error(
          "Length of legal digits exceeds <unsigned long>::max(), mpz_pow only "
          "supports unsigned long as exponents");
    }

    m_start = m_stop;
    m_stop *= 10;  // Expand 10 times more digits in next loop
  }
}
// NOLINTEND
