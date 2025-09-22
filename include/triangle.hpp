#pragma once

#include <iostream>
#include <algorithm>

#include "point.hpp"
#include "aabb.hpp"

enum class PlanesPosition {
    Parallel, Coincide, Intersect
};

class Triangle {
private:
    size_t id_;

    Point p0_;
    Point p1_;
    Point p2_;

    AABB aabb_;
    Vector normal_;

public:
    Triangle(size_t id, Point p0, Point p1, Point p2) 
        : id_(id), p0_(p0), p1_(p1), p2_(p2), normal_(Vector::Cross(p1_ - p0_, p2_ - p1_)) // NOTE
    {
        aabb_ = {
            Point{
                std::min({p0.x, p1.x, p2.x}),
                std::min({p0.y, p1.y, p2.y}),
                std::min({p0.z, p1.z, p2.z})
            },
            Point{
                std::max({p0.x, p1.x, p2.x}),
                std::max({p0.y, p1.y, p2.y}),
                std::max({p0.z, p1.z, p2.z})
            }
        };
    }

    static bool Intersect(const Triangle& tr1, const Triangle& tr2);
    static bool SAT(const Triangle& t1, const Triangle& t2);
    static PlanesPosition RelativePlanesPosition(const Triangle& t1, const Triangle& t2);

    std::pair<double, double> Project(const Vector& axis) const;
    bool Contains(const Triangle& other) const;
    Point operator[](size_t i) const;

    size_t GetId() const noexcept {
        return id_;
    }

    AABB GetAABB() const noexcept {
        return aabb_;
    }

    Vector GetNormal() const noexcept {
        return normal_;
    }
};
