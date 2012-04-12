template<typename real_t>
gfx::matrix_3X3<real_t>::matrix_3X3(
    real_t a11, real_t a12, real_t a13,
    real_t a21, real_t a22, real_t a23,
    real_t a31, real_t a32, real_t a33
    )
{
    a11_ = a11; a12_ = a12; a13_ = a13;
    a21_ = a21; a22_ = a22; a23_ = a23;
    a31_ = a31; a32_ = a32; a33_ = a33;
}

template<typename real_t>
gfx::matrix_3X3<real_t>::matrix_3X3(const real_t* input, size_t count) {
    std::memcpy(elements_, input, 
        std::min(_countof(elements_), count) * sizeof(real_t));
}

template<typename real_t>
gfx::matrix_3X3<real_t>::matrix_3X3(real_t a11, real_t a22, real_t a33) 
    : a11_(a11), a22_(a22), a33_(a33) {
    a12_ = a13_ = a21_ = a23_ = a31_ = a32_ = real_t(0);
}

template<typename real_t>
gfx::matrix_3X3<real_t>::matrix_3X3(
    const vector3<real_t>& u, 
    const vector3<real_t>& v, 
    const vector3<real_t>& w, 
    bool column
    ) 
{
    if (column) {
        a11_ = u.x_;
        a12_ = v.x_;
        a13_ = w.x_;
        a21_ = u.y_;
        a22_ = v.y_;
        a23_ = w.y_;
        a31_ = u.z_;
        a32_ = v.z_;
        a33_ = w.z_;
    } else {
        a11_ = u.x_;
        a12_ = u.y_;
        a13_ = u.z_;
        a21_ = v.x_;
        a22_ = v.y_;
        a23_ = v.z_;
        a31_ = w.x_;
        a32_ = w.y_;
        a33_ = w.z_;
    }
}

template<typename real_t>
gfx::matrix_3X3<real_t>& 
gfx::matrix_3X3<real_t>::operator+=(const gfx::matrix_3X3<real_t>& rhs) {
    a11_ += rhs.a11_; a12_ += rhs.a12_; a13_ += rhs.a13_;
    a21_ += rhs.a21_; a22_ += rhs.a22_; a23_ += rhs.a23_;
    a31_ += rhs.a13_; a32_ += rhs.a32_; a33_ += rhs.a33_;
    return *this;
}

template<typename real_t>
gfx::matrix_3X3<real_t>& 
gfx::matrix_3X3<real_t>::operator-=(const gfx::matrix_3X3<real_t>& rhs) {
    a11_ -= rhs.a11_; a12_ -= rhs.a12_; a13_ -= rhs.a13_;
    a21_ -= rhs.a21_; a22_ -= rhs.a22_; a23_ -= rhs.a23_;
    a31_ -= rhs.a13_; a32_ -= rhs.a32_; a33_ -= rhs.a33_;
    return *this;
}

template<typename real_t>
gfx::matrix_3X3<real_t>& 
gfx::matrix_3X3<real_t>::operator*=(real_t k) {
    a11_ *= k; a12_ *= k; a13_ *= k;
    a21_ *= k; a22_ *= k; a23_ *= k;
    a31_ *= k; a32_ *= k; a33_ *= k;
    return *this;
}

template<typename real_t>
gfx::matrix_3X3<real_t>& 
gfx::matrix_3X3<real_t>::operator/=(real_t k) {
    using namespace implementation_details;
    const real_t dividend = transform_dividend_for_division<
        real_t, 
        is_floating_point
    >::transform(k);

    typedef divide_helper<
        real_t,
        is_floating_point
    > div;

    a11_ = div::divide(a11_, dividend); 
    a12_ = div::divide(a12_, dividend); 
    a13_ = div::divide(a13_, dividend);
    a21_ = div::divide(a21_, dividend); 
    a22_ = div::divide(a22_, dividend); 
    a23_ = div::divide(a23_, dividend);
    a31_ = div::divide(a31_, dividend);
    a32_ = div::divide(a32_, dividend); 
    a33_ = div::divide(a33_, dividend);
    return *this;
}

