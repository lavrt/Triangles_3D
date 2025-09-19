#include "triangle.hpp"

#include <array>

std::pair<double, double> Triangle::Project(const Vector3D& axis) const {
    Vector3D normalized_axis = axis.Normalized();

    std::array<double, 3> projections {
        Vector3D::Dot(p0_.AsVector(), normalized_axis),
        Vector3D::Dot(p1_.AsVector(), normalized_axis),
        Vector3D::Dot(p2_.AsVector(), normalized_axis),
    };

    return {
        *std::min_element(projections.begin(), projections.end()),
        *std::max_element(projections.begin(), projections.end()),
    };
}

bool Triangle::Intersect(const Triangle& a, const Triangle& b) {
    Vector3D a_vectors[] {
        a.p1_ - a.p0_,
        a.p2_ - a.p1_,
        a.p0_ - a.p2_,
    };

    Vector3D b_vectors[] {
        b.p1_ - b.p0_,
        b.p2_ - b.p1_,
        b.p0_ - b.p2_,
    };

    Vector3D separating_axes[] {
        Vector3D::Cross(a_vectors[0], a_vectors[1]),
        Vector3D::Cross(b_vectors[0], b_vectors[1]),

        Vector3D::Cross(a_vectors[0], b_vectors[0]),
        Vector3D::Cross(a_vectors[0], b_vectors[1]),
        Vector3D::Cross(a_vectors[0], b_vectors[2]),

        Vector3D::Cross(a_vectors[1], b_vectors[0]),
        Vector3D::Cross(a_vectors[1], b_vectors[1]),
        Vector3D::Cross(a_vectors[1], b_vectors[2]),

        Vector3D::Cross(a_vectors[2], b_vectors[0]),
        Vector3D::Cross(a_vectors[2], b_vectors[1]),
        Vector3D::Cross(a_vectors[2], b_vectors[2]),
    };

    for (const Vector3D& axis : separating_axes) {
        auto [a_min, a_max] = a.Project(axis);
        auto [b_min, b_max] = b.Project(axis);

        double min = std::min(a_min, b_min);
        double max = std::max(a_max, b_max);

        if (a_max - a_min + b_max - b_min < max - min) {
            return false;
        }
    }

    return true;
}
