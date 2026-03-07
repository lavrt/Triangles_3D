#include <iostream>
#include <stdexcept>

#include "bvh.hpp"
#include "parse_input.hpp"

using Type = double;

int main() {
    try {
        Geometry::Acceleration::BVH tree{app::ParseInput<Type>(std::cin)};
    
        auto answer = tree.FindIntersectingTriangles();
        for (const auto& id : answer) {
            std::cout << id << "\n";
        }

        return 0;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}
