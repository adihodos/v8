#include <algorithm>
#include <gtest/gtest.h>
#include "v8/math/matrix3X3.h"
#include "v8/math/matrix4X4.h"
#include "v8/math/vector4.h"

typedef v8::math::matrix_3X3<int> matrix_3X3I;
typedef v8::math::matrix_4X4<int> matrix_4X4I;
typedef v8::math::vector4<int> vector4I;

template<typename OutputIter, typename real_t>
void
gen_arithmetic_progression(
    OutputIter out, int max_count, real_t start_seq, real_t step
        )
{
    int i = 0;
    while (i < max_count) {
        *out++ = start_seq + i * step;
        ++i;
    }
}

TEST(matrix4tests, initialize_array) {
    int initValues[16];
    int startValue = 0;
    std::generate_n(std::begin(initValues), 16,
                    [&startValue]() { return startValue++; });
    matrix_4X4I m1(initValues, 16);

    for (int i = 0; i < 16; ++i)
        EXPECT_EQ(initValues[i], m1.elements_[i]);
}

TEST(matrix4tests, initialize_vectors) {
    vector4I v1(1, 2, 3, 4);
    matrix_4X4I m1(v1, v1, v1, v1);
    matrix_4X4I m2(v1, v1, v1, v1, false);

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            EXPECT_EQ(v1.elements_[i], m1(i + 1, j + 1));
            EXPECT_EQ(v1.elements_[j], m2(i + 1, j + 1));
        }
    }
}

TEST(matrix4tests, initialize_diagonal) {
    matrix_4X4I m_diag(1, 2, 3, 4);
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (i == j) {
                EXPECT_EQ(i + 1, m_diag(i + 1, j + 1));
            } else {
                EXPECT_EQ(0, m_diag(i + 1, j + 1));
            }
        }
    }
}

TEST(matrix4tests, self_add_assign) {
    int initVal[16];
    gen_arithmetic_progression(initVal, 16, 1, 1);
    matrix_4X4I m1(initVal, 16);
    matrix_4X4I m2(matrix_4X4I::null);
    m2 += m1;
    matrix_4X4I m3 = m1 + m2;

    for (int i = 0; i < 16; ++i) {
        EXPECT_EQ(initVal[i], m2.elements_[i]);
        EXPECT_EQ(initVal[i] * 2, m3.elements_[i]);
    }
}

TEST(matrix4tests, multiplication) {
    matrix_4X4I firstMtx(2, 1, 3, 3,
                         3, -1, 4, 2,
                         4, 1, -1, 5,
                         1, 1, 1, 4);
    matrix_4X4I secondMtx(1, 1, -1, 2,
                          3, 1, 1, 1,
                          1, -1, 3, -3,
                          4, 2, 0, 3);

    const matrix_4X4I multMatrix(20, 6, 8, 5,
                                 12, 2, 8, -1,
                                 26, 16, -6, 27,
                                 21, 9, 3, 12);
    EXPECT_EQ(multMatrix, firstMtx * secondMtx);
}

TEST(matrix4tests, scalar_mul_divide) {
    int initVal[16];
    gen_arithmetic_progression(initVal, 16, 1, 0);
    matrix_4X4I m1(initVal, 16);

    m1 *= 4;

    for (int i = 0; i < 16; ++i)
        EXPECT_EQ(4, m1.elements_[i]);

    m1 /= 4;

    for (int i = 0; i < 16; ++i)
        EXPECT_EQ(1, m1.elements_[i]);
}

TEST(matrix4tests, transpose) {
    matrix_4X4I mtx(0, 1, 2, 3,
                    -1, 0, 3, 4,
                    -2, -3, 0, 5,
                    -3, -4, -5, 0);
    mtx.transpose();

    const matrix_4X4I transposed(0, -1, -2, -3,
                                 1, 0, -3, -4,
                                 2, 3, 0, -5,
                                 3, 4, 5, 0);
    EXPECT_TRUE((mtx == transposed));
}

TEST(matrix4tests, determinant) {
    matrix_4X4I m1(1, 2, 3, 4,
                   2, 3, 4, 1,
                   3, 4, 1, 2,
                   4, 1, 2, 3);

    EXPECT_EQ(160, m1.determinant());
    EXPECT_TRUE(m1.is_invertible());
}

TEST(matrix4tests, adjoint) {
    matrix_4X4I mtx(10, 9, 2, 15,
                    8, 6, 9, 4, 
                    1, 0, 3, 5,
                    2, 11, 18, 4);

    matrix_4X4I adjointMtx;
    mtx.get_adjoint(&adjointMtx);

    const matrix_4X4I expectedResult(-105, 1087, -149, -507,
                                     522, -588, -1482, 483,
                                     -360, 331, 724, 114,
                                     237, -416, 892, 33);
    EXPECT_TRUE(expectedResult == adjointMtx);
}

TEST(matrix4tests, set_get_upper3x3) {
    const int kValues[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    matrix_4X4I srcMatrix;
    srcMatrix.set_upper3x3(kValues);

    int outValues[_countof(kValues)];
    srcMatrix.get_upper3x3(outValues);
    for (int i = 0; i < _countof(kValues); ++i) {
        EXPECT_EQ(kValues[i], outValues[i]);
    }
}

TEST(matrix4tests, set_get_row) {
    const vector4I kValues[] = {
        vector4I(1, 2, 3, 4),
        vector4I(5, 6, 7, 8),
        vector4I(9, 10, 11, 12),
        vector4I(13, 14, 15, 16)
    };

    matrix_4X4I mtx;
    for (int i = 0; i < _countof(kValues); ++i) {
        mtx.set_row(i + 1, kValues[i]);
    }

    for (int i = 0; i < _countof(kValues); ++i) {
        vector4I outData;
        mtx.get_row(i + 1, &outData);
        EXPECT_EQ(kValues[i], outData);
    }
}

TEST(matrix4tests, set_get_column) {
    const vector4I kValues[] = {
        vector4I(1, 2, 3, 4),
        vector4I(5, 6, 7, 8),
        vector4I(9, 10, 11, 12),
        vector4I(13, 14, 15, 16)
    };

    matrix_4X4I mtx;
    for (int i = 0; i < _countof(kValues); ++i) {
        mtx.set_column(i + 1, kValues[i]);
    }

    for (int i = 0; i < _countof(kValues); ++i) {
        vector4I outData;
        mtx.get_column(i + 1, &outData);
        EXPECT_EQ(kValues[i], outData);
    }
}