#include <algorithm>
#include <gtest/gtest.h>
#include <gfx_lib/matrix4X4.h>
#include <gfx_lib/vector4.h>

typedef gfx::matrix_4X4<int> matrix_4X4I;
typedef gfx::vector4<int> vector4I;

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
