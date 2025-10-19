#include <gtest/gtest.h>

#include <cmath>

#include "triangle.hpp"

class TriangleTest : public ::testing::Test {
protected:
    Point<double> p0 = Point<double>{0, 0, 0};
    Point<double> p1 = Point<double>{1, 0, 0};
    Point<double> p2 = Point<double>{0, 1, 0};
    Triangle<double> triangle = Triangle<double>(1, p0, p1, p2);
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
    Vector<double> normal = triangle.GetNormal();

    EXPECT_DOUBLE_EQ(normal.x, 0);
    EXPECT_DOUBLE_EQ(normal.y, 0);
    EXPECT_DOUBLE_EQ(std::abs(normal.z), 1);
}

TEST_F(TriangleTest, AABBComputation) {
    AABB<double> bbox = triangle.GetAABB();
    
    EXPECT_EQ(bbox.min.x, 0);
    EXPECT_EQ(bbox.min.y, 0);
    EXPECT_EQ(bbox.min.z, 0);
    
    EXPECT_EQ(bbox.max.x, 1);
    EXPECT_EQ(bbox.max.y, 1);
    EXPECT_EQ(bbox.max.z, 0);
}

// Project -----------------------------------------------------------------------------------------

TEST_F(TriangleTest, ProjectionOnAxis) {
    Vector<double> x_axis{1, 0, 0};
    auto projection = triangle.Project(x_axis);
    
    EXPECT_DOUBLE_EQ(projection.first, 0);
    EXPECT_DOUBLE_EQ(projection.second, 1);
    
    Vector<double> y_axis{0, 1, 0};
    projection = triangle.Project(y_axis);
    EXPECT_DOUBLE_EQ(projection.first, 0);
    EXPECT_DOUBLE_EQ(projection.second, 1);
    
    Vector<double> diagonal_axis{1, 1, 0};
    projection = triangle.Project(diagonal_axis);
    EXPECT_DOUBLE_EQ(projection.first, 0);
    EXPECT_DOUBLE_EQ(projection.second, 1 / std::sqrt(2));
}

// RelativePlanesPosition --------------------------------------------------------------------------

TEST(RelativePlanesPositionTest, ParallelPlanes) {
    Triangle<double> t1(1, Point<double>{0,0,0}, Point<double>{1,0,0}, Point<double>{0,1,0});
    Triangle<double> t2(2, Point<double>{0,0,1}, Point<double>{1,0,1}, Point<double>{0,1,1});
    
    auto position = Triangle<double>::RelativePlanesPosition(t1, t2);
    EXPECT_EQ(position, PlanesPosition::Parallel);
}

TEST(RelativePlanesPositionTest, CoincidentPlanes) {
    Triangle<double> t1(1, Point<double>{0,0,0}, Point<double>{1,0,0}, Point<double>{0,1,0});
    Triangle<double> t2(2, Point<double>{0,0,0}, Point<double>{0,1,0}, Point<double>{1,0,0});
    
    auto position = Triangle<double>::RelativePlanesPosition(t1, t2);
    EXPECT_EQ(position, PlanesPosition::Coincide);
}

TEST(RelativePlanesPositionTest, IntersectingPlanes) {
    Triangle<double> t1(1, Point<double>{0,0,0}, Point<double>{1,0,0}, Point<double>{0,1,0});
    Triangle<double> t2(2, Point<double>{0,0,0}, Point<double>{0,1,0}, Point<double>{0,0,1});
    
    auto position = Triangle<double>::RelativePlanesPosition(t1, t2);
    EXPECT_EQ(position, PlanesPosition::Intersect);
}

// SAT ---------------------------------------------------------------------------------------------

TEST(TriangleIntersectionTest, NonIntersectingTriangles) {
    Triangle<double> above(2, Point<double>{0,0,1}, Point<double>{1,0,1}, Point<double>{0,1,1});
    Triangle<double> xy_plane(1, Point<double>{0,0,0}, Point<double>{1,0,0}, Point<double>{0,1,0});
    
    EXPECT_FALSE(Triangle<double>::Intersect(xy_plane, above));
    EXPECT_FALSE(Triangle<double>::SAT(xy_plane, above));
}

