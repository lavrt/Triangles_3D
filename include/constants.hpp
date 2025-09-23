#pragma once

#include <limits>

namespace Constants {
    inline constexpr double kEpsilon = 1e-12;
    inline constexpr double kMaxDouble = std::numeric_limits<double>::max();
    inline constexpr double kLowestDouble = std::numeric_limits<double>::lowest();
}
