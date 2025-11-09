#include <vector>
#include <sstream>
#include <fstream>

#include "triangle.hpp"
#include "bvh.hpp"

namespace {

std::vector<Geometry::Triangle<double>> ReadTrianglesFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("File opening error");
    }

    std::string line;

    std::getline(file, line);
    std::stringstream ss(line);
    size_t number_of_triangles = 0;
    ss >> number_of_triangles;

    std::vector<Geometry::Triangle<double>> triangles;
    triangles.reserve(number_of_triangles);

    size_t id = 0;
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

        triangles.emplace_back(
            id++,
            Geometry::Point{points[0], points[1], points[2]},
            Geometry::Point{points[3], points[4], points[5]},
            Geometry::Point{points[6], points[7], points[8]}
        );
    }

    return triangles;
}

} // namespace

int main(int argc, char** argv) {
    std::vector<std::string> args(argv, argv + argc);

    std::string data;
    std::string answers_filename;
    for (size_t i = 1; i < args.size(); ++i) {
        if (args[i] == "--data") {
            data = args[++i];
        } else if (args[i] == "--answers") {
            answers_filename = args[++i];
        }
    }
    
    std::vector<Geometry::Triangle<double>> triangles = ReadTrianglesFromFile("./tests/e2e/test_data/" + data);

    size_t triangles_size = triangles.size();
    Geometry::Acceleration::BVH<double> bvh{std::move(triangles)};
    auto intersections = bvh.FindIntersectingTriangles();

    std::ifstream s("./tests/e2e/test_data/" + answers_filename);
    if (!s.is_open()) {
        throw std::runtime_error("File opening error");
    }

    std::vector<size_t> answers;
    answers.reserve(triangles_size);
    for (size_t res = 0; s >> res;) {
        answers.push_back(res);
    }

    auto it_ref = answers.begin();
    auto it_our = intersections.begin();
    
    for (size_t i = 0; i != answers.size(); ++i) {
        if (*it_our != *it_ref) {
            std::cerr << "*it_our=" << *it_our << " *it_ref=" << *it_ref << "\n";
            std::cerr << "Error\n";
        }
        ++it_our;
        ++it_ref;
    }

    std::cout << data << " passed\n";

    return 0;
}
