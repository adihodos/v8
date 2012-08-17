inline void v8::math::camera::update_projection_view_transform() {
    projection_view_matrix_ = projection_matrix_ * view_matrix_;
}

inline v8::math::camera& v8::math::camera::set_origin(
    const math::vector3F& origin
    ) {
    view_pos_ = origin;

    update_view_matrix();
    update_projection_view_transform();

    return *this;
}

inline const v8::math::vector3F& v8::math::camera::get_origin() const {
    return view_pos_;
}

inline const v8::math::vector3F& v8::math::camera::get_direction_vector() const {
    return view_dir_;
}

inline const v8::math::vector3F& v8::math::camera::get_up_vector() const {
    return view_up_;
}

inline const v8::math::vector3F& v8::math::camera::get_right_vector() const {
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
    const v8::math::matrix_4X4F& mtx
    ) {
    projection_matrix_ = mtx;
    update_projection_view_transform();
}

inline int v8::math::camera::get_projection_type() const {
    return projection_type_;
}

inline 
const v8::math::matrix_4X4F&
v8::math::camera::get_projection_wiew_transform() const {
    return projection_view_matrix_;
}

inline const float* v8::math::camera::get_frustrum() const {
    return frustrum_params_;
}

inline float v8::math::camera::get_dmin() const {
    return frustrum_params_[Frustrum_DMin];
}

inline float v8::math::camera::get_dmax() const {
    return frustrum_params_[Frustrum_DMax];
}

inline float v8::math::camera::get_umin() const {
    return frustrum_params_[Frustrum_UMin];
}

inline float v8::math::camera::get_umax() const {
    return frustrum_params_[Frustrum_UMax];
}

inline float v8::math::camera::get_rmin() const {
    return frustrum_params_[Frustrum_RMin];
}

inline float v8::math::camera::get_rmax() const {
    return frustrum_params_[Frustrum_RMax];
}
