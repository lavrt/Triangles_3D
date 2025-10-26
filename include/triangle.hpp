#pragma once

#include <span>
#include <tuple>
#include <algorithm>
#include <stdexcept>
#include <array>
#include <variant>

#include "segment.hpp"
#include "point.hpp"
// #include "aabb.hpp" // FIXME

namespace Geometry {

enum class PlanesPosition {
    Parallel, Coincide, Intersect
};

enum class TriangleType {
    Normal, Point, Segment
};

template <typename T>
class Triangle;

template <typename T>
using Shape = std::variant<Triangle<T>, Segment<T>, Point<T>>;

template <typename T>
class Triangle { // TODO можно не хранить нормаль и сделать поля public
public:
    size_t id_;

    Point<T> p0_;
    Point<T> p1_;
    Point<T> p2_;

    // AABB<T> aabb_;
    // Vector<T> normal_;
    // TriangleType type_;
    
    static bool Intersect(const Point<T>& p1, const Point<T>& p2) {
        return p1 == p2; // return Dot(p1 - p2, p1 - p2) < ;
    }

    static bool Intersect(const Point<T>& p, const Segment<T>& s) {
        return std::abs((p - s.p0).Length() + (p - s.p1).Length() - s.Length()) < Constants::kEpsilon;
    }

    static bool Intersect(const Segment<T>& s, const Point<T>& p) {
        return Intersect(p, s);
    }

    static bool Intersect(const Segment<T>& s1, const Segment<T>& s2) {
        Vector<T> v1 = s1.p1 - s1.p0;
        Vector<T> v2 = s2.p1 - s2.p0;

        Vector<T> diff = s2.p0 - s1.p0;

        Vector<T> N = Vector<T>::Cross(v1, v2);

        if (N.IsNullVector()) {
            if (!Vector<T>::Cross(diff, v1).IsNullVector()) {
                return false;
            }

            T denom = Vector<T>::Dot(v1, v1);

            T t0 = Vector<T>::Dot(diff, v1) / denom;
            T t1 = Vector<T>::Dot(s2.p1 - s1.p0, v1) / denom;
            
            T proj_min = std::min(t0, t1);
            T proj_max = std::max(t0, t1);

            return std::max(proj_min, 0.0) <= std::min(proj_max, 1.0) + Constants::kEpsilon;
        }

        T dist = std::abs(Vector<T>::Dot(diff, N));
        if (dist > Constants::kEpsilon) {
            return false;
        }
        T denom = Vector<T>::Dot(N, N);
        T t = Vector<T>::Dot(Vector<T>::Cross(diff, v2), N) / denom;
        T s = Vector<T>::Dot(Vector<T>::Cross(diff, v1), N) / denom;

        return t >= 0 - Constants::kEpsilon && t <= 1 + Constants::kEpsilon
            && s >= 0 - Constants::kEpsilon && s <= 1 + Constants::kEpsilon;
    }

    static bool Intersect(const Triangle& t, const Point<T>& p) {
        if (std::abs(Vector<T>::Dot(t.p0_ - p, t.CalculateNormal())) >= Constants::kEpsilon) {
            return false;
        }

        return t.Contains(p);
    }

    static bool Intersect(const Point<T>& p, const Triangle& t) {
        return Intersect(t, p);
    } 

    static bool Intersect(const Triangle& t, const Segment<T>& s) {
        Point<T> p = s.p0;
        Vector<T> vec = s.p0 - s.p1;

        if (std::abs(Vector<T>::Dot(t.CalculateNormal(), vec)) <= Constants::kEpsilon) { 
            if (Vector<T>::Dot(p - t.p0_, t.CalculateNormal())) { 
                return false;
            }
            
            if (Segment<T>::Intersect({t.p0_, t.p1_}, s)) return true;
            if (Segment<T>::Intersect({t.p1_, t.p2_}, s)) return true;
            if (Segment<T>::Intersect({t.p2_, t.p0_}, s)) return true;

            if (t.Contains(s.p0)) return true;
            if (t.Contains(s.p1)) return true;

            return false;
        }

        Vector<T> v0 = t.p0_.AsVector();
        Vector<T> v1 = t.p1_.AsVector();
        Vector<T> v2 = t.p2_.AsVector();

        Vector<T> D = vec;
        Vector<T> E1 = v1 - v0;
        Vector<T> E2 = v2 - v0;
        Vector<T> K = p.AsVector() - v0;
        Vector<T> P = Vector<T>::Cross(D, E2);
        Vector<T> Q = Vector<T>::Cross(K, E1);

        T denom = Vector<T>::Dot(P, E1);

        T k = Vector<T>::Dot(Q, E2) / denom;
        T u = Vector<T>::Dot(P, K) / denom;
        T v = Vector<T>::Dot(Q, D) / denom;

        return u >= 0 && v >= 0 && 1 - u - v >= 0 && std::abs(k) <= 1;
    }

