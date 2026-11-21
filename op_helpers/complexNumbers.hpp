#pragma once
#include <iostream>
#include <cmath>

template<typename T>
class complexNumber {
private:
    T x, y;

public:
    complexNumber(T x = 0.0, T y = 0.0): x(x), y(y) {
    }

    constexpr T re() const {
        return this->x;
    }

    constexpr T im() const {
        return this->y;
    }

    T abs() const {
        return std::hypot(this->x, this->y);
    }

    constexpr T norm() const {
        return this->x * this->x + this->y * this->y;
    }

    constexpr complexNumber<T> conj() const {
        return complexNumber<T>(this->x, -this->y);
    }

    T arg() const {
        return std::atan2(this->y, this->x);
    }

    static complexNumber<T> polar(T r, T theta) {
        return complexNumber<T>(r * std::cos(theta), r * std::sin(theta));
    }

    constexpr complexNumber<T> operator-() const {
        return complexNumber<T>(-this->x, -this->y);
    }

    constexpr complexNumber<T>& operator+=(const complexNumber<T>& z) {
        this->x += z.re();
        this->y += z.im();
        return *this;
    }

    constexpr complexNumber<T>& operator-=(const complexNumber<T>& z) {
        this->x -= z.re();
        this->y -= z.im();
        return *this;
    }

    constexpr complexNumber<T>& operator*=(const complexNumber<T>& z) {
        T nx = this->x * z.re() - this->y * z.im();
        this->y = this->x * z.im() + this->y * z.re();
        this->x = nx;
        return *this;
    }

    complex& operator/=(const complexNumber<T>& z) {
        T d = z.norm();
        T nx = (this->x * z.re() + this->y * z.im()) / d;
        this->y = (this->x * z.im() - this->y * z.re()) / d;
        this->x = nx;
        return *this;
    }

    friend constexpr complexNumber<T> operator+(complexNumber<T> z, const complexNumber<T>& w) {
        return z += w;
    }

    friend constexpr complexNumber<T> operator-(complexNumber<T> z, const complexNumber<T>& w) {
        return z -= w;
    }

    friend constexpr complexNumber<T> operator*(complexNumber<T> z, const complexNumber<T>& w) {
        return z *= w;
    }

    friend complexNumber<T> operator/(complexNumber<T> z, const complexNumber<T>& w) {
        return z /= w;
    }

    friend constexpr bool operator==(const complexNumber<T>& z, const complexNumber<T>& w) {
        return z.re() == w.re() && z.im() == w.im();
    }

    friend constexpr bool operator!=(const complexNumber<T>& z, const complexNumber<T>& w) {
        return z.re() != w.re() || z.im() != w.im();
    }

    friend std::istream& operator>>(std::istream& in, complexNumber<T>& z) {
        T x, y;
        in >> x >> y;
        z = complexNumber(x, y);
        return in;
    }

    friend std::ostream& operator<<(std::ostream& out, const complexNumber<T>& z) {
        T y = z.im();
        bool neg = y < 0;
        if (neg) y = -y;
        out << z.re() << (neg ? '-' : '+') << y << 'i';
        return out;
    }
};