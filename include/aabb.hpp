#pragma once

#include <algorithm>
#include <span>

#include "point.hpp"
#include "details.hpp"
#include "triangle.hpp"

namespace Geometry {

template <typename T>
requires Concepts::Numeric<T>
struct AABB {
    Point<T> min;
    Point<T> max;

    AABB() : min({Limits::MaxValue<T>(), Limits::MaxValue<T>(), Limits::MaxValue<T>()}),
             max({Limits::LowestValue<T>(), Limits::LowestValue<T>(), Limits::LowestValue<T>()}) {}

    AABB(Point<T> min, Point<T> max) : min(min), max(max) {}

    AABB(const Triangle<T>& t)
        : min({std::min({t.p0_.x, t.p1_.x, t.p2_.x}),
               std::min({t.p0_.y, t.p1_.y, t.p2_.y}),
               std::min({t.p0_.z, t.p1_.z, t.p2_.z})}),
          max({std::max({t.p0_.x, t.p1_.x, t.p2_.x}),
               std::max({t.p0_.y, t.p1_.y, t.p2_.y}),
               std::max({t.p0_.z, t.p1_.z, t.p2_.z})})
    {}

    AABB(const std::span<Triangle<T>>& triangles) {
        if (triangles.empty()) {
            min = {0, 0, 0};
            max = {0, 0, 0};
            return;
        }

        AABB<T> aabb{triangles.front()};
        for (auto it = triangles.begin() + 1, ite = triangles.end(); it != ite; ++it) {
            aabb.Expand(*it);
        }

        min = aabb.min;
        max = aabb.max;
    }
    
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

    void Expand(const Triangle<T>& t) {
        AABB aabb{t};
        this->Expand(aabb);
    }

    Point<T> GetCenter() const {
        return Point<T>{(max.x + min.x) / 2, (max.y + min.y) / 2, (max.z + min.z) / 2};
    }
};

} // namespace Geometry