    static bool Intersect(const Segment<T>& s, const Triangle& t) {
        return Intersect(t, s);
    }

// NOTE here was the end of the public fields

    Triangle(size_t id, Point<T> p0, Point<T> p1, Point<T> p2) 
        : id_(id), p0_(p0), p1_(p1), p2_(p2) {}

    Shape<T> MakeShape() const {
        switch (this->DetermineType()) {
            case TriangleType::Normal:  return *this;
            case TriangleType::Point:   return this->ToPoint();
            case TriangleType::Segment: return this->ToSegment();
            default: throw std::runtime_error("Unknown triangle type");
        }
    }
    
    /**
     * @brief The main function for checking the intersection of triangles
     * 
     * Checks whether triangles intersect in 3D, supports processing of degenerate triangles
     * 
     * @param t1 First triangle
     * @param t2 Second triangle
     * @return true if the triangles intersect
     */
    static bool Intersect(const Triangle& t1, const Triangle& t2) {
        if (t1.DetermineType() != TriangleType::Normal || t2.DetermineType() != TriangleType::Normal) {
            return std::visit([](const auto& s1, const auto& s2) {
                return Intersect(s1, s2);
            }, t1.MakeShape(), t2.MakeShape());
        }

        auto relative_planes_position = RelativePlanesPosition(t1, t2);

        if (relative_planes_position == PlanesPosition::Parallel) {
            return false;
        }

        if (relative_planes_position == PlanesPosition::Coincide) { 
            Segment<T> edges1[] {{t1.p0_, t1.p1_}, {t1.p0_, t1.p2_}, {t1.p1_, t1.p2_}};
            Segment<T> edges2[] {{t2.p0_, t2.p1_}, {t2.p0_, t2.p2_}, {t2.p1_, t2.p2_}};

            return Segment<T>::Intersect(edges1, edges2) || t1.Contains(t2) || t2.Contains(t1); 
        }

        return SAT(t1, t2);
    }

    /**
     * @brief Checks whether two non-planar triangles intersect
     * 
     * Implements the separating axis theorem (SAT)
     * 
     * LIMITATIONS:
     * - It is intended only for checking for the intersection of two triangles located in
     * intersecting planes
     * 
     * @param t1 First triangle
     * @param t2 Second triangle
     * @return true if the triangles intersect
     */
    static bool SAT(const Triangle& a, const Triangle& b) {
        auto ProjectionsOverlap = [a, b](const Vector<T>& axis) {
            if (axis.Length() < Constants::kEpsilon) {
                return true;
            }

            auto [a_min, a_max] = a.Project(axis);
            auto [b_min, b_max] = b.Project(axis);

            return !(a_max < b_min - Constants::kEpsilon || b_max < a_min - Constants::kEpsilon);
        };

        Vector<T> a_vectors[] {a.p1_ - a.p0_, a.p2_ - a.p1_, a.p0_ - a.p2_};
        Vector<T> b_vectors[] {b.p1_ - b.p0_, b.p2_ - b.p1_, b.p0_ - b.p2_};

        if (!ProjectionsOverlap(a.CalculateNormal())) {
            return false;
        }

        if (!ProjectionsOverlap(b.CalculateNormal())) {
            return false;
        }

        for (size_t i = 0; i != 9; ++i) {
            if (!ProjectionsOverlap(Vector<T>::Cross(a_vectors[i / 3], b_vectors[i % 3]))) {
                return false;
            }
        }

        return true;
    }

    /**
     * @brief Checks whether all points of the "other" triangle are in the "this" triangle
     * 
     * LIMITATIONS:
     * - It is intended only for checking the nesting of triangles located in the same plane
     * 
     * @param other Second triangle
     * @return true if all points of the "other" triangle are inside the "this" triangle
     */
    bool Contains(const Triangle& other) const {
        return this->Contains(other.p0_) && this->Contains(other.p1_) && this->Contains(other.p2_);
    }

