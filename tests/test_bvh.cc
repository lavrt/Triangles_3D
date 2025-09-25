#include <gtest/gtest.h>

#include <filesystem>

#include "bvh.hpp"

class BVHTest : public ::testing::Test {
protected:    
    std::vector<Triangle> triangles {
        Triangle(1, Point{0,0,0}, Point{1,0,0}, Point{0,1,0}),
        Triangle(2, Point{2,0,0}, Point{3,0,0}, Point{2,1,0}),
        Triangle(3, Point{4,0,0}, Point{5,0,0}, Point{4,1,0}),
        Triangle(4, Point{0,2,0}, Point{1,2,0}, Point{0,3,0}),
        Triangle(5, Point{2,2,0}, Point{3,2,0}, Point{2,3,0}),
        Triangle(6, Point{4,2,0}, Point{5,2,0}, Point{4,3,0})
    };
};

// Constructor and special cases -------------------------------------------------------------------

TEST_F(BVHTest, ConstructionAndBuild) {
    BVH bvh(std::move(triangles));
    
    EXPECT_EQ(bvh.GetRoot(), nullptr);
    
    bvh.Build();
    EXPECT_NE(bvh.GetRoot(), nullptr);
}

TEST_F(BVHTest, EmptyTriangleList) {
    std::vector<Triangle> empty_triangles;
    BVH bvh(std::move(empty_triangles));
    
    EXPECT_NO_THROW(bvh.Build());
}

TEST_F(BVHTest, SingleTriangle) {
    std::vector<Triangle> single_triangle = {
        Triangle(1, Point{0,0,0}, Point{1,0,0}, Point{0,1,0})
    };
    
    BVH bvh(std::move(single_triangle));
    bvh.Build();
    
    ASSERT_NE(bvh.GetRoot(), nullptr);
    EXPECT_TRUE(bvh.GetRoot()->IsLeaf());
    EXPECT_EQ(bvh.GetRoot()->GetNumberOfTriangles(), 1);
}

// Lookup intersections ----------------------------------------------------------------------------

TEST_F(BVHTest, NoIntersectionsForSeparatedTriangles) {
    std::vector<Triangle> separated_triangles = {
        Triangle(1, Point{0,0,0}, Point{1,0,0}, Point{0,1,0}),
        Triangle(2, Point{10,0,0}, Point{11,0,0}, Point{10,1,0})
    };
    
    BVH bvh(std::move(separated_triangles));
    bvh.Build();
    
    auto intersections = bvh.FindIntersectingTriangles();
    EXPECT_TRUE(intersections.empty());
}

TEST_F(BVHTest, IntersectingTrianglesDetection) {
    std::vector<Triangle> intersecting_triangles = {
        Triangle(1, Point{0,0,0}, Point{2,0,0}, Point{0,2,0}),
        Triangle(2, Point{1,1,0}, Point{3,1,0}, Point{1,3,0})
    };
    
    BVH bvh(std::move(intersecting_triangles));
    bvh.Build();
    
    auto intersections = bvh.FindIntersectingTriangles();
    EXPECT_FALSE(intersections.empty());
    EXPECT_GE(intersections.size(), 2);
}

TEST_F(BVHTest, SelfIntersectionNotReported) {
    std::vector<Triangle> single_triangle = {
        Triangle(1, Point{0,0,0}, Point{1,0,0}, Point{0,1,0})
    };
    
    BVH bvh(std::move(single_triangle));
    bvh.Build();
    
    auto intersections = bvh.FindIntersectingTriangles();
    EXPECT_TRUE(intersections.empty());
}

// Special cases -----------------------------------------------------------------------------------

TEST_F(BVHTest, LargeNumberOfTriangles) {
    std::vector<Triangle> large_set;
    const int num_triangles = 1000;
    
    for (int i = 0; i < num_triangles; ++i) {
        double x = static_cast<double>(i);
        large_set.emplace_back(
            i, Point{x,0,0}, Point{x+1,0,0}, Point{x,1,0}
        );
    }
    
    BVH bvh(std::move(large_set));
    
    bvh.Build();
    
    EXPECT_NE(bvh.GetRoot(), nullptr);
}
