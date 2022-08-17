#pragma once

#include <iostream>
#include <vector>
using namespace std;

// C++ template to print vector container elements
template <typename T>
auto operator<<(ostream& ostream, const vector<T>& vector) -> ostream&
{
    os << "[";
    for (int i = 0; i < v.size(); ++i) {
        os << v[i];
        if (i != v.size() - 1)
            os << ", ";
    }
    os << "]";
    return os;
}

// C++ template to print array container elements
template <typename T, size_t size>
auto operator<<(ostream& os, const array<T, size>& a) -> ostream&
{
    os << "[";
    for (size_t i = 0; i < size; ++i) {
        os << a[i];
        if (i != size - 1)
            os << ", ";
    }
    os << "]";
    return os;
}
