#pragma once

#include "point.hpp"
#include "details.hpp"

template <typename T>
struct AABB {
    Point<T> min;
    Point<T> max;

    AABB() : min({Limits::MaxValue<T>(), Limits::MaxValue<T>(), Limits::MaxValue<T>()}),
             max({Limits::LowestValue<T>(), Limits::LowestValue<T>(), Limits::LowestValue<T>()}) {}

    AABB(Point<T> min, Point<T> max) : min(min), max(max) {}
    
    static bool Intersects(const AABB& a, const AABB& b) noexcept {
        return (a.min.x <= b.max.x + Constants::kEpsilon && a.max.x + Constants::kEpsilon >= b.min.x)
            && (a.min.y <= b.max.y + Constants::kEpsilon && a.max.y + Constants::kEpsilon >= b.min.y)
            && (a.min.z <= b.max.z + Constants::kEpsilon && a.max.z + Constants::kEpsilon >= b.min.z);
    }

    void Expand(const AABB& other) {
        min = {
            std::min(min.x, other.min.x),
            std::min(min.y, other.min.y),
            std::min(min.z, other.min.z)
        };

        max = {
            std::max(max.x, other.max.x),
            std::max(max.y, other.max.y),
            std::max(max.z, other.max.z)
        };
    }

    Point<T> GetCenter() const {
        return Point{(max.x + min.x) / 2, (max.y + min.y) / 2, (max.z + min.z) / 2};
    }
};
