#pragma once

#include <cmath>

#include "constants.hpp"

struct Vector {
    double x;
    double y;
    double z;

    Vector(double x, double y, double z) : x(x), y(y), z(z) {}

    static Vector Cross(const Vector& a, const Vector& b) {
        return Vector{
            a.y * b.z - a.z * b.y,
            a.z * b.x - a.x * b.z,
            a.x * b.y - a.y * b.x
        };
    }

    static double Dot(const Vector& a, const Vector& b) {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }

    Vector Normalized() const {
        double len = this->Length();
        return (len < Constants::kEpsilon) ? Vector{0, 0, 0} : Vector{x / len, y / len, z / len};
    }

    double Length() const {
        return sqrt(x * x + y * y + z * z);
    }

    bool Collinear(const Vector& other) const {
        return Vector::Cross(*this, other).Length() < Constants::kEpsilon;
    }
};

namespace Constants {
    namespace Basis {
        inline const Vector x{1, 0, 0}, y{0, 1, 0}, z{0, 0, 1};
    }
}
