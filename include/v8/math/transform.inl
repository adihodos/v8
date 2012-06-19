template<typename real_t>
v8::math::transform<real_t>::transform() {
    make_identity();
    reset_row4();
}

template<typename real_t>
v8::math::transform<real_t>::transform(const vector3<real_t>& translation)
    : translation_component_(translation),
      scale_factor_component_(1),
      matrix_component_(v8::math::matrix_3X3<real_t>::identity),
      cache_valid_(false),
      is_identity_(false),
      is_rotation_reflection_(true),
      is_scale_(false) {
    reset_row4();
}

template<typename real_t>
v8::math::transform<real_t>::transform(float scale_factor)
    : translation_component_(v8::math::vector3<real_t>::zero),
      scale_factor_component_(scale_factor),
      matrix_component_(v8::math::matrix_3X3<real_t>::identity),
      cache_valid_(false),
      is_identity_(false),
      is_rotation_reflection_(true),
      is_scale_(true) {
    reset_row4();
}

template<typename real_t>
v8::math::transform<real_t>::transform(
    const matrix_3X3<real_t>& matrix, 
    bool is_rotation_or_reflection
    )
    : translation_component_(v8::math::vector3<real_t>::zero),
      scale_factor_component_(1),
      matrix_component_(matrix),
      cache_valid_(false),
      is_identity_(false),
      is_rotation_reflection_(is_rotation_or_reflection),
      is_scale_(false) {
    reset_row4();
}

template<typename real_t>
v8::math::transform<real_t>::transform(
    const matrix_3X3<real_t>& matrix_component, 
    bool matrix_is_rotation_or_reflection,
    const vector3<real_t>& translation_component, 
    float scale_component
    )
    : translation_component_(translation_component),
      scale_factor_component_(scale_component),
      matrix_component_(matrix_component),
      cache_valid_(false),
      is_identity_(false),
      is_rotation_reflection_(matrix_is_rotation_or_reflection),
      is_scale_(true) {
    reset_row4();
}

template<typename real_t>
void v8::math::transform<real_t>::reset_row4() const {
    transform_matrix_.set_row(4, real_t(0), real_t(0), real_t(0), real_t(1));
}

template<typename real_t>
inline bool v8::math::transform<real_t>::is_identity() const {
    return is_identity_;
}

template<typename real_t>
inline bool v8::math::transform<real_t>::is_rotation_or_reflection() const {
    return is_rotation_reflection_;
}

template<typename real_t>
inline bool v8::math::transform<real_t>::is_scaling() const {
    return is_scale_;
}

template<typename real_t>
v8::math::transform<real_t>& v8::math::transform<real_t>::make_identity() {
    scale_factor_component_ = real_t(1);
    is_scale_ = false;
    matrix_component_.make_identity();
    is_rotation_reflection_ = true;
    translation_component_ = v8::math::vector3<real_t>::zero;
    cache_valid_ = false;
    is_identity_ = true;
    return *this;
}

template<typename real_t>
v8::math::transform<real_t>& v8::math::transform<real_t>::make_non_scaling() {
    scale_factor_component_ = real_t(1);
    is_scale_ = false;
    cache_valid_ = false;
    return *this;
}

template<typename real_t>
void v8::math::transform<real_t>::compute_transform_matrix() const {
    if (is_scale_) 
        transform_matrix_.set_upper3x3(matrix_component_ * scale_factor_component_);
    else
        transform_matrix_.set_upper3x3(matrix_component_);    

    transform_matrix_.a14_ = translation_component_.x_;
    transform_matrix_.a24_ = translation_component_.y_;
    transform_matrix_.a34_ = translation_component_.z_;
    cache_valid_ = true;
}

template<typename real_t>
void v8::math::transform<real_t>::set_scale_component(float scale_factor) {
    scale_factor_component_ = scale_factor;
    is_scale_ = true;
    is_identity_ = false;
    cache_valid_ = false;
}

template<typename real_t>
inline real_t v8::math::transform<real_t>::get_scale_component() const {
    return scale_factor_component_;
}

