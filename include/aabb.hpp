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

    void Expand(const AABB& other) {
        min_ = {
            std::min(min_.x_, other.min_.x_),
            std::min(min_.y_, other.min_.y_),
            std::min(min_.z_, other.min_.z_)
        };
        max_ = {
            std::max(max_.x_, other.max_.x_),
            std::max(max_.y_, other.max_.y_),
            std::max(max_.z_, other.max_.z_)
        };
    }

    static AABB UniteAABB(std::span<Triangle> triangles);

    Point3D GetCenter() const {
        return Point3D{
            (max_.x_ + min_.x_) / 2,
            (max_.y_ + min_.y_) / 2,
            (max_.z_ + min_.z_) / 2
        };
    }
};

#endif // AABB_HPP
