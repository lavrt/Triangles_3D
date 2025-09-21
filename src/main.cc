#include <iostream>
#include <vector>
#include <unordered_set>

#include "point_3d.hpp"
#include "triangle.hpp"
#include "bvh.hpp"

int main() {
    size_t n = 0;
    std::cin >> n;

    std::vector<Triangle> triangles;
    triangles.reserve(n);

    Point3D p0, p1, p2;
    for (size_t i = 0; i != n; ++i) {
        std::cin >> p0.x_ >> p0.y_ >> p0.z_
                 >> p1.x_ >> p1.y_ >> p1.z_
                 >> p2.x_ >> p2.y_ >> p2.z_;
                
        triangles.push_back({i, p0, p1, p2});
    }

    BVH tree{std::move(triangles)};
    tree.Build();
    tree.Dump("dump");
    tree.Build();
    std::unordered_set<size_t> s = tree.FindIntersectingTriangles();

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
