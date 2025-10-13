#pragma once

#include <span>
#include <tuple>
#include <algorithm>

#include "segment.hpp"
#include "point.hpp"
#include "aabb.hpp"

enum class PlanesPosition {
    Parallel, Coincide, Intersect
};

enum class TriangleType {
    Normal, Point, Segment
};

class Triangle { // TODO можно не хранить нормаль и сделать поля public
private:
    size_t id_;

    Point p0_;
    Point p1_;
    Point p2_;

    AABB aabb_;
    Vector normal_;
    TriangleType type_;

    static bool IntersectDegenerate(const Triangle& t1, const Triangle& t2);
    static bool Intersect(const Point& p1, const Point& p2);
    static bool Intersect(const Point& p, const Segment& s);
    static bool Intersect(const Segment& s1, const Segment& s2);
    static bool Intersect(const Triangle& t, const Point& p);
    static bool Intersect(const Triangle& t, const Segment& s);

public:
    Triangle(size_t id, Point p0, Point p1, Point p2) 
        : id_(id), p0_(p0), p1_(p1), p2_(p2),
          type_(this->DetermineType()),
          normal_(Vector::Cross(p1_ - p0_, p2_ - p1_).Normalized()),
          aabb_({std::min({p0.x, p1.x, p2.x}), std::min({p0.y, p1.y, p2.y}), std::min({p0.z, p1.z, p2.z})}, 
                {std::max({p0.x, p1.x, p2.x}), std::max({p0.y, p1.y, p2.y}), std::max({p0.z, p1.z, p2.z})})
    {}
    
    /**
     * @brief The main function for checking the intersection of triangles
     * 
     * Checks whether triangles intersect in 3D, supports processing of degenerate triangles
     * 
     * @param t1 First triangle
     * @param t2 Second triangle
     * @return true if the triangles intersect
     */
    static bool Intersect(const Triangle& t1, const Triangle& t2);

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
    static bool SAT(const Triangle& t1, const Triangle& t2);

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
    bool Contains(const Point& p) const;

    static PlanesPosition RelativePlanesPosition(const Triangle& t1, const Triangle& t2);
    std::pair<double, double> Project(const Vector& axis) const;

    static AABB ComputeBoundingBox(const std::span<Triangle>& triangles) {
        AABB aabb;

        for (const auto& triangle : triangles) {
            aabb.Expand(triangle.GetAABB());
        }

        return aabb;
    }

    Point ToPoint() const {
        if (p0_ != p1_ || p1_ != p2_ || p2_ != p0_) {
            throw std::runtime_error("The triangle is not degenerate into a point");
        }
        
        return p0_;
    }

    Segment ToSegment() const {
        if (this->GetType() != TriangleType::Segment) {
            throw std::runtime_error("The triangle is not degenerate into a line segment");
        }

        double vector_lengths[] {(p1_ - p0_).Length(), (p2_ - p1_).Length(), (p2_ - p0_).Length()};

        if (std::abs(vector_lengths[0] + vector_lengths[1] - vector_lengths[2]) < Constants::kEpsilon) {
            return {p0_, p2_};
        }

        if (std::abs(vector_lengths[0] + vector_lengths[2] - vector_lengths[1]) < Constants::kEpsilon) {
            return {p1_, p2_};
        }

        return {p0_, p1_};
    }

    Vector ToVector() const {
        Segment segment = this->ToSegment();
        return segment.p0 - segment.p1;
    }

    TriangleType DetermineType() const {
        return normal_ != Constants::null_vec
            ? TriangleType::Normal
            : p0_ == p1_ && p1_ == p2_
                ? TriangleType::Point
                : TriangleType::Segment;
    }

    Point operator[](size_t i) const {
        switch (i) {
            case 0: return p0_;
            case 1: return p1_;
            case 2: return p2_;
            
            default: throw std::out_of_range("Incorrect access to fields of the class: Triangle");
        }
    }

    TriangleType GetType() const noexcept {
        return type_;
    }

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
