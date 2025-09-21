#include <iostream>
#include <vector>
#include <set>

#include "point.hpp"
#include "triangle.hpp"
#include "bvh.hpp"

int main() {
    size_t n = 0;
    std::cin >> n;

    std::vector<Triangle> triangles;
    triangles.reserve(n);

    Point p0, p1, p2;
    for (size_t i = 0; i != n; ++i) {
        std::cin >> p0.x >> p0.y >> p0.z
                 >> p1.x >> p1.y >> p1.z
                 >> p2.x >> p2.y >> p2.z;
                
        triangles.push_back({i, p0, p1, p2});
    }

    std::vector<Triangle> triangles_new = triangles;

    BVH tree{std::move(triangles)};
    tree.Build();
    tree.Dump("dump");
    std::set<size_t> s = tree.FindIntersectingTriangles();

    for (auto d : s) {
        std::cout << d << "\n";
    }

    // std::cout << Triangle::Intersect(triangles_new[4], triangles_new[3]);

    // __________________________________________________

    // Triangle t1 {1,
    //     {10, 10, 0},
    //     {0, 10, 0},
    //     {0, 0, 0},
    // };

    // Triangle t2 {2,
    //     {8, 9, 0},
    //     {1, 9, 0},
    //     {1, 2, 0},
    // };

    // __________________________________________________

    // Triangle t1 {1,
    //     {0, -2, 0},
    //     {2, 0, 0},
    //     {-0.92, 0.8, 3.13},
    // };

    // Triangle t2 {2,
    //     {0, 3, 0},
    //     {-3, 0, 0},
    //     {0, 0, 2},
    // };

    // std::cout << (Triangle::RelativePlanesPosition(t1, t2) == PlanesPosition::Coincide); 
    // std::cout << Triangle::Intersect(t1, t2);

    return 0;
}
