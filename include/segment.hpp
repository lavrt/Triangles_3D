#pragma once

#include <span>

#include "point.hpp"

struct Segment {
    Point p0;
    Point p1;

    static bool IntersectInPlane(const std::pair<size_t, size_t>& plane, const std::span<Segment>& edges1, const std::span<Segment>& edges2);

    double Direction(const std::pair<size_t, size_t>& plane, const Point& p) const;
    bool OnSegment(const std::pair<size_t, size_t>& plane, const Point& p) const;
    static bool IntersectInPlane(const std::pair<size_t, size_t>& plane, const Segment& s1, const Segment& s2);
};
