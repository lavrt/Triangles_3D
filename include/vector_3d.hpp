#pragma once

struct Vector3D {
    double x;
    double y;
    double z;

    static Vector3D Cross(const Vector3D& a, const Vector3D& b);
    static double Dot(const Vector3D& a, const Vector3D& b);

    Vector3D Normalized() const;
    double Length() const;
    bool Collinear(const Vector3D& other) const;
};
