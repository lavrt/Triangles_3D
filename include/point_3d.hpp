#ifndef POINT_3D_HPP
#define POINT_3D_HPP

#include <stdexcept>
#include <iostream>

struct Point3D {
    double x_;
    double y_;
    double z_;

    double operator[](size_t axis) const;
};

std::ostream& operator<<(std::ostream& os, const Point3D& point);

#endif // POINT_3D_HPP
