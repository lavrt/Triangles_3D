#ifndef BVH_HPP
#define BVH_HPP

#include <unordered_set>
#include <memory>
#include <vector>
#include <fstream>

#include "bvh_node.hpp"

class BVH {
private:
    static constexpr int kMaxTrianglesPerLeaf = 2;

    std::unique_ptr<BVHNode> root_;
    std::vector<Triangle> triangles_;
    std::unordered_set<size_t> intersecting_triangles_;

    size_t GetSplitAxis(const AABB& aabb) const;
    std::unique_ptr<BVHNode> RecursiveBuild(size_t start, size_t end);
    void RecursiveFindIntersections(const BVHNode* a, const BVHNode* b);
    void DefiningGraphNodes(std::ofstream& file, BVHNode* node) const;
    void DefiningGraphDependencies(std::ofstream& file, BVHNode* node) const;

public:
    BVH(std::vector<Triangle>&& triangles) : triangles_(std::move(triangles)) {}

    void Build();
    void Dump(const std::string& file_name) const;
    std::unordered_set<size_t> FindIntersectingTriangles();
};

#endif // BVH_HPP
