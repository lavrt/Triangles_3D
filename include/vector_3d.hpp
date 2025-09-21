#pragma once

struct Vector3D {
    double x_;
    double y_;
    double z_;

    Vector3D Normalized() const;
    double Length() const;
    bool Collinear(const Vector3D& other) const;

    static Vector3D Cross(const Vector3D& a, const Vector3D& b);
    static double Dot(const Vector3D& a, const Vector3D& b);
};
