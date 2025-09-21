#pragma once

#include <iostream>

#include "vector.hpp"

struct Point {
    double x;
    double y;
    double z;

    double operator[](size_t axis) const;
    Vector operator-(const Point& other) const;
    Vector AsVector() const;
};

std::ostream& operator<<(std::ostream& os, const Point& point);
