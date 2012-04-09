template<typename real_t>
gfx::matrix_4X4<real_t>::matrix_4X4(
    real_t a11, real_t a12, real_t a13, real_t a14,
    real_t a21, real_t a22, real_t a23, real_t a24,
    real_t a31, real_t a32, real_t a33, real_t a34,
    real_t a41, real_t a42, real_t a43, real_t a44
    )
{
    a11_ = a11; a12_ = a12; a13_ = a13; a14_ = a14;
    a21_ = a21; a22_ = a22; a23_ = a23; a24_ = a24;
    a31_ = a31; a32_ = a32; a33_ = a33; a34_ = a34;
    a41_ = a41; a42_ = a42; a43_ = a43; a44_ = a44;
}

template<typename real_t>
gfx::matrix_4X4<real_t>::matrix_4X4(
    real_t a11, 
    real_t a22, 
    real_t a33,
    real_t a44
    )
{
    a11_ = a11; a12_ = real_t(0); a13_ = real_t(0); a14_ = real_t(0);
    a21_ = real_t(0); a22_ = a22; a23_ = real_t(0); a24_ = real_t(0);
    a31_ = real_t(0); a32_ = real_t(0); a33_ = a33; a34_ = real_t(0);
    a41_ = real_t(0); a42_ = real_t(0); a43_ = real_t(0); a44_ = a44;
}

template<typename real_t>
gfx::matrix_4X4<real_t>::matrix_4X4(
    const gfx::vector4<real_t>& v1,
    const gfx::vector4<real_t>& v2,
    const gfx::vector4<real_t>& v3,
    const gfx::vector4<real_t>& v4,
    bool column
    )
{
    if (column) {
        a11_ = v1.x_;
        a12_ = v2.x_;
        a13_ = v3.x_;
        a14_ = v4.x_;

        a21_ = v1.y_;
        a22_ = v2.y_;
        a23_ = v3.y_;
        a24_ = v4.y;

        a31_ = v1.z_;
        a32_ = v2.z_;
        a33_ = v3.z_;
        a34_ = v4.z_;

        a41_ = v1.w_;
        a42_ = v2.w_;
        a43_ = v3.w_;
        a44_ = v4.w_;
    } else {
        a11_ = v1.x_;
        a12_ = v1.y_;
        a13_ = v1.z_;
        a14_ = v1.w_;

        a21_ = v2.x_;
        a22_ = v2.y_;
        a23_ = v2.z_;
        a24_ = v2.w_;

        a31_ = v3.x_;
        a32_ = v3.y_;
        a33_ = v3.z_;
        a34_ = v3.w_;

        a41_ = v4.x_;
        a42_ = v4.y_;
        a43_ = v4.z_;
        a44_ = v4.w_;
    }
}

template<typename real_t>
gfx::matrix_4X4<real_t>&
gfx::matrix_4X4<real_t>::operator+=(const gfx::matrix_4X4<real_t>& rhs) {
    for (size_t i = 0; i < _countof(elements_); ++i)
        elements_[i] += rhs.elements_[i];
    return *this;
}

template<typename real_t>
gfx::matrix_4X4<real_t>&
gfx::matrix_4X4<real_t>::operator-=(const gfx::matrix_4X4<real_t>& rhs) {
    for (size_t i = 0; i < _countof(elements_); ++i)
        elements_[i] -= rhs.elements_[i];
    return *this;
}

template<typename real_t>
gfx::matrix_4X4<real_t>&
gfx::matrix_4X4<real_t>::operator*=(real_t k) {
    for (size_t i = 0; i < _countof(elements_); ++i)
        elements_[i] *= k;
    return *this;
}

template<typename real_t>
gfx::matrix_4X4<real_t>&
gfx::matrix_4X4<real_t>::operator/=(real_t k) {
    using namespace implementation_details;
    
    const real_t kDivident = transform_dividend_for_division<
        real_t,
        is_floating_point
    >::transform(k);
    
    divide_helper<real_t, is_floating_point> div;
    
    for (size_t i = 0; i < _countof(elements_); ++i)
        elements_[i] = div::divide(elements_[i], kDivident);
    return *this;
}

