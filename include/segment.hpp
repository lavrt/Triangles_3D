#ifndef SEGMENT_HPP
#define SEGMENT_HPP

#include <limits>
#include <span>

#include "point_3d.hpp"

struct Segment {
    Point3D p0;
    Point3D p1;

    static bool Intersect(const Segment& s1, const Segment& s2) {
        double d1 = s2.Direction(s1.p0);
        double d2 = s2.Direction(s1.p1);
        double d3 = s1.Direction(s2.p0);
        double d4 = s1.Direction(s2.p1);

        if (((d1 > kEpsilon && d2 < -kEpsilon) || (d1 < -kEpsilon && d2 > kEpsilon)) &&
            ((d3 > kEpsilon && d4 < -kEpsilon) || (d3 < -kEpsilon && d4 > kEpsilon)))
        {
            return true;
        } 

        if (std::abs(d1) < kEpsilon && s2.OnSegment(s1.p0)) return true;
        if (std::abs(d2) < kEpsilon && s2.OnSegment(s1.p1)) return true;
        if (std::abs(d3) < kEpsilon && s1.OnSegment(s2.p0)) return true;
        if (std::abs(d4) < kEpsilon && s1.OnSegment(s2.p1)) return true;

        return false;
    }

    static bool Intersect(std::span<Segment> edges1, std::span<Segment> edges2) {
        for (const Segment& s1 : edges1) {
            for (const Segment& s2 : edges2) {
                if (Intersect(s1, s2)) {
                    return true;
                }
            }
        }

        return false;
    }

    double Direction(const Point3D& p) const {
        return (p1.x_ - p0.x_) * (p.y_ - p0.y_) - (p1.y_ - p0.y_) * (p.x_ - p0.x_);
    }

    bool OnSegment(const Point3D& p) const {
        return std::min(p0.x_, p1.x_) <= p.x_ && p.x_ <= std::max(p0.x_, p1.x_)
            && std::min(p0.y_, p1.y_) <= p.y_ && p.y_ <= std::max(p0.y_, p1.y_);
    }

private:
    static constexpr double kEpsilon = std::numeric_limits<double>::epsilon();
};

#endif // SEGMENT_HPP