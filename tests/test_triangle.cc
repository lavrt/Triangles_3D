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

// Contains ----------------------------------------------------------------------------------------

TEST(TriangleEdgeCasesTest, ContainsMethod) {
    Triangle large(1, Point{0,0,0}, Point{3,0,0}, Point{0,3,0});
    Triangle small(2, Point{0.5,0.5,0}, Point{1.5,0.5,0}, Point{0.5,1.5,0});
    
    bool result = large.Contains(small);
    EXPECT_TRUE(result);
}

// Custom test cases -------------------------------------------------------------------------------

TEST(TriangleSpecificTest, CustomTestCase1) {
    Triangle t0(0, 
        Point{0, 0, 0}, 
        Point{1, 0, 0}, 
        Point{0, 1, 0}
    );
    
    Triangle t1(1, 
        Point{5, 5, 5}, 
        Point{5, 5, 5}, 
        Point{5, 5, 5}
    );
    
    Triangle t2(2, 
        Point{0, 0.5, -0.5}, 
        Point{0, 0.5, 0.5}, 
        Point{-1, 0, 0}
    );
    
    EXPECT_FALSE(Triangle::Intersect(t0, t1));
    EXPECT_TRUE(Triangle::Intersect(t0, t2));   
    EXPECT_FALSE(Triangle::Intersect(t1, t2));
}

// Special cases -----------------------------------------------------------------------------------

TEST(TriangleDegenerateTest, PointTriangleIntersection) {
    Triangle point_triangle(1, Point{0,0,0}, Point{0,0,0}, Point{0,0,0});
    Triangle normal_triangle(2, Point{0,0,0}, Point{1,0,0}, Point{0,1,0});
    
    EXPECT_TRUE(Triangle::Intersect(point_triangle, normal_triangle));
    EXPECT_TRUE(Triangle::Intersect(normal_triangle, point_triangle));
}

TEST(TriangleDegenerateTest, LineSegmentTriangleIntersection) {
    Triangle line_triangle(1, Point{0,0,0}, Point{1,0,0}, Point{0.5,0,0});
    Triangle normal_triangle(2, Point{0,0,0}, Point{1,0,0}, Point{0,1,0});
    
    EXPECT_TRUE(Triangle::Intersect(line_triangle, normal_triangle));
    EXPECT_TRUE(Triangle::Intersect(normal_triangle, line_triangle));
}

TEST(TriangleDegenerateTest, TwoPointTrianglesNoIntersection) {
    Triangle point1(1, Point{0,0,0}, Point{0,0,0}, Point{0,0,0});
    Triangle point2(2, Point{1,1,1}, Point{1,1,1}, Point{1,1,1});
    
    EXPECT_FALSE(Triangle::Intersect(point1, point2));
}

TEST(TriangleDegenerateTest, LineSegmentAndPointIntersection) {
    Triangle line_triangle(1, Point{0,0,0}, Point{2,0,0}, Point{1,0,0});
    Triangle point_triangle(2, Point{1,0,0}, Point{1,0,0}, Point{1,0,0});
    
    EXPECT_TRUE(Triangle::Intersect(line_triangle, point_triangle));
    EXPECT_TRUE(Triangle::Intersect(point_triangle, line_triangle));
}

TEST(TriangleDegenerateTest, CollinearLineSegmentsIntersection) {
    Triangle line1(1, Point{0,0,0}, Point{2,0,0}, Point{1,0,0});
    Triangle line2(2, Point{1,0,0}, Point{3,0,0}, Point{2,0,0});
    
    EXPECT_TRUE(Triangle::Intersect(line1, line2));
}

TEST(TriangleDegenerateTest, ParallelLineSegmentsNoIntersection) {
    Triangle line1(1, Point{0,0,0}, Point{2,0,0}, Point{1,0,0});
    Triangle line2(2, Point{0,1,0}, Point{2,1,0}, Point{1,1,0});
    
    EXPECT_FALSE(Triangle::Intersect(line1, line2));
}

