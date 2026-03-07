#pragma once

#include "triangle.hpp"

namespace Geometry {

namespace Acceleration {

template <typename T>
struct IndexedTriangle {
    size_t id;
    Triangle<T> triangle;
};

} // namespace Acceleration

} // namespace Geometry
