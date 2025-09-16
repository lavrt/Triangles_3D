#ifndef POINT_3D_HPP
#define POINT_3D_HPP

#include <stdexcept>

struct Point3D {
    double x_;
    double y_;
    double z_;

    double operator[](size_t axis) const {
        switch (axis) {
            case 0: return x_;
            case 1: return y_;
            case 2: return z_;

            default:
                throw std::runtime_error("Incorrect access to fields of the class: Point3D");
        }
    }
};

#endif // POINT_3D_HPP
