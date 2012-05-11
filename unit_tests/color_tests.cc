#include <gtest/gtest.h>
#include "v8/math/gfx_math.h"
#include "v8/math/color.h"

TEST(gfx_color_tests, conversion_from_rgba) {
    v8::math::color color(v8::math::color::from_u32_rgba(0xFF2D57FFU));

    EXPECT_NEAR(1.0f, color.r_, 1.0e-3);
    EXPECT_NEAR(0.176f, color.g_, 1.0e-3);
    EXPECT_NEAR(0.341f, color.b_, 1.0e-3);
    EXPECT_NEAR(1.0f, color.a_, 1.0e-3);
}

TEST(gfx_color_tests, conversion_from_bgra) {
    v8::math::color color(v8::math::color::from_u32_bgra(0x572DFFFF));

    EXPECT_NEAR(1.0f, color.r_, 1.0e-3);
    EXPECT_NEAR(0.176f, color.g_, 1.0e-3);
    EXPECT_NEAR(0.341f, color.b_, 1.0e-3);
    EXPECT_NEAR(1.0f, color.a_, 1.0e-3);
}

TEST(gfx_color_tests, conversion_from_argb) {
    v8::math::color color(v8::math::color::from_u32_argb(0xFFFF2D57));

    EXPECT_NEAR(1.0f, color.r_, 1.0e-3);
    EXPECT_NEAR(0.176f, color.g_, 1.0e-3);
    EXPECT_NEAR(0.341f, color.b_, 1.0e-3);
    EXPECT_NEAR(1.0f, color.a_, 1.0e-3);
}

TEST(gfx_color_tests, conversion_to_uint32_rgba) {
    v8::math::color color(1.0f, 0.5f, 0.25f, 1.0f);

    EXPECT_EQ(0xFF8040FF, color.to_uint32_rgba());
}