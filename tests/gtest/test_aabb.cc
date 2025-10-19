#include <gtest/gtest.h>

#include "aabb.hpp"
#include "point.hpp"
#include "details.hpp"

using namespace Geometry;

class AABBTest : public ::testing::Test {
protected:
    AABB<double> aabb1 = AABB<double>(Point<double>(0, 0, 0), Point<double>(1, 1, 1));
    AABB<double> aabb2 = AABB<double>(Point<double>(2, 2, 2), Point<double>(3, 3, 3));
    AABB<double> aabb_overlap = AABB<double>(Point<double>(0.5, 0.5, 0.5), Point<double>(1.5, 1.5, 1.5));
    AABB<double> aabb_touching = AABB<double>(Point<double>(1, 1, 1), Point<double>(2, 2, 2));
    AABB<double> aabb_negative = AABB<double>(Point<double>(-2, -2, -2), Point<double>(-1, -1, -1));
    AABB<double> aabb_large = AABB<double>(Point<double>(-10, -10, -10), Point<double>(10, 10, 10));
};

// Constructors ------------------------------------------------------------------------------------

TEST_F(AABBTest, DefaultConstructorInitializesToExtremes) {
    AABB<double> aabb;
    
    EXPECT_DOUBLE_EQ(aabb.min.x, Limits::MaxValue<double>());
    EXPECT_DOUBLE_EQ(aabb.min.y, Limits::MaxValue<double>());
    EXPECT_DOUBLE_EQ(aabb.min.z, Limits::MaxValue<double>());
    
    EXPECT_DOUBLE_EQ(aabb.max.x, Limits::LowestValue<double>());
    EXPECT_DOUBLE_EQ(aabb.max.y, Limits::LowestValue<double>());
    EXPECT_DOUBLE_EQ(aabb.max.z, Limits::LowestValue<double>());
}

TEST_F(AABBTest, ParameterizedConstructorStoresMinMax) {
    Point<double> min(1, 2, 3);
    Point<double> max(4, 5, 6);
    AABB<double> aabb(min, max);
    
    EXPECT_DOUBLE_EQ(aabb.min.x, 1);
    EXPECT_DOUBLE_EQ(aabb.min.y, 2);
    EXPECT_DOUBLE_EQ(aabb.min.z, 3);
    
    EXPECT_DOUBLE_EQ(aabb.max.x, 4);
    EXPECT_DOUBLE_EQ(aabb.max.y, 5);
    EXPECT_DOUBLE_EQ(aabb.max.z, 6);
}

TEST_F(AABBTest, ConstructorWithReversedCoordinates) {
    AABB<double> aabb(Point<double>(5, 5, 5), Point<double>(1, 1, 1));
    
    EXPECT_DOUBLE_EQ(aabb.min.x, 5);
    EXPECT_DOUBLE_EQ(aabb.max.x, 1);
}

// Expand ------------------------------------------------------------------------------------------

TEST_F(AABBTest, ExpandWithLargerAABBIncreasesBounds) {
    AABB<double> original(Point<double>(0, 0, 0), Point<double>(1, 1, 1));
    AABB<double> larger(Point<double>(-1, -1, -1), Point<double>(2, 2, 2));
    
    original.Expand(larger);
    
    EXPECT_DOUBLE_EQ(original.min.x, -1);
    EXPECT_DOUBLE_EQ(original.min.y, -1);
    EXPECT_DOUBLE_EQ(original.min.z, -1);
    
    EXPECT_DOUBLE_EQ(original.max.x, 2);
    EXPECT_DOUBLE_EQ(original.max.y, 2);
    EXPECT_DOUBLE_EQ(original.max.z, 2);
}

TEST_F(AABBTest, ExpandWithSmallerAABBNoChange) {
    AABB<double> original(Point<double>(0, 0, 0), Point<double>(3, 3, 3));
    AABB<double> smaller(Point<double>(1, 1, 1), Point<double>(2, 2, 2));
    
    original.Expand(smaller);
    
    EXPECT_DOUBLE_EQ(original.min.x, 0);
    EXPECT_DOUBLE_EQ(original.max.x, 3);
}

TEST_F(AABBTest, ExpandWithPartialOverlapExtendsAppropriately) {
    AABB<double> original(Point<double>(0, 0, 0), Point<double>(2, 2, 2));
    AABB<double> other(Point<double>(1, 1, 1), Point<double>(3, 3, 3));
    
    original.Expand(other);
    
    EXPECT_DOUBLE_EQ(original.min.x, 0);
    EXPECT_DOUBLE_EQ(original.max.x, 3);
}

