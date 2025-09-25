#pragma once

#include <limits>

namespace Constants {
    inline constexpr double kEpsilon = 1e-12;
    inline constexpr double kMaxDouble = std::numeric_limits<double>::max();
    inline constexpr double kLowestDouble = std::numeric_limits<double>::lowest();

    namespace Planes {
        inline constexpr std::pair<size_t, size_t> xy{0, 1}, xz{0, 2}, yz{1, 2};
    }
    
    namespace Basis {
        inline const Vector x{1, 0, 0}, y{0, 1, 0}, z{0, 0, 1};
    }
}
