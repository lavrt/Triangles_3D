#include "triangle.hpp"

#include <array>

#include "segment.hpp"
#include "constants.hpp"

Point Triangle::operator[](size_t i) const {
    switch (i) {
        case 0: return p0_;
        case 1: return p1_;
        case 2: return p2_;
        
        default: throw std::out_of_range("Incorrect access to fields of the class: Triangle");
    }
}

AABB Triangle::ComputeBoundingBox(const std::span<Triangle>& triangles) {
    AABB aabb;

    for (const auto& triangle : triangles) {
        aabb.Expand(triangle.GetAABB());
    }

    return aabb;
}

bool Triangle::Contains(const std::pair<size_t, size_t>& plane, const Triangle& other) const {
    static constexpr size_t kNumberOfChecks = 9;

    Segment edges_of_big_triangle[] {{p0_, p1_}, {p1_, p2_}, {p2_, p0_}};

    size_t number_of_positive = 0;
    size_t number_of_negative = 0;

    for (size_t i = 0; i != kNumberOfChecks; ++i) {
        edges_of_big_triangle[i / 3].Direction(plane, other[i % 3]) > 0
            ? ++number_of_positive
            : ++number_of_negative;
    }

    return number_of_positive == kNumberOfChecks || number_of_negative == kNumberOfChecks;
}

PlanesPosition Triangle::RelativePlanesPosition(const Triangle& t1, const Triangle& t2) {
    Vector normal1 = t1.GetNormal();
    Vector normal2 = t2.GetNormal();
    
    if (!normal1.Collinear(normal2)) {
        return PlanesPosition::Intersect;
    }

    double d1 = -Vector::Dot(normal1, t1.p0_.AsVector());
    double d2 = -Vector::Dot(normal2, t2.p0_.AsVector());

    double distance_between_planes = (Vector::Dot(normal1, normal2) > 0)
        ? std::abs(d1 - d2)
        : std::abs(d1 + d2);

    return distance_between_planes < Constants::kEpsilon
        ? PlanesPosition::Coincide
        : PlanesPosition::Parallel;
}

bool Triangle::Intersect(const Triangle& t1, const Triangle& t2) {
    auto relative_planes_position = RelativePlanesPosition(t1, t2);

    if (relative_planes_position == PlanesPosition::Parallel) {
        return false;
    }

    if (relative_planes_position == PlanesPosition::Coincide) { 
        Segment edges1[] {{t1.p0_, t1.p1_}, {t1.p0_, t1.p2_}, {t1.p1_, t1.p2_}};
        Segment edges2[] {{t2.p0_, t2.p1_}, {t2.p0_, t2.p2_}, {t2.p1_, t2.p2_}};       

        if (Vector::Dot(t1.GetNormal(), Constants::Basis::z)) { 
            return Segment::IntersectInPlane(Constants::Planes::xy, edges1, edges2) ||
                t1.Contains(Constants::Planes::xy, t2) || t2.Contains(Constants::Planes::xy, t1);
        } 
        
        if (Vector::Dot(t1.GetNormal(), Constants::Basis::y)) {
            return Segment::IntersectInPlane(Constants::Planes::xz, edges1, edges2) ||
                t1.Contains(Constants::Planes::xz, t2) || t2.Contains(Constants::Planes::xz, t1);
        } 
        
        if (Vector::Dot(t1.GetNormal(), Constants::Basis::x)) {
            return Segment::IntersectInPlane(Constants::Planes::yz, edges1, edges2) ||
                t1.Contains(Constants::Planes::yz, t2) || t2.Contains(Constants::Planes::yz, t1);
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

        return !(a_max < b_min - Constants::kEpsilon || b_max < a_min - Constants::kEpsilon);
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
