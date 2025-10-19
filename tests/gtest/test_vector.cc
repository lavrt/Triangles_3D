#include <gtest/gtest.h>

#include <cmath>

#include "vector.hpp"

class VectorTest : public ::testing::Test {
protected:
    const Vector<double> v1{1, 2, 3};
    const Vector<double> v2{4, 5, 6};
    const Vector<double> v_zero{0, 0, 0};
    const Vector<double> v_unit_x{1.0, 0.0, 0.0};
    const Vector<double> v_unit_y{0.0, 1.0, 0.0};
    const Vector<double> v_unit_z{0.0, 0.0, 1.0};
};

// Constructor -------------------------------------------------------------------------------------

TEST_F(VectorTest, Constructor) {
    Vector<double> v(1.5, 2.5, 3.5);

    EXPECT_DOUBLE_EQ(v.x, 1.5);
    EXPECT_DOUBLE_EQ(v.y, 2.5);
    EXPECT_DOUBLE_EQ(v.z, 3.5);
}

// Dot ---------------------------------------------------------------------------------------------

TEST_F(VectorTest, DotProduct) {
    double result = Vector<double>::Dot(v1, v2);

    EXPECT_DOUBLE_EQ(result, 32.0);
}

TEST_F(VectorTest, DotProductCommutative) {
    double dot_1 = Vector<double>::Dot(v1, v2);
    double dot_2 = Vector<double>::Dot(v2, v1);

    EXPECT_DOUBLE_EQ(dot_1, dot_2);
}

TEST_F(VectorTest, DotProductWithZeroVector) {
    double result = Vector<double>::Dot(v_zero, v1);

    EXPECT_DOUBLE_EQ(result, 0.0);
}

TEST_F(VectorTest, DotProductOrthogonalVectors) {
    double result = Vector<double>::Dot(v_unit_x, v_unit_y);
    EXPECT_DOUBLE_EQ(result, 0.0);
}

// Cross -------------------------------------------------------------------------------------------

TEST_F(VectorTest, CrossProduct) {
    Vector<double> result = Vector<double>::Cross(v1, v2);
    Vector<double> expected(-3.0, 6.0, -3.0);

    EXPECT_DOUBLE_EQ(result.x, expected.x);
    EXPECT_DOUBLE_EQ(result.y, expected.y);
    EXPECT_DOUBLE_EQ(result.z, expected.z);
}

TEST_F(VectorTest, CrossProductOrthogonal) {
    Vector<double> cross = Vector<double>::Cross(v1, v2);

    EXPECT_DOUBLE_EQ(Vector<double>::Dot(cross, v1), 0.0);
    EXPECT_DOUBLE_EQ(Vector<double>::Dot(cross, v2), 0.0);
}

TEST_F(VectorTest, CrossProductAnticommutative) {
    Vector<double> cross_1 = Vector<double>::Cross(v1, v2);
    Vector<double> cross_2 = Vector<double>::Cross(v2, v1);

    EXPECT_DOUBLE_EQ(cross_1.x, -cross_2.x);
    EXPECT_DOUBLE_EQ(cross_1.y, -cross_2.y);
    EXPECT_DOUBLE_EQ(cross_1.z, -cross_2.z);
}

TEST_F(VectorTest, CrossProductWithZeroVector) {
    Vector<double> result = Vector<double>::Cross(v_zero, v1);

    EXPECT_DOUBLE_EQ(result.x, 0.0);
    EXPECT_DOUBLE_EQ(result.y, 0.0);
    EXPECT_DOUBLE_EQ(result.z, 0.0);
}

