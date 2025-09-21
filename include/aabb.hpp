#pragma once

#include <span>
#include <limits>

#include "point_3d.hpp"

class Triangle;

struct AABB {
    Point3D min_;
    Point3D max_;

    AABB();
    AABB(Point3D min, Point3D max) : min_(min), max_(max) {}
    
    void Expand(const AABB& other);
    Point3D GetCenter() const;

    static bool Intersects(const AABB& a, const AABB& b);
    static AABB UniteAABB(std::span<Triangle> triangles);
};
