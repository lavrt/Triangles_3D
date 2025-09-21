#include "point_3d.hpp"

double Point3D::operator[](size_t axis) const {
    switch (axis) {
        case 0: return x;
        case 1: return y;
        case 2: return z;

        default: throw std::runtime_error("Incorrect access to fields of the class: Point3D");
    }
}

Vector3D Point3D::operator-(const Point3D& other) const {
    return Vector3D{
        x - other.x,
        y - other.y,
        z - other.z
    };
}

Vector3D Point3D::AsVector() const {
    return Vector3D{x, y, z};
}

std::ostream& operator<<(std::ostream& os, const Point3D& point) {
    os << "(" << point.x << ", " << point.y << ", " << point.z << ")";
    return os; 
}
