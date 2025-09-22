#pragma once

#include <span>
#include <limits>

#include "point.hpp"

struct AABB {
    Point min;
    Point max;

    AABB() {
        min = {
            std::numeric_limits<double>::max(),
            std::numeric_limits<double>::max(),
            std::numeric_limits<double>::max()
        };

        max = {
            std::numeric_limits<double>::lowest(),
            std::numeric_limits<double>::lowest(),
            std::numeric_limits<double>::lowest()
        };
    }

    AABB(Point min, Point max) : min(min), max(max) {}
    
    void Expand(const AABB& other);
    Point GetCenter() const;

    static bool Intersects(const AABB& a, const AABB& b);
};
