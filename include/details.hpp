#pragma once

#include <limits>
#include <concepts>

namespace Concepts {

template <typename T>
concept Numeric = std::integral<T> || std::floating_point<T>;

} // namespace Concepts

namespace Constants {

inline constexpr double kEpsilon = 1e-12;

} // namespace Constants

namespace Limits {

template <typename T>
constexpr T MaxValue() {
    return std::numeric_limits<T>::max();
}

template <typename T>
constexpr T LowestValue() {
    return std::numeric_limits<T>::lowest();
}

} // namespace Limits