TEST_F(AABBTest, ExpandWithNegativeCoordinates) {
    AABB<double> original(Point<double>(0, 0, 0), Point<double>(1, 1, 1));
    AABB<double> negative(Point<double>(-2, -2, -2), Point<double>(-1, -1, -1));
    
    original.Expand(negative);
    
    EXPECT_DOUBLE_EQ(original.min.x, -2); 
    EXPECT_DOUBLE_EQ(original.max.x, 1);
}

TEST_F(AABBTest, ExpandEmptyAABBWithNormal) {
    AABB<double> empty;
    AABB<double> normal(Point<double>(0, 0, 0), Point<double>(1, 1, 1));
    
    empty.Expand(normal);
    
    EXPECT_DOUBLE_EQ(empty.min.x, 0);
    EXPECT_DOUBLE_EQ(empty.max.x, 1);
}

TEST_F(AABBTest, ExpandNormalWithEmptyAABB) {
    AABB<double> normal(Point<double>(0, 0, 0), Point<double>(1, 1, 1));
    AABB<double> empty;
    
    normal.Expand(empty);
    
    EXPECT_DOUBLE_EQ(normal.min.x, 0);
    EXPECT_DOUBLE_EQ(normal.max.x, 1);
}

TEST_F(AABBTest, ExpandMultipleTimes) {
    AABB<double> aabb(Point<double>(0, 0, 0), Point<double>(1, 1, 1));
    
    aabb.Expand(AABB<double>(Point<double>(-1, 2, 3), Point<double>(4, 5, 6)));
    aabb.Expand(AABB<double>(Point<double>(7, -2, 1), Point<double>(8, 0, 2)));
    
    EXPECT_DOUBLE_EQ(aabb.min.x, -1);
    EXPECT_DOUBLE_EQ(aabb.min.y, -2);
    EXPECT_DOUBLE_EQ(aabb.min.z, 0); 
    
    EXPECT_DOUBLE_EQ(aabb.max.x, 8);
    EXPECT_DOUBLE_EQ(aabb.max.y, 5);
    EXPECT_DOUBLE_EQ(aabb.max.z, 6);
}

// GetCenter ---------------------------------------------------------------------------------------

TEST_F(AABBTest, GetCenterBasic) {
    Point<double> center = aabb1.GetCenter();
    Point<double> expected(0.5, 0.5, 0.5);
    
    EXPECT_DOUBLE_EQ(center.x, expected.x);
    EXPECT_DOUBLE_EQ(center.y, expected.y);
    EXPECT_DOUBLE_EQ(center.z, expected.z);
}

TEST_F(AABBTest, GetCenterSymmetricNegative) {
    AABB<double> aabb(Point<double>(-2, -2, -2), Point<double>(2, 2, 2));
    Point<double> center = aabb.GetCenter();
    
    EXPECT_DOUBLE_EQ(center.x, 0);
    EXPECT_DOUBLE_EQ(center.y, 0);
    EXPECT_DOUBLE_EQ(center.z, 0);
}

TEST_F(AABBTest, GetCenterAsymmetric) {
    AABB<double> aabb(Point<double>(1, 2, 3), Point<double>(5, 6, 7));
    Point<double> center = aabb.GetCenter();
    
    EXPECT_DOUBLE_EQ(center.x, 3.0);
    EXPECT_DOUBLE_EQ(center.y, 4.0);
    EXPECT_DOUBLE_EQ(center.z, 5.0);
}

TEST_F(AABBTest, GetCenterNegativeRange) {
    AABB<double> aabb(Point<double>(-5, -3, -1), Point<double>(-1, -1, 0));
    Point<double> center = aabb.GetCenter();
    
    EXPECT_DOUBLE_EQ(center.x, -3.0);
    EXPECT_DOUBLE_EQ(center.y, -2.0); 
    EXPECT_DOUBLE_EQ(center.z, -0.5); 
}

TEST_F(AABBTest, GetCenterDegenerate) {
    AABB<double> aabb(Point<double>(1, 1, 1), Point<double>(1, 1, 1));
    Point<double> center = aabb.GetCenter();
    
    EXPECT_DOUBLE_EQ(center.x, 1);
    EXPECT_DOUBLE_EQ(center.y, 1);
    EXPECT_DOUBLE_EQ(center.z, 1);
}

// Intersects --------------------------------------------------------------------------------------

TEST_F(AABBTest, IntersectsSeparatedAABB) {
    EXPECT_FALSE(AABB<double>::Intersects(aabb1, aabb2));
}

TEST_F(AABBTest, IntersectsOverlappingAABB) {
    EXPECT_TRUE(AABB<double>::Intersects(aabb1, aabb_overlap));
}

TEST_F(AABBTest, IntersectsTouchingAABB) {
    EXPECT_TRUE(AABB<double>::Intersects(aabb1, aabb_touching));
}

