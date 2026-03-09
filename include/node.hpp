#pragma once

#include <span>

#include "aabb.hpp"
#include "triangle.hpp"
#include "indexed_triangle.hpp"

namespace Geometry {

namespace Acceleration {

using NodeIdx = int;
inline constexpr NodeIdx invalid_idx = -1;

template <typename T>
requires Concepts::Numeric<T>
class BVHNode final {
public:
    BVHNode(const AABB<T>& aabb, std::span<const IndexedTriangle<T>> triangles)
        : aabb_(aabb), triangles_(triangles), is_leaf_(true) {}

    BVHNode(const AABB<T>& aabb, NodeIdx left, NodeIdx right)
        : aabb_(aabb), left_(left), right_(right), is_leaf_(false) {}

    const AABB<T>& GetAABB() const noexcept {
        return aabb_;
    }

    std::span<const IndexedTriangle<T>> GetTriangles() const noexcept {
        return triangles_;
    }

    size_t GetNumberOfTriangles() const noexcept {
        return triangles_.size();
    }

    NodeIdx GetLeftIdx() const noexcept {
        return left_;
    }

    NodeIdx GetRightIdx() const noexcept {
        return right_;
    }

    bool IsLeaf() const noexcept {
        return is_leaf_;
    }

private:
    AABB<T> aabb_;
    std::span<const IndexedTriangle<T>> triangles_;
    NodeIdx left_ = invalid_idx;
    NodeIdx right_ = invalid_idx;
    bool is_leaf_{true};
};

} // namespace Acceleration

} // namespace Geometry