TEST(TriangleIntersectionTest, PerpendicularIntersectingTriangles) {
    Triangle<double> perpendicular(2, Point<double>{0.2,0.2,0.5}, Point<double>{0.2,0.2,-0.5}, Point<double>{0.8,0.8,0});
    Triangle<double> xy_plane(1, Point<double>{0,0,0}, Point<double>{1,0,0}, Point<double>{0,1,0});
    
    EXPECT_TRUE(Triangle<double>::Intersect(xy_plane, perpendicular));
    EXPECT_TRUE(Triangle<double>::SAT(xy_plane, perpendicular));
}

// ComputeBoundingBox ------------------------------------------------------------------------------

TEST(ComputeBoundingBoxTest, SingleTriangle) {
    Triangle<double> t(1, Point<double>{0,0,0}, Point<double>{2,0,0}, Point<double>{0,3,0});
    std::vector<Triangle<double>> triangles = {t};
    
    AABB<double> bbox = Triangle<double>::ComputeBoundingBox(triangles);
    
    EXPECT_EQ(bbox.min.x, 0);
    EXPECT_EQ(bbox.min.y, 0);
    EXPECT_EQ(bbox.min.z, 0);
    EXPECT_EQ(bbox.max.x, 2);
    EXPECT_EQ(bbox.max.y, 3);
    EXPECT_EQ(bbox.max.z, 0);
}

TEST(ComputeBoundingBoxTest, MultipleTriangles) {
    std::vector<Triangle<double>> triangles = {
        Triangle<double>(1, Point<double>{0,0,0}, Point<double>{1,0,0}, Point<double>{0,1,0}),
        Triangle<double>(2, Point<double>{2,2,1}, Point<double>{3,2,1}, Point<double>{2,3,1}),
        Triangle<double>(3, Point<double>{-1,-1,-1}, Point<double>{0,-1,-1}, Point<double>{-1,0,-1})
    };
    
    AABB<double> bbox = Triangle<double>::ComputeBoundingBox(triangles);
    
    EXPECT_EQ(bbox.min.x, -1);
    EXPECT_EQ(bbox.min.y, -1);
    EXPECT_EQ(bbox.min.z, -1);
    EXPECT_EQ(bbox.max.x, 3);
    EXPECT_EQ(bbox.max.y, 3);
    EXPECT_EQ(bbox.max.z, 1);
}

// Contains ----------------------------------------------------------------------------------------

TEST(TriangleEdgeCasesTest, ContainsMethod) {
    Triangle<double> large(1, Point<double>{0,0,0}, Point<double>{3,0,0}, Point<double>{0,3,0});
    Triangle<double> small(2, Point<double>{0.5,0.5,0}, Point<double>{1.5,0.5,0}, Point<double>{0.5,1.5,0});
    
    bool result = large.Contains(small);
    EXPECT_TRUE(result);
}

// Custom test cases -------------------------------------------------------------------------------

TEST(TriangleSpecificTest, CustomTestCase1) {
    Triangle<double> t0(0, 
        Point<double>{0, 0, 0}, 
        Point<double>{1, 0, 0}, 
        Point<double>{0, 1, 0}
    );
    
    Triangle<double> t1(1, 
        Point<double>{5, 5, 5}, 
        Point<double>{5, 5, 5}, 
        Point<double>{5, 5, 5}
    );
    
    Triangle<double> t2(2, 
        Point<double>{0, 0.5, -0.5}, 
        Point<double>{0, 0.5, 0.5}, 
        Point<double>{-1, 0, 0}
    );
    
    EXPECT_FALSE(Triangle<double>::Intersect(t0, t1));
    EXPECT_TRUE(Triangle<double>::Intersect(t0, t2));   
    EXPECT_FALSE(Triangle<double>::Intersect(t1, t2));
}

// Special cases -----------------------------------------------------------------------------------

TEST(TriangleDegenerateTest, PointTriangleIntersection) {
    Triangle<double> point_triangle(1, Point<double>{0,0,0}, Point<double>{0,0,0}, Point<double>{0,0,0});
    Triangle<double> normal_triangle(2, Point<double>{0,0,0}, Point<double>{1,0,0}, Point<double>{0,1,0});
    
    EXPECT_TRUE(Triangle<double>::Intersect(point_triangle, normal_triangle));
    EXPECT_TRUE(Triangle<double>::Intersect(normal_triangle, point_triangle));
}

