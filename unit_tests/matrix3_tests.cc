#include <gtest/gtest.h>
#include "v8/math/math_constants.h"
#include "v8/math/matrix3X3.h"
#include "v8/math/vector3.h"

using namespace v8::math;

TEST(matrix3tests, constructors) {
    matrix_3X3F testMtx(
        1.0f, 2.0f, 3.0f, 
        4.0f, 5.0f, 6.0f, 
        7.0f, 8.0f, 9.0f);

    EXPECT_NEAR(1.0f, testMtx.a11_, constants::kEpsilon);
    EXPECT_NEAR(2.0f, testMtx.a12_, constants::kEpsilon);
    EXPECT_NEAR(3.0f, testMtx.a13_, constants::kEpsilon);

    EXPECT_NEAR(4.0f, testMtx.a21_, constants::kEpsilon);
    EXPECT_NEAR(5.0f, testMtx.a22_, constants::kEpsilon);
    EXPECT_NEAR(6.0f, testMtx.a23_, constants::kEpsilon);

    EXPECT_NEAR(7.0f, testMtx.a31_, constants::kEpsilon);
    EXPECT_NEAR(8.0f, testMtx.a32_, constants::kEpsilon);
    EXPECT_NEAR(9.0f, testMtx.a33_, constants::kEpsilon);

    const float kInitValues[] = { 
        1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0, 8.0f, 9.0f 
    };

    matrix_3X3F mtx2(kInitValues, _countof(kInitValues));
    for (size_t i = 0; i < _countof(kInitValues); ++i) {
        EXPECT_NEAR(kInitValues[i], mtx2.elements_[i], constants::kEpsilon);
    }

    matrix_3X3F diagMtx(1.0f, 2.0f, 3.0f);
    for (unsigned int i = 0; i < 3; ++i) {
        for (unsigned int j = 0; j < 3; ++j) {
            if (i == j) {
                EXPECT_NEAR(kInitValues[i], diagMtx(i + 1, i + 1), constants::kEpsilon);
            } else {
                EXPECT_NEAR(0.0f, diagMtx(i + 1, j + 1), constants::kEpsilon);
            }
        }
    }

    const vector3F kInitVectors[] = {
        vector3F(1.0f, 2.0f, 3.0f),
        vector3F(4.0f, 5.0f, 6.0f),
        vector3F(7.0f, 8.0f, 9.0f)
    };

    matrix_3X3F vecMtx(kInitVectors[0], kInitVectors[1], kInitVectors[2]);
    for (unsigned int i = 0; i < 3; ++i) {
        for (unsigned int j = 0; j < 3; ++j) {
            EXPECT_NEAR(kInitVectors[j].elements_[i], vecMtx(i + 1, j + 1), 
                        constants::kEpsilon);
        }
    }

    vecMtx = matrix_3X3F(kInitVectors[0], kInitVectors[1], kInitVectors[2], false);
    for (unsigned int i = 0; i < 3; ++i) {
        for (unsigned int j = 0; j < 3; ++j) {
            EXPECT_NEAR(kInitVectors[i].elements_[j], vecMtx(i + 1, j + 1), 
                        constants::kEpsilon);
        }
    }
}

TEST(matrix3tests, make_fn) {
    matrix_3X3F testMtx;
    testMtx.make_skew_symmetric(1.0f, 2.0f, 3.0f);
    const matrix_3X3F expectedSkewSym(0.0f, -3.0f, 1.0f, 
                                      3.0f, 0.0f, -2.0f,
                                      -1.0f, 2.0f, 0.0f);

    EXPECT_TRUE(testMtx == expectedSkewSym);

    const vector3F kScaleFactors(1.0f, 2.0f, 3.0f);
    testMtx.make_scale(kScaleFactors);
    for (unsigned int i = 0; i < 3; ++i) {
        for (unsigned int j = 0; j < 3; ++j) {
            if (i == j) {
                EXPECT_NEAR(kScaleFactors.elements_[i], 
                            testMtx(i + 1, j + 1), constants::kEpsilon);
            } else {
                EXPECT_NEAR(0.0f, testMtx(i + 1, j + 1), constants::kEpsilon);
            }
        }
    }
}