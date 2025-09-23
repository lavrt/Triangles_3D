#include "point.hpp"

double Point::operator[](size_t axis) const {
    switch (axis) {
        case 0: return x;
        case 1: return y;
        case 2: return z;

        default: throw std::out_of_range("Incorrect access to fields of the class: Point");
    }
}

Vector Point::operator-(const Point& other) const {
    return Vector{x - other.x, y - other.y, z - other.z};
}

Vector Point::AsVector() const {
    return Vector{x, y, z};
}

std::ostream& operator<<(std::ostream& os, const Point& point) {
    os << "(" << point.x << ", " << point.y << ", " << point.z << ")";
    return os; 
}
