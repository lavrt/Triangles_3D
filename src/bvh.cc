#include "bvh.hpp"

#include <stdexcept>
#include <cassert>

void BVH::Build() {
    root_ = RecursiveBuild(0, triangles_.size());
}

std::unique_ptr<BVHNode> BVH::RecursiveBuild(size_t start, size_t end) {
    std::span<Triangle> triangles(triangles_.begin() + start, triangles_.begin() + end);

    auto node = std::make_unique<BVHNode>();

    AABB aabb = Triangle::ComputeBoundingBox(triangles);
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

size_t BVH::GetSplitAxis(const AABB& aabb) const {
    Vector diff = aabb.max - aabb.min;
    return (diff.x >= diff.y && diff.x >= diff.z) ? 0 : (diff.y >= diff.z) ? 1 : 2;
}

std::set<size_t> BVH::FindIntersectingTriangles() { // TODO можно индекс не size_t а свой класс (не обяз)
    RecursiveFindIntersections(root_, root_);
    return intersecting_triangles_;
}

void BVH::RecursiveFindIntersections(const std::unique_ptr<BVHNode>& a, const std::unique_ptr<BVHNode>& b) {
    if (!AABB::Intersects(a->GetAABB(), b->GetAABB())) {
        return;
    }

    if (a->IsLeaf() && b->IsLeaf()) {
        std::span<Triangle> a_triangles = a->GetTriangles();
        std::span<Triangle> b_triangles = b->GetTriangles();

        for (const Triangle& a_tr : a_triangles) {
            for (const Triangle& b_tr : b_triangles) {
                if (a_tr.GetId() < b_tr.GetId() && Triangle::Intersect(a_tr, b_tr)) { 
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

void BVH::Dump(const std::string& file_name) const {
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

void BVH::DefiningGraphNodes(std::ofstream& file, const std::unique_ptr<BVHNode>& node) const {
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

void BVH::DefiningGraphDependencies(std::ofstream& file, const std::unique_ptr<BVHNode>& node) const {
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
