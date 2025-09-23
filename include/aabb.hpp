#pragma once

#include "point.hpp"
#include "constants.hpp"

struct AABB {
    Point min;
    Point max;

    AABB() : min({Constants::kMaxDouble, Constants::kMaxDouble, Constants::kMaxDouble}),
             max({Constants::kLowestDouble, Constants::kLowestDouble, Constants::kLowestDouble}) {}

    AABB(Point min, Point max) : min(min), max(max) {}
    
    static bool Intersects(const AABB& a, const AABB& b);

    void Expand(const AABB& other);
    Point GetCenter() const;
};
