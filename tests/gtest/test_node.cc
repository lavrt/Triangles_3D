#include <gtest/gtest.h>

#include <vector>

#include "node.hpp"

using namespace Geometry;
using namespace Acceleration;

class NodeTest : public ::testing::Test {
protected: 
    std::vector<Triangle<double>> triangles {
        Triangle<double>(1, Point<double>{0,0,0}, Point<double>{1,0,0}, Point<double>{0,1,0}),
        Triangle<double>(2, Point<double>{2,0,0}, Point<double>{3,0,0}, Point<double>{2,1,0})
    };
};

// Constructor -------------------------------------------------------------------------------------

TEST_F(NodeTest, DefaultConstruction) {
    BVHNode<double> node;
    
    EXPECT_TRUE(node.IsLeaf());
    EXPECT_EQ(node.GetNumberOfTriangles(), 0);
    EXPECT_EQ(node.GetLeft(), nullptr);
    EXPECT_EQ(node.GetRight(), nullptr);
}

// Other functions ---------------------------------------------------------------------------------

TEST_F(NodeTest, SetTrianglesMakesLeaf) {
    BVHNode<double> node;
    node.SetTriangles(triangles);
    
    EXPECT_TRUE(node.IsLeaf());
    EXPECT_EQ(node.GetNumberOfTriangles(), 2);
    EXPECT_EQ(node.GetTriangles().size(), 2);
    EXPECT_EQ(node.GetLeft(), nullptr);
    EXPECT_EQ(node.GetRight(), nullptr);
}

TEST_F(NodeTest, SetChildrenMakesInternalNode) {
    BVHNode<double> node;
    
    auto left = std::make_unique<BVHNode<double>>();
    auto right = std::make_unique<BVHNode<double>>();
    
    node.SetLeft(std::move(left));
    node.SetRight(std::move(right));
    
    EXPECT_FALSE(node.IsLeaf());
    EXPECT_EQ(node.GetNumberOfTriangles(), 0);
    EXPECT_NE(node.GetLeft(), nullptr);
    EXPECT_NE(node.GetRight(), nullptr);
}

TEST_F(NodeTest, AABBManagement) {
    BVHNode<double> node;
    AABB<double> bbox{Point<double>{0,0,0}, Point<double>{1,1,1}};
    
    node.SetAABB(bbox);
    
    AABB<double> result = node.GetAABB();
    EXPECT_EQ(result.min.x, 0);
    EXPECT_EQ(result.min.y, 0);
    EXPECT_EQ(result.min.z, 0);
    EXPECT_EQ(result.max.x, 1);
    EXPECT_EQ(result.max.y, 1);
    EXPECT_EQ(result.max.z, 1);
}

TEST_F(NodeTest, TriangleSpanIntegrity) {
    BVHNode<double> node;
    node.SetTriangles(triangles);
    
    auto span = node.GetTriangles();
    EXPECT_EQ(span.size(), 2);
    EXPECT_EQ(span[0].GetId(), 1);
    EXPECT_EQ(span[1].GetId(), 2);
}

TEST_F(NodeTest, StateTransitions) {
    BVHNode<double> node;
    
    node.SetLeft(std::make_unique<BVHNode<double>>());
    EXPECT_FALSE(node.IsLeaf());
}
