#include "triangle.hpp"

#include <array>

#include "segment.hpp"

bool Triangle::Contains(const Triangle& other) const {
    Segment edges_of_big_triangle[] {
        {p0_, p1_},
        {p1_, p2_},
        {p2_, p0_},
    };

    std::array<double, 9> directions;

    for (size_t i = 0; i != 9; ++i) {
        directions[i] = edges_of_big_triangle[i / 3].Direction(other[i % 3]);
    }

    size_t number_of_positive = 0;
    size_t number_of_negative = 0;

    for (double d : directions) {
        number_of_positive += d > 0;
        number_of_negative += d < 0;
    }

    return number_of_positive == directions.size() || number_of_negative == directions.size();
}

PlanesPosition Triangle::RelativePlanesPosition(const Triangle& t1, const Triangle& t2) {
    Vector3D normal1 = Vector3D::Cross(t1.p1_ - t1.p0_, t1.p2_ - t1.p1_);
    Vector3D normal2 = Vector3D::Cross(t2.p1_ - t2.p0_, t2.p2_ - t2.p1_);

    if (!normal1.Collinear(normal2)) {
        return PlanesPosition::Intersect;
    }

    double d1 = -Vector3D::Dot(normal1, t1.p0_.AsVector());
    double d2 = -Vector3D::Dot(normal2, t2.p0_.AsVector());

    bool normals_same_direction = Vector3D::Dot(normal1, normal2) > 0;

    double distance_between_planes = normals_same_direction
        ? std::abs(d1 - d2)
        : std::abs(d1 + d2);

    return distance_between_planes < std::numeric_limits<double>::epsilon()
        ? PlanesPosition::Coincide
        : PlanesPosition::Parallel;
}

bool Triangle::Intersect(const Triangle& t1, const Triangle& t2) {
    if (RelativePlanesPosition(t1, t2) == PlanesPosition::Parallel) {
        return false;
    }

    if (RelativePlanesPosition(t1, t2) == PlanesPosition::Coincide) {
        Segment edges1[] {{t1.p0_, t1.p1_}, {t1.p0_, t1.p2_}, {t1.p1_, t1.p2_}};
        Segment edges2[] {{t2.p0_, t2.p1_}, {t2.p0_, t2.p2_}, {t2.p1_, t2.p2_}};

        if (Segment::Intersect(edges1, edges2) || t1.Contains(t2) || t2.Contains(t1)) {
            return true;
        }

        return false;
    }

    return SAT(t1, t2);
}

bool Triangle::SAT(const Triangle& a, const Triangle& b) {
    auto ProjectionsOverlap = [a, b](const Vector3D& axis) {
        if (axis.Length() < std::numeric_limits<double>::epsilon()) {
            return true;
        }

        auto [a_min, a_max] = a.Project(axis);
        auto [b_min, b_max] = b.Project(axis);

        return !(a_max < b_min - std::numeric_limits<double>::epsilon() || b_max < a_min - std::numeric_limits<double>::epsilon());
    };

    Vector3D a_vectors[] {a.p1_ - a.p0_, a.p2_ - a.p1_, a.p0_ - a.p2_};
    Vector3D b_vectors[] {b.p1_ - b.p0_, b.p2_ - b.p1_, b.p0_ - b.p2_};

    Vector3D a_normal = Vector3D::Cross(a_vectors[0], a_vectors[1]);
    if (!ProjectionsOverlap(a_normal)) {
        return false;
    }

    Vector3D b_normal = Vector3D::Cross(b_vectors[0], b_vectors[1]);
    if (!ProjectionsOverlap(b_normal)) {
        return false;
    }

    for (size_t i = 0; i != 9; ++i) {
        if (!ProjectionsOverlap(Vector3D::Cross(a_vectors[i / 3], b_vectors[i % 3]))) {
            return false;
        }
    }

    // Vector3D a_vectors[] {a.p1_ - a.p0_, a.p2_ - a.p1_, a.p0_ - a.p2_};
    // Vector3D b_vectors[] {b.p1_ - b.p0_, b.p2_ - b.p1_, b.p0_ - b.p2_};
    // Vector3D separating_axes[] {
    //     Vector3D::Cross(a_vectors[0], a_vectors[1]),
    //     Vector3D::Cross(b_vectors[0], b_vectors[1]),

    //     Vector3D::Cross(a_vectors[0], b_vectors[0]),
    //     Vector3D::Cross(a_vectors[0], b_vectors[1]),
    //     Vector3D::Cross(a_vectors[0], b_vectors[2]),

    //     Vector3D::Cross(a_vectors[1], b_vectors[0]),
    //     Vector3D::Cross(a_vectors[1], b_vectors[1]),
    //     Vector3D::Cross(a_vectors[1], b_vectors[2]),

    //     Vector3D::Cross(a_vectors[2], b_vectors[0]),
    //     Vector3D::Cross(a_vectors[2], b_vectors[1]),
    //     Vector3D::Cross(a_vectors[2], b_vectors[2]),
    // };

    // for (const Vector3D& axis : separating_axes) {
    //     auto [a_min, a_max] = a.Project(axis);
    //     auto [b_min, b_max] = b.Project(axis);

    //     double min = std::min(a_min, b_min);
    //     double max = std::max(a_max, b_max);

    //     if (a_max - a_min + b_max - b_min < max - min) {
    //         return false;
    //     }
    // }

    return true;
}

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

