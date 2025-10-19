#pragma once

#include <iostream>

#include "vector.hpp"

template <typename T>
struct Point {
    T x;
    T y;
    T z;

    T operator[](size_t axis) const {
        switch (axis) {
            case 0: return x;
            case 1: return y;
            case 2: return z;

            default: throw std::out_of_range("Incorrect access to fields of the class: Point");
        }
    }

    Vector<T> operator-(const Point& other) const {
        return Vector{x - other.x, y - other.y, z - other.z};
    }

    bool operator==(const Point& other) const {
        return std::abs(x - other.x) < Constants::kEpsilon
            && std::abs(y - other.y) < Constants::kEpsilon
            && std::abs(z - other.z) < Constants::kEpsilon;
    }

    Point operator+(const Vector<T>& vector) const {
        return {x + vector.x, y + vector.y, z + vector.z};
    }

    Vector<T> AsVector() const {
        return Vector{x, y, z};
    }
};

template <typename T>
inline std::ostream& operator<<(std::ostream& os, const Point<T>& point) {
    os << "(" << point.x << ", " << point.y << ", " << point.z << ")";
    return os; 
}
