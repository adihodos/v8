//
// static
template<typename real_t>
inline 
v8::math::vector4<real_t> 
v8::math::vector4<real_t>::as_affine_point(
    const v8::math::vector3<real_t>& pt
    ) 
{
    return vector4_t(pt.x_, pt.y_, pt.z_, real_t(1));
}

//
// static
template<typename real_t>
inline 
v8::math::vector4<real_t> 
v8::math::vector4<real_t>::as_affine_vector(
    const v8::math::vector3<real_t>& v
    )
{
    return vector4_t(v.x_, v.y_, v.z_, real_t(0));
}

//
// static
template<typename real_t>
inline
v8::math::vector4<real_t>
v8::math::vector4<real_t>::as_homogeneous_point(
    const v8::math::vector3<real_t>& pt,
    real_t w
    )
{
    return vector4_t(pt.x_, pt.y_, pt.z_, w);
}

template<typename real_t>
v8::math::vector4<real_t>::vector4(
    const real_t* inputs, 
    size_t count
    )
{
    std::memcpy(elements_, inputs, 
                std::min(_countof(elements_), count) * sizeof(real_t));
}

template<typename real_t>
inline 
v8::math::vector4<real_t>& 
v8::math::vector4<real_t>::operator+=(
    const vector4<real_t>& rhs
    ) 
{
    x_ += rhs.x_; y_ += rhs.y_; z_ += rhs.z_;
    return *this;
}

template<typename real_t>
inline
v8::math::vector4<real_t>& 
v8::math::vector4<real_t>::operator-=(
    const vector4<real_t>& rhs
    )
{
    x_ -= rhs.x_; y_ -= rhs.y_; z_ -= rhs.z_;
    return *this;
}

template<typename real_t>
inline
v8::math::vector4<real_t>&
v8::math::vector4<real_t>::operator*=(
    real_t k
    )
{
    x_ *= k; y_ *= k; z_ *= k;
    return *this;
}

template<typename real_t>
inline
v8::math::vector4<real_t>&
v8::math::vector4<real_t>::operator/=(
    real_t k
    )
{
    using namespace internals;
    const real_t dividend = transform_dividend_for_division<
        real_t, 
        is_floating_point
    >::transform(k);

    divide_helper<
        real_t,
        is_floating_point
    > div_helper;

    x_ = div_helper(x_, dividend);
    y_ = div_helper(y_, dividend);
    z_ = div_helper(z_, dividend);
    return *this;
}

template<typename real_t>
inline
real_t
v8::math::vector4<real_t>::sum_components_squared() const {
    return x_ * x_ + y_ * y_ + z_ * z_;
}

template<typename real_t>
inline
real_t
v8::math::vector4<real_t>::magnitude() const {
    return std::sqrt(sum_components_squared());
}

template<typename real_t>
inline
v8::math::vector4<real_t>&
v8::math::vector4<real_t>::normalize() {
    real_t mag(magnitude());
    if (math::is_zero(mag)) {
        x_ = y_ = z_ = real_t(0);
    } else {
        *this /= mag;
    }
    return *this;
}

template<typename real_t>
inline
bool
v8::math::operator==(
    const v8::math::vector4<real_t>& lhs,
    const v8::math::vector4<real_t>& rhs
    ) 
{    
    return  math::operands_eq(lhs.x_, rhs.x_) &&
            math::operands_eq(lhs.y_, rhs.y_) &&
            math::operands_eq(lhs.z_, rhs.z_) &&
            math::operands_eq(lhs.w_, rhs.w_);
}

template<typename real_t>
inline
bool
v8::math::operator!=(
    const v8::math::vector4<real_t>& lhs,
    const v8::math::vector4<real_t>& rhs
    )
{
    return !(lhs == rhs);
}

template<typename real_t>
inline
v8::math::vector4<real_t>
v8::math::operator-(
    const v8::math::vector4<real_t>& vec
    )
{
    return vector4_t(-vec.x_, -vec.y_, -vec.z_);
}

template<typename real_t>
inline
v8::math::vector4<real_t>
v8::math::operator+(
    const v8::math::vector4<real_t>& lhs,
    const v8::math::vector4<real_t>& rhs
    ) 
{
    return vector4_t(lhs.x_ + rhs.x_, lhs.y_ + rhs.y_, lhs.z_ + rhs.z_);
}

template<typename real_t>
inline
v8::math::vector4<real_t>
v8::math::operator-(
    const v8::math::vector4<real_t>& lhs,
    const v8::math::vector4<real_t>& rhs
    )
{
    return vector4_t(lhs.x_ - rhs.x_, 
                     lhs.y_ - rhs.y_, 
                     lhs.z_ - rhs.z_, 
                     real_t(0));
}

template<typename real_t>
inline
v8::math::vector4<real_t>
v8::math::operator*(
    real_t k,
    const v8::math::vector4<real_t>& vec
    )
{
    v8::math::vector4<real_t> res(vec);
    res *= k;
    return res;
}

template<typename real_t>
inline
v8::math::vector4<real_t>
v8::math::operator*(
    const v8::math::vector4<real_t>& vec,
    real_t k
    )
{
    return k * vec;
}

template<typename real_t>
inline
v8::math::vector4<real_t>
v8::math::operator/(
    const v8::math::vector4<real_t>& vec,
    real_t k
    )
{
    v8::math::vector4<real_t> res(vec);
    res /= k;
    return res;
}

template<typename real_t>
inline
v8::math::vector4<real_t>
v8::math::normalized_from(
    const v8::math::vector4<real_t>& vec
    ) 
{
    v8::math::vector4<real_t> res(vec);
    res.normalize();
    return res;
}

template<typename real_t>
inline
real_t
v8::math::dot_product(
    const v8::math::vector4<real_t>& lhs, 
    const v8::math::vector4<real_t>& rhs
    )
{
    return lhs.x_ * rhs.x_ + lhs.y_ * rhs.y_ + lhs.z_ * rhs.z_ + lhs.w_ * rhs.w_;
}
