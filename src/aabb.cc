#include "aabb.hpp"

void AABB::Expand(const AABB& other) {
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

Point AABB::GetCenter() const {
    return Point{
        (max.x + min.x) / 2,
        (max.y + min.y) / 2,
        (max.z + min.z) / 2
    };
}

bool AABB::Intersects(const AABB& a, const AABB& b) {
    return (a.min.x <= b.max.x && a.max.x >= b.min.x)
        && (a.min.y <= b.max.y && a.max.y >= b.min.y)
        && (a.min.z <= b.max.z && a.max.z >= b.min.z);
}
