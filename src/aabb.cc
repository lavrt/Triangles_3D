#include "aabb.hpp"

#include <triangle.hpp>

AABB AABB::UniteAABB(std::span<Triangle> triangles) {
    AABB aabb;

    for (const auto& triangle : triangles) {
        aabb.Expand(triangle.GetAABB());
    }

    return aabb;
}
