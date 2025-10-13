#pragma once

#include <set>
#include <memory>
#include <vector>
#include <fstream>

#include "node.hpp"

class BVH { // TODO BSP скорее, можно шаблон, можно вектор нод и в нодах держать вектор треугольников в единст экземпляре
private:
    static constexpr int kMaxTrianglesPerLeaf = 3;

    std::unique_ptr<BVHNode> root_ = nullptr;
    std::vector<Triangle> triangles_;
    std::set<size_t> intersecting_triangles_;

    size_t GetSplitAxis(const AABB& aabb) const {
        Vector diff = aabb.max - aabb.min;
        return (diff.x >= diff.y && diff.x >= diff.z) ? 0 : (diff.y >= diff.z) ? 1 : 2;
    }

    std::unique_ptr<BVHNode> RecursiveBuild(size_t start, size_t end);
    void RecursiveFindIntersections(const std::unique_ptr<BVHNode>& a, const std::unique_ptr<BVHNode>& b);
    void DefiningGraphNodes(std::ofstream& file, const std::unique_ptr<BVHNode>& node) const;
    void DefiningGraphDependencies(std::ofstream& file, const std::unique_ptr<BVHNode>& node) const;

public:
    BVH(std::vector<Triangle>&& triangles) noexcept : triangles_(std::move(triangles)) {}

    void Build() {
        root_ = RecursiveBuild(0, triangles_.size());
    }

    void Dump(const std::string& file_name) const;

    std::set<size_t> FindIntersectingTriangles() { // TODO можно индекс не size_t а свой класс (не обяз)
        RecursiveFindIntersections(root_, root_);
        return intersecting_triangles_;
    }

    const std::unique_ptr<BVHNode>& GetRoot() const noexcept {
        return root_;
    }
};