TEST(TriangleDegenerateTest, LineSegmentTriangleIntersection) {
    Triangle<double> line_triangle(1, Point<double>{0,0,0}, Point<double>{1,0,0}, Point<double>{0.5,0,0});
    Triangle<double> normal_triangle(2, Point<double>{0,0,0}, Point<double>{1,0,0}, Point<double>{0,1,0});
    
    EXPECT_TRUE(Triangle<double>::Intersect(line_triangle, normal_triangle));
    EXPECT_TRUE(Triangle<double>::Intersect(normal_triangle, line_triangle));
}

TEST(TriangleDegenerateTest, TwoPointTrianglesNoIntersection) {
    Triangle<double> point1(1, Point<double>{0,0,0}, Point<double>{0,0,0}, Point<double>{0,0,0});
    Triangle<double> point2(2, Point<double>{1,1,1}, Point<double>{1,1,1}, Point<double>{1,1,1});
    
    EXPECT_FALSE(Triangle<double>::Intersect(point1, point2));
}

TEST(TriangleDegenerateTest, LineSegmentAndPointIntersection) {
    Triangle<double> line_triangle(1, Point<double>{0,0,0}, Point<double>{2,0,0}, Point<double>{1,0,0});
    Triangle<double> point_triangle(2, Point<double>{1,0,0}, Point<double>{1,0,0}, Point<double>{1,0,0});
    
    EXPECT_TRUE(Triangle<double>::Intersect(line_triangle, point_triangle));
    EXPECT_TRUE(Triangle<double>::Intersect(point_triangle, line_triangle));
}

TEST(TriangleDegenerateTest, CollinearLineSegmentsIntersection) {
    Triangle<double> line1(1, Point<double>{0,0,0}, Point<double>{2,0,0}, Point<double>{1,0,0});
    Triangle<double> line2(2, Point<double>{1,0,0}, Point<double>{3,0,0}, Point<double>{2,0,0});
    
    EXPECT_TRUE(Triangle<double>::Intersect(line1, line2));
}

TEST(TriangleDegenerateTest, ParallelLineSegmentsNoIntersection) {
    Triangle<double> line1(1, Point<double>{0,0,0}, Point<double>{2,0,0}, Point<double>{1,0,0});
    Triangle<double> line2(2, Point<double>{0,1,0}, Point<double>{2,1,0}, Point<double>{1,1,0});
    
    EXPECT_FALSE(Triangle<double>::Intersect(line1, line2));
}

TEST(TriangleDegenerateTest, DegenerateTriangleContainedInNormalTriangle) {
    Triangle<double> degenerate(1, Point<double>{0.2,0.2,0}, Point<double>{0.8,0.2,0}, Point<double>{0.5,0.2,0});
    Triangle<double> normal(2, Point<double>{0,0,0}, Point<double>{1,0,0}, Point<double>{0,1,0});
    
    EXPECT_TRUE(Triangle<double>::Intersect(degenerate, normal));
    EXPECT_TRUE(Triangle<double>::Intersect(normal, degenerate));
}

TEST(TriangleDegenerateTest, DegenerateTriangleOnEdgeOfNormalTriangle) {
    Triangle<double> point_triangle(1, Point<double>{0.5,0,0}, Point<double>{0.5,0,0}, Point<double>{0.5,0,0});
    Triangle<double> normal_triangle(2, Point<double>{0,0,0}, Point<double>{1,0,0}, Point<double>{0,1,0});
    
    EXPECT_TRUE(Triangle<double>::Intersect(point_triangle, normal_triangle));
    EXPECT_TRUE(Triangle<double>::Intersect(normal_triangle, point_triangle));
}

TEST(DegenerateSegmentNormalTest, SegmentInsideNormalTriangle) {
    Triangle<double> segment(1, Point<double>{0.3,0.3,0}, Point<double>{0.7,0.3,0}, Point<double>{0.5,0.3,0});
    Triangle<double> normal(2, Point<double>{0,0,0}, Point<double>{1,0,0}, Point<double>{0,1,0});
    EXPECT_TRUE(Triangle<double>::Intersect(segment, normal));
}

