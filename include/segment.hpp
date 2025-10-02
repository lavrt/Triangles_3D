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

    /**
     * @brief Checks if two non-degenerate line segments intersect in 3D space
     * 
     * LIMITATIONS:
     * - It is designed to check the intersection of only non-degenerate segments
     * 
     * @param s1 First segment to test
     * @param s2 Second segment to test
     * @return true if segments intersect (including endpoints touching), false otherwise
     */
    static bool Intersect(const Segment& s1, const Segment& s2) {
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
            double t1 = Vector::Dot(s2.p1 - s1.p0, v1);
            double proj_min = std::min(t0, t1);
            double proj_max = std::max(t0, t1);

            return std::max(proj_min, 0.0) <= std::min(proj_max, 1.0) + Constants::kEpsilon;
        }

        if (std::abs(Vector::Dot(diff, N)) >= Constants::kEpsilon) {
            return false;
        }

        double denom = Vector::Dot(N, N);
        double t = Vector::Dot(Vector::Cross(diff, v2), N) / denom;
        double s = Vector::Dot(Vector::Cross(diff, v1), N) / denom;

        return t >= -Constants::kEpsilon && t <= 1 + Constants::kEpsilon
            && s >= -Constants::kEpsilon && s <= 1 + Constants::kEpsilon;
    }

    /**
     * @brief Checks whether at least one segment from the set "edges1" intersects with any segment
     * of the set "edges2"
     * 
     * @param edges1 First set
     * @param edges2 Second set
     * @return true if at least one triangle of the set "edges1" intersects with any triangle of
     * the set "edges2"
     */
    static bool Intersect(const std::span<Segment>& edges1, const std::span<Segment>& edges2)
    {
        for (const Segment& s1 : edges1) {
            for (const Segment& s2 : edges2) {
                if (Intersect(s1, s2)) {
                    return true;
                }
            }
        }
        return false;
    }
};
