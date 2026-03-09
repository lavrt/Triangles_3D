#pragma once

#include <cmath>
#include <stdexcept>

#include "details.hpp"

namespace geometry {

template <typename T>
requires concepts::Numeric<T>
struct Vector {
    T x;
    T y;
    T z;

    Vector(T x, T y, T z) : x(x), y(y), z(z) {}

    Vector() : x(0), y(0), z(0) {}

    const T& operator[](size_t axis) const {
        switch (axis) {
            case 0: return x;
            case 1: return y;
            case 2: return z;

            default: throw std::out_of_range("Incorrect access to fields of the class");
        }
    }

    T& operator[](size_t axis) {
        switch (axis) {
            case 0: return x;
            case 1: return y;
            case 2: return z;

            default: throw std::out_of_range("Incorrect access to fields of the class");
        }
    }

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
        return (len < constants::kEpsilon) ? Vector<T>{0, 0, 0} : Vector{x / len, y / len, z / len};
    }

    T Length() const {
        return sqrt(x * x + y * y + z * z);
    }

    bool Collinear(const Vector& other) const {
        return Vector::Cross(*this, other).Length() < constants::kEpsilon;
    }

    bool IsNullVector() const {
        return std::abs(x) < constants::kEpsilon
            && std::abs(y) < constants::kEpsilon
            && std::abs(z) < constants::kEpsilon;
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
        if (std::abs(k) < constants::kEpsilon) {
            throw std::runtime_error("Division by zero");
        }

        return {x / k, y / k, z / k};
    }

    Vector& operator/=(T k) {
        if (std::abs(k) < constants::kEpsilon) {
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
        return std::abs(x - other.x) < constants::kEpsilon
            && std::abs(y - other.y) < constants::kEpsilon
            && std::abs(z - other.z) < constants::kEpsilon;
    }

    bool operator!=(const Vector& other) const {
        return !(*this == other);
    }
};

} // namespace geometry

template <typename T>
requires concepts::Numeric<T>
geometry::Vector<T> operator*(T k, const geometry::Vector<T>& v) {
    return v * k;
}

template <typename T>
requires concepts::Numeric<T>
inline std::ostream& operator<<(std::ostream& os, const geometry::Vector<T>& vector) {
    os << "(" << vector.x << ", " << vector.y << ", " << vector.z << ")";
    return os;
}