template<typename real_t>
real_t
gfx::matrix_4X4<real_t>::determinant() const {
    return a11_ * (
      a22_ * a33_ * a44_	+
      a32_ * a43_ * a24_	+
      a42_ * a23_ * a34_	-
      a24_ * a33_ * a42_	-
      a34_ * a43_ * a22_	-
      a44_ * a23_ * a32_
      )
      +
    a12_ * (
      a21_ * a33_ * a44_ 	+
      a31_ * a43_ * a24_	+
      a41_ * a23_ * a34_	-
      a24_ * a33_ * a41_	-
      a34_ * a43_ * a21_	-
      a44_ * a23_ * a31_
      )
      -
    a13_ * (
      a21_ * a32_ * a44_ 	+
      a31_ * a42_ * a24_	+
      a41_ * a22_ * a34_	-
      a24_ * a32_ * a41_ 	-
      a34_ * a42_ * a21_	-
      a44_ * a22_ * a31_
      )
      +
    a14_ * (
      a21_ * a32_ * a43_	+
      a31_ * a42_ * a23_	+
      a41_ * a22_ * a33_	-
      a23_ * a32_ * a41_	-
      a33_ * a42_ * a21_	-
      a43_ * a22_ * a31_
      );
}

template<typename real_t>
gfx::matrix_4X4<real_t>&
gfx::matrix_4X4<real_t>::invert() {
    const real_t kDetValue = determinant();
    matrix4X4_t result(adjoint_of(*this));
    result /= kDetValue;
    std::memcpy(elements_, result.elements_, _countof(elements_) * sizeof(real_t));
    return *this;
}

template<typename real_t>
gfx::matrix_4X4<real_t>&
gfx::matrix_4X4<real_t>::transpose() {
    using namespace implementation_details;
    
    swap(a12_, a21_);
    swap(a13_, a31_);
    swap(a14_, a41_);

    swap(a23_, a32_);
    swap(a24_, a42_);

    swap(a34_, a43_);

    return *this;
}

template<typename real_t>
gfx::matrix_4X4<real_t>&
gfx::matrix_4X4<real_t>::set_upper3x3(const real_t* data) {
    for (size_t i = 0; i < 3; ++i) {
        size_t row = i * 4;
        for (size_t j = 0; j < 3; ++j) {
            elements_[row + j] = data[i * 3 + j];
        }
    }
    return *this;
}
template<typename real_t>
void
gfx::matrix_4X4<real_t>::get_upper3x3(real_t* data) const {
    for (size_t i = 0; i < 3; ++i) {
        size_t row = i * 4;
        for (size_t j = 0; j < 3; ++j) {
            data[i * 3 + j] = elements_[row + j];
        }
    }
}

template<typename real_t>
gfx::matrix_4X4<real_t>&
gfx::matrix_4X4<real_t>::set_row(int row, const real_t* row_data) {
    real_t* row_ptr = elements_ + (row - 1) * 4;
    for (size_t i = 0; i < 4; ++i)
        row_ptr[i] = row_data[i];
    return *this;
}

template<typename real_t>
gfx::matrix_4X4<real_t>&
gfx::matrix_4X4<real_t>::set_column(int column, const real_t* data) {
    for (size_t i = 0; i < 4; ++i)
        elements_[i * 4 + column - 1] = data[i];
    return *this;
}

template<typename real_t>
void
gfx::matrix_4X4<real_t>::get_row(int row, real_t* data) const {
    const real_t* row_ptr = elements_ + (row - 1) * 4;
    for (size_t i = 0; i < 4; ++i)
        data[i] = row_ptr[i];
}

template<typename real_t>
void
gfx::matrix_4X4<real_t>::get_column(int col, real_t* data) const {
    for (size_t i = 0; i < 4; ++i)
        data[i] = elements_[i * 4 + col - 1];
}

template<typename real_t>
gfx::matrix_4X4<real_t>&
gfx::matrix_4X4<real_t>::transform_affine_vector(
    gfx::vector4<real_t>* pvec
    ) const
{
    pvec->x_ = a11_ * pvec->x_ + a12_ * pvec->y_ + a13_ * pvec->z_;
    pvec->y_ = a21_ * pvec->x_ + a22_ * pvec->y_ + a23_ * pvec->z_;
    pvec->z_ = a31_ * pvec->x_ + a32_ * pvec->y_ + a33_ * pvec->z_;

    return *this;
}

