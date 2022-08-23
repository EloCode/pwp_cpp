#pragma once

#include <array>
#include <iostream>
#include <vector>
using namespace std;

// C++ template to print vector container elements
template <typename T>
auto operator<<(ostream& out, const vector<T>& vec) -> ostream& {
  out << "[";
  for (int i = 0; i < vec.size(); ++i) {
    out << vec[i];
    if (i != vec.size() - 1)
      out << ", ";
  }
  out << "]";
  return out;
}

// C++ template to print array container elements
template <typename T, size_t size>
auto operator<<(ostream& out, const array<T, size>& arr) -> ostream& {
  out << "[";
  for (size_t i = 0; i < size; ++i) {
    out << arr[i];
    if (i != size - 1)
      out << ", ";
  }
  out << "]";
  return out;
}
