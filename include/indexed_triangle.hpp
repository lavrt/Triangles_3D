#pragma once

#include <cstddef>

#include "triangle.hpp"

namespace Geometry {

namespace Acceleration {

using TrIndex = size_t;

template <typename T>
struct IndexedTriangle {
    TrIndex id;
    Triangle<T> triangle;
};

} // namespace Acceleration

} // namespace Geometry
