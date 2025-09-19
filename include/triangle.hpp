#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include <iostream>
#include <algorithm>

#include "point_3d.hpp"
#include "aabb.hpp"

class Triangle {
private:
    size_t id_;

    Point3D p0_;
    Point3D p1_;
    Point3D p2_;

    AABB aabb_;

public:
    Triangle(size_t id, Point3D p0, Point3D p1, Point3D p2) 
        : id_(id), p0_(p0), p1_(p1), p2_(p2),
          aabb_{
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
          }
    {}

    std::pair<double, double> Project(const Vector3D& axis) const;

    static bool Intersect(const Triangle& tr1, const Triangle& tr2);

    AABB GetAABB() const {
        return aabb_;
    }
};

#endif // TRIANGLE_HPP