TEST(TriangleDegenerateTest, DegenerateTriangleContainedInNormalTriangle) {
    Triangle degenerate(1, Point{0.2,0.2,0}, Point{0.8,0.2,0}, Point{0.5,0.2,0});
    Triangle normal(2, Point{0,0,0}, Point{1,0,0}, Point{0,1,0});
    
    EXPECT_TRUE(Triangle::Intersect(degenerate, normal));
    EXPECT_TRUE(Triangle::Intersect(normal, degenerate));
}

TEST(TriangleDegenerateTest, DegenerateTriangleOnEdgeOfNormalTriangle) {
    Triangle point_triangle(1, Point{0.5,0,0}, Point{0.5,0,0}, Point{0.5,0,0});
    Triangle normal_triangle(2, Point{0,0,0}, Point{1,0,0}, Point{0,1,0});
    
    EXPECT_TRUE(Triangle::Intersect(point_triangle, normal_triangle));
    EXPECT_TRUE(Triangle::Intersect(normal_triangle, point_triangle));
}

TEST(DegenerateSegmentNormalTest, SegmentInsideNormalTriangle) {
    Triangle segment(1, Point{0.3,0.3,0}, Point{0.7,0.3,0}, Point{0.5,0.3,0});
    Triangle normal(2, Point{0,0,0}, Point{1,0,0}, Point{0,1,0});
    EXPECT_TRUE(Triangle::Intersect(segment, normal));
}

TEST(DegenerateSegmentNormalTest, SegmentThroughNormalTriangle) {
    Triangle segment(1, Point{0.5,-0.5,0}, Point{0.5,1.5,0}, Point{0.5,0.5,0});
    Triangle normal(2, Point{0,0,0}, Point{1,0,0}, Point{0,1,0});
    EXPECT_TRUE(Triangle::Intersect(segment, normal));
}

TEST(DegenerateSegmentNormalTest, SegmentOutsideNormalTriangle) {
    Triangle segment(1, Point{1.5,1.5,0}, Point{2.5,1.5,0}, Point{2.0,1.5,0});
    Triangle normal(2, Point{0,0,0}, Point{1,0,0}, Point{0,1,0});
    EXPECT_FALSE(Triangle::Intersect(segment, normal));
}

TEST(DegenerateSegmentNormalTest, SegmentTouchingNormalTriangleVertex) {
    Triangle segment(1, Point{1,0,0}, Point{2,0,0}, Point{1.5,0,0});
    Triangle normal(2, Point{0,0,0}, Point{1,0,0}, Point{0,1,0});
    EXPECT_TRUE(Triangle::Intersect(segment, normal));
}

TEST(DegenerateSegmentNormalTest, SegmentAlongNormalTriangleEdge) {
    Triangle segment(1, Point{0,0,0}, Point{0.5,0,0}, Point{0.25,0,0});
    Triangle normal(2, Point{0,0,0}, Point{1,0,0}, Point{0,1,0});
    EXPECT_TRUE(Triangle::Intersect(segment, normal));
}

TEST(DegenerateSegmentNormalTest, SegmentInDifferentPlaneNoIntersection) {
    Triangle segment(1, Point{0.3,0.3,1}, Point{0.7,0.3,1}, Point{0.5,0.3,1});
    Triangle normal(2, Point{0,0,0}, Point{1,0,0}, Point{0,1,0});
    EXPECT_FALSE(Triangle::Intersect(segment, normal));
}

TEST(DegeneratePointNormalTest, PointInsideNormalTriangle) {
    Triangle point(1, Point{0.3,0.3,0}, Point{0.3,0.3,0}, Point{0.3,0.3,0});
    Triangle normal(2, Point{0,0,0}, Point{1,0,0}, Point{0,1,0});
    EXPECT_TRUE(Triangle::Intersect(point, normal));
}

TEST(DegeneratePointNormalTest, PointOnNormalTriangleVertex) {
    Triangle point(1, Point{1,0,0}, Point{1,0,0}, Point{1,0,0});
    Triangle normal(2, Point{0,0,0}, Point{1,0,0}, Point{0,1,0});
    EXPECT_TRUE(Triangle::Intersect(point, normal));
}

