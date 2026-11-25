#pragma once
#include <cstdint>
#include <vector>
#include <iostream>
#include <stdexcept>
#include <string>

template<typename T>
class Matrix {
private:
    int32_t n, m;
    std::vector<std::vector<T>> mat;

public:
    Matrix(int32_t n, int32_t m, std::vector<std::vector<T>> v = {}): n(n), m(m), mat(v.empty() ? std::vector<std::vector<T>>(n, std::vector<T>(m, T())) : std::move(v)) {
    }

    int32_t lin() const {
        return n;
    }

    int32_t col() const {
        return m;
    }

    const std::vector<T>& operator[](int32_t i) const {
        return mat[i];
    }

    std::vector<T>& operator[](int32_t i) {
        return mat[i];
    }

    Matrix<T>& operator+=(const Matrix<T>& m) {
        if (this->n != m.lin() || this->m != m.col()) {
            throw std::invalid_argument("Matrix +=: dimensions don't corelate properly -> (" + std::to_string(this->n) + "x" + std::to_string(this->m) + " vs " + std::to_string(m.lin()) + "x" + std::to_string(m.col()) + ")");
        }

        for (int32_t i = 0; i < this->n; ++i) {
            for (int32_t j = 0; j < this->m; ++j) {
                this->mat[i][j] += m[i][j];
            }
        }

        return *this;
    }

    Matrix<T>& operator-=(const Matrix<T>& m) {
        if (this->n != m.lin() || this->m != m.col()) {
            throw std::invalid_argument("Matrix -=: dimensions don't corelate properly -> (" + std::to_string(this->n) + "x" + std::to_string(this->m) + " vs " + std::to_string(m.lin()) + "x" + std::to_string(m.col()) + ")");
        }

        for (int32_t i = 0; i < this->n; ++i) {
            for (int32_t j = 0; j < this->m; ++j) {
                this->mat[i][j] -= m[i][j];
            }
        }

        return *this;
    }

    Matrix<T>& operator*=(const Matrix<T>& m) {
        if (this->m != m.lin()) {
            throw std::invalid_argument("Matrix *=: dimensions don't corelate properly -> (" + std::to_string(this->n) + "x" + std::to_string(this->m) + " vs " + std::to_string(m.lin()) + "x" + std::to_string(m.col()) + ")");
        }

        std::vector<std::vector<T>> ans(this->n, std::vector<T>(m.col(), T()));
        for (int32_t i = 0; i < this->n; ++i) {
            for (int32_t k = 0; k < m.lin(); ++k) {
                for (int32_t j = 0; j < m.col(); ++j) {
                    ans[i][j] += this->mat[i][k] * m[k][j];
                }
            }
        }

        this->mat = std::move(ans);
        this->m = m.col();
        return *this;
    }

    Matrix<T>& operator*=(T x) {
        for (int32_t i = 0; i < this->n; ++i) {
            for (int32_t j = 0; j < this->m; ++j) {
                this->mat[i][j] *= x;
            }
        }

        return *this;
    }

    Matrix<T>& operator/=(T x) {
        for (int32_t i = 0; i < this->n; ++i) {
            for (int32_t j = 0; j < this->m; ++j) {
                this->mat[i][j] /= x;
            }
        }

        return *this;
    }

    friend Matrix<T> operator+(Matrix<T> a, const Matrix<T>& b) {
        return a += b;
    }

    friend Matrix<T> operator-(Matrix<T> a, const Matrix<T>& b) {
        return a -= b;
    }

    friend Matrix<T> operator*(Matrix<T> a, const Matrix<T>& b) {
        return a *= b;
    }

    friend Matrix<T> operator*(T x, Matrix<T> a) {
        return a *= x;
    }

    friend Matrix<T> operator*(Matrix<T> a, T x) {
        return a *= x;
    }

    friend Matrix<T> operator/(Matrix<T> a, T x) {
        return a /= x;
    }

    friend Matrix<T> tensorial_product(const Matrix<T>& a, const Matrix<T>& b) {
        int32_t n1 = a.lin(), m1 = a.col(), n2 = b.lin(), m2 = b.col();
        Matrix<T> ans(n1 * n2, m1 * m2);

        for (int32_t i = 0; i < n1; ++i) {
            for (int32_t j = 0; j < m1; ++j) {
                for (int32_t k = 0; k < n2; ++k) {
                    for (int32_t l = 0; l < m2; ++l) {
                        ans[i * n2 + k][j * m2 + l] = a[i][j] * b[k][l];
                    }
                }
            }
        }

        return ans;
    }

    friend std::istream& operator>>(std::istream& in, Matrix<T>& mat) {
        int32_t n, m;
        in >> n >> m;
        std::vector<std::vector<T>> v(n, std::vector<T>(m));

        for (int32_t i = 0; i < n; ++i) {
            for (int32_t j = 0; j < m; ++j) {
                in >> v[i][j];
            }
        }

        mat = Matrix<T>(n, m, v);
        return in;
    }

    friend std::ostream& operator<<(std::ostream& out, const Matrix<T>& mat) {
        int32_t n = mat.lin(), m = mat.col();
        for (int32_t i = 0; i < n; ++i) {
            for (int32_t j = 0; j < m; ++j) {
                out << mat[i][j] << " \n"[j == m - 1];
            }
        }

        return out;
    }
};