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

    std::ofstream out("./tests/e2e/test_data/" + output);

    for (size_t i = 0; i != triangles.size(); ++i) {
        for (size_t j = i + 1; j < triangles.size(); ++j) {
            out << tester.TestIntersection(triangles[i], triangles[j]) << "\n";
        }
    }

    return 0;
}