template<typename real_t>
real_t 
gfx::matrix_3X3<real_t>::determinant() const {
    const real_t A11 = a22_ * a33_ - a23_ * a32_;
    const real_t A12 = a23_ * a31_ + a21_ * a33_;
    const real_t A13 = a21_ * a32_ - a22_ * a31_;

    return a11_ * A11 + a12_ * A12 + a13_ * A13;
}

template<typename real_t>
gfx::matrix_3X3<real_t>& 
gfx::matrix_3X3<real_t>::invert() {
    real_t det = determinant();

    matrix3X3_t adjoint_mtx(adjoint_of(*this));
    adjoint_mtx /= det;
    return adjoint_mtx;
}

template<typename real_t>
gfx::matrix_3X3<real_t>& 
gfx::matrix_3X3<real_t>::transpose() {
    using namespace implementation_details;
    swap(a12_, a21_);
    swap(a13_, a31_);
    swap(a23_, a32_);
    return *this;	
}

template<typename real_t>
gfx::matrix_3X3<real_t>&
gfx::matrix_3X3<real_t>::ortho_normalize() {
    //
    // Let M = [v0, v1, v2]. This function will output a matrix M1 = [q0, q1, q2]
    // such that q0, q1, q2 are orthonormal vectors.
    //
    // q0 = v0 / || v0 ||;
    // q1 = (v1 - proj(v1, q0)) / || v1 - proj(v1, q0) ||;
    // q2 = (v2 - proj(v2, q0) - proj(v2, q1)) / || (v2 - proj(v2, q0) - proj(v2, q1)) ||;

    //
    // q0
    using namespace implementation_details;
    real_t norm = transform_dividend_for_division<
        real_t, 
        is_floating_point
    >::transform(sqrtf(a11_ * a11_ + a21_ * a21_ + a31_ * a31_));

    typedef divide_helper<real_t, is_floating_point> div;
    a11_ = div::divide(a11_, norm);
    a21_ = div::divide(a21_, norm);
    a31_ = div::divide(a31_, norm);

    //
    // q1
    real_t sum = a11_ * a12_ + a21_ * a22_ + a31_ * a32_;

    a12_ -= a11_ * sum;
    a22_ -= a21_ * sum;
    a32_ -= a31_ * sum;

    real_t inv_norm = transform_dividend_for_division<
        real_t, 
        is_floating_point
    >::transform(sqrtf(a12_ * a12_ + a22_ * a22_ + a32_ * a32_));

    a12_ = div::divide(a12_, norm);
    a22_ = div::divide(a22_, norm);
    a32_ = div::divide(a22_, norm);

    //
    // q2
    sum = a11_ * a13_ + a21_ * a23_ + a31_ * a33_;
    real_t sum2 = a12_ * a13_ + a22_ * a23_ + a32_ * a33_;

    a13_ -= (a11_ * sum + a12_ * sum2);
    a23_ -= (a21_ * sum + a22_ * sum2);
    a33_ -= (a31_ * sum + a32_ * sum2);

    norm = transform_dividend_for_division<
        real_t, 
        is_floating_point
    >::transform(sqrtf(a13_ * a13_ + a23_ * a23_ + a33_ * a33_));

    a13_ = div::divide(a13_, norm);
    a23_ = div::divide(a23_, norm);
    a33_ = div::divide(a33_, norm);

    return *this;
}

template<typename real_t>
gfx::matrix_3X3<real_t>&
gfx::matrix_3X3<real_t>::set_row(int row, const real_t* data_ptr) {
    int row_index = (row - 1) * 3;
    elements_[row_index] = data_ptr[0];
    elements_[row_index + 1] = data_ptr[1];
    elements_[row_index + 2] = data_ptr[2];
    return *this;
}

