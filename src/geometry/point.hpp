#pragma once

#include <iostream>
#include <cmath>
#include <stdexcept>

#include "vector.hpp"

namespace geometry {

template <typename T>
requires concepts::Numeric<T>
struct Point {
    T x;
    T y;
    T z;

    const T& operator[](size_t axis) const {
        switch (axis) {
            case 0: return x;
            case 1: return y;
            case 2: return z;

            default: throw std::out_of_range("Incorrect access to fields of the class");
        }
    }

    T& operator[](size_t axis) {
        switch (axis) {
            case 0: return x;
            case 1: return y;
            case 2: return z;

            default: throw std::out_of_range("Incorrect access to fields of the class");
        }
    }

    Vector<T> operator-(const Point& other) const {
        return {x - other.x, y - other.y, z - other.z};
    }

    bool operator==(const Point& other) const {
        return std::abs(x - other.x) < constants::kEpsilon
            && std::abs(y - other.y) < constants::kEpsilon
            && std::abs(z - other.z) < constants::kEpsilon;
    }

    bool operator!=(const Point& other) const {
        return !(*this == other);
    }

    Point operator+(const Vector<T>& vector) const {
        return {x + vector.x, y + vector.y, z + vector.z};
    }

    Vector<T> AsVector() const {
        return Vector{x, y, z};
    }
};

} // namespace geometry

template <typename T>
requires concepts::Numeric<T>
inline std::ostream& operator<<(std::ostream& os, const geometry::Point<T>& point) {
    os << "(" << point.x << ", " << point.y << ", " << point.z << ")";
    return os; 
}
