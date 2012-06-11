#include <gtest/gtest.h>
#include "v8/math/vector3.h"

using v8::math::vector3F;

TEST(vector3_tests, spherical_coordinates_conversions) {
    const vector3F result = v8::math::point_from_spherical_coordinates(
        11.5326f, 69.7056f, 33.6901f);

    EXPECT_NEAR(6.0f, result.x_, 1.0e-3f);
    EXPECT_NEAR(4.0f, result.y_, 1.0e-3f);
    EXPECT_NEAR(9.0f, result.z_, 1.0e-3f);

    const vector3F sph_coords(v8::math::point_to_spherical_coordinates(
        v8::math::vector3F(6.0f, 4.0f, 9.0f)));

    EXPECT_NEAR(11.5326f, sph_coords.x_, 1.0e-3f);
    EXPECT_NEAR(69.7056f, v8::math::to_degrees(sph_coords.y_), 
                1.0e-3f);
    EXPECT_NEAR(33.6901f, v8::math::to_degrees(sph_coords.z_),
                1.0e-3f);
}