TEST(DegenerateSegmentNormalTest, SegmentThroughNormalTriangle) {
    Triangle<double> segment(1, Point<double>{0.5,-0.5,0}, Point<double>{0.5,1.5,0}, Point<double>{0.5,0.5,0});
    Triangle<double> normal(2, Point<double>{0,0,0}, Point<double>{1,0,0}, Point<double>{0,1,0});
    EXPECT_TRUE(Triangle<double>::Intersect(segment, normal));
}

TEST(DegenerateSegmentNormalTest, SegmentOutsideNormalTriangle) {
    Triangle<double> segment(1, Point<double>{1.5,1.5,0}, Point<double>{2.5,1.5,0}, Point<double>{2.0,1.5,0});
    Triangle<double> normal(2, Point<double>{0,0,0}, Point<double>{1,0,0}, Point<double>{0,1,0});
    EXPECT_FALSE(Triangle<double>::Intersect(segment, normal));
}

TEST(DegenerateSegmentNormalTest, SegmentTouchingNormalTriangleVertex) {
    Triangle<double> segment(1, Point<double>{1,0,0}, Point<double>{2,0,0}, Point<double>{1.5,0,0});
    Triangle<double> normal(2, Point<double>{0,0,0}, Point<double>{1,0,0}, Point<double>{0,1,0});
    EXPECT_TRUE(Triangle<double>::Intersect(segment, normal));
}

TEST(DegenerateSegmentNormalTest, SegmentAlongNormalTriangleEdge) {
    Triangle<double> segment(1, Point<double>{0,0,0}, Point<double>{0.5,0,0}, Point<double>{0.25,0,0});
    Triangle<double> normal(2, Point<double>{0,0,0}, Point<double>{1,0,0}, Point<double>{0,1,0});
    EXPECT_TRUE(Triangle<double>::Intersect(segment, normal));
}

TEST(DegenerateSegmentNormalTest, SegmentInDifferentPlaneNoIntersection) {
    Triangle<double> segment(1, Point<double>{0.3,0.3,1}, Point<double>{0.7,0.3,1}, Point<double>{0.5,0.3,1});
    Triangle<double> normal(2, Point<double>{0,0,0}, Point<double>{1,0,0}, Point<double>{0,1,0});
    EXPECT_FALSE(Triangle<double>::Intersect(segment, normal));
}

TEST(DegeneratePointNormalTest, PointInsideNormalTriangle) {
    Triangle<double> point(1, Point<double>{0.3,0.3,0}, Point<double>{0.3,0.3,0}, Point<double>{0.3,0.3,0});
    Triangle<double> normal(2, Point<double>{0,0,0}, Point<double>{1,0,0}, Point<double>{0,1,0});
    EXPECT_TRUE(Triangle<double>::Intersect(point, normal));
}

TEST(DegeneratePointNormalTest, PointOnNormalTriangleVertex) {
    Triangle<double> point(1, Point<double>{1,0,0}, Point<double>{1,0,0}, Point<double>{1,0,0});
    Triangle<double> normal(2, Point<double>{0,0,0}, Point<double>{1,0,0}, Point<double>{0,1,0});
    EXPECT_TRUE(Triangle<double>::Intersect(point, normal));
}

TEST(DegeneratePointNormalTest, PointOnNormalTriangleEdge) {
    Triangle<double> point(1, Point<double>{0.5,0,0}, Point<double>{0.5,0,0}, Point<double>{0.5,0,0});
    Triangle<double> normal(2, Point<double>{0,0,0}, Point<double>{1,0,0}, Point<double>{0,1,0});
    EXPECT_TRUE(Triangle<double>::Intersect(point, normal));
}

TEST(DegeneratePointNormalTest, PointOutsideNormalTriangle) {
    Triangle<double> point(1, Point<double>{1.5,1.5,0}, Point<double>{1.5,1.5,0}, Point<double>{1.5,1.5,0});
    Triangle<double> normal(2, Point<double>{0,0,0}, Point<double>{1,0,0}, Point<double>{0,1,0});
    EXPECT_FALSE(Triangle<double>::Intersect(point, normal));
}

TEST(DegeneratePointNormalTest, PointInDifferentPlaneNoIntersection) {
    Triangle<double> point(1, Point<double>{0.3,0.3,1}, Point<double>{0.3,0.3,1}, Point<double>{0.3,0.3,1});
    Triangle<double> normal(2, Point<double>{0,0,0}, Point<double>{1,0,0}, Point<double>{0,1,0});
    EXPECT_FALSE(Triangle<double>::Intersect(point, normal));
}