TEST(DegeneratePointNormalTest, PointOnNormalTriangleEdge) {
    Triangle point(1, Point{0.5,0,0}, Point{0.5,0,0}, Point{0.5,0,0});
    Triangle normal(2, Point{0,0,0}, Point{1,0,0}, Point{0,1,0});
    EXPECT_TRUE(Triangle::Intersect(point, normal));
}

TEST(DegeneratePointNormalTest, PointOutsideNormalTriangle) {
    Triangle point(1, Point{1.5,1.5,0}, Point{1.5,1.5,0}, Point{1.5,1.5,0});
    Triangle normal(2, Point{0,0,0}, Point{1,0,0}, Point{0,1,0});
    EXPECT_FALSE(Triangle::Intersect(point, normal));
}

TEST(DegeneratePointNormalTest, PointInDifferentPlaneNoIntersection) {
    Triangle point(1, Point{0.3,0.3,1}, Point{0.3,0.3,1}, Point{0.3,0.3,1});
    Triangle normal(2, Point{0,0,0}, Point{1,0,0}, Point{0,1,0});
    EXPECT_FALSE(Triangle::Intersect(point, normal));
}

TEST(DegenerateSegmentSegmentTest, CollinearSegmentsOverlap) {
    Triangle seg1(1, Point{0,0,0}, Point{2,0,0}, Point{1,0,0});
    Triangle seg2(2, Point{1,0,0}, Point{3,0,0}, Point{2,0,0});
    EXPECT_TRUE(Triangle::Intersect(seg1, seg2));
}

TEST(DegenerateSegmentSegmentTest, CollinearSegmentsTouch) {
    Triangle seg1(1, Point{0,0,0}, Point{1,0,0}, Point{0.5,0,0});
    Triangle seg2(2, Point{1,0,0}, Point{2,0,0}, Point{1.5,0,0});
    EXPECT_TRUE(Triangle::Intersect(seg1, seg2));
}

TEST(DegenerateSegmentSegmentTest, CollinearSegmentsNoOverlap) {
    Triangle seg1(1, Point{0,0,0}, Point{1,0,0}, Point{0.5,0,0});
    Triangle seg2(2, Point{2,0,0}, Point{3,0,0}, Point{2.5,0,0});
    EXPECT_FALSE(Triangle::Intersect(seg1, seg2));
}

TEST(DegenerateSegmentSegmentTest, IntersectingSegments) {
    Triangle seg1(1, Point{0,0,0}, Point{2,2,0}, Point{1,1,0});
    Triangle seg2(2, Point{0,2,0}, Point{2,0,0}, Point{1,1,0});
    EXPECT_TRUE(Triangle::Intersect(seg1, seg2));
}

TEST(DegenerateSegmentSegmentTest, ParallelSegmentsNoIntersection) {
    Triangle seg1(1, Point{0,0,0}, Point{2,0,0}, Point{1,0,0});
    Triangle seg2(2, Point{0,1,0}, Point{2,1,0}, Point{1,1,0});
    EXPECT_FALSE(Triangle::Intersect(seg1, seg2));
}

TEST(DegenerateSegmentSegmentTest, SegmentsInDifferentPlanes) {
    Triangle seg1(1, Point{0,0,0}, Point{2,0,0}, Point{1,0,0});
    Triangle seg2(2, Point{0,0,1}, Point{2,0,1}, Point{1,0,1});
    EXPECT_FALSE(Triangle::Intersect(seg1, seg2));
}

TEST(DegenerateSegmentPointTest, PointInsideSegment) {
    Triangle segment(1, Point{0,0,0}, Point{2,0,0}, Point{1,0,0});
    Triangle point(2, Point{1,0,0}, Point{1,0,0}, Point{1,0,0});
    EXPECT_TRUE(Triangle::Intersect(segment, point));
}

TEST(DegenerateSegmentPointTest, PointOnSegmentEndpoint) {
    Triangle segment(1, Point{0,0,0}, Point{2,0,0}, Point{1,0,0});
    Triangle point(2, Point{0,0,0}, Point{0,0,0}, Point{0,0,0});
    EXPECT_TRUE(Triangle::Intersect(segment, point));
}