template<typename real_t>
gfx::matrix_4X4<real_t>&
gfx::matrix_4X4<real_t>::transform_affine_point(
    gfx::vector4<real_t>* apt
    ) const
{
    apt->x_ = a11_ * apt->x_ + a12_ * apt->y_ + a13_ * apt->z_ + a14_;
    apt->y_ = a21_ * apt->x_ + a22_ * apt->y_ + a23_ * apt->z_ + a24_;
    apt->z_ = a31_ * apt->x_ + a32_ * apt->y_ + a33_ * apt->z_ + a34_;

    return *this;
}

template<typename real_t>
gfx::matrix_4X4<real_t>&
gfx::matrix_4X4<real_t>::transform_homogeneous_point(
    gfx::vector4<real_t>* hpt
    ) const
{
    hpt->x_ = a11_ * hpt->x_ + a12_ * hpt->y_ + a13_ * hpt->z_ + a14_ * hpt->w_;
    hpt->y_ = a21_ * hpt->x_ + a22_ * hpt->y_ + a23_ * hpt->z_ + a24_ * hpt->w_;
    hpt->z_ = a31_ * hpt->x_ + a32_ * hpt->y_ + a33_ * hpt->z_ + a34_ * hpt->w_;
    hpt->w_ = a41_ * hpt->x_ + a42_ * hpt->y_ + a43_ * hpt->z_ + a44_ * hpt->w_;

    return *this;
}

template<typename real_t>
inline
gfx::matrix_4X4<real_t>
operator+(
    const gfx::matrix_4X4<real_t>& lhs,
    const gfx::matrix_4X4<real_t>& rhs
    )
{
    matrix_4X4<real_t> result(lhs);
    result += rhs;
    return result;
}

template<typename real_t>
inline
gfx::matrix_4X4<real_t>
operator-(
    const gfx::matrix_4X4<real_t>& lhs,
    const gfx::matrix_4X4<real_t>& rhs
    )
{
    matrix_4X4<real_t> result(lhs);
    result -= rhs;
    return result;
}

template<typename real_t>
inline
gfx::matrix_4X4<real_t>
operator-(
    const gfx::matrix_4X4<real_t>& mtx
    )
{
    return matrix_4X4<real_t>(
        -mtx.a11_, -mtx.a12_, -mtx.a13_, -mtx.a14_,
        -mtx.a21_, -mtx.a22_, -mtx.a23_, -mtx.a24_,
        -mtx.a31_, -mtx.a32_, -mtx.a33_, -mtx.a34_,
        -mtx.a41_, -mtx.a42_, -mtx.a43_, -mtx.a44_
        );
}

