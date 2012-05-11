inline v8::math::camera::projection_type v8::math::camera::get_projection_type() const {
    return projection_type_;
}

inline const v8::math::vector4F& v8::math::camera::get_origin() const {
    return view_pos_;
}

inline const v8::math::vector4F& v8::math::camera::get_direction_vector() const {
    return view_dir_;
}

inline const v8::math::vector4F& v8::math::camera::get_up_vector() const {
    return view_up_;
}

inline const v8::math::vector4F& v8::math::camera::get_right_vector() const {
    return view_side_;
}

inline const v8::math::matrix_4X4F& v8::math::camera::get_view_transform() const {
    return view_matrix_;
}

inline const v8::math::matrix_4X4F& v8::math::camera::get_projection_transform() const {
    return projection_matrix_;
}