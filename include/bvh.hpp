#pragma once

#include <set>
#include <memory>
#include <vector>
#include <fstream>
#include <stdexcept>

#include "node.hpp"

namespace Geometry {

namespace Acceleration {

template <typename T>
requires Concepts::Numeric<T>
class BVH {
private:
    static constexpr int kMaxTrianglesPerLeaf = 3;

    std::unique_ptr<BVHNode<T>> root_ = nullptr;
    std::vector<Triangle<T>> triangles_;
    std::set<size_t> intersecting_triangles_;

    size_t GetSplitAxis(const AABB<T>& aabb) const {
        Vector<T> diff = aabb.max - aabb.min;
        return (diff.x >= diff.y && diff.x >= diff.z) ? 0 : (diff.y >= diff.z) ? 1 : 2;
    }

    std::unique_ptr<BVHNode<T>> RecursiveBuild(size_t start, size_t end) {
        std::span<Triangle<T>> triangles(triangles_.begin() + start, triangles_.begin() + end);

        auto node = std::make_unique<BVHNode<T>>();

        AABB<T> aabb{triangles};
        node->SetAABB(aabb);

        if (end - start <= kMaxTrianglesPerLeaf) {
            node->SetTriangles(triangles);
            return node;
        }

        size_t axis = GetSplitAxis(aabb);

        std::sort(
            triangles.begin(),
            triangles.end(),
            [axis](const Triangle<T>& a, const Triangle<T>& b) {
                AABB aabb_a{a};
                AABB aabb_b{b};
                return aabb_a.GetCenter()[axis] < aabb_b.GetCenter()[axis];
            }
        );

        size_t mid = start + (end - start) / 2;

        node->SetLeft(RecursiveBuild(start, mid));
        node->SetRight(RecursiveBuild(mid, end));

        return node;
    }

    void RecursiveFindIntersections(const std::unique_ptr<BVHNode<T>>& a, const std::unique_ptr<BVHNode<T>>& b) {
        if (!AABB<T>::Intersects(a->GetAABB(), b->GetAABB())) {
            return;
        }

        if (a->IsLeaf() && b->IsLeaf()) {
            std::span<Triangle<T>> a_triangles = a->GetTriangles();
            std::span<Triangle<T>> b_triangles = b->GetTriangles();

            for (const Triangle<T>& a_tr : a_triangles) {
                for (const Triangle<T>& b_tr : b_triangles) {
                    if (a_tr.GetId() < b_tr.GetId() && Triangle<T>::Intersect(a_tr, b_tr)) { 
                        intersecting_triangles_.insert(a_tr.GetId());
                        intersecting_triangles_.insert(b_tr.GetId());
                    }
                }
            }

            return;
        }

        if (!a->IsLeaf() && !b->IsLeaf()) { 
            RecursiveFindIntersections(a->GetLeft(), b->GetLeft());
            RecursiveFindIntersections(a->GetLeft(), b->GetRight());
            RecursiveFindIntersections(a->GetRight(), b->GetLeft());
            RecursiveFindIntersections(a->GetRight(), b->GetRight());
        } else if (!a->IsLeaf()) {
            RecursiveFindIntersections(a->GetLeft(), b);
            RecursiveFindIntersections(a->GetRight(), b);
        } else {
            RecursiveFindIntersections(a, b->GetLeft());
            RecursiveFindIntersections(a, b->GetRight());
        }
    }

    void DefiningGraphNodes(std::ofstream& file, const std::unique_ptr<BVHNode<T>>& node) const {
        static size_t rank = 0;
        file << "    node_" << node.get() << " [rank=" << rank << ",label=\" { node: " << node.get()
            << " | aabb: \\{" << node->GetAABB().min << ", " << node->GetAABB().max << "\\} | ";

        if (!node->IsLeaf()) {
            file << "{ left: " << node->GetLeft().get() << " | right: " << node->GetRight().get() << " }";
        } else {
            file << "triangles_.size() = " << node->GetNumberOfTriangles();
        }
        file << "} \", color = \"#EBAEE6\"];\n";

        if (node->GetLeft()) {
            rank++;
            DefiningGraphNodes(file, node->GetLeft());
        }
        if (node->GetRight()) {
            rank++;
            DefiningGraphNodes(file, node->GetRight());
        }
        rank--;
    }

    void DefiningGraphDependencies(std::ofstream& file, const std::unique_ptr<BVHNode<T>>& node) const {
        static int flag = 0;
        if (node->GetLeft()) {
            if (flag++) {
                file << "-> node_" << node->GetLeft().get() << " ";
            } else {
                file << "    node_" << node.get() << " -> node_" << node->GetLeft().get() << " ";
            }
            DefiningGraphDependencies(file, node->GetLeft());
        }
        if (node->GetRight()) {
            if (flag++) {
                file << "-> node_" << node->GetRight().get() << " ";
            } else {
                file << "    node_" << node.get() << " -> node_" << node->GetRight().get() << " ";
            }
            DefiningGraphDependencies(file, node->GetRight());
        }
        if (flag) {
            file << ";\n";
        }
        flag = 0;
    }

public:
    BVH(std::vector<Triangle<T>>&& triangles) : triangles_(std::move(triangles)) {
        root_ = RecursiveBuild(0, triangles_.size());
    }

    void Dump(const std::string& file_name) const {
        if (root_ == nullptr) {
            throw std::runtime_error("Empty tree dump");
        }

        std::ofstream file(file_name + ".gv");
        if (!file) {
            throw std::runtime_error("Cannot open file: " + file_name + ".gv");
        }

        file << "digraph\n"
            << "{\n    "
            << "rankdir = TB;\n    "
            << "node [shape=record,style = filled,penwidth = 2.5];\n    "
            << "bgcolor = \"#FDFBE4\";\n\n";

        DefiningGraphNodes(file, root_);
        file << "\n";
        DefiningGraphDependencies(file, root_);

        file << "}\n";

        file.close();
    }

    std::set<size_t> FindIntersectingTriangles() {
        RecursiveFindIntersections(root_, root_);
        return intersecting_triangles_;
    }

    const std::unique_ptr<BVHNode<T>>& GetRoot() const noexcept {
        return root_;
    }
};

} // namespace Acceleration

} // namespace Geometry
