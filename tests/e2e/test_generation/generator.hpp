#include <stdexcept>
#include <fstream>
#include <iomanip>
#include <random>
#include <vector>

class Genetator {
private:
    std::random_device rd;
    std::mt19937 gen;
    std::uniform_real_distribution<double> distr;

    double min_value;
    double max_value;

public: 
    Genetator(double min_val, double max_val)
        : gen(rd()), distr(min_val, max_val), min_value(min_val), max_value(max_val) {}

    std::vector<double> GenereteTriangle() {
        std::vector<double> triangle;
        triangle.reserve(9);

        for (size_t i = 0; i != 9; ++i) {
            triangle.push_back(distr(gen));
        }

        return triangle;
    }

    std::vector<std::vector<double>> GenereteTriangles(size_t count) {
        std::vector<std::vector<double>> triangles;
        triangles.reserve(count);

        for (size_t i = 0; i != count; ++i) {
            triangles.push_back(GenereteTriangle());
        }

        return triangles;
    }

    bool SaveToFile(const std::vector<std::vector<double>> triangles, const std::string& filename) {
        std::ofstream file(filename);
        if (!file.is_open()) {
            return false;
        }

        file << triangles.size() << "\n";

        file << std::fixed << std::setprecision(6);

        for (const auto& triangle : triangles) {
            for (size_t i = 0; i != 9; ++i) {
                file << triangle[i] << ((i < triangle.size() - 1) ? " " : "");
            }
            file << "\n";
        }

        return true;
    }

    bool GenerateAndSave(size_t count, const std::string& filename = "test.dat") {
        auto triangles = GenereteTriangles(count);
        return SaveToFile(triangles, filename);
    }

    void SetRange(double min_val, double max_val) {
        min_value = min_val;
        max_value = max_val;
        distr = std::uniform_real_distribution<double>(min_val, max_val);
    }
};