template<typename real_t>
gfx::matrix_4X4<real_t>
operator*(
    const gfx::matrix_4X4<real_t>& lhs,
    const gfx::matrix_4X4<real_t>& rhs
    )
{
    matrix_4X4<real_t> res;
    res.a11_ = lhs.a11_ * rhs.a11_ + lhs.a12_ * rhs.a21_ 
        + lhs.a13_ * rhs.a31_ + lhs.a14_ * rhs.a41_;
    res.a12_ = lhs.a11_ * rhs.a12_ + lhs.a12_ * rhs.a22_ 
        + lhs.a13_ * rhs.a32_ + lhs.a14_ * rhs.a42_;
    res.a13_ = lhs.a11_ * rhs.a13_ + lhs.a12_ * rhs.a23_ 
        + lhs.a13_ * rhs.a33_ + lhs.a14_ * rhs.a43_;
    res.a14_ = lhs.a11_ * rhs.a14_ + lhs.a12_ * rhs.a24_ 
        + lhs.a13_ * rhs.a34_ + lhs.a14_ * rhs.a44_;

    res.a21_ = lhs.a21_ * rhs.a11_ + lhs.a22_ * rhs.a21_ 
        + lhs.a23_ * rhs.a31_ + lhs.a24_ * rhs.a41_;
    res.a22_ = lhs.a21_ * rhs.a12_ + lhs.a22_ * rhs.a22_ 
        + lhs.a23_ * rhs.a32_ + lhs.a24_ * rhs.a42_;
    res.a23_ = lhs.a21_ * rhs.a13_ + lhs.a22_ * rhs.a23_ 
        + lhs.a23_ * rhs.a33_ + lhs.a24_ * rhs.a43_;
    res.a24_ = lhs.a21_ * rhs.a14_ + lhs.a22_ * rhs.a24_ 
        + lhs.a23_ * rhs.a34_ + lhs.a24_ * rhs.a44_;

    res.a31_ = lhs.a31_ * rhs.a11_ + lhs.a32_ * rhs.a21_ 
        + lhs.a33_ * rhs.a31_ + lhs.a34_ * rhs.a41_;
    res.a32_ = lhs.a31_ * rhs.a12_ + lhs.a32_ * rhs.a22_ 
        + lhs.a33_ * rhs.a32_ + lhs.a34_ * rhs.a42_;
    res.a33_ = lhs.a31_ * rhs.a13_ + lhs.a32_ * rhs.a23_ 
        + lhs.a33_ * rhs.a33_ + lhs.a34_ * rhs.a43_;
    res.a34_ = lhs.a31_ * rhs.a14_ + lhs.a32_ * rhs.a24_ 
        + lhs.a33_ * rhs.a34_ + lhs.a34_ * rhs.a44_;

    res.a41_ = lhs.a41_ * rhs.a11_ + lhs.a42_ * rhs.a21_ 
        + lhs.a43_ * rhs.a31_ + lhs.a44_ * rhs.a41_;
    res.a42_ = lhs.a41_ * rhs.a12_ + lhs.a42_ * rhs.a22_ 
        + lhs.a43_ * rhs.a32_ + lhs.a44_ * rhs.a42_;
    res.a43_ = lhs.a41_ * rhs.a13_ + lhs.a42_ * rhs.a23_ 
        + lhs.a43_ * rhs.a33_ + lhs.a44_ * rhs.a43_;
    res.a44_ = lhs.a41_ * rhs.a14_ + lhs.a42_ * rhs.a24_ 
        + lhs.a43_ * rhs.a34_ + lhs.a44_ * rhs.a44_;

    return res;
}

template<typename real_t>
inline
gfx::matrix_4X4<real_t>
operator*(
    float k,
    const gfx::matrix_4X4<real_t>& mtx
    )
{
    matrix_4X4<real_t> result(mtx);
    result *= k;
    return result;
}

template<typename real_t>
inline
gfx::matrix_4X4<real_t>
operator*(
    const gfx::matrix_4X4<real_t>& mtx,
    real_t k
    )
{
    return k * mtx;
}

template<typename real_t>
inline
gfx::matrix_4X4<real_t>
operator/(
    const gfx::matrix_4X4<real_t>& mtx,
    real_t k
    )
{
    matrix_4X4<real_t> result(mtx);
    result /= k;
    return result; 
}

template<typename real_t>
inline
gfx::matrix_4X4<real_t>
transpose_of(
    const gfx::matrix_4X4<real_t>& mtx
    )
{
    return matrix_4X4<real_t>(
        mtx.a11_, mtx.a21_, mtx.a31_, mtx.a41_,
        mtx.a12_, mtx.a22_, mtx.a32_, mtx.a42_,
        mtx.a13_, mtx.a23_, mtx.a33_, mtx.a43_,
        mtx.a14_, mtx.a24_, mtx.a34_, mtx.a44_
        );
}

template<typename real_t>
inline
gfx::matrix_4X4<real_t>
gfx::inverse_of(
    const gfx::matrix_4X4<real_t>& mtx
    )
{
     matrix_4X4<real_t> result(mtx);
     return result.invert();
}

