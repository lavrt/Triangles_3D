#pragma once

#include <format>
#include <vector>
#include <stdexcept>

#include "indexed_triangle.hpp"
#include "point.hpp"

namespace app {

template <typename T>
std::vector<Geometry::Acceleration::IndexedTriangle<T>> ParseInput(std::istream& stream) {
    size_t n = 0;
    stream >> n;

    if (!stream.good()) {
        throw std::runtime_error("Input error: expected number of triangles");
    }

    std::vector<Geometry::Acceleration::IndexedTriangle<T>> triangles;
    triangles.reserve(n);

    Geometry::Point<T> p0, p1, p2;
    for (size_t i = 0; i != n; ++i) {
        stream >> p0.x >> p0.y >> p0.z
               >> p1.x >> p1.y >> p1.z
               >> p2.x >> p2.y >> p2.z;

        if (!stream.good()) {
            throw std::runtime_error(std::format("Input error on the triangle {}", i));
        }

        triangles.emplace_back(i, Geometry::Triangle{p0, p1, p2});
    }

    return triangles;
}

} // namespace app
