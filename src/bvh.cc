#include "bvh.hpp"

#include <stdexcept>

void BVH::Build() {
    root_ = RecursiveBuild(0, triangles_.size());
}

std::unique_ptr<BVHNode> BVH::RecursiveBuild(size_t start, size_t end) {
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

size_t BVH::GetSplitAxis(const AABB& aabb) const {
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

void BVH::Dump(const std::string& file_name) const {
    std::ofstream file(file_name + ".gv");
    if (!file) {
        throw std::runtime_error("Cannot open file: " + file_name + ".gv");
    }

    file << "digraph\n"
         << "{\n    "
         << "rankdir = TB;\n    "
         << "node [shape=record,style = filled,penwidth = 2.5];\n    "
         << "bgcolor = \"#FDFBE4\";\n\n";

    DefiningGraphNodes(file, root_.get());
    DefiningGraphDependencies(file, root_.get());

    file << "}\n";

    file.close();

    std::string dotCmd = "dot " + file_name + ".gv -Tpng -o " + file_name + ".png";
    std::system(dotCmd.c_str());
}

void BVH::DefiningGraphNodes(std::ofstream& file, BVHNode* node) const {
    static size_t rank = 0;
    file << "    node_" << node << " [rank=" << rank << ",label=\" { node: " << node
         << " | aabb: \\{" << node->GetAABB().min_ << ", " << node->GetAABB().max_ << "\\} | ";


    if (!node->IsLeaf()) {
        file << "{ left: " << node->GetLeft() << " | right: " << node->GetRight() << " }";
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

void BVH::DefiningGraphDependencies(std::ofstream& file, BVHNode* node) const {
    static int flag = 0;
    if (node->GetLeft()) {
        if (flag++) {
            file << "-> node_" << node->GetLeft() << " ";
        } else {
            file << "    node_" << node << " -> node_" << node->GetLeft() << " ";
        }
        DefiningGraphDependencies(file, node->GetLeft());
    }
    if (node->GetRight()) {
        if (flag++) {
            file << "-> node_" << node->GetRight() << " ";
        } else {
            file << "    node_" << node << " -> node_" << node->GetRight() << " ";
        }
        DefiningGraphDependencies(file, node->GetRight());
    }
    if (flag) {
        file << ";\n";
    }
    flag = 0;
}