template<typename real_t>
gfx::matrix_4X4<real_t>
gfx::adjoint_of(
    const gfx::matrix_4X4<real_t>& mtx
    )
{
    float m1 = mtx.a33_ * mtx.a44_ - mtx.a34_ * mtx.a43_;
    float m2 = mtx.a32_ * mtx.a44_ - mtx.a34_ * mtx.a42_;
    float m3 = mtx.a32_ * mtx.a43_ - mtx.a33_ * mtx.a42_;
    float m4 = mtx.a13_ * mtx.a24_ - mtx.a14_ * mtx.a23_;
    float m5 = mtx.a12_ * mtx.a24_ - mtx.a14_ * mtx.a22_;
    float m6 = mtx.a12_ * mtx.a23_ - mtx.a13_ * mtx.a22_;
    float m7 = mtx.a31_ * mtx.a44_ - mtx.a34_ * mtx.a41_;
    float m8 = mtx.a31_ * mtx.a43_ - mtx.a33_ * mtx.a41_;
    float m9 = mtx.a23_ * mtx.a44_ - mtx.a24_ * mtx.a43_;
    float m10 = mtx.a21_ * mtx.a44_ - mtx.a24_ * mtx.a41_;
    float m11 = mtx.a21_ * mtx.a43_ - mtx.a23_ * mtx.a41_;
    float m12 = mtx.a11_ * mtx.a24_ - mtx.a14_ * mtx.a21_;
    float m13 = mtx.a11_ * mtx.a23_ - mtx.a13_ * mtx.a21_;
    float m14 = mtx.a31_ * mtx.a42_ - mtx.a32_ * mtx.a41_;
    float m15 = mtx.a11_ * mtx.a22_ - mtx.a12_ * mtx.a21_;
    float m16 = mtx.a31_ * mtx.a42_ - mtx.a32_ * mtx.a41_;

    return matrix_4X4<real_t>(
        mtx.a22_ * m1 - mtx.a23_ * m2 + mtx.a24_ * m3,
        -mtx.a12_ * m1 + mtx.a13_ * m2 - mtx.a14_ * m3,
        mtx.a42_ * m4 - mtx.a43_ * m5 + mtx.a44_ * m6,
        -mtx.a32_ * m4 + mtx.a33_ * m5 - mtx.a34_ * m6,

        -mtx.a21_ * m1 + mtx.a23_ * m7 - mtx.a24_ * m8,
        mtx.a11_ * m1 - mtx.a13_ * m7 + mtx.a14_ * m8,
        -mtx.a11_ * m9 + mtx.a13_ * m10 - mtx.a14_ * m11,
        mtx.a31_ * m4 - mtx.a33_ * m12 + mtx.a34_ * m13,

        mtx.a21_ * m2 - mtx.a22_ * m7  + mtx.a24_ * m16,
        -mtx.a11_ * m2 + mtx.a12_ * m7 - mtx.a14_ * m16,
        mtx.a41_ * m5 - mtx.a42_ * m12 + mtx.a44_ * m15,
        -mtx.a31_ * m5 + mtx.a32_ * m12 - mtx.a34_ * m15,

        -mtx.a21_ * m3 + mtx.a22_ * m8 - mtx.a23_ * m14,
        mtx.a11_ * m3 - mtx.a12_ * m8 + mtx.a13_ * m14,
        -mtx.a41_ * m6 + mtx.a42_ * m13 - mtx.a43_ * m15,
        mtx.a31_ * m6 - mtx.a32_ * m13 + mtx.a33_ * m15
        );
}

template<typename real_t>
gfx::matrix_4X4<real_t>&
gfx::matrix_4X4<real_t>::make_perspective_projection_lh( 
    real_t aspect_ratio, 
    real_t vertical_fov, 
    real_t near_plane, 
    real_t far_plane, 
    real_t depth_min, 
    real_t depth_max 
    )
{
    const real_t kDistToProjWnd = real_t(1) / std::tan(vertical_fov / 2);
    const real_t kNDmin = near_plane * depth_min;
    const real_t kFDMax = far_plane * depth_max;
    const real_t kNFDiff = far_plane - near_plane;
    const real_t kFDMin = far_plane * depth_min;

    a11_ = kDistToProjWnd / aspect_ratio;
    a12_ = a13_ = a14_ = real_t(0);

    a21_ = a23_ = a24_ = real_t(0);
    a22_ = kDistToProjWnd;

    a31_ = a32_ = real_t(0);
    a33_ = (kNDmin + kFDMax) / kNFDiff;
    a34_ = near_plane * ((kFDMin - 2 * kNDmin - kFDMax) / kNFDiff);

    a41_ = a42_ = a44_ = real_t(0);
    a43_ = real_t(1);

    return *this;
}

