#pragma once

#include <limits>
#include <concepts>

namespace concepts {

template <typename T>
concept Numeric = std::integral<T> || std::floating_point<T>;

} // namespace concepts

namespace constants {

inline constexpr double kEpsilon = 1e-12;

} // namespace constants

namespace limits {

template <typename T>
requires concepts::Numeric<T>
constexpr T MaxValue() {
    return std::numeric_limits<T>::max();
}

template <typename T>
requires concepts::Numeric<T>
constexpr T LowestValue() {
    return std::numeric_limits<T>::lowest();
}

} // namespace limits