TEST(DegenerateSegmentPointTest, PointOutsideSegment) {
    Triangle segment(1, Point{0,0,0}, Point{2,0,0}, Point{1,0,0});
    Triangle point(2, Point{3,0,0}, Point{3,0,0}, Point{3,0,0});
    EXPECT_FALSE(Triangle::Intersect(segment, point));
}

TEST(DegenerateSegmentPointTest, PointNotOnSegmentLine) {
    Triangle segment(1, Point{0,0,0}, Point{2,0,0}, Point{1,0,0});
    Triangle point(2, Point{1,1,0}, Point{1,1,0}, Point{1,1,0});
    EXPECT_FALSE(Triangle::Intersect(segment, point));
}

TEST(DegenerateSegmentPointTest, SegmentAndPointInDifferentPlanes) {
    Triangle segment(1, Point{0,0,0}, Point{2,0,0}, Point{1,0,0});
    Triangle point(2, Point{1,0,1}, Point{1,0,1}, Point{1,0,1});
    EXPECT_FALSE(Triangle::Intersect(segment, point));
}

TEST(DegeneratePointPointTest, SamePoints) {
    Triangle point1(1, Point{1,2,3}, Point{1,2,3}, Point{1,2,3});
    Triangle point2(2, Point{1,2,3}, Point{1,2,3}, Point{1,2,3});
    EXPECT_TRUE(Triangle::Intersect(point1, point2));
}

TEST(DegeneratePointPointTest, DifferentPoints) {
    Triangle point1(1, Point{1,2,3}, Point{1,2,3}, Point{1,2,3});
    Triangle point2(2, Point{4,5,6}, Point{4,5,6}, Point{4,5,6});
    EXPECT_FALSE(Triangle::Intersect(point1, point2));
}

TEST(DegeneratePointPointTest, CloseButDifferentPoints) {
    Triangle point1(1, Point{1,2,3}, Point{1,2,3}, Point{1,2,3});
    Triangle point2(2, Point{1.0001,2.0001,3.0001}, Point{1.0001,2.0001,3.0001}, Point{1.0001,2.0001,3.0001});
    EXPECT_FALSE(Triangle::Intersect(point1, point2));
}

TEST(DegeneratePointPointTest, PointsInDifferentPlanes) {
    Triangle point1(1, Point{1,2,0}, Point{1,2,0}, Point{1,2,0});
    Triangle point2(2, Point{1,2,1}, Point{1,2,1}, Point{1,2,1});
    EXPECT_FALSE(Triangle::Intersect(point1, point2));
}

TEST(DegenerateEdgeCasesTest, ZeroLengthSegment) {
    Triangle zero_segment(1, Point{1,1,1}, Point{1,1,1}, Point{1,1,1});
    Triangle normal(2, Point{0,0,0}, Point{2,0,0}, Point{0,2,0});
    EXPECT_FALSE(Triangle::Intersect(zero_segment, normal));
}

TEST(DegenerateEdgeCasesTest, SegmentExactlyOnTrianglePlaneButOutside) {
    Triangle segment(1, Point{2,2,0}, Point{3,2,0}, Point{2.5,2,0});
    Triangle normal(2, Point{0,0,0}, Point{1,0,0}, Point{0,1,0});
    EXPECT_FALSE(Triangle::Intersect(segment, normal));
}

TEST(DegenerateEdgeCasesTest, PointOnTriangleBoundary) {
    Triangle point(1, Point{0.5,0.5,0}, Point{0.5,0.5,0}, Point{0.5,0.5,0});
    Triangle normal(2, Point{0,0,0}, Point{1,0,0}, Point{0,1,0});
    EXPECT_TRUE(Triangle::Intersect(point, normal));
}

TEST(DegenerateEdgeCasesTest, DegenerateTriangleWithNegativeCoordinates) {
    Triangle segment(1, Point{-1,-1,0}, Point{1,1,0}, Point{0,0,0});
    Triangle normal(2, Point{0,0,0}, Point{2,0,0}, Point{0,2,0});
    EXPECT_TRUE(Triangle::Intersect(segment, normal));
}