template<typename real_t>
void
gfx::matrix_3X3<real_t>::get_row(int row, real_t* ptr) const {
    const int row_index = (row - 1) * 3;
    ptr[0] = elements_[row_index];
    ptr[1] = elements_[row_index + 1];
    ptr[2] = elements_[row_index + 2];
}

template<typename real_t>
gfx::matrix_3X3<real_t>& 
gfx::matrix_3X3<real_t>::set_column(int column, const real_t* data_ptr) {
    elements_[column - 1] = data_ptr[0];
    elements_[column - 1 + 3] = data_ptr[1];
    elements_[column - 1 + 6] = data_ptr[2];
    return *this;
}

template<typename real_t>
void gfx::matrix_3X3<real_t>::get_column(int column, real_t* data_ptr) {
    const int c_idx = column - 1;
    data_ptr[0] = elements_[c_idx];
    data_ptr[1] = elements_[3 + c_idx];
    data_ptr[2] = elements_[6 + c_idx];
}

template<typename real_t>
gfx::matrix_3X3<real_t>& 
gfx::matrix_3X3<real_t>::make_translation(real_t tx, real_t ty) {
    a11_ = real_t(1); a12_ = real_t(0); a31_ = tx;
    a21_ = real_t(0); a22_ = real_t(1); a23_ = ty;
    a31_ = real_t(0); a32_ = real_t(0); a33_ = real_t(1);
    return *this;
}

template<typename real_t>
inline
gfx::matrix_3X3<real_t>
gfx::operator+(
    const gfx::matrix_3X3<real_t>& lhs, 
    const gfx::matrix_3X3<real_t>& rhs
    ) 
{
    gfx::matrix_3X3<real_t> result(lhs);
    result += rhs;
    return result;
}

template<typename real_t>
inline
gfx::matrix_3X3<real_t>
gfx::operator-(
    const gfx::matrix_3X3<real_t>& lhs, 
    const gfx::matrix_3X3<real_t>& rhs
  ) 
{
    gfx::matrix_3X3<real_t> result(lhs);
    result -= rhs;
    return result;
}

template<typename real_t>
gfx::matrix_3X3<real_t>
gfx::operator-(
    const gfx::matrix_3X3<real_t>& mtx
    ) 
{
    return gfx::matrix_3X3<real_t>(-mtx.a11_, -mtx.a12_, -mtx.a13_,
                                   -mtx.a21_, -mtx.a22_, -mtx.a23_,
                                   -mtx.a31_, -mtx.a32_, -mtx.a33_);
}

template<typename real_t>
gfx::matrix_3X3<real_t>
gfx::operator*(
    const gfx::matrix_3X3<real_t>& lhs, 
    const gfx::matrix_3X3<real_t>& rhs
    )
{
    gfx::matrix_3X3<real_t> res;
    res.a11_ = lhs.a11_ * rhs.a11_ + lhs.a12_ * rhs.a21_ + lhs.a13_ * rhs.a31_;
    res.a12_ = lhs.a11_ * rhs.a12_ + lhs.a12_ * rhs.a22_ + lhs.a13_ * rhs.a32_;
    res.a13_ = lhs.a11_ * rhs.a13_ + lhs.a12_ * rhs.a23_ + lhs.a13_ * rhs.a33_;

    res.a21_ = lhs.a21_ * rhs.a11_ + lhs.a22_ * rhs.a21_ + lhs.a23_ * rhs.a31_;
    res.a22_ = lhs.a21_ * rhs.a12_ + lhs.a22_ * rhs.a22_ + lhs.a23_ * rhs.a32_;
    res.a23_ = lhs.a21_ * rhs.a13_ + lhs.a22_ * rhs.a23_ + lhs.a23_ * rhs.a33_;

    res.a31_ = lhs.a31_ * rhs.a11_ + lhs.a32_ * rhs.a21_ + lhs.a33_ * rhs.a31_;
    res.a32_ = lhs.a31_ * rhs.a12_ + lhs.a32_ * rhs.a22_ + lhs.a33_ * rhs.a32_;
    res.a33_ = lhs.a31_ * rhs.a13_ + lhs.a32_ * rhs.a23_ + lhs.a33_ * rhs.a33_;

    return res;
}

