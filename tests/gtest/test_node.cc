#include <gtest/gtest.h>

#include <vector>

#include "node.hpp"

using namespace Geometry;
using namespace Geometry::Acceleration;

class NodeTest : public ::testing::Test {
protected:
    std::vector<IndexedTriangle<double>> triangles {
        IndexedTriangle<double>(1, {Point<double>{0,0,0}, Point<double>{1,0,0}, Point<double>{0,1,0}}),
        IndexedTriangle<double>(2, {Point<double>{2,0,0}, Point<double>{3,0,0}, Point<double>{2,1,0}})
    };

    AABB<double> bbox{Point<double>{0,0,0}, Point<double>{1,1,1}};
};

// Constructors -------------------------------------------------------------------------------------

TEST_F(NodeTest, LeafConstruction) {
    BVHNode<double> node(bbox, std::span<const IndexedTriangle<double>>(triangles));

    EXPECT_TRUE(node.IsLeaf());
    EXPECT_EQ(node.GetNumberOfTriangles(), 2u);
    EXPECT_EQ(node.GetTriangles().size(), 2u);
    EXPECT_EQ(node.GetLeftIdx(), invalid_idx);
    EXPECT_EQ(node.GetRightIdx(), invalid_idx);
}

TEST_F(NodeTest, InternalConstruction) {
    constexpr NodeIdx L = 10;
    constexpr NodeIdx R = 11;

    BVHNode<double> node(bbox, L, R);

    EXPECT_FALSE(node.IsLeaf());
    EXPECT_EQ(node.GetNumberOfTriangles(), 0u);
    EXPECT_EQ(node.GetTriangles().size(), 0u);
    EXPECT_EQ(node.GetLeftIdx(), L);
    EXPECT_EQ(node.GetRightIdx(), R);
}

// AABB ---------------------------------------------------------------------------------------------

TEST_F(NodeTest, AABBIntegrityLeaf) {
    BVHNode<double> node(bbox, std::span<const IndexedTriangle<double>>(triangles));

    const auto& result = node.GetAABB();
    EXPECT_EQ(result.min.x, 0);
    EXPECT_EQ(result.min.y, 0);
    EXPECT_EQ(result.min.z, 0);
    EXPECT_EQ(result.max.x, 1);
    EXPECT_EQ(result.max.y, 1);
    EXPECT_EQ(result.max.z, 1);
}

TEST_F(NodeTest, AABBIntegrityInternal) {
    constexpr NodeIdx L = 0;
    constexpr NodeIdx R = 1;

    BVHNode<double> node(bbox, L, R);

    const auto& result = node.GetAABB();
    EXPECT_EQ(result.min.x, 0);
    EXPECT_EQ(result.min.y, 0);
    EXPECT_EQ(result.min.z, 0);
    EXPECT_EQ(result.max.x, 1);
    EXPECT_EQ(result.max.y, 1);
    EXPECT_EQ(result.max.z, 1);
}

// Triangle span ------------------------------------------------------------------------------------

TEST_F(NodeTest, TriangleSpanIntegrity) {
    BVHNode<double> node(bbox, std::span<const IndexedTriangle<double>>(triangles));

    auto span = node.GetTriangles();
    ASSERT_EQ(span.size(), 2u);
    EXPECT_EQ(span[0].id, 1u);
    EXPECT_EQ(span[1].id, 2u);
}
