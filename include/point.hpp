#pragma once

#include <iostream>

#include "vector.hpp"

struct Point {
    double x;
    double y;
    double z;

    double operator[](size_t axis) const {
        switch (axis) {
            case 0: return x;
            case 1: return y;
            case 2: return z;

            default: throw std::out_of_range("Incorrect access to fields of the class: Point");
        }
    }

    Vector operator-(const Point& other) const {
        return Vector{x - other.x, y - other.y, z - other.z};
    }

    Vector AsVector() const {
        return Vector{x, y, z};
    }
};

inline std::ostream& operator<<(std::ostream& os, const Point& point) {
    os << "(" << point.x << ", " << point.y << ", " << point.z << ")";
    return os; 
}
