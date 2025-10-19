#include <gtest/gtest.h>

#include <vector>
#include <span>

#include "segment.hpp"
#include "details.hpp"

class SegmentTest : public ::testing::Test {
protected:
    Segment<double> seg_horizontal = Segment<double>{Point<double>(0, 0, 0), Point<double>(2, 0, 0)};
    Segment<double> seg_vertical = Segment<double>{Point<double>(1, -1, 0), Point<double>(1, 1, 0)};
    Segment<double> seg_diagonal = Segment<double>{Point<double>(0, 0, 0), Point<double>(2, 2, 0)};
    Segment<double> seg_3d = Segment<double>{Point<double>(0, 0, 0), Point<double>(1, 1, 1)};
    Segment<double> seg_degenerate = Segment<double>{Point<double>(1, 1, 1), Point<double>(1, 1, 1)};
};

// Constructor -------------------------------------------------------------------------------------

TEST_F(SegmentTest, ConstructorStoresPoints) {
    Segment<double> seg{Point<double>(1, 2, 3), Point<double>(4, 5, 6)};
    
    EXPECT_DOUBLE_EQ(seg.p0.x, 1);
    EXPECT_DOUBLE_EQ(seg.p0.y, 2);
    EXPECT_DOUBLE_EQ(seg.p0.z, 3);
    
    EXPECT_DOUBLE_EQ(seg.p1.x, 4);
    EXPECT_DOUBLE_EQ(seg.p1.y, 5);
    EXPECT_DOUBLE_EQ(seg.p1.z, 6);
}

// Intersect ---------------------------------------------------------------------------------------

TEST_F(SegmentTest, IntersectCrossingSegments) {
    EXPECT_TRUE(Segment<double>::Intersect(seg_horizontal, seg_vertical));
}

TEST_F(SegmentTest, IntersectParallelSegments) {
    Segment<double> parallel{Point<double>(0, 1, 0), Point<double>(2, 1, 0)};
    EXPECT_FALSE(Segment<double>::Intersect(seg_horizontal, parallel));
}

TEST_F(SegmentTest, IntersectCollinearOverlapping) {
    Segment<double> overlapping{Point<double>(1, 0, 0), Point<double>(3, 0, 0)};
    EXPECT_TRUE(Segment<double>::Intersect(seg_horizontal, overlapping));
}

TEST_F(SegmentTest, IntersectCollinearTouching) {
    Segment<double> touching{Point<double>(2, 0, 0), Point<double>(4, 0, 0)};
    EXPECT_TRUE(Segment<double>::Intersect(seg_horizontal, touching));
}

TEST_F(SegmentTest, IntersectCollinearSeparated) {
    Segment<double> separated{Point<double>(3, 0, 0), Point<double>(5, 0, 0)};
    EXPECT_FALSE(Segment<double>::Intersect(seg_horizontal, separated));
}

TEST_F(SegmentTest, IntersectEndpointsTouching) {
    Segment<double> touching_end{Point<double>(2, 0, 0), Point<double>(2, 2, 0)};
    EXPECT_TRUE(Segment<double>::Intersect(seg_horizontal, touching_end));
}

TEST_F(SegmentTest, IntersectSharedEndpoint) {
    Segment<double> shared_end{Point<double>(0, 0, 0), Point<double>(0, 2, 0)};
    EXPECT_TRUE(Segment<double>::Intersect(seg_horizontal, shared_end));
}

TEST_F(SegmentTest, Intersect3DSegments) {
    Segment<double> seg1{Point<double>(0, 0, 0), Point<double>(2, 2, 0)};
    Segment<double> seg2{Point<double>(0, 2, 0), Point<double>(2, 0, 0)};
    EXPECT_TRUE(Segment<double>::Intersect(seg1, seg2));
}

TEST_F(SegmentTest, IntersectCommutative) {
    EXPECT_EQ(Segment<double>::Intersect(seg_horizontal, seg_vertical),
              Segment<double>::Intersect(seg_vertical, seg_horizontal));
}
