#include "vector_3d.hpp"

#include <cmath>
#include <stdexcept>
#include <limits>
#include <iostream>

#include "constants.hpp"

Vector3D Vector3D::Normalized() const {
    double len = this->Length();

    if (len < Constants::kEpsilon) {
        throw std::runtime_error("Division by zero");
    }

    return Vector3D{x_ / len, y_ / len, z_ / len};
}

double Vector3D::Length() const {
    return sqrt(x_ * x_ + y_ * y_ + z_ * z_);
}

Vector3D Vector3D::Cross(const Vector3D& a, const Vector3D& b) {
    return Vector3D{
        a.y_ * b.z_ - a.z_ * b.y_,
        a.z_ * b.x_ - a.x_ * b.z_,
        a.x_ * b.y_ - a.y_ * b.x_
    };
}

double Vector3D::Dot(const Vector3D& a, const Vector3D& b) {
    return a.x_ * b.x_ + a.y_ * b.y_ + a.z_ * b.z_;
}

bool Vector3D::Collinear(const Vector3D& other) const {
    return Vector3D::Cross(*this, other).Length() < Constants::kEpsilon;
}
