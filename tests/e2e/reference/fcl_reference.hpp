#pragma once

#include <fcl/narrowphase/collision.h>
#include <fcl/geometry/bvh/BVH_model.h>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <vector>
#include <memory>

struct Triangle {
    std::vector<fcl::Vector3d> vertices;
    
    Triangle(const std::vector<fcl::Vector3d>& vertices) : vertices(vertices) {}
};

class TriangleTester {
public:
    bool TestIntersection(const Triangle& t1, const Triangle& t2) {
        auto mesh1 = CreateMeshFromTriangle(t1);
        auto mesh2 = CreateMeshFromTriangle(t2);

        fcl::CollisionObjectd obj1(mesh1);
        fcl::CollisionObjectd obj2(mesh2);

        fcl::CollisionRequestd request;
        fcl::CollisionResultd result;

        return fcl::collide(&obj1, &obj2, request, result);
    }

private:
    std::shared_ptr<fcl::BVHModel<fcl::OBBRSSd>> CreateMeshFromTriangle(const Triangle& t) {
        auto mesh = std::make_shared<fcl::BVHModel<fcl::OBBRSSd>>();

        mesh->beginModel(1, 3);
        mesh->addTriangle(t.vertices[0], t.vertices[1], t.vertices[2]);
        mesh->endModel();

        return mesh;
    }
};

std::vector<Triangle> ReadTrianglesFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("File opening error");
    }

    std::string line;

    std::getline(file, line);
    std::stringstream ss(line);
    size_t number_of_triangles = 0;
    ss >> number_of_triangles;

    std::vector<Triangle> triangles;
    triangles.reserve(number_of_triangles);

    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') {
            continue;
        }

        std::vector<double> points;
        std::stringstream ss(line);
        double value;

        while (ss >> value) {
            points.push_back(value);
        }

        if (points.size() != 9) {
            throw std::runtime_error(
                "Invalid line, expected 9 numbers, got "
                + std::to_string(points.size()) + ": " + line
            );
        }

        std::vector<fcl::Vector3d> vertices {
            fcl::Vector3d(points[0], points[1], points[2]),
            fcl::Vector3d(points[3], points[4], points[5]),
            fcl::Vector3d(points[6], points[7], points[8])
        };

        triangles.emplace_back(vertices);
    }

    return triangles;
}
