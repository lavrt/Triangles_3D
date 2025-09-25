#include <gtest/gtest.h>

#include <cmath>

#include "triangle.hpp"

class TriangleTest : public ::testing::Test {
protected:
    Point p0 = Point{0, 0, 0};
    Point p1 = Point{1, 0, 0};
    Point p2 = Point{0, 1, 0};
    Triangle triangle = Triangle(1, p0, p1, p2);
};

// Constructor -------------------------------------------------------------------------------------

TEST_F(TriangleTest, ConstructorAndGetters) {
    EXPECT_EQ(triangle.GetId(), 1);
    
    EXPECT_EQ(triangle[0].x, 0);
    EXPECT_EQ(triangle[0].y, 0);
    EXPECT_EQ(triangle[0].z, 0);
    
    EXPECT_EQ(triangle[1].x, 1);
    EXPECT_EQ(triangle[1].y, 0);
    EXPECT_EQ(triangle[1].z, 0);
    
    EXPECT_EQ(triangle[2].x, 0);
    EXPECT_EQ(triangle[2].y, 1);
    EXPECT_EQ(triangle[2].z, 0);
}

TEST_F(TriangleTest, NormalCalculation) {
    Vector normal = triangle.GetNormal();

    EXPECT_DOUBLE_EQ(normal.x, 0);
    EXPECT_DOUBLE_EQ(normal.y, 0);
    EXPECT_DOUBLE_EQ(std::abs(normal.z), 1);
}

TEST_F(TriangleTest, AABBComputation) {
    AABB bbox = triangle.GetAABB();
    
    EXPECT_EQ(bbox.min.x, 0);
    EXPECT_EQ(bbox.min.y, 0);
    EXPECT_EQ(bbox.min.z, 0);
    
    EXPECT_EQ(bbox.max.x, 1);
    EXPECT_EQ(bbox.max.y, 1);
    EXPECT_EQ(bbox.max.z, 0);
}

// Project -----------------------------------------------------------------------------------------

TEST_F(TriangleTest, ProjectionOnAxis) {
    Vector x_axis{1, 0, 0};
    auto projection = triangle.Project(x_axis);
    
    EXPECT_DOUBLE_EQ(projection.first, 0);
    EXPECT_DOUBLE_EQ(projection.second, 1);
    
    Vector y_axis{0, 1, 0};
    projection = triangle.Project(y_axis);
    EXPECT_DOUBLE_EQ(projection.first, 0);
    EXPECT_DOUBLE_EQ(projection.second, 1);
    
    Vector diagonal_axis{1, 1, 0};
    projection = triangle.Project(diagonal_axis);
    EXPECT_DOUBLE_EQ(projection.first, 0);
    EXPECT_DOUBLE_EQ(projection.second, 1 / std::sqrt(2));
}

// RelativePlanesPosition --------------------------------------------------------------------------

TEST(RelativePlanesPositionTest, ParallelPlanes) {
    Triangle t1(1, Point{0,0,0}, Point{1,0,0}, Point{0,1,0});
    Triangle t2(2, Point{0,0,1}, Point{1,0,1}, Point{0,1,1});
    
    auto position = Triangle::RelativePlanesPosition(t1, t2);
    EXPECT_EQ(position, PlanesPosition::Parallel);
}

TEST(RelativePlanesPositionTest, CoincidentPlanes) {
    Triangle t1(1, Point{0,0,0}, Point{1,0,0}, Point{0,1,0});
    Triangle t2(2, Point{0,0,0}, Point{0,1,0}, Point{1,0,0});
    
    auto position = Triangle::RelativePlanesPosition(t1, t2);
    EXPECT_EQ(position, PlanesPosition::Coincide);
}

TEST(RelativePlanesPositionTest, IntersectingPlanes) {
    Triangle t1(1, Point{0,0,0}, Point{1,0,0}, Point{0,1,0});
    Triangle t2(2, Point{0,0,0}, Point{0,1,0}, Point{0,0,1});
    
    auto position = Triangle::RelativePlanesPosition(t1, t2);
    EXPECT_EQ(position, PlanesPosition::Intersect);
}

// SAT ---------------------------------------------------------------------------------------------

TEST(TriangleIntersectionTest, NonIntersectingTriangles) {
    Triangle above(2, Point{0,0,1}, Point{1,0,1}, Point{0,1,1});
    Triangle xy_plane{1, Point{0,0,0}, Point{1,0,0}, Point{0,1,0}};
    
    EXPECT_FALSE(Triangle::Intersect(xy_plane, above));
    EXPECT_FALSE(Triangle::SAT(xy_plane, above));
}

TEST(TriangleIntersectionTest, PerpendicularIntersectingTriangles) {
    Triangle perpendicular(2, Point{0.2,0.2,0.5}, Point{0.2,0.2,-0.5}, Point{0.8,0.8,0});
    Triangle xy_plane{1, Point{0,0,0}, Point{1,0,0}, Point{0,1,0}};
    
    EXPECT_TRUE(Triangle::Intersect(xy_plane, perpendicular));
    EXPECT_TRUE(Triangle::SAT(xy_plane, perpendicular));
}

// ComputeBoundingBox ------------------------------------------------------------------------------

TEST(ComputeBoundingBoxTest, SingleTriangle) {
    Triangle t(1, Point{0,0,0}, Point{2,0,0}, Point{0,3,0});
    std::vector<Triangle> triangles = {t};
    
    AABB bbox = Triangle::ComputeBoundingBox(triangles);
    
    EXPECT_EQ(bbox.min.x, 0);
    EXPECT_EQ(bbox.min.y, 0);
    EXPECT_EQ(bbox.min.z, 0);
    EXPECT_EQ(bbox.max.x, 2);
    EXPECT_EQ(bbox.max.y, 3);
    EXPECT_EQ(bbox.max.z, 0);
}

TEST(ComputeBoundingBoxTest, MultipleTriangles) {
    std::vector<Triangle> triangles = {
        Triangle(1, Point{0,0,0}, Point{1,0,0}, Point{0,1,0}),
        Triangle(2, Point{2,2,1}, Point{3,2,1}, Point{2,3,1}),
        Triangle(3, Point{-1,-1,-1}, Point{0,-1,-1}, Point{-1,0,-1})
    };
    
    AABB bbox = Triangle::ComputeBoundingBox(triangles);
    
    EXPECT_EQ(bbox.min.x, -1);
    EXPECT_EQ(bbox.min.y, -1);
    EXPECT_EQ(bbox.min.z, -1);
    EXPECT_EQ(bbox.max.x, 3);
    EXPECT_EQ(bbox.max.y, 3);
    EXPECT_EQ(bbox.max.z, 1);
}

// Special cases -----------------------------------------------------------------------------------

TEST(TriangleEdgeCasesTest, ContainsMethod) {
    Triangle large(1, Point{0,0,0}, Point{3,0,0}, Point{0,3,0});
    Triangle small(2, Point{0.5,0.5,0}, Point{1.5,0.5,0}, Point{0.5,1.5,0});
    
    bool result = large.Contains(Constants::Planes::xy, small);
    EXPECT_TRUE(result);
}
