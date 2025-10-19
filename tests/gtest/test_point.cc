#include <gtest/gtest.h>

#include "point.hpp"

class PointTest : public ::testing::Test {
protected:
    const Point<double> p1{1.0, 2.0, 3.0};
    const Point<double> p2{4.0, 5.0, 6.0};
    const Point<double> p_origin{0.0, 0.0, 0.0};
    const Point<double> p_negative{-1.0, -2.0, -3.0};
};

// Constructor -------------------------------------------------------------------------------------

TEST_F(PointTest, Constructor) {
    Point<double> p(1.5, 2.5, 3.5);
    
    EXPECT_DOUBLE_EQ(p.x, 1.5);
    EXPECT_DOUBLE_EQ(p.y, 2.5);
    EXPECT_DOUBLE_EQ(p.z, 3.5);
}

// Operator[] --------------------------------------------------------------------------------------

TEST_F(PointTest, IndexOperator) {
    EXPECT_DOUBLE_EQ(p1[0], 1.0);
    EXPECT_DOUBLE_EQ(p1[1], 2.0);
    EXPECT_DOUBLE_EQ(p1[2], 3.0);
}

TEST_F(PointTest, IndexOperatorConstCorrectness) {
    const Point<double> p = p1;
    
    EXPECT_DOUBLE_EQ(p[0], 1.0);
    EXPECT_DOUBLE_EQ(p[1], 2.0);
    EXPECT_DOUBLE_EQ(p[2], 3.0);
}

TEST_F(PointTest, IndexOperatorEdgeCases) {
    EXPECT_THROW(p1[3], std::out_of_range);
}

// Operator- ---------------------------------------------------------------------------------------

TEST_F(PointTest, PointSubtraction) {
    Vector<double> result = p2 - p1;
    Vector<double> expected(3.0, 3.0, 3.0);
    
    EXPECT_DOUBLE_EQ(result.x, expected.x);
    EXPECT_DOUBLE_EQ(result.y, expected.y);
    EXPECT_DOUBLE_EQ(result.z, expected.z);
}

TEST_F(PointTest, PointSubtractionCommutativeProperty) {
    Vector<double> v1 = p2 - p1;
    Vector<double> v2 = p1 - p2;
    
    EXPECT_DOUBLE_EQ(v1.x, -v2.x);
    EXPECT_DOUBLE_EQ(v1.y, -v2.y);
    EXPECT_DOUBLE_EQ(v1.z, -v2.z);
}

TEST_F(PointTest, PointSubtractionWithOrigin) {
    Vector<double> result = p1 - p_origin;
    
    EXPECT_DOUBLE_EQ(result.x, p1.x);
    EXPECT_DOUBLE_EQ(result.y, p1.y);
    EXPECT_DOUBLE_EQ(result.z, p1.z);
}

TEST_F(PointTest, PointSubtractionSamePoint) {
    Vector<double> result = p1 - p1;
    Vector<double> expected(0.0, 0.0, 0.0);
    
    EXPECT_DOUBLE_EQ(result.x, expected.x);
    EXPECT_DOUBLE_EQ(result.y, expected.y);
    EXPECT_DOUBLE_EQ(result.z, expected.z);
}

TEST_F(PointTest, PointSubtractionNegativeCoordinates) {
    Vector<double> result = p_negative - p_origin;
    Vector<double> expected(-1.0, -2.0, -3.0);
    
    EXPECT_DOUBLE_EQ(result.x, expected.x);
    EXPECT_DOUBLE_EQ(result.y, expected.y);
    EXPECT_DOUBLE_EQ(result.z, expected.z);
}

// AsVector ----------------------------------------------------------------------------------------

TEST_F(PointTest, AsVector) {
    Vector<double> result = p1.AsVector();
    Vector<double> expected(1.0, 2.0, 3.0);
    
    EXPECT_DOUBLE_EQ(result.x, expected.x);
    EXPECT_DOUBLE_EQ(result.y, expected.y);
    EXPECT_DOUBLE_EQ(result.z, expected.z);
}

TEST_F(PointTest, AsVectorOrigin) {
    Vector<double> result = p_origin.AsVector();
    Vector<double> expected(0.0, 0.0, 0.0);
    
    EXPECT_DOUBLE_EQ(result.x, expected.x);
    EXPECT_DOUBLE_EQ(result.y, expected.y);
    EXPECT_DOUBLE_EQ(result.z, expected.z);
}

TEST_F(PointTest, AsVectorNegative) {
    Vector<double> result = p_negative.AsVector();
    Vector<double> expected(-1.0, -2.0, -3.0);
    
    EXPECT_DOUBLE_EQ(result.x, expected.x);
    EXPECT_DOUBLE_EQ(result.y, expected.y);
    EXPECT_DOUBLE_EQ(result.z, expected.z);
}

// operator<< --------------------------------------------------------------------------------------

TEST_F(PointTest, OutputOperator) {
    std::ostringstream oss;
    oss << p1;
    std::string result = oss.str();
    
    EXPECT_EQ(result, "(1, 2, 3)");
}

TEST_F(PointTest, OutputOperatorOrigin) {
    std::ostringstream oss;
    oss << p_origin;
    std::string result = oss.str();
    
    EXPECT_EQ(result, "(0, 0, 0)");
}

TEST_F(PointTest, OutputOperatorNegative) {
    std::ostringstream oss;
    oss << p_negative;
    std::string result = oss.str();

    EXPECT_EQ(result, "(-1, -2, -3)");
}

TEST_F(PointTest, OutputOperatorChaining) {
    std::ostringstream oss;
    oss << p1 << " " << p2; 
    std::string result = oss.str();

    EXPECT_EQ(result, "(1, 2, 3) (4, 5, 6)");
}

// Consistency of operations -----------------------------------------------------------------------

TEST_F(PointTest, ConsistencyBetweenOperations) {
    Vector<double> v1 = p1.AsVector();
    Vector<double> v2 = p1 - p_origin;
    
    EXPECT_DOUBLE_EQ(v1.x, v2.x);
    EXPECT_DOUBLE_EQ(v1.y, v2.y);
    EXPECT_DOUBLE_EQ(v1.z, v2.z);
}

TEST_F(PointTest, VectorPointRelationship) {
    Point<double> p(1, 2, 3);
    Vector<double> v = p.AsVector();
    
    EXPECT_DOUBLE_EQ(p.x, v.x);
    EXPECT_DOUBLE_EQ(p.y, v.y);
    EXPECT_DOUBLE_EQ(p.z, v.z);
}