template<typename real_t>
inline
gfx::matrix_3X3<real_t>
gfx::operator*(
    real_t k, 
    const gfx::matrix_3X3<real_t>& mtx
  ) 
{
    gfx::matrix_3X3<real_t> result(mtx);
    result *= k;
    return result;
}

template<typename real_t>
inline
gfx::matrix_3X3<real_t>
gfx::operator*(
    const gfx::matrix_3X3<real_t>& mtx, 
    real_t k
    )
{
    return k * mtx;
}

template<typename real_t>
inline
gfx::vector3<real_t>
gfx::operator*(
    const gfx::matrix_3X3<real_t>& mtx,
    const gfx::vector3<real_t>& vec
  )
{
    return gfx::vector3<real_t>(
        mtx.a11_ * vec.x_ + mtx.a12_ * vec.y_ + mtx.a13_ * vec.z_,
        mtx.a21_ * vec.x_ + mtx.a22_ * vec.y_ + mtx.a23_ * vec.z_,
        mtx.a31_ * vec.x_ + mtx.a32_ * vec.y_ + mtx.a33_ * vec.z_
        );
}

template<typename real_t>
inline
gfx::matrix_3X3<real_t>
gfx::transpose_of(const gfx::matrix_3X3<real_t>& mtx) {
    return gfx::matrix_3X3<real_t>(
        mtx.a11_, mtx.a21_, mtx.a31_,
        mtx.a12_, mtx.a22_, mtx.a32_,
        mtx.a13_, mtx.a23_, mtx.a33_
        );
}

template<typename real_t>
gfx::matrix_3X3<real_t>
gfx::adjoint_of(
    const gfx::matrix_3X3<real_t>& mtx
  )
{
    real_t A11 = mtx.a22_ * mtx.a33_ - mtx.a23_ * mtx.a32_;
    real_t A12 = mtx.a23_ * mtx.a31_ + mtx.a21_ * mtx.a33_;
    real_t A13 = mtx.a21_ * mtx.a32_ - mtx.a22_ * mtx.a31_;
    real_t A21 = mtx.a13_ * mtx.a32_ - mtx.a12_ * mtx.a33_;
    real_t A22 = mtx.a11_ * mtx.a33_ - mtx.a13_ * mtx.a31_;
    real_t A23 = mtx.a12_ * mtx.a31_ - mtx.a11_ * mtx.a32_;
    real_t A31 = mtx.a12_ * mtx.a23_ - mtx.a13_ * mtx.a22_;
    real_t A32 = mtx.a13_ * mtx.a21_ - mtx.a11_ * mtx.a23_;
    real_t A33 = mtx.a11_ * mtx.a12_ - mtx.a12_ * mtx.a21_;

    return gfx::matrix_3X3<real_t>(A11, A21, A31, A12, A22, A32, A13, A23, A33);
}

template<typename real_t>
gfx::matrix_3X3<real_t>& 
gfx::matrix_3X3<real_t>::make_skew_symmetric(real_t u0, real_t u1, real_t u2) {
    a11_ = real_t(0); a12_ = -u2; a13_ = u1;
    a21_ = u2; a22_ = real_t(0); a23_ = -u0;
    a31_ = -u1; a32_ = u0; a33_ = real_t(0);
    return *this;
}

