#include "triangle.hpp"

#include <stdexcept>
#include <array>

#include "segment.hpp"
#include "constants.hpp"

TriangleType Triangle::DetermineType() const {
    return this->GetNormal() != Constants::null_vec
        ? TriangleType::Normal
        : p0_ == p1_ && p1_ == p2_
            ? TriangleType::Point
            : TriangleType::Segment;
}

Segment Triangle::ToSegment() const {
    if (this->GetType() != TriangleType::Segment) {
        throw std::runtime_error(""); // TODO
    }

    double vector_lengths[] {(p1_ - p0_).Length(), (p2_ - p1_).Length(), (p2_ - p0_).Length()};

    if (std::abs(vector_lengths[0] + vector_lengths[1] - vector_lengths[2]) < Constants::kEpsilon) {
        return {p0_, p2_};
    }

    if (std::abs(vector_lengths[0] + vector_lengths[2] - vector_lengths[1]) < Constants::kEpsilon) {
        return {p1_, p2_};
    }

    return {p0_, p1_};
}

Vector Triangle::ToVector() const {
    Segment segment = this->ToSegment();
    return segment.p0 - segment.p1;
}

std::tuple<double, double, double> Triangle::IntersectRayTriangle(const Point& p, const Vector& vec, const Triangle& triangle) {
    Vector v0 = triangle.p0_.AsVector();
    Vector v1 = triangle.p1_.AsVector();
    Vector v2 = triangle.p2_.AsVector();

    Vector D = vec;
    Vector E1 = v1 - v0;
    Vector E2 = v2 - v0;
    Vector T = p.AsVector() - v0;
    Vector P = Vector::Cross(D, E2);
    Vector Q = Vector::Cross(T, E1);

    double denom = Vector::Dot(P, E1);
    
    std::cout << "Vector::Dot(Q, E2) = " << Vector::Dot(Q, E2) << "\n";

    double t = Vector::Dot(Q, E2) / denom;
    double u = Vector::Dot(P, T) / denom;
    double v = Vector::Dot(Q, D) / denom;

    std::cout << "t = " << t << ", u = " << u << ", v = " << v << "\n";

    return {t, u, v};
}

bool Triangle::Contains(const Point& p) const {
    Vector U = p1_ - p0_;
    Vector V = p2_ - p0_;
    Vector W = p - p0_;

    double UU = Vector::Dot(U, U);
    double UV = Vector::Dot(U, V);
    double VV = Vector::Dot(V, V);
    double WU = Vector::Dot(W, U);
    double WV = Vector::Dot(W, V);

    double denom = UV * UV - UU * VV;

    double beta = (UV * WV - VV * WU) / denom;
    double gamma = (UV * WU - UU * WV) / denom;
    double alpha = 1 - beta - gamma;

    return alpha >= -Constants::kEpsilon && beta >= -Constants::kEpsilon && gamma >= -Constants::kEpsilon;
}

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

