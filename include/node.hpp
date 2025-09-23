#pragma once

#include <span>
#include <vector>
#include <memory>
#include <stdexcept>

#include "aabb.hpp"
#include "triangle.hpp"

class BVHNode {
private:
    AABB aabb_;

    std::unique_ptr<BVHNode> left_;
    std::unique_ptr<BVHNode> right_;

    bool is_leaf_;
    std::span<Triangle> triangles_;

public:
    void SetAABB(const AABB& aabb) {
        aabb_ = aabb;
    }

    void SetTriangles(std::span<Triangle> triangles) {
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

    AABB GetAABB() const noexcept {
        return aabb_;
    }

    std::span<Triangle> GetTriangles() const noexcept {
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