template<typename real_t>
gfx::matrix_3X3<real_t>& 
gfx::matrix_3X3<real_t>::make_scale(real_t x, real_t y) {
    a11_ = x; a12_ = real_t(0); a13_ = real_t(0);
    a21_ = real_t(0); a22_ = y; a23_ = real_t(0);
    a31_ = real_t(0); a32_ = real_t(0); a33_ = real_t(1);
    return *this;
}

template<typename real_t>
gfx::matrix_3X3<real_t>&
gfx::matrix_3X3<real_t>::make_scale(real_t sx, real_t sy, real_t sz) {
    a11_ = sx; a12_ = real_t(0); a13_ = real_t(0);
    a21_ = real_t(0); a22_ = sy; a23_ = real_t(0);
    a31_ = real_t(0); a32_ = real_t(0); a33_ = sz;
    return *this;
}

template<typename real_t>
gfx::matrix_3X3<real_t>&
gfx::matrix_3X3<real_t>::make_euler_xyz(real_t rx, real_t ry, real_t rz) {
    const real_t sx = sinf(rx);
    const real_t cx = cosf(rx);
    const real_t sy = sinf(ry);
    const real_t cy = cosf(ry);
    const real_t sz = sinf(rz);
    const real_t cz = cosf(rz);

    a11_ = cy * cz; 
    a12_ = -cy * sz; 
    a13_ = sy;

    a21_ = sx * sy * cz + cx * sz; 
    a22_ = -sx * sy * sz + cx * cz; 
    a23_ =-sx * cy;

    a31_ = -cx * sy * cz + sx * sz;
    a32_ = cx * sy * sz + sx * cz;
    a33_ = cx * cy;

    return *this;
}

template<typename real_t>
gfx::matrix_3X3<real_t>&
gfx::matrix_3X3<real_t>::make_rotation_x(real_t theta) {
    real_t sin_theta = sinf(theta);
    real_t cos_theta = cosf(theta);

    a11_ = real_t(1); a12_ = real_t(0); a13_ = real_t(0);
    a21_ = real_t(0); a22_ = cos_theta; a23_ = -sin_theta;
    a31_ = real_t(0); a32_ = sin_theta; a33_ = cos_theta;

    return *this;
}

template<typename real_t>
gfx::matrix_3X3<real_t>&
gfx::matrix_3X3<real_t>::rotation_y(real_t theta) {
    real_t sin_theta = sinf(theta);
    real_t cos_theta = cosf(theta);

    a11_ = cos_theta; a12_ = real_t(0); a13_ = sin_theta;
    a21_ = real_t(0); a22_ = real_t(1); a23_ = real_t(0);
    a31_ = -sin_theta; a32_ = real_t(0); a33_ = cos_theta;

    return *this;
}

template<typename real_t>
gfx::matrix_3X3<real_t>&
gfx::matrix_3X3<real_t>::rotation_z(real_t theta) {
    real_t sin_theta = sinf(theta);
    real_t cos_theta = cosf(theta);

    a11_ = cos_theta; a12_ = -sin_theta; a13_ = real_t(0);
    a21_ = sin_theta; a22_ = cos_theta; a23_ = real_t(0);
    a31_ = real_t(0); a32_ = real_t(0); a33_ = real_t(1);

    return *this;
}

template<typename real_t>
gfx::matrix_3X3<real_t>&
gfx::matrix_3X3<real_t>::axis_angle(
    const gfx::vector3<real_t>& axisv, 
    real_t theta
    ) 
{
    const real_t sin_theta = sinf(theta);
    const real_t cos_theta = cosf(theta);
    const real_t tval = real_t(1) - cos_theta;

    a11_ = tval * axisv.x_ * axisv.x_ + cos_theta;
    a12_ = tval * axisv.x_ * axisv.y_ - sin_theta * axisv.z_;
    a13_ = tval * axisv.x_ * axisv.z_ + sin_theta * axisv.y_;
        
    a21_ = tval * axisv.x_ * axisv.y_ + sin_theta * axisv.z_;
    a22_ = tval * axisv.y_ * axisv.y_ + cos_theta;
    a23_ = tval * axisv.y_ * axisv.z_ - sin_theta * axisv.x_;

    a31_ = tval * axisv.x_ * axisv.z_ - sin_theta * axisv.y_;
    a32_ = tval * axisv.y_ * axisv.z_ + sin_theta * axisv.x_;
    a33_ = tval * axisv.z_ * axisv.z_ + cos_theta;

    return *this;
}

