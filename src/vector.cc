#include "vector.hpp"

#include <cmath>
#include <stdexcept>
#include <limits>
#include <iostream>

#include "constants.hpp"

Vector Vector::Normalized() const {
    double len = this->Length();

    if (len < Constants::kEpsilon) {
        throw std::runtime_error("An attempt to normalize the null vector");
    }

    return Vector{x / len, y / len, z / len};
}

double Vector::Length() const {
    return sqrt(x * x + y * y + z * z);
}

Vector Vector::Cross(const Vector& a, const Vector& b) {
    return Vector{
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}

double Vector::Dot(const Vector& a, const Vector& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

bool Vector::Collinear(const Vector& other) const {
    return Vector::Cross(*this, other).Length() < Constants::kEpsilon;
}
