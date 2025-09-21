#pragma once

#include <span>
#include <vector>
#include <memory>

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
        is_leaf_ = true;
        triangles_ = triangles;
    }

    void SetLeft(std::unique_ptr<BVHNode> left) {
        is_leaf_ = false;
        left_ = std::move(left);
    }

    void SetRight(std::unique_ptr<BVHNode> right) {
        is_leaf_ = false;
        right_ = std::move(right);
    }

    AABB GetAABB() const {
        return aabb_;
    }

    std::span<Triangle> GetTriangles() const {
        return triangles_;
    }

    size_t GetNumberOfTriangles() const {
        return triangles_.size();
    }

    const std::unique_ptr<BVHNode>& GetLeft() const {
        return left_;
    }

    const std::unique_ptr<BVHNode>& GetRight() const {
        return right_;
    }

    bool IsLeaf() const {
        return is_leaf_;
    }
};
