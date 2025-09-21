#pragma once

#include <iostream>
#include <algorithm>

#include "point_3d.hpp"
#include "aabb.hpp"

enum class PlanesPosition {
    Parallel, Coincide, Intersect
};

class Triangle {
private:
    size_t id_;

    Point3D p0_;
    Point3D p1_;
    Point3D p2_;

    AABB aabb_;
    Vector3D normal_;

public:
    Triangle(size_t id, Point3D p0, Point3D p1, Point3D p2) 
        : id_(id), p0_(p0), p1_(p1), p2_(p2)
    {
        aabb_ = {
            Point3D{
                std::min({p0.x, p1.x, p2.x}),
                std::min({p0.y, p1.y, p2.y}),
                std::min({p0.z, p1.z, p2.z})
            },
            Point3D{
                std::max({p0.x, p1.x, p2.x}),
                std::max({p0.y, p1.y, p2.y}),
                std::max({p0.z, p1.z, p2.z})
            }
        };

        normal_ = Vector3D::Cross(p1_ - p0_, p2_ - p1_);
    }

    static bool Intersect(const Triangle& tr1, const Triangle& tr2);
    static bool SAT(const Triangle& t1, const Triangle& t2);
    static PlanesPosition RelativePlanesPosition(const Triangle& t1, const Triangle& t2);

    std::pair<double, double> Project(const Vector3D& axis) const;
    bool Contains(const Triangle& other) const;
    Point3D operator[](size_t i) const;

    size_t GetId() const noexcept {
        return id_;
    }

    AABB GetAABB() const noexcept {
        return aabb_;
    }

    Vector3D GetNormal() const noexcept {
        return normal_;
    }
};
