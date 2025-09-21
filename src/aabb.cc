#include "aabb.hpp"

#include <triangle.hpp>

AABB::AABB() {
    min_ = {
        std::numeric_limits<double>::max(),
        std::numeric_limits<double>::max(),
        std::numeric_limits<double>::max()
    };
    
    max_ = {
        std::numeric_limits<double>::lowest(),
        std::numeric_limits<double>::lowest(),
        std::numeric_limits<double>::lowest()
    };
}

AABB AABB::UniteAABB(std::span<Triangle> triangles) {
    AABB aabb;

    for (const auto& triangle : triangles) {
        aabb.Expand(triangle.GetAABB());
    }

    return aabb;
}

void AABB::Expand(const AABB& other) {
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

Point3D AABB::GetCenter() const {
    return Point3D{
        (max_.x_ + min_.x_) / 2,
        (max_.y_ + min_.y_) / 2,
        (max_.z_ + min_.z_) / 2
    };
}

bool AABB::Intersects(const AABB& a, const AABB& b) {
    return (a.min_.x_ <= b.max_.x_ && a.max_.x_ >= b.min_.x_)
        && (a.min_.y_ <= b.max_.y_ && a.max_.y_ >= b.min_.y_)
        && (a.min_.z_ <= b.max_.z_ && a.max_.z_ >= b.min_.z_);
}
