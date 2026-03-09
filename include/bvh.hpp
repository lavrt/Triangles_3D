#pragma once

#include <set>
#include <memory>
#include <vector>
#include <fstream>
#include <stdexcept>

#include "node.hpp"
#include "indexed_triangle.hpp"

namespace Geometry {

namespace Acceleration {

template <typename T>
requires Concepts::Numeric<T>
class BVH {
    template <typename U> requires Concepts::Numeric<U> friend class BvhVisualizer;

public:
    BVH(std::vector<IndexedTriangle<T>>&& triangles) : triangles_(std::move(triangles)) {
        root_ = RecursiveBuild(0, triangles_.size());
    }

    std::set<TrIndex> FindIntersectingTriangles() {
        RecursiveFindIntersections(root_, root_);
        return intersecting_triangles_;
    }

    const BVHNode<T>* GetRoot() const noexcept {
        return &nodes_[root_];
    }

private:
    static constexpr int kMaxTrianglesPerLeaf = 3;

    NodeIdx root_ = invalid_idx;
    std::vector<BVHNode<T>> nodes_;
    std::vector<IndexedTriangle<T>> triangles_;
    std::set<TrIndex> intersecting_triangles_;

    size_t GetSplitAxis(const AABB<T>& aabb) const {
        Vector<T> diff = aabb.max - aabb.min;
        return (diff.x >= diff.y && diff.x >= diff.z) ? 0 : (diff.y >= diff.z) ? 1 : 2;
    }

    NodeIdx RecursiveBuild(size_t start, size_t end) {
        std::span<IndexedTriangle<T>> triangles(triangles_.begin() + start, triangles_.begin() + end);

        // auto node = std::make_unique<BVHNode<T>>();

        AABB<T> aabb;
        for (auto&& tr : triangles) {
            aabb.Expand(tr.triangle);
        }
        // node->SetAABB(aabb);

        if (end - start <= kMaxTrianglesPerLeaf) {
            // node->SetTriangles(triangles);
            nodes_.emplace_back(aabb, triangles);
            return nodes_.size() - 1;
        }

        size_t axis = GetSplitAxis(aabb);

        std::sort(
            triangles.begin(),
            triangles.end(),
            [axis](const IndexedTriangle<T>& a, const IndexedTriangle<T>& b) {
                AABB aabb_a{a.triangle};
                AABB aabb_b{b.triangle};
                return aabb_a.GetCenter()[axis] < aabb_b.GetCenter()[axis];
            }
        );

        size_t mid = start + (end - start) / 2;

        // node->SetLeft(RecursiveBuild(start, mid));
        // node->SetRight(RecursiveBuild(mid, end));

        nodes_.emplace_back(aabb, RecursiveBuild(start, mid), RecursiveBuild(mid, end));

        return nodes_.size() - 1;
    }

    void RecursiveFindIntersections(NodeIdx a_idx, NodeIdx b_idx) {
        const auto& a = nodes_[a_idx];
        const auto& b = nodes_[b_idx];

        if (!AABB<T>::Intersects(a.GetAABB(), b.GetAABB())) {
            return;
        }

        if (a.IsLeaf() && b.IsLeaf()) {
            auto a_triangles = a.GetTriangles();
            auto b_triangles = b.GetTriangles();

            for (const auto& a_tr : a_triangles) {
                for (const auto& b_tr : b_triangles) {
                    if (a_tr.id < b_tr.id && Triangle<T>::Intersect(a_tr.triangle, b_tr.triangle)) { 
                        intersecting_triangles_.insert(a_tr.id);
                        intersecting_triangles_.insert(b_tr.id);
                    }
                }
            }

            return;
        }

        if (!a.IsLeaf() && !b.IsLeaf()) { 
            RecursiveFindIntersections(a.GetLeftIdx(), b.GetLeftIdx());
            RecursiveFindIntersections(a.GetLeftIdx(), b.GetRightIdx());
            RecursiveFindIntersections(a.GetRightIdx(), b.GetLeftIdx());
            RecursiveFindIntersections(a.GetRightIdx(), b.GetRightIdx());
        } else if (!a.IsLeaf()) {
            RecursiveFindIntersections(a.GetLeftIdx(), b_idx);
            RecursiveFindIntersections(a.GetRightIdx(), b_idx);
        } else {
            RecursiveFindIntersections(a_idx, b.GetLeftIdx());
            RecursiveFindIntersections(a_idx, b.GetRightIdx());
        }
    }

    const BVHNode<T>* GetNode(NodeIdx idx) const {
        return &nodes_[idx];
    }
};

} // namespace Acceleration

} // namespace Geometry
