#include <gtest/gtest.h>

#include <vector>

#include "node.hpp"

class NodeTest : public ::testing::Test {
protected: 
    std::vector<Triangle> triangles {
        Triangle(1, Point{0,0,0}, Point{1,0,0}, Point{0,1,0}),
        Triangle(2, Point{2,0,0}, Point{3,0,0}, Point{2,1,0})
    };
};

// Constructor -------------------------------------------------------------------------------------

TEST_F(NodeTest, DefaultConstruction) {
    BVHNode node;
    
    EXPECT_TRUE(node.IsLeaf());
    EXPECT_EQ(node.GetNumberOfTriangles(), 0);
    EXPECT_EQ(node.GetLeft(), nullptr);
    EXPECT_EQ(node.GetRight(), nullptr);
}

// Other functions ---------------------------------------------------------------------------------

TEST_F(NodeTest, SetTrianglesMakesLeaf) {
    BVHNode node;
    node.SetTriangles(triangles);
    
    EXPECT_TRUE(node.IsLeaf());
    EXPECT_EQ(node.GetNumberOfTriangles(), 2);
    EXPECT_EQ(node.GetTriangles().size(), 2);
    EXPECT_EQ(node.GetLeft(), nullptr);
    EXPECT_EQ(node.GetRight(), nullptr);
}

TEST_F(NodeTest, SetChildrenMakesInternalNode) {
    BVHNode node;
    
    auto left = std::make_unique<BVHNode>();
    auto right = std::make_unique<BVHNode>();
    
    node.SetLeft(std::move(left));
    node.SetRight(std::move(right));
    
    EXPECT_FALSE(node.IsLeaf());
    EXPECT_EQ(node.GetNumberOfTriangles(), 0);
    EXPECT_NE(node.GetLeft(), nullptr);
    EXPECT_NE(node.GetRight(), nullptr);
}

TEST_F(NodeTest, AABBManagement) {
    BVHNode node;
    AABB bbox{Point{0,0,0}, Point{1,1,1}};
    
    node.SetAABB(bbox);
    
    AABB result = node.GetAABB();
    EXPECT_EQ(result.min.x, 0);
    EXPECT_EQ(result.min.y, 0);
    EXPECT_EQ(result.min.z, 0);
    EXPECT_EQ(result.max.x, 1);
    EXPECT_EQ(result.max.y, 1);
    EXPECT_EQ(result.max.z, 1);
}

TEST_F(NodeTest, TriangleSpanIntegrity) {
    BVHNode node;
    node.SetTriangles(triangles);
    
    auto span = node.GetTriangles();
    EXPECT_EQ(span.size(), 2);
    EXPECT_EQ(span[0].GetId(), 1);
    EXPECT_EQ(span[1].GetId(), 2);
}

TEST_F(NodeTest, StateTransitions) {
    BVHNode node;
    
    node.SetLeft(std::make_unique<BVHNode>());
    EXPECT_FALSE(node.IsLeaf());
}
