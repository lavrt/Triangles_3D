#include <gtest/gtest.h>

#include <limits>

#include "aabb.hpp"
#include "point.hpp"

class AABBTest : public ::testing::Test {
protected:
    AABB aabb1 = AABB(Point(0, 0, 0), Point(1, 1, 1));
    AABB aabb2 = AABB(Point(2, 2, 2), Point(3, 3, 3));
    AABB aabb_overlap = AABB(Point(0.5, 0.5, 0.5), Point(1.5, 1.5, 1.5));
    AABB aabb_touching = AABB(Point(1, 1, 1), Point(2, 2, 2));
    AABB aabb_negative = AABB(Point(-2, -2, -2), Point(-1, -1, -1));
    AABB aabb_large = AABB(Point(-10, -10, -10), Point(10, 10, 10));
};

// Constructors ------------------------------------------------------------------------------------

TEST_F(AABBTest, DefaultConstructorInitializesToExtremes) {
    AABB aabb;
    
    EXPECT_DOUBLE_EQ(aabb.min.x, std::numeric_limits<double>::max());
    EXPECT_DOUBLE_EQ(aabb.min.y, std::numeric_limits<double>::max());
    EXPECT_DOUBLE_EQ(aabb.min.z, std::numeric_limits<double>::max());
    
    EXPECT_DOUBLE_EQ(aabb.max.x, std::numeric_limits<double>::lowest());
    EXPECT_DOUBLE_EQ(aabb.max.y, std::numeric_limits<double>::lowest());
    EXPECT_DOUBLE_EQ(aabb.max.z, std::numeric_limits<double>::lowest());
}

TEST_F(AABBTest, ParameterizedConstructorStoresMinMax) {
    Point min(1, 2, 3);
    Point max(4, 5, 6);
    AABB aabb(min, max);
    
    EXPECT_DOUBLE_EQ(aabb.min.x, 1);
    EXPECT_DOUBLE_EQ(aabb.min.y, 2);
    EXPECT_DOUBLE_EQ(aabb.min.z, 3);
    
    EXPECT_DOUBLE_EQ(aabb.max.x, 4);
    EXPECT_DOUBLE_EQ(aabb.max.y, 5);
    EXPECT_DOUBLE_EQ(aabb.max.z, 6);
}

TEST_F(AABBTest, ConstructorWithReversedCoordinates) {
    AABB aabb(Point(5, 5, 5), Point(1, 1, 1));
    
    EXPECT_DOUBLE_EQ(aabb.min.x, 5);
    EXPECT_DOUBLE_EQ(aabb.max.x, 1);
}

// Expand ------------------------------------------------------------------------------------------

TEST_F(AABBTest, ExpandWithLargerAABBIncreasesBounds) {
    AABB original(Point(0, 0, 0), Point(1, 1, 1));
    AABB larger(Point(-1, -1, -1), Point(2, 2, 2));
    
    original.Expand(larger);
    
    EXPECT_DOUBLE_EQ(original.min.x, -1);
    EXPECT_DOUBLE_EQ(original.min.y, -1);
    EXPECT_DOUBLE_EQ(original.min.z, -1);
    
    EXPECT_DOUBLE_EQ(original.max.x, 2);
    EXPECT_DOUBLE_EQ(original.max.y, 2);
    EXPECT_DOUBLE_EQ(original.max.z, 2);
}

TEST_F(AABBTest, ExpandWithSmallerAABBNoChange) {
    AABB original(Point(0, 0, 0), Point(3, 3, 3));
    AABB smaller(Point(1, 1, 1), Point(2, 2, 2));
    
    original.Expand(smaller);
    
    EXPECT_DOUBLE_EQ(original.min.x, 0);
    EXPECT_DOUBLE_EQ(original.max.x, 3);
}

TEST_F(AABBTest, ExpandWithPartialOverlapExtendsAppropriately) {
    AABB original(Point(0, 0, 0), Point(2, 2, 2));
    AABB other(Point(1, 1, 1), Point(3, 3, 3));
    
    original.Expand(other);
    
    EXPECT_DOUBLE_EQ(original.min.x, 0);
    EXPECT_DOUBLE_EQ(original.max.x, 3);
}

