#pragma once

#include <cmath>

#include "details.hpp"

template <typename T>
struct Vector {
    T x;
    T y;
    T z;

    Vector(T x, T y, T z) : x(x), y(y), z(z) {}

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
        T len = this->Length();
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

    Vector operator-(const Vector& other) const {
        return {x - other.x, y - other.y, z - other.z};
    }
};
