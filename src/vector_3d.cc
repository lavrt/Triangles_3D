#include "vector_3d.hpp"

#include <cmath>
#include <stdexcept>
#include <limits>
#include <iostream>

#include "constants.hpp"

Vector3D Vector3D::Normalized() const {
    double len = this->Length();

    if (len < Constants::kEpsilon) {
        throw std::runtime_error("An attempt to normalize the null vector");
    }

    return Vector3D{x / len, y / len, z / len};
}

double Vector3D::Length() const {
    return sqrt(x * x + y * y + z * z);
}

Vector3D Vector3D::Cross(const Vector3D& a, const Vector3D& b) {
    return Vector3D{
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
}

double Vector3D::Dot(const Vector3D& a, const Vector3D& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

bool Vector3D::Collinear(const Vector3D& other) const {
    return Vector3D::Cross(*this, other).Length() < Constants::kEpsilon;
}
