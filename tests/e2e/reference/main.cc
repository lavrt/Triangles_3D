#include <iostream>
#include <fstream>
#include <vector>

#include "fcl_reference.hpp"

int main(int argc, char** argv) {
    std::vector<std::string> args(argv, argv + argc);

    std::string input;
    std::string output;
    for (size_t i = 1; i < args.size(); ++i) {
        if (args[i] == "--input") {
            input = args[++i];
        } else if (args[i] == "--output") {
            output = args[++i];
        }
    }

    std::vector<Triangle> triangles = ReadTrianglesFromFile("./tests/e2e/test_data/" + input);

    if (triangles.size() < 2) {
        std::cerr << "Need at least 2 triangles for testing\n";
        return 1;
    }

    TriangleTester tester;

    std::set<size_t> intersected_indices;

    for (size_t i = 0; i < triangles.size(); ++i) {
        for (size_t j = i + 1; j < triangles.size(); ++j) {
            if (tester.TestIntersection(triangles[i], triangles[j])) {
                intersected_indices.insert(i);
                intersected_indices.insert(j);
            }
        }
    }

    std::ofstream out("./tests/e2e/test_data/" + output);
    if (!out.is_open()) {
        std::cerr << "Cannot open output file: " << output << "\n";
        return 1;
    }

    for (size_t idx : intersected_indices) {
        out << idx << "\n";
    }

    std::cout << "Found " << intersected_indices.size() << " intersecting triangles.\n";
    return 0;
}
