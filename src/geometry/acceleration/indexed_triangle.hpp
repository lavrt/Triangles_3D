#pragma once

#include <cstddef>

#include "triangle.hpp"

namespace geometry {

namespace acceleration {

using TrIndex = size_t;

template <typename T>
struct IndexedTriangle {
    TrIndex id;
    Triangle<T> triangle;
};

} // namespace acceleration

} // namespace geometry
