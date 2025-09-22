#pragma once

struct Vector {
    double x;
    double y;
    double z;

    Vector(double x, double y, double z) : x(x), y(y), z(z) {}

    static Vector Cross(const Vector& a, const Vector& b);
    static double Dot(const Vector& a, const Vector& b);

    Vector Normalized() const;
    double Length() const;
    bool Collinear(const Vector& other) const;
};
