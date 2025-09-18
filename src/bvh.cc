#include "bvh.hpp"

#include <stdexcept>

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
