template<typename real_t>
inline v8::math::matrix_2X3<real_t>::matrix_2X3(
    real_t a11, real_t a12, real_t a13,
    real_t a21, real_t a22, real_t a23
    ) {
    a11_ = a11; a12_ = a12; a13_ = a13;
    a21_ = a21; a22_ = a22; a23_ = a23;
}

template<typename real_t>
template<typename real_u>
inline v8::math::matrix_2X3<real_t>::matrix_2X3(
    const v8::math::matrix_2X3<real_u>& other
    ) {
    a11_ = other.a11_; a12_ = other.a12_; a13_ = other.a13_;
    a21_ = other.a21_; a22_ = other.a22_; a23_ = other.a23_;
}

template<typename real_t>
inline v8::math::matrix_2X3<real_t>::matrix_2X3(
    const real_t* data,
    uint32_t count
    ) {
    memcpy(elements_, data, min(count_of_array(elements_), count));
}

template<typename real_t>
template<typename real_u>
inline v8::math::matrix_2X3<real_t>& 
v8::math::matrix_2X3<real_t>::operator=(
    const v8::math::matrix_2X3<real_u>& other) {
    a11_ = other.a11_; a12_ = other.a12_; a13_ = other.a13_;
    a21_ = other.a21_; a22_ = other.a22_; a23_ = other.a23_;
    return *this;
}

template<typename real_t>
template<typename real_u>
inline v8::math::matrix_2X3<real_t>& 
v8::math::matrix_2X3<real_t>::operator+=(
    const v8::math::matrix_2X3<real_u>& other
    ) {
    for (uint32_t i = 0; i < count_of_array(elements_); ++i)
        elements_[i] += other.elements_[i];
    return *this;
}

template<typename real_t>
template<typename real_u>
inline v8::math::matrix_2X3<real_t>& 
v8::math::matrix_2X3<real_t>::operator-=(
    const v8::math::matrix_2X3<real_u>& other
    ) {
    for (uint32_t i = 0; i < count_of_array(elements_); ++i)
        elements_[i] -= other.elements_[i];
    return *this;
}

template<typename real_t>
template<typename real_u>
inline v8::math::matrix_2X3<real_t>& 
v8::math::matrix_2X3<real_t>::operator*=(
    real_u conv_scalar
    ) {
    for (uint32_t i = 0; i < count_of_array(elements_); ++i)
        elements_[i] *= conv_scalar;
    return *this;
}

template<typename real_t>
inline v8::math::matrix_2X3<real_t>&
v8::math::matrix_2X3<real_t>::make_zero() {
    memcpy(elements_, matrix_2X3<real_t>::zero.elements_, 
           count_of_array(elements_));
    return *this;
}

template<typename real_t>
inline v8::math::matrix_2X3<real_t>&
v8::math::matrix_2X3<real_t>::make_identity() {
    memcpy(elements_, matrix_2X3<real_t>::identity.elements_,
           count_of_array(elements_));
    return *this;
}

template<typename real_t>
inline v8::math::matrix_2X3<real_t>&
v8::math::matrix_2X3<real_t>::make_scale(real_t sx, real_t sy) {
    a11_ = sx; a12_ = a13_ = a21_ = a23_ = real_t(0);
    a22_ = sy;
    return *this;
}

template<typename real_t>
inline v8::math::matrix_2X3<real_t>&
v8::math::matrix_2X3<real_t>::make_translate(real_t tx, real_t ty) {
    a11_ = a12_ = a21_ = a22_ = real_t(0);
    a13_ = tx; a23_ = ty;
    return *this;
}

template<typename real_t>
inline v8::math::matrix_2X3<real_t>&
v8::math::matrix_2X3<real_t>::make_rotate(float theta) {
    const real_t sin_theta = sin(theta);
    const real_t cos_theta = cos(theta);

    a11_ = cos_theta; a12_ = -sin_theta; a13_ = real_t(0);
    a21_ = sin_theta; a22_ = cos_theta; a23_ = real_t(0);
    return *this;
}

template<typename real_t>
inline v8::math::matrix_2X3<real_t>&
v8::math::matrix_2X3<real_t>::make_reflect(
    const v8::math::vector2<real_t>& dir
    ) {
    const vector2<real_t> N(vector2<real_t>(-dir.y_, dir.x_).normalize());

    a11_ = real_t(1) - 2 * N.x_ * N.x_;
    a12_ = a21_ = -2 * N.x_ * N.y_;
    a22_ = real_t(1) - 2 * N.y_ * N.y_;
    a13_ = a23_ = real_t(0);
    return *this;
}

template<typename real_t>
inline v8::math::matrix_2X3<real_t>&
v8::math::matrix_2X3<real_t>::make_reflect(
    real_t theta
    ) {
    const real_t sin2theta = sin(2 * theta);
    const real_t cos2theta = cos(2 * theta);

    a11_ = cos2theta; a12_ = sin2theta; a13_ = real_t(0);
    a21_ = sin2theta; a22_ = -cos2theta; a23_ = real_t(0);
    return *this;
}

template<typename real_t>
inline v8::math::matrix_2X3<real_t>&
v8::math::matrix_2X3<real_t>::make_reflect_slope(
    real_t slope
    ) {
    const real_t ssq = slope * slope;
    const real_t mul_factor = real_t(1) / (ssq + 1);

    a11_ = (real_t(1) - ssq) * mul_factor;
    a12_ = 2 * ssq * mul_factor;
    a13_ = real_t(0);

    a21_ = a12_;
    a22_ = (ssq - real_t(1)) * mul_factor;
    a32_ = real_t(0);

    return *this;
}

template<typename real_t>
template<typename real_u>
inline v8::math::matrix_2X3<real_t>&
v8::math::matrix_2X3<real_t>::transform_vector(
    v8::math::vector2<real_u>* vec
    ) {
    vec->x_ = a11_ * vec->x_ + a12_ * vec->y_;
    vec->y_ = a21_ * vec->x_ + a22_ * vec->y_;
    return *this;
}

template<typename real_t>
template<typename real_u>
inline v8::math::matrix_2X3<real_t>&
v8::math::matrix_2X3<real_t>::transform_point(
    v8::math::vector2<real_u>* pt
    ) {
    pt->x_ = a11_ * pt->x_ + a12_ * pt->y_ + a13_;
    pt->y_ = a21_ * pt->x_ + a22_ * pt->y_ + a23_;
    return *this;
}
