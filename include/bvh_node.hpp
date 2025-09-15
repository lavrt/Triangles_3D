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
    BVHNode(std::unique_ptr<BVHNode> left, std::unique_ptr<BVHNode> right)
        : isLeaf(false), left_(std::move(left)), right_(std::move(right)) {} // NOTE

    BVHNode(std::vector<Triangle> triangles)
        : isLeaf(true), left_(nullptr), right_(nullptr), triangles_(triangles) {} // NOTE
};

#endif // BVH_NODE_HPP
