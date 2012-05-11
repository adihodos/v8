template<typename real_t>
v8::math::matrix_2X2<real_t>::matrix_2X2(
    real_t a11, 
    real_t a12, 
    real_t a21, 
    real_t a22
    )
{
    a11_ = a11; a12_ = a22; a21_ = a21; a22_ = a22;
}

template<typename real_t>
v8::math::matrix_2X2<real_t>::matrix_2X2(real_t a11, real_t a22) {
    a11_ = a11; a12_ = a21_ = real_t(0); a22_ = a22;
}

template<typename real_t>
v8::math::matrix_2X2<real_t>::matrix_2X2( 
    const v8::math::vector2<real_t>& v1, 
    const v8::math::vector2<real_t>& v2, 
    bool column /* = true */ 
    )
{
    if (column) {
        a11_ = v1.x_;
        a21_ = v1.y_;
        a12_ = v2.x_;
        a22_ = v2.y_;
    } else {
        a11_ = v1.x_;
        a12_ = v1.y_;
        a21_ = v2.x_;
        a22_ = v2.y_;
    }
}

template<typename real_t>
v8::math::matrix_2X2<real_t>::matrix_2X2(const real_t* input, size_t count) {
    memcpy(elements_, input, min(_countof(elements_), count) * sizeof(real_t));
}

template<typename real_t>
v8::math::matrix_2X2<real_t>&
v8::math::matrix_2X2<real_t>::make_tensor_product( 
    const v8::math::vector2<real_t>& u, 
    const v8::math::vector2<real_t>& v 
    )
{
    a11_ = u.x_ * v.x_;
    a12_ = u.x_ * v.y_;
    a21_ = u.y_ * v.x_; 
    a22_ = u.y_ * v.y_;
    return *this;
}

template<typename real_t>
v8::math::matrix_2X2<real_t>&
v8::math::matrix_2X2<real_t>::make_rotation(float theta) {
    const real_t sin_theta = sin(theta);
    const real_t cos_theta = cos(theta);
    a11_ = cos_theta; a12_ = -sin_theta;
    a21_ = sin_theta; a22_ = cos_theta;
    return *this;
}

template<typename real_t>
v8::math::matrix_2X2<real_t>&
v8::math::matrix_2X2<real_t>::make_skew_symmetric(real_t val) {
    a11_ = real_t(0); a12_ = val;
    a21_ = -val; a22_ = real_t(0);
    return *this;
}

template<typename real_t>
v8::math::matrix_2X2<real_t>& v8::math::matrix_2X2<real_t>::operator +=(
    const v8::math::matrix_2X2<real_t>& right
    )
{
    a11_ += right.a11_; a12_ += right.a12_;
    a21_ += right.a21_; a22_ += right.a22_;
    return *this;
}

template<typename real_t>
v8::math::matrix_2X2<real_t>& v8::math::matrix_2X2<real_t>::operator -=(
    const v8::math::matrix_2X2<real_t>& right
    )
{
    a11_ -= right.a11_; a12_ -= right.a12_;
    a21_ -= right.a21_; a22_ -= right.a22_;
    return *this;
}

template<typename real_t>
v8::math::matrix_2X2<real_t>& v8::math::matrix_2X2<real_t>::operator *=(
    real_t k
    )
{
    a11_ *= k; a12_ *= k;
    a21_ *= k; a22_ *= k;
    return *this;
}

template<typename real_t>
v8::math::matrix_2X2<real_t>& v8::math::matrix_2X2<real_t>::operator /=(
    real_t k
    )
{
    using namespace internals;
    const real_t divident = transform_dividend_for_division<
        real_t, 
        is_floating_point
    >::transform(k);

    divide_helper<real_t, is_floating_point> div;
    a11_ = div::divide(divident); a12_ = div::divide(divident);
    a21_ = div::divide(divident); a22_ = div::divide(divident);

    return *this;
}

template<typename real_t>
v8::math::matrix_2X2<real_t>& v8::math::matrix_2X2<real_t>::invert() {
    const real_t kDetVal = determinant();
    const real_t kA11A22 = a11_ * a22_;
    const real_t kA12A21 = -(a12_ * a21_);

    using namespace internals;
    const real_t kDivideValue = transform_dividend_for_division<
        real_t,
        is_floating_point
    >::transform(kDetVal);

    typedef divide_helper<real_t, is_floating_point> div_t;

    a11_ = a22_ = div_t::divide(kA11A22, kDivideValue);
    a21_ = a12_ = div_t::divide(kA12A21, kDivideValue);

    return *this;
}

template<typename real_t>
v8::math::matrix_2X2<real_t>
adjoint_of(const v8::math::matrix_2X2<real_t>& mtx) {
    return matrix_2X2<real_t>(
        mtx.a11_ * mtx.a22_, -mtx.a12_ * mtx.a21,
        -mtx.a11_ * mtx.a21_, mtx.a11_ * mtx.a22_
        );
}

