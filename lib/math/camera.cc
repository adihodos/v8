#include "pch_hdr.h"
#include "v8/math/camera.h"

v8::math::camera::camera()
    :   view_pos_(vector3F::zero),
        view_side_(vector3F::unit_x),
        view_up_(vector3F::unit_y),
        view_dir_(vector3F::unit_z),
        view_matrix_(matrix_4X4F::identity),
        projection_matrix_(matrix_4X4F::identity),
        projection_view_matrix_(matrix_4X4F::identity) {
}

void v8::math::camera::update_view_matrix() {
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
}

v8::math::camera&
v8::math::camera::set_view_frame( 
    const v8::math::vector3F& origin, 
    const v8::math::vector3F& dir_vector, 
    const v8::math::vector3F& up_vector, 
    const v8::math::vector3F& right_vector 
    ) {
    view_pos_ = origin;
    return set_axes(dir_vector, up_vector, right_vector);
}

v8::math::camera& 
v8::math::camera::set_axes( 
    const v8::math::vector3F& dir_vector, 
    const v8::math::vector3F& up_vector, 
    const v8::math::vector3F& right_vector
    ) {
    assert(is_zero(v8::math::dot_product(dir_vector, up_vector)));
    assert(is_zero(v8::math::dot_product(dir_vector, right_vector)));
    assert(is_zero(v8::math::dot_product(up_vector, right_vector)));
    
    view_side_ = right_vector;
    view_up_ = up_vector;
    view_dir_ = dir_vector;
    
    update_view_matrix();
    update_projection_view_transform();
    return *this;
}

v8::math::camera& v8::math::camera::look_at( 
    const math::vector3F& origin, 
    const math::vector3F& world_up, 
    const math::vector3F& target
    ) {
    const vector3F D = (target - origin).normalize();
    const vector3F R = cross_product(world_up, D).normalize();
    const vector3F U = cross_product(D, R);

    return set_view_frame(origin, D, U, R);
}

void v8::math::camera::set_projection_matrix(
    const v8::math::vector4F& first_col, 
    const v8::math::vector4F& second_col, 
    const v8::math::vector4F& third_col, 
    const v8::math::vector4F& fourth_col
    ) {
    projection_matrix_.set_column(1, first_col);
    projection_matrix_.set_column(2, second_col);
    projection_matrix_.set_column(3, third_col);
    projection_matrix_.set_column(4, fourth_col);
    
    update_projection_view_transform();
}

void v8::math::camera::handle_frustrum_param_change() {
    const float dmin = frustrum_params_[Frustrum_DMin];
    const float dmax = frustrum_params_[Frustrum_DMax];
    const float umin = frustrum_params_[Frustrum_UMin];
    const float umax = frustrum_params_[Frustrum_UMax];
    const float rmin = frustrum_params_[Frustrum_RMin];
    const float rmax = frustrum_params_[Frustrum_RMax];
    
    if (projection_type_ == Projection_Perspective) {
        const float inv_rdiff = 1.0f / (rmax - rmin);
        const float inv_udiff = 1.0f / (umax - umin);
        const float inv_ddiff = 1.0f / (dmax - dmin);

        projection_matrix_(1, 1) = 2 * dmin * inv_rdiff;
        projection_matrix_(2, 1) = 0.0f;
        projection_matrix_(3, 1) = 0.0f;
        projection_matrix_(4, 1) = 0.0f;

        projection_matrix_(1, 2) = 0.0f;
        projection_matrix_(2, 2) = 2 * dmin * inv_udiff;
        projection_matrix_(3, 2) = 0.0f;
        projection_matrix_(4, 2) = 0.0f;

        projection_matrix_(1, 3) = -(rmax + rmin) * inv_rdiff;
        projection_matrix_(2, 3) = -(umax + umin) * inv_udiff;
        projection_matrix_(3, 3) = dmax * inv_ddiff;
        projection_matrix_(4, 3) = 1.0f;

        projection_matrix_(1, 4) = 0.0f;
        projection_matrix_(2, 4) = 0.0f;
        projection_matrix_(3, 4) = -(dmin * dmax) * inv_ddiff;
        projection_matrix_(4, 4) = 0.0f;
    } else {
        const float inv_width = 1.0f / (rmax - rmin);
        const float inv_height = 1.0f / (umax - umin);
        const float inv_depth = 1.0f / (dmax - dmin);

        projection_matrix_(1, 1) = 2 * inv_width;
        projection_matrix_(2, 1) = 0.0f;
        projection_matrix_(3, 1) = 0.0f;
        projection_matrix_(4, 1) = 0.0f;

        projection_matrix_(1, 2) = 0.0f;
        projection_matrix_(2, 2) = 2 * inv_height;
        projection_matrix_(3, 2) = 0.0f;
        projection_matrix_(4, 2) = 0.0f;

        projection_matrix_(1, 3) = 0.0f;
        projection_matrix_(2, 3) = 0.0f;
        projection_matrix_(3, 3) = inv_depth;
        projection_matrix_(3, 4) = 0.0f;

        projection_matrix_(1, 4) = -(rmax + rmin) * inv_width;
        projection_matrix_(2, 4) = -(umax + umin) * inv_height;
        projection_matrix_(3, 4) = -dmin * inv_depth;
        projection_matrix_(4, 4) = 1.0f;
    }
}

void v8::math::camera::set_frustrum(
    float dmin, 
    float dmax, 
    float umin, 
    float umax, 
    float rmin, 
    float rmax,
    int projection_type
    ) {
    assert((projection_type >= Projection_Perspective) 
           && (projection_type < Projection_Last));

    projection_type_ = projection_type;
    frustrum_params_[Frustrum_DMin] = dmin;
    frustrum_params_[Frustrum_DMax] = dmax;
    frustrum_params_[Frustrum_UMin] = umin;
    frustrum_params_[Frustrum_UMax] = umax;
    frustrum_params_[Frustrum_RMin] = rmin;
    frustrum_params_[Frustrum_RMax] = rmax;
    handle_frustrum_param_change();
}

void v8::math::camera::set_symmetric_frustrum(
    float fov_angle, float aspect_ratio, float dmin, float dmax
    ) {
    const float tan_half_angle = tan(to_radians(fov_angle / 2));
    frustrum_params_[Frustrum_DMin] = dmin;
    frustrum_params_[Frustrum_DMax] = dmax;
    frustrum_params_[Frustrum_UMax] = dmin * tan_half_angle;
    frustrum_params_[Frustrum_UMin] = -frustrum_params_[Frustrum_UMax];
    frustrum_params_[Frustrum_RMax] = 
        aspect_ratio * frustrum_params_[Frustrum_UMax];
    frustrum_params_[Frustrum_RMin] = -frustrum_params_[Frustrum_RMax];

    projection_type_ = Projection_Perspective;
    handle_frustrum_param_change();
}