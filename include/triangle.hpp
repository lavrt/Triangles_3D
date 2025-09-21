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
    Triangle(size_t id, Point3D p0, Point3D p1, Point3D p2);

    std::pair<double, double> Project(const Vector3D& axis) const;
    bool Contains(const Triangle& other) const;

    static bool Intersect(const Triangle& tr1, const Triangle& tr2);
    static bool SAT(const Triangle& t1, const Triangle& t2);
    static PlanesPosition RelativePlanesPosition(const Triangle& t1, const Triangle& t2);

    size_t GetId() const {
        return id_;
    }

    AABB GetAABB() const {
        return aabb_;
    }

    Vector3D GetNormal() const {
        return normal_;
    }

    Point3D operator[](size_t i) const;
};
