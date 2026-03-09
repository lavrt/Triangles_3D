#pragma once

#include <fstream>
#include <string>

#include "bvh.hpp"

namespace Geometry::Acceleration {

template <typename T>
requires Concepts::Numeric<T>
class BvhVisualizer {
public:
    static void Dump(const BVH<T>& t, const std::string& filename) {
        std::ofstream file(filename + ".gv");
        if (!file) {
            throw std::runtime_error("Cannot open file: " + filename + ".gv");
        }

        file << "digraph\n"
            << "{\n    "
            << "rankdir = TB;\n    "
            << "node [shape=record,style = filled,penwidth = 2.5];\n    "
            << "bgcolor = \"#F8F9FA\";\n\n";

        DefiningGraphNodes(file, t, t.GetRoot());
        file << "\n";
        DefiningGraphDependencies(file, t, t.GetRoot());

        file << "}\n";

        file.close();
    }

private:
    static void DefiningGraphNodes(std::ofstream& file, const BVH<T>& t, const BVHNode<T>* node) {
        static size_t rank = 0;
        file << "    node_" << node << " [rank=" << rank << ",label=\" { node: " << node
            << " | aabb: \\{" << node->GetAABB().min << ", " << node->GetAABB().max << "\\} | ";

        if (!node->IsLeaf()) {
            file << "{ left: " << t.GetNode(node->GetLeftIdx())
                << " | right: " << t.GetNode(node->GetRightIdx()) << " }";
        } else {
            file << "triangles_.size() = " << node->GetNumberOfTriangles();
        }
        file << "} \", color = \"#EBAEE6\"];\n";

        if (node->GetLeftIdx() != invalid_idx) {
            rank++;
            DefiningGraphNodes(file, t, t.GetNode(node->GetLeftIdx()));
        }
        if (node->GetRightIdx() != invalid_idx) {
            rank++;
            DefiningGraphNodes(file, t, t.GetNode(node->GetRightIdx()));
        }
        rank--;
    }

    static void DefiningGraphDependencies(std::ofstream& file, const BVH<T>& t, const BVHNode<T>* node) {
        static int flag = 0;
        if (node->GetLeftIdx() != -1) {
            if (flag++) {
                file << "-> node_" << t.GetNode(node->GetLeftIdx()) << " ";
            } else {
                file << "    node_" << node << " -> node_" << t.GetNode(node->GetLeftIdx()) << " ";
            }
            DefiningGraphDependencies(file, t, t.GetNode(node->GetLeftIdx()));
        }
        if (node->GetRightIdx() != -1) {
            if (flag++) {
                file << "-> node_" << t.GetNode(node->GetRightIdx()) << " ";
            } else {
                file << "    node_" << node << " -> node_" << t.GetNode(node->GetRightIdx()) << " ";
            }
            DefiningGraphDependencies(file, t, t.GetNode(node->GetRightIdx()));
        }
        if (flag) {
            file << ";\n";
        }
        flag = 0;
    }
};

} // namespace Geometry::Acceleration
