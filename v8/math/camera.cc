#include "pch_hdr.h"
#include "v8/math/camera.h"

v8::math::camera::camera()
    :   view_pos_(vector4F::zero),
        view_side_(vector4F::unit_x),
        view_up_(vector4F::unit_y),
        view_dir_(vector4F::unit_z),
        viewmatrix_cache_valid_(true),
        view_matrix_(matrix_4X4F::identity),
        projection_type_(ptype_perspective) 
{
}

void v8::math::camera::update_view_matrix() const {
    view_matrix_.a11_ = view_side_.x_;
    view_matrix_.a12_ = view_side_.y_;
    view_matrix_.a13_ = view_side_.z_;
    view_matrix_.a14_ = -v8::math::dot_product(view_side_, view_pos_);

    view_matrix_.a21_ = view_up_.x_;
    view_matrix_.a22_ = view_up_.y_;
    view_matrix_.a23_ = view_up_.z_;
    view_matrix_.a24_ = -v8::math::dot_product(view_up_, view_pos_);

    view_matrix_.a31_ = view_dir_.x_;
    view_matrix_.a32_ = view_dir_.y_;
    view_matrix_.a33_ = view_dir_.z_;
    view_matrix_.a34_ = -v8::math::dot_product(view_dir_, view_pos_);

    view_matrix_.set_row(4, v8::math::vector4F(0.0f, 0.0f, 0.0f, 1.0f));
    viewmatrix_cache_valid_ = true;
}

v8::math::camera&
v8::math::camera::set_view_frame( 
    const v8::math::vector4F& origin, 
    const v8::math::vector4F& dir_vector, 
    const v8::math::vector4F& up_vector, 
    const v8::math::vector4F& right_vector 
    )
{
    view_pos_ = origin;
    view_dir_ = dir_vector;
    view_up_ = up_vector;
    view_side_ = right_vector;
    viewmatrix_cache_valid_ = false;
    return *this;
}

v8::math::camera& 
v8::math::camera::set_axes( 
    const v8::math::vector4F& dir_vector, 
    const v8::math::vector4F& up_vector, 
    const v8::math::vector4F& right_vector
    )
{
    view_side_ = right_vector;
    view_up_ = up_vector;
    view_dir_ = dir_vector;
    viewmatrix_cache_valid_ = false;
    return *this;
}

void
v8::math::camera::set_projection_matrix( 
    const v8::math::matrix_4X4F& proj, 
    projection_type ptype 
    )
{
    projection_matrix_ = proj;
    projection_type_ = ptype;
}

v8::math::camera& v8::math::camera::look_at( 
    const math::vector3F& origin, 
    const math::vector3F& world_up, 
    const math::vector3F& target
    )
{
    const vector3F D = (target - origin).normalize();
    const vector3F R = cross_product(world_up, D).normalize();
    const vector3F U = cross_product(D, R);

    return set_view_frame(
        vector4F::as_affine_vector(origin),
        vector4F::as_affine_vector(D),
        vector4F::as_affine_vector(U),
        vector4F::as_affine_vector(R)
        );
}