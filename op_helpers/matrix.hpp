#pragma once
#include <vector>
#include <iostream>

template<typename T>
class Matrix {
private:
    int n, m;
    std::vector<std::vector<T>> mat;

public:
    Matrix(int n, int m, std::vector<std::vector<T>> v = {}): n(n), m(m), mat(v) {
    }

    int lin() const {
        return n;
    }

    int col() const {
        return m;
    }

    std::vector<T> operator[](int i) const {
        return mat[i];
    }
};