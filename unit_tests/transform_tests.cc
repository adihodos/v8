#include <gtest/gtest.h>
#include "v8/math/gfx_math.h"
#include "v8/math/matrix4X4.h"
#include "v8/math/transform.h"

TEST(transform_tests, default_constructor) {
    v8::math::transformF tfm;
    EXPECT_TRUE(tfm.is_identity());
    EXPECT_TRUE(tfm.is_rotation_or_reflection());
    EXPECT_FALSE(tfm.is_scaling());
    EXPECT_TRUE(tfm.get_transform_matrix() == v8::math::matrix_4X4F::identity);
}

TEST(transform_tests, translate_ctor) {
    const v8::math::vector3F translation(10.0f, 100.0f, -10.0f);
    v8::math::transformF tfm(translation);
    EXPECT_FALSE(tfm.is_identity());
    EXPECT_TRUE(tfm.is_rotation_or_reflection());
    EXPECT_FALSE(tfm.is_scaling());
    const v8::math::matrix_4X4F transform(
        v8::math::vector4F(1.0f, 0.0f, 0.0f, 0.0f),
        v8::math::vector4F(0.0f, 1.0f, 0.0f, 0.0f),
        v8::math::vector4F(0.0f, 0.0f, 1.0f, 0.0f),
        v8::math::vector4F::as_affine_point(translation));
    EXPECT_EQ(transform, tfm.get_transform_matrix());
}

TEST(transform_tests, scale_ctor) {
    const float scale_factor = 5.0f;
    v8::math::transformF tr(scale_factor);
    EXPECT_FALSE(tr.is_identity());
    EXPECT_TRUE(tr.is_rotation_or_reflection());
    EXPECT_TRUE(tr.is_scaling());
    v8::math::matrix_4X4F transform;
    transform.set_upper3x3(v8::math::matrix_3X3F(scale_factor, scale_factor, scale_factor));
    transform.set_column(4, 0.0f, 0.0f, 0.0f, 1.0f);
    transform.set_row(4, 0.0f, 0.0f, 0.0f, 1.0f);
    EXPECT_EQ(transform, tr.get_transform_matrix());
}

TEST(transform_tests, rotate_ctor) {
    v8::math::matrix_3X3F rot;
    rot.make_euler_xyz(10.0f, 10.0f, 10.0f);
    v8::math::transformF tr(rot, true);
    EXPECT_FALSE(tr.is_identity());
    EXPECT_TRUE(tr.is_rotation_or_reflection());
    EXPECT_FALSE(tr.is_scaling());
    v8::math::matrix_4X4F transform;
    transform.set_upper3x3(rot);
    transform.set_column(4, 0.0f, 0.0f, 0.0f, 1.0f);
    transform.set_row(4, 0.0f, 0.0f, 0.0f, 1.0f);
    EXPECT_EQ(transform, tr.get_transform_matrix());
}

TEST(transform_tests, all_ctor) {
    v8::math::matrix_3X3F rot;
    rot.make_euler_xyz(10.0f, 10.0f, 10.0f);
    const float scale_factor = 5.0f;
    const v8::math::vector3F translation(10.0f, 100.0f, -10.0f);
    v8::math::transformF tr(rot, true, translation, scale_factor);
    EXPECT_FALSE(tr.is_identity());
    EXPECT_TRUE(tr.is_rotation_or_reflection());
    EXPECT_TRUE(tr.is_scaling());
    v8::math::matrix_4X4F transform;
    transform.set_upper3x3(rot * scale_factor);
    transform.set_column(4, v8::math::vector4F::as_affine_point(translation));
    transform.set_row(4, 0.0f, 0.0f, 0.0f, 1.0f);
    EXPECT_EQ(transform, tr.get_transform_matrix());
}

TEST(transform_tests, compute_inverse) {
    v8::math::matrix_3X3F rot;
    rot.make_rotation_x(v8::math::to_radians(90.0f));
    const float scale_factor = 2.0f;
    const v8::math::vector3F translation(10.0f, 10.0f, 10.0f);
    v8::math::transformF tr(rot, true, translation, scale_factor);
    v8::math::matrix_4X4F expected_inv(0.5f, 0.0f, 0.0f, -5.0f,
                                  0.0f, 0.0f, 0.5f, -5.0f,
                                  0.0f, -0.5f, 0.0f, 5.0f,
                                  0.0f, 0.0f, 0.0f, 1.0f);
    v8::math::matrix_4X4F computed_inv;
    tr.compute_inverse(&computed_inv);
    EXPECT_EQ(expected_inv, computed_inv);
}

TEST(transform_tests, invert) {
    v8::math::matrix_3X3F rot;
    rot.make_rotation_x(v8::math::to_radians(90.0f));
    const float scale_factor = 2.0f;
    const v8::math::vector3F translation(10.0f, 10.0f, 10.0f);
    v8::math::transformF tr(rot, true, translation, scale_factor);

    tr.invert();
    EXPECT_TRUE(v8::math::operands_eq(0.5f, tr.get_scale_component()));
    EXPECT_EQ(rot.transpose(), tr.get_matrix_component());
    EXPECT_EQ(v8::math::vector3F(-5.0f, -5.0f, 5.0f), tr.get_translation_component());
}