#include "segment.hpp"

#include "constants.hpp"

bool Segment::IntersectInPlane(const Segment& s1, const Segment& s2) {
    double d1 = s2.Direction(s1.p0);
    double d2 = s2.Direction(s1.p1);
    double d3 = s1.Direction(s2.p0);
    double d4 = s1.Direction(s2.p1);

    if (((d1 > Constants::kEpsilon && d2 < -Constants::kEpsilon)
        || (d1 < -Constants::kEpsilon && d2 > Constants::kEpsilon))
        && ((d3 > Constants::kEpsilon && d4 < -Constants::kEpsilon)
        || (d3 < -Constants::kEpsilon && d4 > Constants::kEpsilon)))
    {
        return true;
    } 

    if (std::abs(d1) < Constants::kEpsilon && s2.OnSegment(s1.p0)) return true;
    if (std::abs(d2) < Constants::kEpsilon && s2.OnSegment(s1.p1)) return true;
    if (std::abs(d3) < Constants::kEpsilon && s1.OnSegment(s2.p0)) return true;
    if (std::abs(d4) < Constants::kEpsilon && s1.OnSegment(s2.p1)) return true;

    return false;
}

bool Segment::IntersectInPlane(std::span<Segment> edges1, std::span<Segment> edges2) {
    for (const Segment& s1 : edges1) {
        for (const Segment& s2 : edges2) {
            if (IntersectInPlane(s1, s2)) {
                return true;
            }
        }
    }
    return false;
}

double Segment::Direction(const Point& p) const {
    return (p1.x - p0.x) * (p.y - p0.y) - (p1.y - p0.y) * (p.x - p0.x);
}

bool Segment::OnSegment(const Point& p) const {
    return std::min(p0.x, p1.x) <= p.x && p.x <= std::max(p0.x, p1.x)
        && std::min(p0.y, p1.y) <= p.y && p.y <= std::max(p0.y, p1.y);
}