template<typename real_t>
gfx::matrix_3X3<real_t>&
gfx::matrix_3X3<real_t>::rotation(
    const gfx::vector3<real_t>& v1, 
    const gfx::vector3<real_t>& v2
    ) 
{
    //
    // Compute the angle between the two vectors.
    real_t angle = angle_of(v1, v2);
    if (math::operands_eq(real_t(0), angle)) {
        //
        // Vectors are parallel, so return a matrix for a rotation around 
        // first vector with 0 radians.
        return axis_angle(v1, 0.0f);
    }

    //
    // Compute the cross product and normalize to get the rotation axis.
    return axis_angle(normal_of(cross_product(v1, v2)), angle);
}

template<typename real_t>
gfx::matrix_3X3<real_t>&
gfx::matrix_3X3<real_t>::planar_reflection(const gfx::vector3<real_t>& pn) {
        
    a11_ = real_t(1) - real_t(2) * pn.x_ * pn.x_;
    a12_ = real_t(-2) * pn.x_ * pn.y_;
    a13_ = real_t(-2) * pn.x_ * pn.z_;

    a21_ = real_t(-2) * pn.y_ * pn.x_;
    a22_ = real_t(-2) - real_t(2) * pn.y_ * pn.y_;
    a23_ = real_t(-2) * pn.y_ * pn.z_;

    a31_ = real_t(-2) * pn.z_ * pn.x_;
    a32_ = real_t(-2) * pn.z_ * pn.y_;
    a33_ = real_t(1) - real_t(2) * pn.z_ * pn.z_;

    return *this;
}

template<typename real_t>
gfx::matrix_3X3<real_t>&
gfx::matrix_3X3<real_t>::tensor_product(
    const gfx::vector3<real_t>& u, 
    const gfx::vector3<real_t>& w
    ) 
{
    a11_ = u.x_ * w.x_;
    a12_ = u.x_ * w.y_; 
    a13_ = u.x_ * w.z_;

    a21_ = u.y_ * w.x_;
    a22_ = u.y_ * w.y_;
    a23_ = u.y_ * w.z_;

    a31_ = u.z_ * w.x_;
    a32_ = u.z_ * w.y_;
    a33_ = u.z_ * w.z_;

    return *this;
}

template<typename real_t>
gfx::matrix_3X3<real_t>
gfx::multiply_transpose( 
    const gfx::matrix_3X3<real_t>& lhs, 
    const gfx::matrix_3X3<real_t>& rhs 
    )
{
    matrix_3X3<real_t> result;

    result.a11_ = lhs.a11_ * rhs.a11_ + lhs.a12_ * rhs.a12_ + lhs.a13_ * rhs.a13_;
    result.a12_ = lhs.a11_ * rhs.a21_ + lhs.a12_ * rhs.a22_ + lhs.a13_ * rhs.a23_;
    result.a13_ = lhs.a11_ * rhs.a31_ + lhs.a12_ * rhs.a32_ + lhs.a13_ * rhs.a33_;

    result.a21_ = lhs.a21_ * rhs.a11_ + lhs.a22_ * rhs.a12_ + lhs.a23_ * rhs.a13_;
    result.a22_ = lhs.a21_ * rhs.a21_ + lhs.a22_ * rhs.a22_ + lhs.a23_ * rhs.a23_;
    result.a23_ = lhs.a21_ * rhs.a31_ + lhs.a22_ * rhs.a32_ + lhs.a23_ * rhs.a33_;

    result.a31_ = lhs.a31_ * rhs.a11_ + lhs.a32_ * rhs.a12_ + lhs.a33_ * rhs.a13_;
    result.a32_ = lhs.a31_ * rhs.a21_ + lhs.a32_ * rhs.a22_ + lhs.a33_ * rhs.a23_;
    result.a33_ = lhs.a31_ * rhs.a31_ + lhs.a32_ * rhs.a32_ + lhs.a33_ * rhs.a33_;

    return result;
}