template<typename real_t>
gfx::matrix_4X4<real_t>&
gfx::matrix_4X4<real_t>::make_perspective_projection_rh( 
    real_t aspect_ratio, 
    real_t vertical_fov, 
    real_t near_plane, 
    real_t far_plane, 
    real_t depth_min, 
    real_t depth_max 
    )
{
    const real_t kDistToProjWnd = 1.0f / (std::tan(vertical_fov / 2));
    const real_t kFDMax = far_plane * depth_max;
    const real_t kFDMin = far_plane * depth_min;
    const real_t kNDMin = near_plane * depth_min;
    const real_t kNFDiff = near_plane - far_plane;

    a11_ = kDistToProjWnd / aspect_ratio;
    a12_ = a13_ = a14_ = real_t(0);

    a22_ = kDistToProjWnd;
    a21_ = a23_ = a24_ = real_t(0);

    a31_ = a32_ = real_t(0);
    a33_ = (kFDMax - kNDMin) / kNFDiff;
    a34_ = near_plane * ((kFDMax - kFDMin) / kNFDiff);

    a41_ = a42_ = real_t(0);
    a43_ = real_t(-1);
    a44_ = real_t(0);

    return *this;
}

template<typename real_t>
gfx::matrix_4X4<real_t>&
gfx::matrix_4X4<real_t>::make_perspective_projection_oblique_lh( 
    real_t near_plane, 
    real_t far_plane, 
    real_t wtop, 
    real_t wbottom, 
    real_t wleft, 
    real_t wright, 
    real_t depth_min, 
    real_t depth_max 
    )
{
    const real_t kRLDiff = wright - wleft;
    const real_t kRLSum = wright + wleft;
    const real_t kTBDiff = wtop - wbottom;
    const real_t kTBSum = wtop + wbottom;
    const real_t kNDmin = near_plane * depth_min;
    const real_t kFDMax = far_plane * depth_max;
    const real_t kNFDiff = far_plane - near_plane;
    const real_t kFDMin = far_plane * depth_min;

    a11_ = (2 * near_plane) / kRLDiff;
    a12_ = real_t(0);
    a13_ = -(kRLSum / kRLDiff);
    a14_ = real_t(0);

    a21_ = real_t(0);
    a22_ = (2 * near_plane) / kTBDiff;
    a23_ = -(kTBSum / kTBDiff);
    a24_ = real_t(0);

    a31_ = a32_ = real_t(0);
    a33_ = (kNDmin + kFDMax) / kNFDiff;
    a34_ = near_plane * ((kFDMin - 2 * kNDmin - kFDMax) / kNFDiff);

    a41_ = a42_ = a44_ = real_t(0);
    a43_ = real_t(1);

    return *this;
}

template<typename real_t>
gfx::matrix_4X4<real_t>&
gfx::matrix_4X4<real_t>::make_ortho_parallel_projection_lh(
    real_t near_plane,
    real_t far_plane,
    real_t wtop,
    real_t wbottom,
    real_t wleft,
    real_t wright,
    real_t depth_min,
    real_t depth_max
    )
{
    const real_t kRLDiff = wright - wleft;
    const real_t kRLSum = wright + wleft;
    const real_t kTBDiff = wtop - wbottom;
    const real_t kTBSum = wtop + wbottom;
    const real_t kNDMax = near_plane * depth_max;
    const real_t kFDMin = far_plane * depth_min;
    const real_t kNFDiff = far_plane - near_plane;
    const real_t kZScaleFactor = (depth_min - depth_max) / (kNFDiff);

    a11_ = real_t(2) / kRLDiff;
    a12_ = a13_ = real_t(0);
    a14_ = -(kRLSum / kRLDiff);

    a21_ = a23_ = real_t(0);
    a22_ = real_t(2) / kTBDiff;
    a24_ = -(kTBSum / kTBDiff);

    a31_ = a32_ = real_t(0);
    a33_ = kZScaleFactor;
    a34_ = (-kNDMax + kFDMin) / kNFDiff;

    a41_ = a42_ = a43_ = real_t(1);
    a44_ = real_t(1);

    return *this;
}

template<typename real_t>
inline
gfx::matrix_4X4<real_t>&
gfx::matrix_4X4<real_t>::make_zero() {
    *this = matrix4X4<real_t>::null;
    return *this;
}

template<typename real_t>
inline
gfx::matrix_4X4<real_t>&
gfx::matrix_4X4<real_t>::make_identity() {
    *this = matrix4X4<real_t>::identity;
    return *this;
}