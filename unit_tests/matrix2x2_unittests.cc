#include <stdint.h>
#include <gtest/gtest.h>
#include "v8/base/count_of.h"
#include "v8/math/matrix2X2.h"
#include "v8/math/math_utils.h"
#include "v8/math/vector2.h"

using namespace v8::math;
const float C_Epsilon = 1.0e-3f;

TEST(matrix_2X2_tests, constructors) {
    matrix_2X2F m1(1.0f, 2.0f, 3.0f, 4.0f);

    EXPECT_NEAR(1.0f, m1.a11_, C_Epsilon);
    EXPECT_NEAR(2.0f, m1.a12_, C_Epsilon);
    EXPECT_NEAR(3.0f, m1.a21_, C_Epsilon);
    EXPECT_NEAR(4.0f, m1.a22_, C_Epsilon);

    matrix_2X2F diag(1.0f, 2.0f);
    EXPECT_NEAR(1.0f, diag.a11_, C_Epsilon);
    EXPECT_NEAR(2.0f, diag.a22_, C_Epsilon);
    EXPECT_TRUE(operands_eq(diag.a12_, diag.a21_));

    const vector2F v1(1.0f, 2.0f);
    const vector2F v2(3.0f, 4.0f);

    matrix_2X2F m3(v1, v2, true);
    EXPECT_NEAR(1.0f, m3.a11_, C_Epsilon);
    EXPECT_NEAR(2.0f, m3.a21_, C_Epsilon);
    EXPECT_NEAR(3.0f, m3.a12_, C_Epsilon);
    EXPECT_NEAR(4.0f, m3.a22_, C_Epsilon);

    matrix_2X2F m4(v1, v2, false);
    EXPECT_NEAR(1.0f, m4.a11_, C_Epsilon);
    EXPECT_NEAR(2.0f, m4.a12_, C_Epsilon);
    EXPECT_NEAR(3.0f, m4.a21_, C_Epsilon);
    EXPECT_NEAR(4.0f, m4.a22_, C_Epsilon);

    const float input_values[] = { 4.0f, 15.0f, 21.0f, 30.0f };
    matrix_2X2F m5(input_values, v8::base::count_of_array(input_values));
    for (uint32_t i = 0; i < v8::base::count_of_array(input_values); ++i) {
        EXPECT_NEAR(input_values[i], m5.elements_[i], C_Epsilon);
    }

    matrix_2X2<int> int_mtx(1, 2, 3, 4);
    matrix_2X2F m6(int_mtx);
    EXPECT_NEAR(1.0f, m6.a11_, C_Epsilon);
    EXPECT_NEAR(2.0f, m6.a12_, C_Epsilon);
    EXPECT_NEAR(3.0f, m6.a21_, C_Epsilon);
    EXPECT_NEAR(4.0f, m6.a22_, C_Epsilon);
}

TEST(matrix_2X2_tests, make_functions) {
    //
    // Tensor product
    const vector2F u(5.0f, 9.0f);
    const vector2F v(3.0f, 2.0f);
    matrix_2X2F m1;
    m1.make_tensor_product(u, v);

    EXPECT_NEAR(15.0f, m1.a11_, C_Epsilon);
    EXPECT_NEAR(10.0f, m1.a12_, C_Epsilon);
    EXPECT_NEAR(27.0f, m1.a21_, C_Epsilon);
    EXPECT_NEAR(18.0f, m1.a22_, C_Epsilon);

    m1.make_skew_symmetric(10.0f);
    EXPECT_NEAR(0.0f, m1.a11_, C_Epsilon);
    EXPECT_NEAR(10.0f, m1.a12_, C_Epsilon);
    EXPECT_NEAR(-10.0f, m1.a21_, C_Epsilon);
    EXPECT_NEAR(0.0f, m1.a22_, C_Epsilon);

    m1.make_symmetric(5.0f);
    EXPECT_NEAR(0.0f, m1.a11_, C_Epsilon);
    EXPECT_NEAR(5.0f, m1.a12_, C_Epsilon);
    EXPECT_NEAR(5.0f, m1.a21_, C_Epsilon);
    EXPECT_NEAR(0.0f, m1.a22_, C_Epsilon);

    m1.make_rotation(to_radians(45.0f));
    EXPECT_NEAR(0.707f, m1.a11_, C_Epsilon);
    EXPECT_NEAR(-0.707f, m1.a12_, C_Epsilon);
    EXPECT_NEAR(0.707f, m1.a21_, C_Epsilon);
    EXPECT_NEAR(0.707f, m1.a22_, C_Epsilon);

    vector2F scale(10.0f, 20.0f);
    m1.make_scale(scale);
    EXPECT_NEAR(10.0f, m1.a11_, C_Epsilon);
    EXPECT_NEAR(0.0f, m1.a12_, C_Epsilon);
    EXPECT_NEAR(0.0f, m1.a21_, C_Epsilon);
    EXPECT_NEAR(20.0f, m1.a22_, C_Epsilon);
}

TEST(matrix_2X2_tests, det_transpose_invert_trace) {
    matrix_2X2F m(2.0f, 4.0f, 3.0f, 9.0f);

    EXPECT_NEAR(6.0f, m.determinant(), C_Epsilon);
    EXPECT_TRUE(m.is_invertible());
    EXPECT_NEAR(11.0f, m.trace(), C_Epsilon);

    m.transpose();
    EXPECT_NEAR(2.0f, m.a11_, C_Epsilon);
    EXPECT_NEAR(3.0f, m.a12_, C_Epsilon);
    EXPECT_NEAR(4.0f, m.a21_, C_Epsilon);
    EXPECT_NEAR(9.0f, m.a22_, C_Epsilon);

    m.invert();
    EXPECT_NEAR(1.5f, m.a11_, C_Epsilon);
    EXPECT_NEAR(-0.5f, m.a12_, C_Epsilon);
    EXPECT_NEAR(-0.666f, m.a21_, C_Epsilon);
    EXPECT_NEAR(0.333f, m.a22_, C_Epsilon);
}