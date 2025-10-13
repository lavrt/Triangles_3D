#include "triangle.hpp"

#include <stdexcept>
#include <array>

#include "segment.hpp"
#include "constants.hpp"

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

    return alpha >= -Constants::kEpsilon && beta >= -Constants::kEpsilon
        && gamma >= -Constants::kEpsilon;
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
    if (t1.GetType() != TriangleType::Normal || t2.GetType() != TriangleType::Normal) {
        return IntersectDegenerate(t1, t2);
    }

    auto relative_planes_position = RelativePlanesPosition(t1, t2);

    if (relative_planes_position == PlanesPosition::Parallel) {
        return false;
    }

    if (relative_planes_position == PlanesPosition::Coincide) { 
        Segment edges1[] {{t1.p0_, t1.p1_}, {t1.p0_, t1.p2_}, {t1.p1_, t1.p2_}};
        Segment edges2[] {{t2.p0_, t2.p1_}, {t2.p0_, t2.p2_}, {t2.p1_, t2.p2_}};

        return Segment::Intersect(edges1, edges2) || t1.Contains(t2) || t2.Contains(t1); 
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

// Triangle::Degenerate ----------------------------------------------------------------------------

bool Triangle::IntersectDegenerate(const Triangle& t1, const Triangle& t2) {
    if (t1.GetType() == TriangleType::Point && t2.GetType() == TriangleType::Point) {
        return Intersect(t1.ToPoint(), t2.ToPoint());
    }

    if (t1.GetType() == TriangleType::Point && t2.GetType() == TriangleType::Segment) {
        return Intersect(t1.ToPoint(), t2.ToSegment());
    }

    if (t1.GetType() == TriangleType::Segment && t2.GetType() == TriangleType::Point) {
        return Intersect(t2.ToPoint(), t1.ToSegment());
    }

    if (t1.GetType() == TriangleType::Segment && t2.GetType() == TriangleType::Segment) {
        return Intersect(t1.ToSegment(), t2.ToSegment());
    }

    if (t1.GetType() == TriangleType::Normal && t2.GetType() == TriangleType::Point) {
        return Intersect(t1, t2.ToPoint());
    }

    if (t1.GetType() == TriangleType::Point && t2.GetType() == TriangleType::Normal) {
        return Intersect(t2, t1.ToPoint());
    }

    if (t1.GetType() == TriangleType::Normal && t2.GetType() == TriangleType::Segment) {
        return Intersect(t1, t2.ToSegment());
    }

    if (t1.GetType() == TriangleType::Segment && t2.GetType() == TriangleType::Normal) {
        return Intersect(t2, t1.ToSegment());
    }

    throw std::runtime_error("Triangles are not degenerate");
}

bool Triangle::Intersect(const Point& p1, const Point& p2) {
    return p1 == p2;
}

bool Triangle::Intersect(const Point& p, const Segment& s) {
    return std::abs((p - s.p0).Length() + (p - s.p1).Length() - s.Length()) < Constants::kEpsilon;
}

bool Triangle::Intersect(const Segment& s1, const Segment& s2) {
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

bool Triangle::Intersect(const Triangle& t, const Point& p) {
    if (std::abs(Vector::Dot(t.p0_ - p, t.GetNormal())) >= Constants::kEpsilon) {
        return false;
    }

    return t.Contains(p);
}

bool Triangle::Intersect(const Triangle& t, const Segment& s) {
    Point p = s.p0;
    Vector vec = s.p0 - s.p1;

    if (std::abs(Vector::Dot(t.GetNormal(), vec)) <= Constants::kEpsilon) { 
        if (Vector::Dot(p - t.p0_, t.GetNormal())) { 
            return false;
        }
        
        if (Segment::Intersect({t.p0_, t.p1_}, s)) return true;
        if (Segment::Intersect({t.p1_, t.p2_}, s)) return true;
        if (Segment::Intersect({t.p2_, t.p0_}, s)) return true;

        if (t.Contains(s.p0)) return true;
        if (t.Contains(s.p1)) return true;

        return false;
    }

    Vector v0 = t.p0_.AsVector();
    Vector v1 = t.p1_.AsVector();
    Vector v2 = t.p2_.AsVector();

    Vector D = vec;
    Vector E1 = v1 - v0;
    Vector E2 = v2 - v0;
    Vector T = p.AsVector() - v0;
    Vector P = Vector::Cross(D, E2);
    Vector Q = Vector::Cross(T, E1);

    double denom = Vector::Dot(P, E1);

    double k = Vector::Dot(Q, E2) / denom;
    double u = Vector::Dot(P, T) / denom;
    double v = Vector::Dot(Q, D) / denom;

    return u >= 0 && v >= 0 && 1 - u - v >= 0 && std::abs(k) <= 1;
}
