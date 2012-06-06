inline void v8::math::camera::update_projection_view_transform() {
    projection_view_matrix_ = projection_matrix_ * view_matrix_;
}

inline v8::math::camera& v8::math::camera::set_origin(
    const math::vector4F& origin
    ) {
    view_pos_ = origin;
    update_cam_data();
    return *this;
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

inline 
const v8::math::matrix_4X4F& 
v8::math::camera::get_projection_transform() const {
    return projection_matrix_;
}

inline 
void 
v8::math::camera::set_projection_matrix(
    const v8::math::matrix_4X4F& mtx, 
    v8::math::camera::projection_type type
    )
{
    projection_matrix_ = mtx;
    projection_type_ = type;
    update_cam_data();
}

inline int v8::math::camera::get_projection_type() const {
    return projection_type_;
}

inline 
v8::math::matrix_4X4F 
v8::math::camera::get_projection_wiew_transform() const {
    return projection_view_matrix_;
}