bool Triangle::IntersectDegenerateTriangles(const Triangle& t1, const Triangle& t2) {
    if (t1.GetType() == TriangleType::Point && t2.GetType() == TriangleType::Point) {
        return t1.p0_ == t2.p0_;
    }

    if (t1.GetType() == TriangleType::Point && t2.GetType() == TriangleType::Segment ||
        t1.GetType() == TriangleType::Segment && t2.GetType() == TriangleType::Point)
    {
        Segment segment;
        Point point;

        if (t1.GetType() == TriangleType::Segment) {
            segment = t1.ToSegment();
            point = t2.p0_;
        } else {
            segment = t2.ToSegment();
            point = t1.p0_;
        }
        
        Vector vectors_to_point[] = {point - segment.p0, point - segment.p1};

        return std::abs(vectors_to_point[0].Length() + vectors_to_point[1].Length() - segment.Length()) < Constants::kEpsilon;
    }

    if (t1.GetType() == TriangleType::Segment && t2.GetType() == TriangleType::Segment) {
        Segment s1 = t1.ToSegment();
        Segment s2 = t2.ToSegment();

        Vector v1 = s1.p1 - s1.p0;
        Vector v2 = s2.p1 - s2.p0;

        Vector diff = s2.p0 - s1.p0;

        Vector N = Vector::Cross(v1, v2);

        if (N == Constants::null_vec) {
            if (Vector::Cross(diff, v1) != Constants::null_vec) {
                return false;
            }

            double denom = Vector::Dot(v1, v1);

            double t0 = Vector::Dot(diff, v1) / denom;
            double t1 = Vector::Dot(s2.p1 - s1.p0, v1) / denom;
            
            double proj_min = std::min(t0, t1);
            double proj_max = std::max(t0, t1);

            return std::max(proj_min, 0.0) <= std::min(proj_max, 1.0) + Constants::kEpsilon;
        }

        double dist = std::abs(Vector::Dot(diff, N));
        if (dist > Constants::kEpsilon) {
            return false;
        }
        double denom = Vector::Dot(N, N);
        double t = Vector::Dot(Vector::Cross(diff, v2), N) / denom;
        double s = Vector::Dot(Vector::Cross(diff, v1), N) / denom;

        return t >= 0 - Constants::kEpsilon && t <= 1 + Constants::kEpsilon
            && s >= 0 - Constants::kEpsilon && s <= 1 + Constants::kEpsilon;
    }

    if (t1.GetType() == TriangleType::Normal && t2.GetType() == TriangleType::Point ||
        t1.GetType() == TriangleType::Point && t2.GetType() == TriangleType::Normal)
    {
        const Triangle* triangle;
        const Triangle* degenerate_triangle;
        
        if (t1.GetType() == TriangleType::Normal) {
            triangle = &t1;
            degenerate_triangle = &t2;
        } else {
            triangle = &t2;
            degenerate_triangle = &t1;
        }

        if (std::abs(Vector::Dot(triangle->p0_ - degenerate_triangle->p0_, triangle->GetNormal())) >= Constants::kEpsilon) {
            return false;
        }

        Segment segments[] {
            {triangle->p0_, triangle->p1_},
            {triangle->p0_, triangle->p2_},
            {triangle->p2_, triangle->p1_} 
        };

        if (std::abs(Vector::Dot(triangle->GetNormal(), Constants::Basis::z)) > Constants::kEpsilon) { 
            return triangle->Contains(Constants::Planes::xy, *degenerate_triangle)
                || segments[0].OnSegment(Constants::Planes::xy, degenerate_triangle->p0_)
                || segments[1].OnSegment(Constants::Planes::xy, degenerate_triangle->p0_)
                || segments[2].OnSegment(Constants::Planes::xy, degenerate_triangle->p0_);
        } 
        
        if (std::abs(Vector::Dot(triangle->GetNormal(), Constants::Basis::y)) > Constants::kEpsilon) {
            return triangle->Contains(Constants::Planes::xz, *degenerate_triangle)
                || segments[0].OnSegment(Constants::Planes::xz, degenerate_triangle->p0_)
                || segments[1].OnSegment(Constants::Planes::xz, degenerate_triangle->p0_)
                || segments[2].OnSegment(Constants::Planes::xz, degenerate_triangle->p0_);
        } 
        
        if (std::abs(Vector::Dot(triangle->GetNormal(), Constants::Basis::x)) > Constants::kEpsilon) {
            return triangle->Contains(Constants::Planes::yz, *degenerate_triangle)
                || segments[0].OnSegment(Constants::Planes::yz, degenerate_triangle->p0_)
                || segments[1].OnSegment(Constants::Planes::yz, degenerate_triangle->p0_)
                || segments[2].OnSegment(Constants::Planes::yz, degenerate_triangle->p0_);
        }

        throw std::runtime_error(""); // TODO
    }

    if (t1.GetType() == TriangleType::Normal && t2.GetType() == TriangleType::Segment ||
        t1.GetType() == TriangleType::Segment && t2.GetType() == TriangleType::Normal)
    {
        const Triangle* triangle;
        const Triangle* degenerate_triangle;
        
        if (t1.GetType() == TriangleType::Normal) {
            triangle = &t1;
            degenerate_triangle = &t2;
        } else {
            triangle = &t2;
            degenerate_triangle = &t1;
        }

        Segment segment = degenerate_triangle->ToSegment();
        Point p = segment.p0;
        Vector vec = segment.p0 - segment.p1;

        if (std::abs(Vector::Dot(triangle->GetNormal(), vec)) <= Constants::kEpsilon) { 
            if (Vector::Dot(p - triangle->p0_, triangle->GetNormal())) { 
                return false;
            }
            
            if (Segment::Intersect({triangle->p0_, triangle->p1_}, segment)) return true;
            if (Segment::Intersect({triangle->p1_, triangle->p2_}, segment)) return true;
            if (Segment::Intersect({triangle->p2_, triangle->p0_}, segment)) return true;

            if (triangle->Contains(segment.p0)) return true;
            if (triangle->Contains(segment.p1)) return true;

            return false;
        }

        auto [t, u, v] = IntersectRayTriangle(p, vec, *triangle);

        return u >= 0 && v >= 0 && 1 - u - v >= 0 && std::abs(t) <= 1;
    }

    throw std::runtime_error(""); // TODO
}

bool Triangle::Intersect(const Triangle& t1, const Triangle& t2) {
    if (t1.GetType() != TriangleType::Normal || t2.GetType() != TriangleType::Normal) {
        return IntersectDegenerateTriangles(t1, t2);
    }

    auto relative_planes_position = RelativePlanesPosition(t1, t2);

    if (relative_planes_position == PlanesPosition::Parallel) { // std::cout << "_________\n"; // NOTE debug
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
