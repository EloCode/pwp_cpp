#pragma once

#include <array>
#include <iostream>
#include <vector>

// C++ template to print vector container elements
template <typename T>
auto operator<<(std::ostream& out, const std::vector<T>& vec) -> std::ostream& {
  out << "[";
  for (std::size_t i = 0; i < vec.size(); ++i) {
    out << vec[i];
    if (i != vec.size() - 1)
      out << ", ";
  }
  out << "]";
  return out;
}

// C++ template to print array container elements
template <typename T, std::size_t size>
auto operator<<(std::ostream& out, const std::array<T, size>& arr) -> std::ostream& {
  out << "[";
  for (std::size_t i = 0; i < size; ++i) {
    out << arr[i];
    if (i != size - 1)
      out << ", ";
  }
  out << "]";
  return out;
}
