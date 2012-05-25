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
    if (!viewmatrix_cache_valid_)
        update_view_matrix();
    return view_matrix_;
}
