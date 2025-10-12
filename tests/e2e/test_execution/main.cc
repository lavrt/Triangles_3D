#include <vector>
#include <sstream>
#include <fstream>

#include "triangle.hpp"

namespace {

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

        triangles.emplace_back(
            0,
            Point{points[0], points[1], points[2]},
            Point{points[3], points[4], points[5]},
            Point{points[6], points[7], points[8]}
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

    std::vector<Triangle> triangles = ReadTrianglesFromFile("./tests/e2e/test_data/" + data);

    std::ifstream s("./tests/e2e/test_data/" + answers_filename);
    if (!s.is_open()) {
        throw std::runtime_error("File opening error");
    }

    std::vector<bool> answers;
    answers.reserve(triangles.size());
    for (bool res = 0; s >> res;) {
        answers.push_back(res);
    }

    auto it = answers.begin();    
    for (size_t i = 0; i != triangles.size(); ++i) {
        for (size_t j = i + 1; j < triangles.size(); ++j) {
            if (Triangle::Intersect(triangles[i], triangles[j]) != *(it++)) {
                std::cerr << "Error:\n"
                          << "Expected: " << (*(it - 1) ? "HIT" : "MISS") << "\n"
                          << "Got: " << (!*(it - 1) ? "HIT" : "MISS") << "\n"
                          << "i=" << i << ", j=" << j << "\n";
                return 1;
            };
        }
    }

    std::cout << data << " passed\n";

    return 0;
}