    /**
     * @brief Point-in-triangle test for 2D triangles
     * 
     * Uses barycentric coordinate method:
     * - Represents point p as: p = alpha*p0 + beta*p1 + gamma*p2
     * - Where alpha + beta + gamma = 1 and alpha,beta,gamma >= 0 for interior points
     * 
     * LIMITATIONS:
     * - It is intended only for checking the intersection of a triangle with a point located in
     *   the plane of the triangle. If the point is not in the plane of the triangle, it may give
     *   a false positive result because the point is projected onto the triangle.
     * 
     * @param p Test point
     * @return true if the point is located inside the boundaries of the triangle or on the boundary
     */
    bool Contains(const Point<T>& p) const {
        Vector<T> U = p1_ - p0_;
        Vector<T> V = p2_ - p0_;
        Vector<T> W = p - p0_;

        T UU = Vector<T>::Dot(U, U);
        T UV = Vector<T>::Dot(U, V);
        T VV = Vector<T>::Dot(V, V);
        T WU = Vector<T>::Dot(W, U);
        T WV = Vector<T>::Dot(W, V);

        T denom = UV * UV - UU * VV;

        T beta = (UV * WV - VV * WU) / denom;
        T gamma = (UV * WU - UU * WV) / denom;
        T alpha = 1 - beta - gamma;

        return alpha >= -Constants::kEpsilon && beta >= -Constants::kEpsilon
            && gamma >= -Constants::kEpsilon;
    }

    static PlanesPosition RelativePlanesPosition(const Triangle& t1, const Triangle& t2) {
        Vector<T> normal1 = t1.CalculateNormal();
        Vector<T> normal2 = t2.CalculateNormal();
        
        if (!normal1.Collinear(normal2)) {
            return PlanesPosition::Intersect;
        }

        T d1 = -Vector<T>::Dot(normal1, t1.p0_.AsVector());
        T d2 = -Vector<T>::Dot(normal2, t2.p0_.AsVector());

        T distance_between_planes = (Vector<T>::Dot(normal1, normal2) > 0)
            ? std::abs(d1 - d2)
            : std::abs(d1 + d2);

        return distance_between_planes < Constants::kEpsilon
            ? PlanesPosition::Coincide
            : PlanesPosition::Parallel;
    }

    std::pair<T, T> Project(const Vector<T>& axis) const {
        Vector<T> normalized_axis = axis.Normalized();

        std::array<T, 3> projections {
            Vector<T>::Dot(p0_.AsVector(), normalized_axis),
            Vector<T>::Dot(p1_.AsVector(), normalized_axis),
            Vector<T>::Dot(p2_.AsVector(), normalized_axis),
        };

        return {
            *std::min_element(projections.begin(), projections.end()),
            *std::max_element(projections.begin(), projections.end()),
        };
    }

    Point<T> ToPoint() const {
        if (p0_ != p1_ || p1_ != p2_ || p2_ != p0_) {
            throw std::runtime_error("The triangle is not degenerate into a point");
        }
        
        return p0_;
    }

    Segment<T> ToSegment() const {
        if (this->DetermineType() != TriangleType::Segment) { // NOTE ?
            throw std::runtime_error("The triangle is not degenerate into a line segment");
        }

        T vector_lengths[] {(p1_ - p0_).Length(), (p2_ - p1_).Length(), (p2_ - p0_).Length()};

        if (std::abs(vector_lengths[0] + vector_lengths[1] - vector_lengths[2]) < Constants::kEpsilon) {
            return {p0_, p2_};
        }

        if (std::abs(vector_lengths[0] + vector_lengths[2] - vector_lengths[1]) < Constants::kEpsilon) {
            return {p1_, p2_};
        }

        return {p0_, p1_};
    }

    Vector<T> ToVector() const {
        Segment segment = this->ToSegment();
        return segment.p0 - segment.p1;
    }

    TriangleType DetermineType() const {
        return !this->CalculateNormal().IsNullVector()
            ? TriangleType::Normal
            : p0_ == p1_ && p1_ == p2_
                ? TriangleType::Point
                : TriangleType::Segment;
    }

    Point<T> operator[](size_t i) const {
        switch (i) {
            case 0: return p0_;
            case 1: return p1_;
            case 2: return p2_;
            
            default: throw std::out_of_range("Incorrect access to fields of the class: Triangle");
        }
    }

    size_t GetId() const noexcept {
        return id_;
    }

    Vector<T> CalculateNormal() const { // NOTE new function
        return Vector<T>::Cross(p1_ - p0_, p2_ - p1_).Normalized();
    }

    // AABB<T> CalculateAABB() const {
    //     AABB aabb{*this};
    //     return aabb;
    // }
};

} // namespace Geometry