template<typename real_t>
void v8::math::transform<real_t>::set_matrix_component_rotate_reflect(
    const v8::math::matrix_3X3<real_t>& rotate_reflect
    )
{
    matrix_component_ = rotate_reflect;
    is_rotation_reflection_ = true;
    is_identity_ = false;
    cache_valid_ = false;
}

template<typename real_t>
void v8::math::transform<real_t>::set_matrix_component(
    const v8::math::matrix_3X3<real_t>& matrix
    )
{
    matrix_component_ = matrix;
    is_rotation_reflection_ = false;
    is_identity_ = false;
    cache_valid_ = false;
}

template<typename real_t>
inline const v8::math::matrix_3X3<real_t>& 
v8::math::transform<real_t>::get_matrix_component() const {
    return matrix_component_;
}

template<typename real_t>
void v8::math::transform<real_t>::set_translation_component(
    const v8::math::vector3<real_t>& translation
    )
{
    translation_component_ = translation;
    is_identity_ = false;
    cache_valid_ = false;
}

template<typename real_t>
inline const v8::math::vector3<real_t>& 
v8::math::transform<real_t>::get_translation_component() const {
    return translation_component_;
}

template<typename real_t>
const v8::math::matrix_4X4<real_t>& 
v8::math::transform<real_t>::get_transform_matrix() const {
    if (!cache_valid_)
        compute_transform_matrix();
    return transform_matrix_;
}

template<typename real_t>
void v8::math::transform<real_t>::compute_inverse(v8::math::matrix_4X4<real_t>* inv) const {
    if (is_identity_) {
        inv->make_identity();
        return;
    }

    v8::math::matrix_3X3<real_t> mtx_inv;
    if (is_rotation_reflection_) {
        matrix_component_.get_transpose(&mtx_inv);
    } else {
        matrix_component_.get_inverse(&mtx_inv);
    }

    if (is_scale_) {
        mtx_inv *= (real_t(1) / scale_factor_component_);
    }

    inv->set_upper3x3(mtx_inv);

    const v8::math::vector3<real_t>& vt = translation_component_;
    inv->a14_ = -(mtx_inv.a11_ * vt.x_ + mtx_inv.a12_ * vt.y_ + mtx_inv.a13_ * vt.z_);
    inv->a24_ = -(mtx_inv.a21_ * vt.x_ + mtx_inv.a22_ * vt.y_ + mtx_inv.a23_ * vt.z_);
    inv->a34_ = -(mtx_inv.a31_ * vt.x_ + mtx_inv.a32_ * vt.y_ + mtx_inv.a33_ * vt.z_);
    inv->set_row(4, real_t(0), real_t(0), real_t(0), real_t(1));
}

template<typename real_t>
v8::math::transform<real_t>& v8::math::transform<real_t>::invert() {
    if (is_identity_)
        return *this;

    if (is_rotation_reflection_)
        matrix_component_.transpose();
    else
        matrix_component_.invert();

    translation_component_ = -(matrix_component_ * translation_component_);

    if (is_scale_) {
        scale_factor_component_ = real_t(1) / scale_factor_component_;
        translation_component_ *= scale_factor_component_;
    }
    return *this;
}

template<typename real_t>
v8::math::transform<real_t>&
v8::math::transform<real_t>::operator*=(const v8::math::transform<real_t>& rhs) {
    if (rhs.is_identity())
        return *this;

    if (is_identity_) {
        *this = rhs;
        return *this;
    }

    cache_valid_ = false;
    matrix_component_ = matrix_component_ * rhs.get_matrix_component();
    is_rotation_reflection_ = is_rotation_reflection_ 
                              & rhs.is_rotation_or_reflection();

    rotation_component_ = rhs.get_matrix_component() * rotation_component_;
    if (rhs.is_scaling()) {
        is_scale_ = true;
        scale_factor_component_ *= rhs.get_scale_component();
        rotation_component_ *= rhs.get_scale_component();
    }

    rotation_component_ += rhs.get_translation_component();
    return *this;
}

template<typename real_t>
inline
v8::math::transform<real_t>
v8::math::operator*(
    const v8::math::transform<real_t>& lhs, 
    const v8::math::transform<real_t>& rhs
    )
{
    transform<real_t>& result(lhs);
    result *= rhs;
    return result;
}