#include <iostream>
#include <vector>
#include <set>

#include "point.hpp"
#include "triangle.hpp"
#include "bvh.hpp"

int main() {
    size_t n = 0;
    std::cin >> n;

    if (!std::cin.good()) {
        std::cerr << "Input error: expected number of triangles\n";
        return 1;
    }


    std::vector<Triangle> triangles;
    triangles.reserve(n);

    Point p0, p1, p2;
    for (size_t i = 0; i != n; ++i) {
        std::cin >> p0.x >> p0.y >> p0.z
                 >> p1.x >> p1.y >> p1.z
                 >> p2.x >> p2.y >> p2.z;

        if (!std::cin.good()) {
            std::cerr << "Input error on the triangle " << i << "\n";
            return 1;
        }
                
        triangles.push_back({i, p0, p1, p2});
    }

    BVH tree{std::move(triangles)};

    tree.Build();
    
    std::set<size_t> answer = tree.FindIntersectingTriangles();

    for (size_t id : answer) {
        std::cout << id << "\n";
    }

    return 0;
}