TEST_F(VectorTest, CrossProductUnitVectors) {
    Vector<double> result_ij = Vector<double>::Cross(v_unit_x, v_unit_y);
    EXPECT_DOUBLE_EQ(result_ij.x, v_unit_z.x);
    EXPECT_DOUBLE_EQ(result_ij.y, v_unit_z.y);
    EXPECT_DOUBLE_EQ(result_ij.z, v_unit_z.z);
    
    Vector<double> result_jk = Vector<double>::Cross(v_unit_y, v_unit_z);
    EXPECT_DOUBLE_EQ(result_jk.x, v_unit_x.x);
    EXPECT_DOUBLE_EQ(result_jk.y, v_unit_x.y);
    EXPECT_DOUBLE_EQ(result_jk.z, v_unit_x.z);
    
    Vector<double> result_ki = Vector<double>::Cross(v_unit_z, v_unit_x);
    EXPECT_DOUBLE_EQ(result_ki.x, v_unit_y.x);
    EXPECT_DOUBLE_EQ(result_ki.y, v_unit_y.y);
    EXPECT_DOUBLE_EQ(result_ki.z, v_unit_y.z);
}

TEST_F(VectorTest, CrossProductParallelVectors) {
    Vector<double> parallel_v1{2.0, 4.0, 6.0};
    Vector<double> result = Vector<double>::Cross(v1, parallel_v1);
    
    EXPECT_DOUBLE_EQ(result.x, 0.0);
    EXPECT_DOUBLE_EQ(result.y, 0.0);
    EXPECT_DOUBLE_EQ(result.z, 0.0);
}

// Length ------------------------------------------------------------------------------------------

TEST_F(VectorTest, Length) {
    double result = v1.Length();
    double expected = std::sqrt(14);

    EXPECT_DOUBLE_EQ(result, expected);
}

TEST_F(VectorTest, LengthOfUnitVector) {
    EXPECT_DOUBLE_EQ(v_unit_x.Length(), 1.0);
    EXPECT_DOUBLE_EQ(v_unit_y.Length(), 1.0);
    EXPECT_DOUBLE_EQ(v_unit_z.Length(), 1.0);
}

TEST_F(VectorTest, LengthOfZeroVector) {
    EXPECT_DOUBLE_EQ(v_zero.Length(), 0.0);
}

// Normalization -----------------------------------------------------------------------------------

TEST_F(VectorTest, NormalizedVector) {
    Vector<double> normalized = v1.Normalized();
    double length = normalized.Length();
    
    EXPECT_DOUBLE_EQ(length, 1.0);
    
    double ratio_x = normalized.x / v1.x;
    double ratio_y = normalized.y / v1.y;
    double ratio_z = normalized.z / v1.z;
    
    EXPECT_DOUBLE_EQ(ratio_x, ratio_y);
    EXPECT_DOUBLE_EQ(ratio_y, ratio_z); 
}

TEST_F(VectorTest, NormalizedUnitVector) {
    Vector<double> normalized = v_unit_x.Normalized();
    EXPECT_DOUBLE_EQ(normalized.x, 1.0);
    EXPECT_DOUBLE_EQ(normalized.y, 0.0);
    EXPECT_DOUBLE_EQ(normalized.z, 0.0);
    EXPECT_DOUBLE_EQ(normalized.Length(), 1.0);
}

TEST_F(VectorTest, NormalizedZeroVector) {
    Vector<double> zero_normalized = v_zero.Normalized();

    EXPECT_DOUBLE_EQ(zero_normalized.x, 0.0);
    EXPECT_DOUBLE_EQ(zero_normalized.y, 0.0);
    EXPECT_DOUBLE_EQ(zero_normalized.z, 0.0);
}

// Collinearity ------------------------------------------------------------------------------------

TEST_F(VectorTest, CollinearVectors) {
    Vector<double> v1_doubled{2, 4, 6};

    EXPECT_TRUE(v1.Collinear(v1_doubled));
    EXPECT_TRUE(v1_doubled.Collinear(v1));
}

TEST_F(VectorTest, NonCollinearVectors) {
    EXPECT_FALSE(v1.Collinear(v2));
    EXPECT_FALSE(v_unit_x.Collinear(v_unit_y));
}

TEST_F(VectorTest, CollinearWithZeroVector) {
    EXPECT_TRUE(v1.Collinear(v_zero));
    EXPECT_TRUE(v_zero.Collinear(v1));
    EXPECT_TRUE(v_zero.Collinear(v_zero));
}

TEST_F(VectorTest, CollinearOppositeDirection) {
    Vector<double> v1_opposite{-1.0, -2.0, -3.0};
    EXPECT_TRUE(v1.Collinear(v1_opposite));
}