TEST(DegenerateSegmentSegmentTest, CollinearSegmentsOverlap) {
    Triangle<double> seg1(1, Point<double>{0,0,0}, Point<double>{2,0,0}, Point<double>{1,0,0});
    Triangle<double> seg2(2, Point<double>{1,0,0}, Point<double>{3,0,0}, Point<double>{2,0,0});
    EXPECT_TRUE(Triangle<double>::Intersect(seg1, seg2));
}

TEST(DegenerateSegmentSegmentTest, CollinearSegmentsTouch) {
    Triangle<double> seg1(1, Point<double>{0,0,0}, Point<double>{1,0,0}, Point<double>{0.5,0,0});
    Triangle<double> seg2(2, Point<double>{1,0,0}, Point<double>{2,0,0}, Point<double>{1.5,0,0});
    EXPECT_TRUE(Triangle<double>::Intersect(seg1, seg2));
}

TEST(DegenerateSegmentSegmentTest, CollinearSegmentsNoOverlap) {
    Triangle<double> seg1(1, Point<double>{0,0,0}, Point<double>{1,0,0}, Point<double>{0.5,0,0});
    Triangle<double> seg2(2, Point<double>{2,0,0}, Point<double>{3,0,0}, Point<double>{2.5,0,0});
    EXPECT_FALSE(Triangle<double>::Intersect(seg1, seg2));
}

TEST(DegenerateSegmentSegmentTest, IntersectingSegments) {
    Triangle<double> seg1(1, Point<double>{0,0,0}, Point<double>{2,2,0}, Point<double>{1,1,0});
    Triangle<double> seg2(2, Point<double>{0,2,0}, Point<double>{2,0,0}, Point<double>{1,1,0});
    EXPECT_TRUE(Triangle<double>::Intersect(seg1, seg2));
}

TEST(DegenerateSegmentSegmentTest, ParallelSegmentsNoIntersection) {
    Triangle<double> seg1(1, Point<double>{0,0,0}, Point<double>{2,0,0}, Point<double>{1,0,0});
    Triangle<double> seg2(2, Point<double>{0,1,0}, Point<double>{2,1,0}, Point<double>{1,1,0});
    EXPECT_FALSE(Triangle<double>::Intersect(seg1, seg2));
}

TEST(DegenerateSegmentSegmentTest, SegmentsInDifferentPlanes) {
    Triangle<double> seg1(1, Point<double>{0,0,0}, Point<double>{2,0,0}, Point<double>{1,0,0});
    Triangle<double> seg2(2, Point<double>{0,0,1}, Point<double>{2,0,1}, Point<double>{1,0,1});
    EXPECT_FALSE(Triangle<double>::Intersect(seg1, seg2));
}

TEST(DegenerateSegmentPointTest, PointInsideSegment) {
    Triangle<double> segment(1, Point<double>{0,0,0}, Point<double>{2,0,0}, Point<double>{1,0,0});
    Triangle<double> point(2, Point<double>{1,0,0}, Point<double>{1,0,0}, Point<double>{1,0,0});
    EXPECT_TRUE(Triangle<double>::Intersect(segment, point));
}

TEST(DegenerateSegmentPointTest, PointOnSegmentEndPoint) {
    Triangle<double> segment(1, Point<double>{0,0,0}, Point<double>{2,0,0}, Point<double>{1,0,0});
    Triangle<double> point(2, Point<double>{0,0,0}, Point<double>{0,0,0}, Point<double>{0,0,0});
    EXPECT_TRUE(Triangle<double>::Intersect(segment, point));
}

TEST(DegenerateSegmentPointTest, PointOutsideSegment) {
    Triangle<double> segment(1, Point<double>{0,0,0}, Point<double>{2,0,0}, Point<double>{1,0,0});
    Triangle<double> point(2, Point<double>{3,0,0}, Point<double>{3,0,0}, Point<double>{3,0,0});
    EXPECT_FALSE(Triangle<double>::Intersect(segment, point));
}

