#ifndef AABB_HPP
#define AABB_HPP

#include <span>
#include <limits>

#include "point_3d.hpp"

class Triangle;

struct AABB {
    Point3D min_;
    Point3D max_;

    AABB(Point3D min, Point3D max)
        : min_(min), max_(max) {}

    AABB() :
        min_({
            std::numeric_limits<double>::max(),
            std::numeric_limits<double>::max(),
            std::numeric_limits<double>::max()
        }),
        max_({
            std::numeric_limits<double>::lowest(),
            std::numeric_limits<double>::lowest(),
            std::numeric_limits<double>::lowest()
        }) {}

    void Expand(const AABB& other);
    Point3D GetCenter() const;

    static AABB UniteAABB(std::span<Triangle> triangles);
};

#endif // AABB_HPP
