#include <algorithm>
#include <cstdint>
#include <gtest/gtest.h>

#include "v8/base/count_of.h"
#include "v8/math/math_utils.h"
#include "v8/math/quaternion.h"
#include "v8/math/vector3.h"

using namespace v8::base;
using namespace v8::math;

const float Epsilon_Value = 1.0e-4f;

TEST(quaternion_tests, constructors) {
    float init_vals[] = { 1.0f, 2.0f, 3.0f, 4.0f };
    quaternionF q0(init_vals[0], init_vals[1], init_vals[2], init_vals[3]);
    quaternionF q1(init_vals);
    
    for (uint32_t i = 0; i < count_of_array(init_vals); ++i) {
        EXPECT_NEAR(q0.elements_[i], init_vals[i], Epsilon_Value);
        EXPECT_NEAR(q1.elements_[i], init_vals[i], Epsilon_Value);
    }

    quaternionF q2(to_radians(30.0f), vector3F(2.0f, 5.0f, 4.0f));
    EXPECT_NEAR(0.965925f, q2.w_, Epsilon_Value);
    EXPECT_NEAR(0.077164f, q2.x_, Epsilon_Value);
    EXPECT_NEAR(0.192912f, q2.y_, Epsilon_Value);
    EXPECT_NEAR(0.154329f, q2.z_, Epsilon_Value);
}