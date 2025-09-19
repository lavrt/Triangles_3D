#include "point_3d.hpp"

double Point3D::operator[](size_t axis) const {
    switch (axis) {
        case 0: return x_;
        case 1: return y_;
        case 2: return z_;

        default:
            throw std::runtime_error("Incorrect access to fields of the class: Point3D");
    }
}

std::ostream& operator<<(std::ostream& os, const Point3D& point) {
    os << "(" << point.x_ << ", " << point.y_ << ", " << point.z_ << ")";
    return os; 
}
