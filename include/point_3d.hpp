#pragma once

#include <iostream>

#include "vector_3d.hpp"

struct Point3D {
    double x_;
    double y_;
    double z_;

    double operator[](size_t axis) const;
    Vector3D operator-(const Point3D& other) const;
    Vector3D AsVector() const;
};

std::ostream& operator<<(std::ostream& os, const Point3D& point);
