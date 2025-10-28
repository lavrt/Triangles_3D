#pragma once

#include <span>
#include <cmath>

#include "point.hpp"
#include "details.hpp"

namespace Geometry {

template <typename T> // TODO concept
struct Segment {
    Point<T> p0;
    Point<T> p1;

    T Length() const {
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
        Vector<T> v1 = s1.p1 - s1.p0;
        Vector<T> v2 = s2.p1 - s2.p0;
        Vector<T> diff = s2.p0 - s1.p0;

        Vector<T> N = Vector<T>::Cross(v1, v2);

        if (N.IsNullVector()) {
            if (!Vector<T>::Cross(diff, v1).IsNullVector()) {
                return false;
            }

            T denom = Vector<T>::Dot(v1, v1);
            T t0 = Vector<T>::Dot(diff, v1) / denom;
            T t1 = Vector<T>::Dot(s2.p1 - s1.p0, v1);
            T proj_min = std::min(t0, t1);
            T proj_max = std::max(t0, t1);

            return std::max(proj_min, 0.0) <= std::min(proj_max, 1.0) + Constants::kEpsilon;
        }

        if (std::abs(Vector<T>::Dot(diff, N)) >= Constants::kEpsilon) {
            return false;
        }

        T denom = Vector<T>::Dot(N, N);
        T t = Vector<T>::Dot(Vector<T>::Cross(diff, v2), N) / denom;
        T s = Vector<T>::Dot(Vector<T>::Cross(diff, v1), N) / denom;

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

} // namespace Geometry
