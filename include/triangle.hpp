#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

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
                std::min({p0.x_, p1.x_, p2.x_}),
                std::min({p0.y_, p1.y_, p2.y_}),
                std::min({p0.z_, p1.z_, p2.z_})
            },
            Point3D{
                std::max({p0.x_, p1.x_, p2.x_}),
                std::max({p0.y_, p1.y_, p2.y_}),
                std::max({p0.z_, p1.z_, p2.z_})
            }
        };

        normal_ = Vector3D::Cross(p1_ - p0_, p2_ - p1_);
    }

    std::pair<double, double> Project(const Vector3D& axis) const;
    bool Contains(const Triangle& other) const;

    static bool Intersect(const Triangle& tr1, const Triangle& tr2);
    static bool SAT(const Triangle& t1, const Triangle& t2);
    static PlanesPosition RelativePlanesPosition(const Triangle& t1, const Triangle& t2);

    AABB GetAABB() const {
        return aabb_;
    }

    Vector3D GetNormal() const {
        return normal_;
    }

    Point3D operator[](size_t i) const {
        switch (i) {
            case 0: return p0_;
            case 1: return p1_;
            case 2: return p2_;
            
            default: throw std::runtime_error("");
        }
    }
};

#endif // TRIANGLE_HPP
