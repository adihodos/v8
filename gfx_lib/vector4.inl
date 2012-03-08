//
// static
template<typename real_t>
inline 
gfx::vector4<real_t> 
gfx::vector4<real_t>::as_affine_point(
    const gfx::vector3<real_t>& pt
    ) 
{
    return vector4_t(pt.x_, pt.y_, pt.z_, real_t(1));
}

//
// static
template<typename real_t>
inline 
gfx::vector4<real_t> 
gfx::vector4<real_t>::as_affine_vector(
    const gfx::vector3<real_t>& v
    )
{
    return vector4_t(v.x_, v.y_, v.z_, real_t(0));
}

//
// static
template<typename real_t>
inline
gfx::vector4<real_t>
gfx::vector4<real_t>::as_homogeneous_point(
    const gfx::vector3<real_t>& pt,
    real_t w
    )
{
    return vector4_t(pt.x_, pt.y_, pt.z_, w);
}

template<typename real_t>
inline 
gfx::vector4<real_t>::vector4(
    const real_t* inputs, 
    size_t count
    )
{
    std::memcpy(elements_, inputs, 
                std::min(_countof(elements_), count) * sizeof(real_t));
}

template<typename real_t>
inline 
gfx::vector4<real_t>& 
gfx::vector4<real_t>::operator+=(
    const vector4<real_t>& rhs
    ) 
{
    x_ += rhs.x_; y_ += rhs.y_; z_ += rhs.z_;
    return *this;
}

template<typename real_t>
inline
gfx::vector4<real_t>& 
gfx::vector4<real_t>::operator-=(
    const vector4<real_t>& rhs
    )
{
    x_ -= rhs.x_; y_ -= rhs.y_; z_ -= rhs.z_;
    return *this;
}

template<typename real_t>
inline
gfx::vector4<real_t>&
gfx::vector4<real_t>::operator*=(
    real_t k
    )
{
    x_ *= k; y_ *= k; z_ *= k;
    return *this;
}

template<typename real_t>
inline
gfx::vector4<real_t>&
gfx::vector4<real_t>::operator/=(
    real_t k
    )
{
    using namespace implementation_details;
    const real_t dividend = transform_dividend_for_division<
        real_t, 
        types_eq<real_t, float> ||
        types_eq<real_t, double> ||
        types_eq<real_t, long double>
    >::transform(k);

    divide_helper<
        real_t,
        types_eq<real_t, float> ||
        types_eq<real_t, double> ||
        types_eq<real_t, long double>
    > div_helper;

    x_ = div_helper(x_, dividend);
    y_ = div_helper(y_, dividend);
    z_ = div_helper(z_, dividend);
    return *this;
}

template<typename real_t>
inline
real_t
gfx::vector4<real_t>::sum_components_squared() const {
    return x_ * x_ + y_ * y_ + z_ * z_;
}

template<typename real_t>
inline
real_t
gfx::vector4<real_t>::magnitude() const {
    return std::sqrt(sum_components_squared());
}

template<typename real_t>
inline
gfx::vector4<real_t>&
gfx::vector4<real_t>::normalize() {
    real_t mag(magnitude());
    if (math::zero_test<real_t, is_floating_point>::result(mag)) {
        x_ = y_ = z_ = real_t(0);
    } else {
        *this /= mag;
    }
    return *this;
}

template<typename real_t>
inline
bool
gfx::operator==(
    const gfx::vector4<real_t>& lhs,
    const gfx::vector4<real_t>& rhs
    ) 
{    
    typedef math::zero_test<real_t, is_floating_point> zero_test_t;
    typedef math::op_eq<real_t, is_floating_point> op_eq_t;
    return op_eq_t::result(lhs.x_, rhs.x_) 
           && op_eq_t::result(lhs.y_, rhs.y_)
           && op_eq_t::result(lhs.z_, rhs.z_)
           && op_eq_t::result(lhs.w_, rhs.w_);
}

template<typename real_t>
inline
bool
gfx::operator!=(
    const gfx::vector4<real_t>& lhs,
    const gfx::vector4<real_t>& rhs
    )
{
    return !(lhs == rhs);
}

template<typename real_t>
inline
gfx::vector4<real_t>
gfx::operator-(
    const gfx::vector4<real_t>& vec
    )
{
    return vector4_t(-vec.x_, -vec.y_, -vec.z_);
}

template<typename real_t>
inline
gfx::vector4<real_t>
operator+(
    const gfx::vector4<real_t>& lhs,
    const gfx::vector4<real_t>& rhs
    ) 
{
    return vector4_t(lhs.x_ + rhs.x_, lhs.y_ + rhs.y_, lhs.z_ + rhs.z_);
}

template<typename real_t>
inline
gfx::vector4<real_t>
operator-(
    const gfx::vector4<real_t>& lhs,
    const gfx::vector4<real_t>& rhs
    )
{
    return vector4_t(lhs.x_ - rhs.x_, 
                     lhs.y_ - rhs.y_, 
                     lhs.z_ - rhs.z_, 
                     real_t(0));
}

template<typename real_t>
inline
gfx::vector4<real_t>
operator*(
    float k,
    const gfx::vector4<real_t>& vec,
    )
{
    vector4_t res(vec);
    res *= k;
    return res;
}

template<typename real_t>
inline
gfx::vector4<real_t>
operator*(
    const gfx::vector4<real_t>& vec,
    float k
    )
{
    return k * vec;
}

template<typename real_t>
inline
gfx::vector4<real_t>
operator/(
    const gfx::vector4<real_t>& vec,
    float k
    )
{
    vector4 res(vec);
    res /= k;
    return res;
}

template<typename real_t>
inline
gfx::vector4<real_t>
normalized_from(
    const gfx::vector4<real_t>& vec
    ) 
{
    vector4 res(vec);
    res.normalize();
    return res;
}