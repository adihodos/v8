inline gfx::camera::projection_type gfx::camera::get_projection_type() const {
    return projection_type_;
}

inline const gfx::vector4& gfx::camera::get_origin() const {
    return view_pos_;
}

inline const gfx::vector4& gfx::camera::get_direction_vector() const {
    return view_dir_;
}

inline const gfx::vector4& gfx::camera::get_up_vector() const {
    return view_up_;
}

inline const gfx::vector4& gfx::camera::get_right_vector() const {
    return view_side_;
}

inline const gfx::matrix4X4& gfx::camera::get_view_transform() const {
    return view_matrix_;
}

inline const gfx::matrix4X4& gfx::camera::get_projection_transform() const {
    return projection_matrix_;
}