#pragma once

#include <cmath>

#include "details.hpp"

namespace Geometry {

template <typename T>
requires Concepts::Numeric<T>
struct Vector {
    T x;
    T y;
    T z;

    Vector(T x, T y, T z) : x(x), y(y), z(z) {}

    Vector() : x(0), y(0), z(0) {}

    static Vector Cross(const Vector& a, const Vector& b) {
        return Vector{
            a.y * b.z - a.z * b.y,
            a.z * b.x - a.x * b.z,
            a.x * b.y - a.y * b.x
        };
    }

    static T Dot(const Vector& a, const Vector& b) {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }

    Vector Normalized() const {
        T len = Length();
        return (len < Constants::kEpsilon) ? Vector<T>{0, 0, 0} : Vector{x / len, y / len, z / len};
    }

    T Length() const {
        return sqrt(x * x + y * y + z * z);
    }

    bool Collinear(const Vector& other) const {
        return Vector::Cross(*this, other).Length() < Constants::kEpsilon;
    }

    bool IsNullVector() const {
        return std::abs(x) < Constants::kEpsilon
            && std::abs(y) < Constants::kEpsilon
            && std::abs(z) < Constants::kEpsilon;
    }

    Vector operator*(T k) const {
        return {x * k, y * k, z * k};
    }

    Vector& operator*=(T k) {
        x *= k;
        y *= k;
        z *= k;
        return *this;
    }

    Vector operator/(T k) const {
        if (std::abs(k) < Constants::kEpsilon) {
            throw std::runtime_error("Division by zero");
        }

        return {x / k, y / k, z / k};
    }

    Vector& operator/=(T k) {
        if (std::abs(k) < Constants::kEpsilon) {
            throw std::runtime_error("Division by zero");
        }

        x /= k;
        y /= k;
        z /= k;
        return *this;
    }

    Vector operator-(const Vector& other) const {
        return {x - other.x, y - other.y, z - other.z};
    }

    Vector& operator-=(const Vector& other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }

    Vector operator+(const Vector& other) const {
        return {x + other.x, y + other.y, z + other.z};
    }

    Vector& operator+=(const Vector& other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    Vector operator-() const {
        return {-x, -y, -z};
    }

    bool operator==(const Vector& other) const {
        return std::abs(x - other.x) < Constants::kEpsilon
            && std::abs(y - other.y) < Constants::kEpsilon
            && std::abs(z - other.z) < Constants::kEpsilon;
    }

    bool operator!=(const Vector& other) const {
        return !(*this == other);
    }
};

} // namespace Geometry

template <typename T>
requires Concepts::Numeric<T>
Geometry::Vector<T> operator*(T k, const Geometry::Vector<T>& v) {
    return v * k;
}