TEST_F(AABBTest, ExpandWithNegativeCoordinates) {
    AABB original(Point(0, 0, 0), Point(1, 1, 1));
    AABB negative(Point(-2, -2, -2), Point(-1, -1, -1));
    
    original.Expand(negative);
    
    EXPECT_DOUBLE_EQ(original.min.x, -2); 
    EXPECT_DOUBLE_EQ(original.max.x, 1);
}

TEST_F(AABBTest, ExpandEmptyAABBWithNormal) {
    AABB empty;
    AABB normal(Point(0, 0, 0), Point(1, 1, 1));
    
    empty.Expand(normal);
    
    EXPECT_DOUBLE_EQ(empty.min.x, 0);
    EXPECT_DOUBLE_EQ(empty.max.x, 1);
}

TEST_F(AABBTest, ExpandNormalWithEmptyAABB) {
    AABB normal(Point(0, 0, 0), Point(1, 1, 1));
    AABB empty;
    
    normal.Expand(empty);
    
    EXPECT_DOUBLE_EQ(normal.min.x, 0);
    EXPECT_DOUBLE_EQ(normal.max.x, 1);
}

TEST_F(AABBTest, ExpandMultipleTimes) {
    AABB aabb(Point(0, 0, 0), Point(1, 1, 1));
    
    aabb.Expand(AABB(Point(-1, 2, 3), Point(4, 5, 6)));
    aabb.Expand(AABB(Point(7, -2, 1), Point(8, 0, 2)));
    
    EXPECT_DOUBLE_EQ(aabb.min.x, -1);
    EXPECT_DOUBLE_EQ(aabb.min.y, -2);
    EXPECT_DOUBLE_EQ(aabb.min.z, 0); 
    
    EXPECT_DOUBLE_EQ(aabb.max.x, 8);
    EXPECT_DOUBLE_EQ(aabb.max.y, 5);
    EXPECT_DOUBLE_EQ(aabb.max.z, 6);
}

// GetCenter ---------------------------------------------------------------------------------------

TEST_F(AABBTest, GetCenterBasic) {
    Point center = aabb1.GetCenter();
    Point expected(0.5, 0.5, 0.5);
    
    EXPECT_DOUBLE_EQ(center.x, expected.x);
    EXPECT_DOUBLE_EQ(center.y, expected.y);
    EXPECT_DOUBLE_EQ(center.z, expected.z);
}

TEST_F(AABBTest, GetCenterSymmetricNegative) {
    AABB aabb(Point(-2, -2, -2), Point(2, 2, 2));
    Point center = aabb.GetCenter();
    
    EXPECT_DOUBLE_EQ(center.x, 0);
    EXPECT_DOUBLE_EQ(center.y, 0);
    EXPECT_DOUBLE_EQ(center.z, 0);
}

TEST_F(AABBTest, GetCenterAsymmetric) {
    AABB aabb(Point(1, 2, 3), Point(5, 6, 7));
    Point center = aabb.GetCenter();
    
    EXPECT_DOUBLE_EQ(center.x, 3.0);
    EXPECT_DOUBLE_EQ(center.y, 4.0);
    EXPECT_DOUBLE_EQ(center.z, 5.0);
}

TEST_F(AABBTest, GetCenterNegativeRange) {
    AABB aabb(Point(-5, -3, -1), Point(-1, -1, 0));
    Point center = aabb.GetCenter();
    
    EXPECT_DOUBLE_EQ(center.x, -3.0);
    EXPECT_DOUBLE_EQ(center.y, -2.0); 
    EXPECT_DOUBLE_EQ(center.z, -0.5); 
}

TEST_F(AABBTest, GetCenterDegenerate) {
    AABB aabb(Point(1, 1, 1), Point(1, 1, 1));
    Point center = aabb.GetCenter();
    
    EXPECT_DOUBLE_EQ(center.x, 1);
    EXPECT_DOUBLE_EQ(center.y, 1);
    EXPECT_DOUBLE_EQ(center.z, 1);
}

// Intersects --------------------------------------------------------------------------------------

TEST_F(AABBTest, IntersectsSeparatedAABB) {
    EXPECT_FALSE(AABB::Intersects(aabb1, aabb2));
}

TEST_F(AABBTest, IntersectsOverlappingAABB) {
    EXPECT_TRUE(AABB::Intersects(aabb1, aabb_overlap));
}

