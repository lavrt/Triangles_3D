#include <gtest/gtest.h>

#include <vector>
#include <span>

#include "segment.hpp"

class SegmentTest : public ::testing::Test {
protected:
    Segment seg_horizontal = Segment{Point(0, 0, 0), Point(2, 0, 0)};
    Segment seg_vertical = Segment{Point(1, -1, 0), Point(1, 1, 0)};
    Segment seg_diagonal = Segment{Point(0, 0, 0), Point(2, 2, 0)};
    Segment seg_3d = Segment{Point(0, 0, 0), Point(1, 1, 1)};
    Segment seg_degenerate = Segment{Point(1, 1, 1), Point(1, 1, 1)};
};

// Constructor -------------------------------------------------------------------------------------

TEST_F(SegmentTest, ConstructorStoresPoints) {
    Segment seg{Point(1, 2, 3), Point(4, 5, 6)};
    
    EXPECT_DOUBLE_EQ(seg.p0.x, 1);
    EXPECT_DOUBLE_EQ(seg.p0.y, 2);
    EXPECT_DOUBLE_EQ(seg.p0.z, 3);
    
    EXPECT_DOUBLE_EQ(seg.p1.x, 4);
    EXPECT_DOUBLE_EQ(seg.p1.y, 5);
    EXPECT_DOUBLE_EQ(seg.p1.z, 6);
}

// Direction ---------------------------------------------------------------------------------------

TEST_F(SegmentTest, DirectionLeftTurn) {
    Point left(0, 1, 0); 
    double result = seg_horizontal.Direction(left);
    EXPECT_GT(result, 0);
}

TEST_F(SegmentTest, DirectionRightTurn) {
    Point right(0, -1, 0); 
    double result = seg_horizontal.Direction(right);
    EXPECT_LT(result, 0); 
}

TEST_F(SegmentTest, DirectionCollinear) {
    Point collinear(1, 0, 0); 
    double result = seg_horizontal.Direction(collinear);
    EXPECT_DOUBLE_EQ(result, 0);
}

TEST_F(SegmentTest, Direction3DSegment) {
    Point test_point(0, 0, 1);
    double result = seg_3d.Direction(test_point);
    EXPECT_DOUBLE_EQ(result, 0);
}

TEST_F(SegmentTest, DirectionDegenerateSegment) {
    Point any_point(2, 2, 2);
    double result = seg_degenerate.Direction(any_point);
    EXPECT_DOUBLE_EQ(result, 0);
}

// OnSegment ---------------------------------------------------------------------------------------

TEST_F(SegmentTest, OnSegmentEndpoint) {
    EXPECT_TRUE(seg_horizontal.OnSegment(seg_horizontal.p0));
    EXPECT_TRUE(seg_horizontal.OnSegment(seg_horizontal.p1));
}

TEST_F(SegmentTest, OnSegmentMiddle) {
    Point middle(1, 0, 0);
    EXPECT_TRUE(seg_horizontal.OnSegment(middle));
}

TEST_F(SegmentTest, OnSegmentCollinearButOutside) {
    Point outside_before(-1, 0, 0);
    Point outside_after(3, 0, 0);
    
    EXPECT_FALSE(seg_horizontal.OnSegment(outside_before));
    EXPECT_FALSE(seg_horizontal.OnSegment(outside_after));
}

TEST_F(SegmentTest, OnSegmentNotCollinear) {
    Point not_collinear(1, 1, 0);
    EXPECT_FALSE(seg_horizontal.OnSegment(not_collinear));
}

TEST_F(SegmentTest, OnSegmentDiagonal) {
    Point middle_diag(1, 1, 0);
    EXPECT_TRUE(seg_diagonal.OnSegment(middle_diag));
}

TEST_F(SegmentTest, OnSegmentDegenerate) {
    EXPECT_TRUE(seg_degenerate.OnSegment(seg_degenerate.p0));
    
    Point other_point(2, 2, 2);
    EXPECT_FALSE(seg_degenerate.OnSegment(other_point));
}

TEST_F(SegmentTest, OnSegmentPrecision) {
    Point near_segment(1, 0.0000001, 0);
    EXPECT_FALSE(seg_horizontal.OnSegment(near_segment));
}

// Intersect ---------------------------------------------------------------------------------------

TEST_F(SegmentTest, IntersectCrossingSegments) {
    EXPECT_TRUE(Segment::IntersectInPlane(seg_horizontal, seg_vertical));
}

TEST_F(SegmentTest, IntersectParallelSegments) {
    Segment parallel{Point(0, 1, 0), Point(2, 1, 0)};
    EXPECT_FALSE(Segment::IntersectInPlane(seg_horizontal, parallel));
}

TEST_F(SegmentTest, IntersectCollinearOverlapping) {
    Segment overlapping{Point(1, 0, 0), Point(3, 0, 0)};
    EXPECT_TRUE(Segment::IntersectInPlane(seg_horizontal, overlapping));
}

TEST_F(SegmentTest, IntersectCollinearTouching) {
    Segment touching{Point(2, 0, 0), Point(4, 0, 0)};
    EXPECT_TRUE(Segment::IntersectInPlane(seg_horizontal, touching));
}

TEST_F(SegmentTest, IntersectCollinearSeparated) {
    Segment separated{Point(3, 0, 0), Point(5, 0, 0)};
    EXPECT_FALSE(Segment::IntersectInPlane(seg_horizontal, separated));
}

TEST_F(SegmentTest, IntersectEndpointsTouching) {
    Segment touching_end{Point(2, 0, 0), Point(2, 2, 0)};
    EXPECT_TRUE(Segment::IntersectInPlane(seg_horizontal, touching_end));
}

TEST_F(SegmentTest, IntersectSharedEndpoint) {
    Segment shared_end{Point(0, 0, 0), Point(0, 2, 0)};
    EXPECT_TRUE(Segment::IntersectInPlane(seg_horizontal, shared_end));
}

TEST_F(SegmentTest, IntersectDegenerateSegments) {
    Segment deg1{Point(1, 0, 0), Point(1, 0, 0)};
    Segment deg2{Point(1, 0, 0), Point(1, 0, 0)};
    
    EXPECT_TRUE(Segment::IntersectInPlane(deg1, deg2));
    
    EXPECT_TRUE(Segment::IntersectInPlane(deg1, seg_horizontal));
    
    Segment deg_outside{Point(5, 5, 5), Point(5, 5, 5)};
    EXPECT_FALSE(Segment::IntersectInPlane(deg_outside, seg_horizontal));
}

TEST_F(SegmentTest, Intersect3DSegments) {
    Segment seg1{Point(0, 0, 0), Point(2, 2, 0)};
    Segment seg2{Point(0, 2, 0), Point(2, 0, 0)};
    EXPECT_TRUE(Segment::IntersectInPlane(seg1, seg2));
}

TEST_F(SegmentTest, IntersectCommutative) {
    EXPECT_EQ(Segment::IntersectInPlane(seg_horizontal, seg_vertical),
              Segment::IntersectInPlane(seg_vertical, seg_horizontal));
}
