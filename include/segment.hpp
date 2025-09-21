#pragma once

#include <limits>
#include <span>

#include "point.hpp"

struct Segment {
    Point p0;
    Point p1;

    static bool Intersect(const Segment& s1, const Segment& s2);
    static bool Intersect(std::span<Segment> edges1, std::span<Segment> edges2);

    double Direction(const Point& p) const;
    bool OnSegment(const Point& p) const;
};