TEST_F(AABBTest, IntersectsTouchingAABB) {
    EXPECT_TRUE(AABB::Intersects(aabb1, aabb_touching));
}

TEST_F(AABBTest, IntersectsIdenticalAABB) {
    EXPECT_TRUE(AABB::Intersects(aabb1, aabb1));
}

TEST_F(AABBTest, IntersectsContainedAABB) {
    AABB small(Point(0.25, 0.25, 0.25), Point(0.75, 0.75, 0.75));
    EXPECT_TRUE(AABB::Intersects(aabb1, small));
    EXPECT_TRUE(AABB::Intersects(small, aabb1));
}

TEST_F(AABBTest, IntersectsNegativeCoordinates) {
    AABB neg1(Point(-3, -3, -3), Point(-2, -2, -2));
    AABB neg2(Point(-2.5, -2.5, -2.5), Point(-1, -1, -1));
    
    EXPECT_TRUE(AABB::Intersects(neg1, neg2));
}

TEST_F(AABBTest, IntersectsNoOverlapOnSomeAxes) {
    AABB x_overlap(Point(0.5, 5, 5), Point(1.5, 6, 6));
    EXPECT_FALSE(AABB::Intersects(aabb1, x_overlap));
}

TEST_F(AABBTest, IntersectsEdgeTouching) {
    AABB right_touch(Point(1, 0.25, 0.25), Point(2, 0.75, 0.75));
    EXPECT_TRUE(AABB::Intersects(aabb1, right_touch));
}

TEST_F(AABBTest, IntersectsDegenerateAABB) {
    AABB point_inside(Point(0.5, 0.5, 0.5), Point(0.5, 0.5, 0.5));
    EXPECT_TRUE(AABB::Intersects(aabb1, point_inside));
    
    AABB point_outside(Point(5, 5, 5), Point(5, 5, 5));
    EXPECT_FALSE(AABB::Intersects(aabb1, point_outside));
}

TEST_F(AABBTest, IntersectsLargeContainsSmall) {
    EXPECT_TRUE(AABB::Intersects(aabb_large, aabb1));
    EXPECT_TRUE(AABB::Intersects(aabb1, aabb_large));
}

TEST_F(AABBTest, IntersectsCommutative) {
    EXPECT_EQ(AABB::Intersects(aabb1, aabb2), AABB::Intersects(aabb2, aabb1));
    EXPECT_EQ(AABB::Intersects(aabb1, aabb_overlap), AABB::Intersects(aabb_overlap, aabb1));
}

// Special cases -----------------------------------------------------------------------------------

TEST_F(AABBTest, DegenerateAABBBehavior) {
    AABB degenerate(Point(1, 1, 1), Point(1, 1, 1));
    
    Point center = degenerate.GetCenter();
    EXPECT_DOUBLE_EQ(center.x, 1);
    
    EXPECT_TRUE(AABB::Intersects(degenerate, degenerate));
    
    EXPECT_TRUE(AABB::Intersects(aabb_large, degenerate));
}

TEST_F(AABBTest, VeryLargeCoordinates) {
    AABB large(
        Point(-1e6, -1e6, -1e6),
        Point(1e6, 1e6, 1e6)
    );
    
    Point center = large.GetCenter();
    EXPECT_DOUBLE_EQ(center.x, 0);
    EXPECT_DOUBLE_EQ(center.y, 0);
    EXPECT_DOUBLE_EQ(center.z, 0);
    
    EXPECT_TRUE(AABB::Intersects(large, aabb1));
}

TEST_F(AABBTest, PrecisionHandling) {
    AABB aabb(Point(0.1, 0.2, 0.3), Point(0.4, 0.5, 0.6));
    
    Point center = aabb.GetCenter();
    EXPECT_NEAR(center.x, 0.25, 1e-10);
    EXPECT_NEAR(center.y, 0.35, 1e-10);
    EXPECT_NEAR(center.z, 0.45, 1e-10);
}

TEST_F(AABBTest, ExpandWithDegenerate) {
    AABB normal(Point(0, 0, 0), Point(1, 1, 1));
    AABB degenerate(Point(2, 2, 2), Point(2, 2, 2));
    
    normal.Expand(degenerate);
    
    EXPECT_DOUBLE_EQ(normal.min.x, 0);  
    EXPECT_DOUBLE_EQ(normal.max.x, 2);  
}
