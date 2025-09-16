#ifndef BVH_HPP
#define BVH_HPP

#include <algorithm>
#include <memory>
#include <vector>

#include "bvh_node.hpp"

class BVH {
private:
    std::unique_ptr<BVHNode> root_;
    std::vector<Triangle> triangles_;

    static constexpr int kMaxTrianglesPerLeaf = 4;

    size_t GetSplitAxis(const AABB& aabb) const {
        double len_of_x = aabb.max_.x_ - aabb.min_.x_;
        double len_of_y = aabb.max_.y_ - aabb.min_.y_;
        double len_of_z = aabb.max_.z_ - aabb.min_.z_;

        double max_len = std::max({len_of_x, len_of_y, len_of_z});

        if (max_len == len_of_x) {
            return 0;
        } else if (max_len == len_of_y) {
            return 1;
        } else {
            return 2;
        }
    }

    std::unique_ptr<BVHNode> RecursiveBuild(size_t start, size_t end) {
        std::span<Triangle> triangles(triangles_.begin() + start, triangles_.begin() + end);

        auto node = std::make_unique<BVHNode>();

        AABB aabb = AABB::UniteAABB(triangles);
        node->SetAABB(aabb);

        if (end - start <= kMaxTrianglesPerLeaf) {
            node->SetTriangles(triangles);
            return node;
        }

        size_t axis = GetSplitAxis(aabb);

        std::sort(
            triangles.begin(),
            triangles.end(),
            [axis](const Triangle& a, const Triangle& b) {
                return a.GetAABB().GetCenter()[axis] < b.GetAABB().GetCenter()[axis];
            }
        );

        size_t mid = start + (end - start) / 2;

        node->SetLeft(RecursiveBuild(start, mid));
        node->SetRight(RecursiveBuild(mid, end));

        return node;
    }

public:
    BVH(std::vector<Triangle>&& triangles)
        : triangles_(std::move(triangles)) {}

    void Build() {
        root_ = RecursiveBuild(0, triangles_.size());
    }

    void Dump() const {

    }
};

#endif // BVH_HPP
