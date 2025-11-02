#pragma once

#include <span>
#include <vector>
#include <memory>
#include <stdexcept>

#include "aabb.hpp"
#include "triangle.hpp"

namespace Geometry {

namespace Acceleration {

template <typename T>
requires Concepts::Numeric<T>
class BVHNode {
private:
    AABB<T> aabb_;
    std::span<Triangle<T>> triangles_;
    bool is_leaf_ = true;

    std::unique_ptr<BVHNode> left_ = nullptr;
    std::unique_ptr<BVHNode> right_ = nullptr;    

public:
    void SetAABB(const AABB<T>& aabb) {
        aabb_ = aabb;
    }

    void SetTriangles(const std::span<Triangle<T>>& triangles) {
        if (left_ != nullptr || right_ != nullptr) {
            throw std::runtime_error(
                "An attempt to change the node type from a leaf node to an internal node"
            );
        }

        is_leaf_ = true;
        triangles_ = triangles;
    }

    void SetLeft(std::unique_ptr<BVHNode> left) {
        if (triangles_.size() != 0) {
            throw std::runtime_error(
                "An attempt to change the node type from an internal node to a leaf node"
            );
        }

        is_leaf_ = false;
        left_ = std::move(left);
    }

    void SetRight(std::unique_ptr<BVHNode> right) {
        is_leaf_ = false;
        right_ = std::move(right);
    }

    AABB<T> GetAABB() const noexcept {
        return aabb_;
    }

    std::span<Triangle<T>> GetTriangles() const noexcept {
        return triangles_;
    }

    size_t GetNumberOfTriangles() const noexcept {
        return triangles_.size();
    }

    const std::unique_ptr<BVHNode>& GetLeft() const noexcept {
        return left_;
    }

    const std::unique_ptr<BVHNode>& GetRight() const noexcept {
        return right_;
    }

    bool IsLeaf() const noexcept {
        return is_leaf_;
    }
};

} // namespace Acceleration

} // namespace Geometry
