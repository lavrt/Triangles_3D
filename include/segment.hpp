#pragma once

#include <limits>
#include <span>

#include "point_3d.hpp"

struct Segment {
    Point3D p0;
    Point3D p1;

    static bool Intersect(const Segment& s1, const Segment& s2);

    static bool Intersect(std::span<Segment> edges1, std::span<Segment> edges2);

    double Direction(const Point3D& p) const;

    bool OnSegment(const Point3D& p) const;
};
