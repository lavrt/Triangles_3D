#pragma once

#include <span>
#include <cmath>

#include "point.hpp"
#include "constants.hpp"

struct Segment {
    Point p0;
    Point p1;

    double Length() const {
        return (p1 - p0).Length();
    }

    static bool Intersect(const Segment& s1, const Segment& s2) {
        Vector v1 = s1.p1 - s1.p0;
        Vector v2 = s2.p1 - s2.p0;
        Vector diff = s2.p0 - s1.p0;

        Vector N = Vector::Cross(v1, v2);

        if (N == Constants::null_vec) {
            Vector cross_diff_v1 = Vector::Cross(diff, v1);
            if (cross_diff_v1 != Constants::null_vec) {
                return false;
            }
            double denom = Vector::Dot(v1, v1);
            double t0 = Vector::Dot(diff, v1) / denom;
            double t1 = Vector::Dot(s2.p1 - s1.p0, v1);
            double proj_min = std::min(t0, t1);
            double proj_max = std::max(t0, t1);
            return std::max(proj_min, 0.0) <= std::min(proj_max, 1.0) + Constants::kEpsilon;
        }

        double dist = std::abs(Vector::Dot(diff, N)) / N.Length();
        if (dist >= Constants::kEpsilon) {
            return false;
        }

        double denom = Vector::Dot(N, N);
        double t = Vector::Dot(Vector::Cross(diff, v2), N) / denom;
        double s = Vector::Dot(Vector::Cross(diff, v1), N) / denom;

        return t >= -Constants::kEpsilon && t <= 1 + Constants::kEpsilon
            && s >= -Constants::kEpsilon && s <= 1 + Constants::kEpsilon;
    }

    static bool IntersectInPlane(const std::pair<size_t, size_t>& plane,
        const std::span<Segment>& edges1, const std::span<Segment>& edges2)
    {
        for (const Segment& s1 : edges1) {
            for (const Segment& s2 : edges2) {
                if (IntersectInPlane(plane, s1, s2)) {
                    return true;
                }
            }
        }
        return false;
    }

    double Direction(const std::pair<size_t, size_t>& plane, const Point& p) const {
        return (p1[plane.first] - p0[plane.first]) * (p[plane.second] - p0[plane.second])
            - (p1[plane.second] - p0[plane.second]) * (p[plane.first] - p0[plane.first]);
    }


    bool OnSegment(const std::pair<size_t, size_t>& plane, const Point& p) const {
        return std::min(p0[plane.first], p1[plane.first]) <= p[plane.first]
            && p[plane.first] <= std::max(p0[plane.first], p1[plane.first])
            && std::min(p0[plane.second], p1[plane.second]) <= p[plane.second]
            && p[plane.second] <= std::max(p0[plane.second], p1[plane.second]);
    }

    static bool IntersectInPlane(const std::pair<size_t, size_t>& plane,
        const Segment& s1, const Segment& s2)
    {
        double d1 = s2.Direction(plane, s1.p0);
        double d2 = s2.Direction(plane, s1.p1);
        double d3 = s1.Direction(plane, s2.p0);
        double d4 = s1.Direction(plane, s2.p1);

        if (((d1 > Constants::kEpsilon && d2 < -Constants::kEpsilon)
            || (d1 < -Constants::kEpsilon && d2 > Constants::kEpsilon))
            && ((d3 > Constants::kEpsilon && d4 < -Constants::kEpsilon)
            || (d3 < -Constants::kEpsilon && d4 > Constants::kEpsilon)))
        {
            return true;
        } 

        if (std::abs(d1) < Constants::kEpsilon && s2.OnSegment(plane, s1.p0)) return true;
        if (std::abs(d2) < Constants::kEpsilon && s2.OnSegment(plane, s1.p1)) return true;
        if (std::abs(d3) < Constants::kEpsilon && s1.OnSegment(plane, s2.p0)) return true;
        if (std::abs(d4) < Constants::kEpsilon && s1.OnSegment(plane, s2.p1)) return true;

        return false;
    }
};
