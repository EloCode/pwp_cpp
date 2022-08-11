#pragma once

#include <iostream>
#include <vector>
using namespace std;

// C++ template to print vector container elements
template <typename T>
ostream& operator<<(ostream& os, const vector<T>& v)
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
ostream& operator<<(ostream& os, const array<T, size>& a)
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
