#include <gtest/gtest.h>

#include <filesystem>

#include "bvh.hpp"
#include "indexed_triangle.hpp"

using namespace Geometry;
using namespace Acceleration;

class BVHTest : public ::testing::Test {
protected:    
    std::vector<IndexedTriangle<double>> triangles {
        IndexedTriangle<double>(1, {Point<double>{0,0,0}, Point<double>{1,0,0}, Point<double>{0,1,0}}),
        IndexedTriangle<double>(2, {Point<double>{2,0,0}, Point<double>{3,0,0}, Point<double>{2,1,0}}),
        IndexedTriangle<double>(3, {Point<double>{4,0,0}, Point<double>{5,0,0}, Point<double>{4,1,0}}),
        IndexedTriangle<double>(4, {Point<double>{0,2,0}, Point<double>{1,2,0}, Point<double>{0,3,0}}),
        IndexedTriangle<double>(5, {Point<double>{2,2,0}, Point<double>{3,2,0}, Point<double>{2,3,0}}),
        IndexedTriangle<double>(6, {Point<double>{4,2,0}, Point<double>{5,2,0}, Point<double>{4,3,0}})
    };
};

// Constructor and special cases -------------------------------------------------------------------

TEST_F(BVHTest, ConstructionAndBuild) {
    BVH bvh(std::move(triangles));
    
    EXPECT_NE(bvh.GetRoot(), nullptr);
}

TEST_F(BVHTest, SingleTriangle) {
    std::vector<IndexedTriangle<double>> single_triangle = {
        IndexedTriangle<double>(1, {Point<double>{0,0,0}, Point<double>{1,0,0}, Point<double>{0,1,0}})
    };
    
    BVH bvh(std::move(single_triangle));

    std::cerr << "__________________________________\n" << bvh.GetRoot()->GetNumberOfTriangles() << "______________________\n";
    
    ASSERT_NE(bvh.GetRoot(), nullptr);
    EXPECT_TRUE(bvh.GetRoot()->IsLeaf());
    EXPECT_EQ(bvh.GetRoot()->GetNumberOfTriangles(), 1);
}

// Lookup intersections ----------------------------------------------------------------------------

TEST_F(BVHTest, NoIntersectionsForSeparatedTriangles) {
    std::vector<IndexedTriangle<double>> separated_triangles {
        IndexedTriangle<double>(1, {Point<double>{0,0,0}, Point<double>{1,0,0}, Point<double>{0,1,0}}),
        IndexedTriangle<double>(2, {Point<double>{10,0,0}, Point<double>{11,0,0}, Point<double>{10,1,0}})
    };
    
    BVH<double> bvh(std::move(separated_triangles));
    
    auto intersections = bvh.FindIntersectingTriangles();
    EXPECT_TRUE(intersections.empty());
}

TEST_F(BVHTest, IntersectingTrianglesDetection) {
    std::vector<IndexedTriangle<double>> intersecting_triangles {
        IndexedTriangle<double>(1, {Point<double>{0,0,0}, Point<double>{2,0,0}, Point<double>{0,2,0}}),
        IndexedTriangle<double>(2, {Point<double>{1,1,0}, Point<double>{3,1,0}, Point<double>{1,3,0}})
    };
    
    BVH<double> bvh(std::move(intersecting_triangles));
    
    auto intersections = bvh.FindIntersectingTriangles();
    EXPECT_FALSE(intersections.empty());
    EXPECT_GE(intersections.size(), 2);
}

TEST_F(BVHTest, SelfIntersectionNotReported) {
    std::vector<IndexedTriangle<double>> single_triangle {
        IndexedTriangle<double>(1, {Point<double>{0,0,0}, Point<double>{1,0,0}, Point<double>{0,1,0}})
    };
    
    BVH<double> bvh(std::move(single_triangle));
    
    auto intersections = bvh.FindIntersectingTriangles();
    EXPECT_TRUE(intersections.empty());
}

// Special cases -----------------------------------------------------------------------------------

TEST_F(BVHTest, LargeNumberOfTriangles) {
    std::vector<IndexedTriangle<double>> large_set;
    const int num_triangles = 1000;
    
    for (int i = 0; i < num_triangles; ++i) {
        double x = static_cast<double>(i);
        large_set.emplace_back(
            i, Geometry::Triangle{Point<double>{x,0,0}, Point<double>{x+1,0,0}, Point<double>{x,1,0}}
        );
    }
    
    BVH<double> bvh(std::move(large_set));
    
    EXPECT_NE(bvh.GetRoot(), nullptr);
}
