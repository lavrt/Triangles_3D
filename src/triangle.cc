#include "triangle.hpp"

#include <array>

#include "segment.hpp"
#include "constants.hpp"

Point Triangle::operator[](size_t i) const {
    switch (i) {
        case 0: return p0_;
        case 1: return p1_;
        case 2: return p2_;
        
        default: throw std::runtime_error("Incorrect access to fields of the class: Triangle");
    }
}

AABB Triangle::ComputeBoundingBox(std::span<Triangle> triangles) {
    AABB aabb;

    for (const auto& triangle : triangles) {
        aabb.Expand(triangle.GetAABB());
    }

    return aabb;
}

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
    // NOTE можно написать получше, необязательно directions[]
    return number_of_positive == directions.size() || number_of_negative == directions.size();
}

PlanesPosition Triangle::RelativePlanesPosition(const Triangle& t1, const Triangle& t2) {
    Vector normal1 = t1.GetNormal();
    Vector normal2 = t2.GetNormal();

    if (!normal1.Collinear(normal2)) {
        return PlanesPosition::Intersect;
    }

    double d1 = -Vector::Dot(normal1, t1.p0_.AsVector());
    double d2 = -Vector::Dot(normal2, t2.p0_.AsVector());

    bool normals_same_direction = Vector::Dot(normal1, normal2) > 0;

    double distance_between_planes = normals_same_direction
        ? std::abs(d1 - d2)
        : std::abs(d1 + d2);

    return distance_between_planes < Constants::kEpsilon
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

        if (Segment::IntersectInPlane(edges1, edges2) || t1.Contains(t2) || t2.Contains(t1)) {
            return true;
        }

        return false;
    }

    return SAT(t1, t2);
}

bool Triangle::SAT(const Triangle& a, const Triangle& b) {
    auto ProjectionsOverlap = [a, b](const Vector& axis) {
        if (axis.Length() < Constants::kEpsilon) {
            return true;
        }

        auto [a_min, a_max] = a.Project(axis);
        auto [b_min, b_max] = b.Project(axis);

        return !(a_max < b_min - Constants::kEpsilon
            || b_max < a_min - Constants::kEpsilon);
    };

    Vector a_vectors[] {a.p1_ - a.p0_, a.p2_ - a.p1_, a.p0_ - a.p2_};
    Vector b_vectors[] {b.p1_ - b.p0_, b.p2_ - b.p1_, b.p0_ - b.p2_};

    if (!ProjectionsOverlap(a.GetNormal())) {
        return false;
    }

    if (!ProjectionsOverlap(b.GetNormal())) {
        return false;
    }

    for (size_t i = 0; i != 9; ++i) {
        if (!ProjectionsOverlap(Vector::Cross(a_vectors[i / 3], b_vectors[i % 3]))) {
            return false;
        }
    }

    return true;
}

std::pair<double, double> Triangle::Project(const Vector& axis) const {
    Vector normalized_axis = axis.Normalized();

    std::array<double, 3> projections {
        Vector::Dot(p0_.AsVector(), normalized_axis),
        Vector::Dot(p1_.AsVector(), normalized_axis),
        Vector::Dot(p2_.AsVector(), normalized_axis),
    };

    return {
        *std::min_element(projections.begin(), projections.end()),
        *std::max_element(projections.begin(), projections.end()),
    };
}