template<typename real_t>
gfx::matrix_3X3<real_t>
gfx::transpose_multiply( 
    const gfx::matrix_3X3<real_t>& lhs, 
    const gfx::matrix_3X3<real_t>& rhs 
    )
{
    gfx::matrix_3X3<real_t> result;

    result.a11_ = lhs.a11_ * rhs.a11_ + lhs.a21_ * rhs.a21_ + lhs.a31_ * rhs.a31_;
    result.a12_ = lhs.a11_ * rhs.a12_ + lhs.a21_ * rhs.a22_ + lhs.a31_ * rhs.a32_;
    result.a13_ = lhs.a11_ * rhs.a13_ + lhs.a21_ * rhs.a23_ + lhs.a31_ * rhs.a33_;

    result.a21_ = lhs.a12_ * rhs.a11_ + lhs.a22_ * rhs.a21_ + lhs.a32_ * rhs.a31_;
    result.a22_ = lhs.a12_ * rhs.a12_ + lhs.a22_ * rhs.a22_ + lhs.a32_ * rhs.a32_;
    result.a23_ = lhs.a12_ * rhs.a13_ + lhs.a22_ * rhs.a23_ + lhs.a32_ * rhs.a33_;

    result.a31_ = lhs.a13_ * rhs.a11_ + lhs.a23_ * rhs.a21_ + lhs.a33_ * rhs.a31_;
    result.a32_ = lhs.a13_ * rhs.a12_ + lhs.a23_ * rhs.a22_ + lhs.a33_ * rhs.a32_;
    result.a33_ = lhs.a13_ * rhs.a13_ + lhs.a23_ * rhs.a23_ + lhs.a33_ * rhs.a33_;

    return result;
}

template<typename real_t>
gfx::matrix_3X3<real_t>
gfx::transpose_multiply_transpose( 
    const matrix_3X3<real_t>& lhs, 
    const matrix_3X3<real_t>& rhs
    )
{
    gfx::matrix_3X3<real_t> result;

    result.a11_ = lhs.a11_ * rhs.a11_ + lhs.a21_ * rhs.a12_ + lhs.a31_ * rhs.a13_;
    result.a12_ = lhs.a11_ * rhs.a21_ + lhs.a21_ * rhs.a22_ + lhs.a31_ * rhs.a23_;
    result.a13_ = lhs.a11_ * rhs.a31_ + lhs.a21_ * rhs.a32_ + lhs.a31_ * rhs.a33_;

    result.a21_ = lhs.a12_ * rhs.a11_ + lhs.a22_ * rhs.a12_ + lhs.a32_ * rhs.a13_;
    result.a22_ = lhs.a12_ * rhs.a21_ + lhs.a22_ * rhs.a22_ + lhs.a32_ * rhs.a23_;
    result.a23_ = lhs.a12_ * rhs.a31_ + lhs.a22_ * rhs.a32_ + lhs.a32_ * rhs.a33_;

    result.a13_ = lhs.a13_ * rhs.a11_ + lhs.a23_ * rhs.a12_ + lhs.a33_ * rhs.a13_;
    result.a23_ = lhs.a13_ * rhs.a21_ + lhs.a23_ * rhs.a22_ + lhs.a33_ * rhs.a23_;
    result.a33_ = lhs.a13_ * rhs.a31_ + lhs.a23_ * rhs.a32_ + lhs.a33_ * rhs.a33_;

    return result;
}