TEST(DegenerateSegmentPointTest, PointNotOnSegmentLine) {
    Triangle<double> segment(1, Point<double>{0,0,0}, Point<double>{2,0,0}, Point<double>{1,0,0});
    Triangle<double> point(2, Point<double>{1,1,0}, Point<double>{1,1,0}, Point<double>{1,1,0});
    EXPECT_FALSE(Triangle<double>::Intersect(segment, point));
}

TEST(DegenerateSegmentPointTest, SegmentAndPointInDifferentPlanes) {
    Triangle<double> segment(1, Point<double>{0,0,0}, Point<double>{2,0,0}, Point<double>{1,0,0});
    Triangle<double> point(2, Point<double>{1,0,1}, Point<double>{1,0,1}, Point<double>{1,0,1});
    EXPECT_FALSE(Triangle<double>::Intersect(segment, point));
}

TEST(DegeneratePointPointTest, SamePoints) {
    Triangle<double> point1(1, Point<double>{1,2,3}, Point<double>{1,2,3}, Point<double>{1,2,3});
    Triangle<double> point2(2, Point<double>{1,2,3}, Point<double>{1,2,3}, Point<double>{1,2,3});
    EXPECT_TRUE(Triangle<double>::Intersect(point1, point2));
}

TEST(DegeneratePointPointTest, DifferentPoints) {
    Triangle<double> point1(1, Point<double>{1,2,3}, Point<double>{1,2,3}, Point<double>{1,2,3});
    Triangle<double> point2(2, Point<double>{4,5,6}, Point<double>{4,5,6}, Point<double>{4,5,6});
    EXPECT_FALSE(Triangle<double>::Intersect(point1, point2));
}

TEST(DegeneratePointPointTest, CloseButDifferentPoints) {
    Triangle<double> point1(1, Point<double>{1,2,3}, Point<double>{1,2,3}, Point<double>{1,2,3});
    Triangle<double> point2(2, Point<double>{1.0001,2.0001,3.0001}, Point<double>{1.0001,2.0001,3.0001}, Point<double>{1.0001,2.0001,3.0001});
    EXPECT_FALSE(Triangle<double>::Intersect(point1, point2));
}

TEST(DegeneratePointPointTest, PointsInDifferentPlanes) {
    Triangle<double> point1(1, Point<double>{1,2,0}, Point<double>{1,2,0}, Point<double>{1,2,0});
    Triangle<double> point2(2, Point<double>{1,2,1}, Point<double>{1,2,1}, Point<double>{1,2,1});
    EXPECT_FALSE(Triangle<double>::Intersect(point1, point2));
}

TEST(DegenerateEdgeCasesTest, ZeroLengthSegment) {
    Triangle<double> zero_segment(1, Point<double>{1,1,1}, Point<double>{1,1,1}, Point<double>{1,1,1});
    Triangle<double> normal(2, Point<double>{0,0,0}, Point<double>{2,0,0}, Point<double>{0,2,0});
    EXPECT_FALSE(Triangle<double>::Intersect(zero_segment, normal));
}

TEST(DegenerateEdgeCasesTest, SegmentExactlyOnTrianglePlaneButOutside) {
    Triangle<double> segment(1, Point<double>{2,2,0}, Point<double>{3,2,0}, Point<double>{2.5,2,0});
    Triangle<double> normal(2, Point<double>{0,0,0}, Point<double>{1,0,0}, Point<double>{0,1,0});
    EXPECT_FALSE(Triangle<double>::Intersect(segment, normal));
}

TEST(DegenerateEdgeCasesTest, PointOnTriangleBoundary) {
    Triangle<double> point(1, Point<double>{0.5,0.5,0}, Point<double>{0.5,0.5,0}, Point<double>{0.5,0.5,0});
    Triangle<double> normal(2, Point<double>{0,0,0}, Point<double>{1,0,0}, Point<double>{0,1,0});
    EXPECT_TRUE(Triangle<double>::Intersect(point, normal));
}

TEST(DegenerateEdgeCasesTest, DegenerateTriangleWithNegativeCoordinates) {
    Triangle<double> segment(1, Point<double>{-1,-1,0}, Point<double>{1,1,0}, Point<double>{0,0,0});
    Triangle<double> normal(2, Point<double>{0,0,0}, Point<double>{2,0,0}, Point<double>{0,2,0});
    EXPECT_TRUE(Triangle<double>::Intersect(segment, normal));
}