template<typename real_t>
inline
v8::math::matrix_2X2<real_t>
v8::math::transpose_of(const v8::math::matrix_2X2<real_t>& mtx) {
    matrix_2X2<real_t> other(mtx);
    return other.transpose();
}

template<typename real_t>
inline
v8::math::matrix_2X2<real_t>
v8::math::inverse_of(const v8::math::matrix_2X2<real_t>& mtx) {
    matrix_2X2<real_t> other(mtx);
    return other.invert();
}

template<typename real_t>
v8::math::matrix_2X2<real_t>
v8::math::multiply_transpose(
    const v8::math::matrix_2X2<real_t>& lhs, 
    const v8::math::matrix_2X2<real_t>& rhs
    )
{
    return matrix_2X2<real_t>(
        lhs.a11_ * rhs.a11_ + lhs.a12_ * rhs.a12_,
        lhs.a11_ * rhs.a21_ + lhs.a12_ * rhs.a22_,
        lhs.a21_ * rhs.a11_ + lhs.a22_ * rhs.a12_,
        lhs.a21_ * rhs.a21_ + lhs.a22_ * rhs.a22_
        );
}

template<typename real_t>
v8::math::matrix_2X2<real_t>
v8::math::transpose_multiply(
    const v8::math::matrix_2X2<real_t>& lhs, 
    const v8::math::matrix_2X2<real_t>& rhs
    )
{
    return matrix_2X2<real_t>(
        lhs.a11_ * rhs.a11_ + lhs.a21_ * rhs.a21_,
        lhs.a11_ * rhs.a12_ + lhs.a21_ * rhs.a22_,
        lhs.a12_ * rhs.a11_ + lhs.a22_ * rhs.a21_,
        lhs.a12_ * rhs.a12_ + lhs.a22_ * rhs.a22_
        );
}

template<typename real_t>
v8::math::matrix_2X2<real_t>
v8::math::transpose_multiply_transpose(
    const v8::math::matrix_2X2<real_t>& lhs, 
    const v8::math::matrix_2X2<real_t>& rhs
    )
{
    return matrix_2X2<real_t>(
        lhs.a11_ * rhs.a11_ + lhs.a21_ * rhs.a12_,
        lhs.a11_ * rhs.a21_ + lhs.a21_ * rhs.a22_,
        lhs.a12_ * rhs.a11_ + lhs.a22_ * rhs.a12_,
        lhs.a12_ * rhs.a21_ + lhs.a22_ * rhs.a22_
        );
}

template<typename real_t>
inline
v8::math::matrix_2X2<real_t>
v8::math::operator+(
    const v8::math::matrix_2X2<real_t>& lhs,
    const v8::math::matrix_2X2<real_t>& rhs
    )
{
    matrix_2X2<real_t> result(lhs);
    return result += rhs;
}

template<typename real_t>
inline
v8::math::matrix_2X2<real_t>
v8::math::operator-(
    const v8::math::matrix_2X2<real_t>& lhs,
    const v8::math::matrix_2X2<real_t>& rhs
    )
{
    matrix_2X2<real_t> result(lhs);
    return result -= rhs;
}

template<typename real_t>
v8::math::matrix_2X2<real_t>
v8::math::operator*(
    const v8::math::matrix_2X2<real_t>& lhs,
    const v8::math::matrix_2X2<real_t>& rhs
    )
{
    return matrix_2X2(
        lhs.a11_ * rhs.a11_ + lhs.a12_ * rhs.a21_,
        lhs.a11_ * rhs.a12_ + lhs.a12_ * rhs.a22_,
        lhs.a21_ * rhs.a11_ + lhs.a22_ * rhs.a21_,
        lhs.a21_ * rhs.a12_ + lhs.a22_ * rhs.a22_
        );
}

template<typename real_t>
inline
v8::math::matrix_2X2<real_t>
v8::math::operator*(
    real_t k,
    const v8::math::matrix_2X2<real_t>& rhs
    )
{
    matrix_2X2<real_t> result(lhs);
    return lhs *= k;
}

template<typename real_t>
inline
v8::math::matrix_2X2<real_t>
v8::math::operator*(
    const v8::math::matrix_2X2<real_t>& lhs,
    real_t k
    )
{
    return k * lhs;
}

template<typename real_t>
inline
v8::math::vector2<real_t>
v8::math::operator*(
    const v8::math::matrix_2X2<real_t>& mtx,
    const v8::math::vector2<real_t>& vec
    )
{
    return vector2<real_t>(
        mtx.a11_ * vec.x_ + mtx.a12_ * vec.y_,
        mtx.a21_ * vec.x_ + mtx.a22_ * vec.y_
        );
}

template<typename real_t>
inline
v8::math::matrix_2X2<real_t>
v8::math::operator/(
    const v8::math::matrix_2X2<real_t>& lhs,
    real_t k
    )
{
    matrix_2X2<real_t> result(lhs);
    return result /= k;
}
