#pragma once

#include <span>
#include <limits>

#include "point_3d.hpp"

class Triangle;

struct AABB {
    Point3D min;
    Point3D max;

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

    AABB(Point3D min, Point3D max) : min(min), max(max) {}
    
    void Expand(const AABB& other);
    Point3D GetCenter() const;

    static bool Intersects(const AABB& a, const AABB& b);
    static AABB UniteAABB(std::span<Triangle> triangles);
};