TEST_F(AABBTest, IntersectsIdenticalAABB) {
    EXPECT_TRUE(AABB<double>::Intersects(aabb1, aabb1));
}

TEST_F(AABBTest, IntersectsContainedAABB) {
    AABB<double> small(Point<double>(0.25, 0.25, 0.25), Point<double>(0.75, 0.75, 0.75));
    EXPECT_TRUE(AABB<double>::Intersects(aabb1, small));
    EXPECT_TRUE(AABB<double>::Intersects(small, aabb1));
}

TEST_F(AABBTest, IntersectsNegativeCoordinates) {
    AABB<double> neg1(Point<double>(-3, -3, -3), Point<double>(-2, -2, -2));
    AABB<double> neg2(Point<double>(-2.5, -2.5, -2.5), Point<double>(-1, -1, -1));
    
    EXPECT_TRUE(AABB<double>::Intersects(neg1, neg2));
}

TEST_F(AABBTest, IntersectsNoOverlapOnSomeAxes) {
    AABB<double> x_overlap(Point<double>(0.5, 5, 5), Point<double>(1.5, 6, 6));
    EXPECT_FALSE(AABB<double>::Intersects(aabb1, x_overlap));
}

TEST_F(AABBTest, IntersectsEdgeTouching) {
    AABB<double> right_touch(Point<double>(1, 0.25, 0.25), Point<double>(2, 0.75, 0.75));
    EXPECT_TRUE(AABB<double>::Intersects(aabb1, right_touch));
}

TEST_F(AABBTest, IntersectsDegenerateAABB) {
    AABB<double> point_inside(Point<double>(0.5, 0.5, 0.5), Point<double>(0.5, 0.5, 0.5));
    EXPECT_TRUE(AABB<double>::Intersects(aabb1, point_inside));
    
    AABB<double> point_outside(Point<double>(5, 5, 5), Point<double>(5, 5, 5));
    EXPECT_FALSE(AABB<double>::Intersects(aabb1, point_outside));
}

TEST_F(AABBTest, IntersectsLargeContainsSmall) {
    EXPECT_TRUE(AABB<double>::Intersects(aabb_large, aabb1));
    EXPECT_TRUE(AABB<double>::Intersects(aabb1, aabb_large));
}

TEST_F(AABBTest, IntersectsCommutative) {
    EXPECT_EQ(AABB<double>::Intersects(aabb1, aabb2), AABB<double>::Intersects(aabb2, aabb1));
    EXPECT_EQ(AABB<double>::Intersects(aabb1, aabb_overlap), AABB<double>::Intersects(aabb_overlap, aabb1));
}

// Special cases -----------------------------------------------------------------------------------

TEST_F(AABBTest, DegenerateAABBBehavior) {
    AABB<double> degenerate(Point<double>(1, 1, 1), Point<double>(1, 1, 1));
    
    Point<double> center = degenerate.GetCenter();
    EXPECT_DOUBLE_EQ(center.x, 1);
    
    EXPECT_TRUE(AABB<double>::Intersects(degenerate, degenerate));
    
    EXPECT_TRUE(AABB<double>::Intersects(aabb_large, degenerate));
}

TEST_F(AABBTest, VeryLargeCoordinates) {
    AABB<double> large(
        Point<double>(-1e6, -1e6, -1e6),
        Point<double>(1e6, 1e6, 1e6)
    );
    
    Point<double> center = large.GetCenter();
    EXPECT_DOUBLE_EQ(center.x, 0);
    EXPECT_DOUBLE_EQ(center.y, 0);
    EXPECT_DOUBLE_EQ(center.z, 0);
    
    EXPECT_TRUE(AABB<double>::Intersects(large, aabb1));
}

TEST_F(AABBTest, PrecisionHandling) {
    AABB<double> aabb(Point<double>(0.1, 0.2, 0.3), Point<double>(0.4, 0.5, 0.6));
    
    Point<double> center = aabb.GetCenter();
    EXPECT_DOUBLE_EQ(center.x, 0.25);
    EXPECT_DOUBLE_EQ(center.y, 0.35);
    EXPECT_DOUBLE_EQ(center.z, 0.45);
}

TEST_F(AABBTest, ExpandWithDegenerate) {
    AABB<double> normal(Point<double>(0, 0, 0), Point<double>(1, 1, 1));
    AABB<double> degenerate(Point<double>(2, 2, 2), Point<double>(2, 2, 2));
    
    normal.Expand(degenerate);
    
    EXPECT_DOUBLE_EQ(normal.min.x, 0);  
    EXPECT_DOUBLE_EQ(normal.max.x, 2);  
}
