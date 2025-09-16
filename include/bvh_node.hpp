#ifndef BVH_NODE_HPP
#define BVH_NODE_HPP

#include <vector>
#include <memory>

#include "aabb.hpp"
#include "triangle.hpp"

class BVHNode {
private:
    AABB aabb_;

    std::unique_ptr<BVHNode> left_;
    std::unique_ptr<BVHNode> right_;

    bool isLeaf;
    std::vector<Triangle> triangles_;

public:
    void SetAABB(const AABB& aabb) {
        aabb_ = aabb;
    }

    void SetTriangles(std::span<Triangle> triangles) {
        isLeaf = true;
        triangles_.assign(triangles.begin(), triangles.end());
    }

    void SetLeft(std::unique_ptr<BVHNode> left) {
        isLeaf = false;
        left_ = std::move(left);
    }

    void SetRight(std::unique_ptr<BVHNode> right) {
        isLeaf = false;
        right_ = std::move(right);
    }
};

#endif // BVH_NODE_HPP
