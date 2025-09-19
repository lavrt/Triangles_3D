#ifndef VEC3_HPP
#define VEC3_HPP

struct Vector3D {
    double x_;
    double y_;
    double z_;

    Vector3D Normalized() const;
    double Length() const;

    static Vector3D Cross(const Vector3D& a, const Vector3D& b);
    static double Dot(const Vector3D& a, const Vector3D& b);
};

#endif // VEC3_HPP
