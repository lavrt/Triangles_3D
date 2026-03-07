#include <iostream>

#include "bvh.hpp"
#include "parse_input.hpp"

using Type = double;

int main() {
    Geometry::Acceleration::BVH tree{app::ParseInput<Type>(std::cin)};
    
    auto answer = tree.FindIntersectingTriangles();
    for (const auto& id : answer) {
        std::cout << id << "\n";
    }

    return 0;
}
