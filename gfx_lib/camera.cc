#include "pch_hdr.h"
#include "camera.h"

gfx::camera::camera()
    :   view_pos_(vector4::null),
        view_side_(vector4::unit_x),
        view_up_(vector4::unit_y),
        view_dir_(vector4::unit_z),
        viewmatrix_cache_valid_(true),
        view_matrix_(matrix4X4::identity),
        projection_type_(ptype_perspective) 
{
}

void gfx::camera::update_view_matrix() const {
    view_matrix_.a11_ = view_side_.x_;
    view_matrix_.a12_ = view_side_.y_;
    view_matrix_.a13_ = view_side_.z_;
    view_matrix_.a14_ = -gfx::dot_product(view_side_, view_pos_);

    view_matrix_.a21_ = view_up_.x_;
    view_matrix_.a22_ = view_up_.y_;
    view_matrix_.a23_ = view_up_.z_;
    view_matrix_.a24_ = -gfx::dot_product(view_up_, view_pos_);

    view_matrix_.a31_ = view_dir_.x_;
    view_matrix_.a32_ = view_dir_.y_;
    view_matrix_.a33_ = view_dir_.z_;
    view_matrix_.a34_ = -gfx::dot_product(view_dir_, view_pos_);

    view_matrix_.set_row(4, gfx::vector4(0.0f, 0.0f, 0.0f, 1.0f));
    viewmatrix_cache_valid_ = true;
}

gfx::camera&
gfx::camera::set_view_frame( 
    const gfx::vector4& origin, 
    const gfx::vector4& dir_vector, 
    const gfx::vector4& up_vector, 
    const gfx::vector4& right_vector 
    )
{
    view_pos_ = origin;
    view_dir_ = dir_vector;
    view_up_ = up_vector;
    view_side_ = right_vector;
    viewmatrix_cache_valid_ = false;
    return *this;
}

gfx::camera& 
gfx::camera::set_axes( 
    const gfx::vector4& dir_vector, 
    const gfx::vector4& up_vector, 
    const gfx::vector4& right_vector
    )
{
    view_side_ = right_vector;
    view_up_ = up_vector;
    view_dir_ = dir_vector;
    viewmatrix_cache_valid_ = false;
    return *this;
}

void
gfx::camera::set_projection_matrix( 
    const gfx::matrix4X4& proj, 
    projection_type ptype 
    )
{
    projection_matrix_ = proj;
    projection_type_ = ptype;
}