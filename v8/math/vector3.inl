template<typename real_t>
const v8::math::vector3<real_t> 
v8::math::vector3<real_t>::zero(real_t(0), real_t(0), real_t(0));

template<typename real_t>
const v8::math::vector3<real_t> 
v8::math::vector3<real_t>::unit_x(real_t(1), real_t(0), real_t(0));

template<typename real_t>
const v8::math::vector3<real_t> 
v8::math::vector3<real_t>::unit_y(real_t(0), real_t(1), real_t(0));

template<typename real_t>
const v8::math::vector3<real_t> 
v8::math::vector3<real_t>::unit_z(real_t(0), real_t(0), real_t(1));

template<typename real_t>
v8::math::vector3<real_t>::vector3(const real_t* input, size_t count) {
    std::memcpy(elements_, input, std::min(_countof(elements_), 
                count * sizeof(real_t)));
}

template<typename real_t>
inline
v8::math::vector3<real_t>&
v8::math::vector3<real_t>::operator+=(
    const vector3<real_t>& rhs
    ) 
{
    x_ += rhs.x_;
    y_ += rhs.y_;
    z_ += rhs.z_;
    return *this;
}

template<typename real_t>
inline
v8::math::vector3<real_t>&
v8::math::vector3<real_t>::operator-=(
    const vector3<real_t>& rhs
    ) 
{
    x_ -= rhs.x_;
    y_ -= rhs.y_;
    z_ -= rhs.z_;
    return *this;
}

template<typename real_t>
inline
v8::math::vector3<real_t>&
v8::math::vector3<real_t>::operator *=(real_t k) {
    x_ *= k;
    y_ *= k;
    z_ *= k;
    return *this;
}

template<typename real_t>
inline
v8::math::vector3<real_t>&
v8::math::vector3<real_t>::operator /=(real_t k) {
    using namespace v8::math::internals;
    const real_t dividend = transform_dividend_for_division<
        real_t, is_floating_point
    >::transform(k);

    typedef divide_helper<real_t, is_floating_point> div_t;

    x_ = div_t::divide(x_, dividend);
    y_ = div_t::divide(y_, dividend);
    z_ = div_t::divide(z_, dividend);
    return *this;
}

template<typename real_t>
inline
real_t
v8::math::vector3<real_t>::sum_components_squared() const {
    return x_ * x_ + y_ * y_ + z_ * z_;
}

template<typename real_t>
inline
real_t
v8::math::vector3<real_t>::magnitude() const {
    return std::sqrt(sum_components_squared());
}

template<typename real_t>
inline
v8::math::vector3<real_t>&
v8::math::vector3<real_t>::normalize() {
    float magn(magnitude());
    if (math::is_zero(magn)) {
        x_ = y_ = z_ = real_t(0);
    } else {
        *this /= magn;
    }
    return *this;
}

template<typename real_t>
inline
bool
v8::math::operator==(
    const v8::math::vector3<real_t>& lhs, 
    const v8::math::vector3<real_t>& rhs
    ) {
    return  math::operands_eq(lhs.x_, rhs.x_) &&
            math::operands_eq(lhs.y_, rhs.y_) &&
            math::operands_eq(lhs.z_, rhs.z_);
}

template<typename real_t>
inline
bool
v8::math::operator!=(const v8::math::vector3<real_t>& lhs, const v8::math::vector3<real_t>& rhs) {
    return !(lhs == rhs);
}

template<typename real_t>
inline
v8::math::vector3<real_t>
v8::math::operator+(const v8::math::vector3<real_t>& lhs, const v8::math::vector3<real_t>& rhs) {
    vector3<real_t> res(lhs);
    res += rhs;
    return res;
}

template<typename real_t>
inline
v8::math::vector3<real_t>
v8::math::operator-(const v8::math::vector3<real_t>& lhs, const vector3<real_t>& rhs) {
    vector3<real_t> res(lhs);
    res -= rhs;
    return res;
}

template<typename real_t>
inline
v8::math::vector3<real_t>
v8::math::operator-(const v8::math::vector3<real_t>& vec) {
    return vector3<real_t>(-vec.x_, -vec.y_, -vec.z_);
}

template<typename real_t>
inline
v8::math::vector3<real_t>
v8::math::operator*(const v8::math::vector3<real_t>& vec, real_t k) {
    vector3<real_t> result = vec;
    result *= k;
    return result;
}

template<typename real_t>
inline
v8::math::vector3<real_t>
v8::math::operator*(real_t k, const v8::math::vector3<real_t>& vec) {
    return vec * k;
}

template<typename real_t>
inline
v8::math::vector3<real_t>
v8::math::operator/(const v8::math::vector3<real_t>& vec, real_t k) {
    v8::math::vector3<real_t> result(vec);
    result /= k;
    return result;
}

template<typename real_t>
inline
real_t
v8::math::dot_product(const v8::math::vector3<real_t>& lhs, const v8::math::vector3<real_t>& rhs) {
    return lhs.x_ * rhs.x_ + lhs.y_ * rhs.y_ + lhs.z_ * rhs.z_;
}

template<typename real_t>
inline
v8::math::vector3<real_t>
v8::math::cross_product(const v8::math::vector3<real_t>& lhs, const v8::math::vector3<real_t>& rhs) {
    return vector3<real_t>(
        lhs.y_ * rhs.z_ - lhs.z_ * rhs.y_,
        lhs.z_ * rhs.x_ - lhs.x_ * rhs.z_,
        lhs.x_ * rhs.y_ - lhs.y_ * rhs.x_
        );
}

template<typename real_t>
inline
bool
v8::math::ortho_test(
    const v8::math::vector3<real_t>& lhs, 
    const v8::math::vector3<real_t>& rhs
    )
{
    return math::is_zero(dot_product(lhs, rhs));
}

template<typename real_t>
inline
float
v8::math::angle_of(
    const v8::math::vector3<real_t>& lhs, 
    const v8::math::vector3<real_t>& rhs
    )
{
    return std::acos(dot_product(lhs, rhs) / (lhs.magnitude() * rhs.magnitude()));
}

template<typename real_t>
inline
v8::math::vector3<real_t>
v8::math::project_vector_on_vector(
    const v8::math::vector3<real_t>& lhs, 
    const v8::math::vector3<real_t>& rhs
    )
{
    return (dot_product(lhs, rhs) / rhs.sum_components_squared()) * rhs;
}

template<typename real_t>
inline
v8::math::vector3<real_t>
v8::math::project_vector_on_unit_vector(
    const v8::math::vector3<real_t>& lhs, 
    const v8::math::vector3<real_t>& rhs
    )
{
    dot_product(lhs, rhs) * rhs;
}

template<typename real_t>
inline
v8::math::vector3<real_t>
v8::math::normal_of(
    const v8::math::vector3<real_t>& vec
    )
{
    vector3<real_t> res(vec);
    res.normalize();
    return res;
}

template<typename real_t>
inline
real_t
v8::math::triple_scalar_product(
    const v8::math::vector3<real_t>& v1, 
    const v8::math::vector3<real_t>& v2, 
    const v8::math::vector3<real_t>& v3
    )
{
    return dot_product(v1, cross_product(v2, v3));
}

template<typename real_t>
inline
v8::math::vector3<real_t>
v8::math::triple_vector_product(
    const v8::math::vector3<real_t>& v1, 
    const v8::math::vector3<real_t>& v2, 
    const v8::math::vector3<real_t>& v3
    )
{
    return cross_product(v1, cross_product(v2, v3));
}

template<typename real_t>
inline
real_t
v8::math::distance_squared(
    const v8::math::vector3<real_t>& point1, 
    const v8::math::vector3<real_t>& point2
    )
{
    float px = point2.x_ - point1.x_;
    float py = point2.y_ - point1.y_;
    float pz = point2.z_ - point1.z_;
    return px * px + py * py + pz * pz;
}

template<typename real_t>
inline
real_t
v8::math::distance(
    const v8::math::vector3<real_t>& point1, 
    const v8::math::vector3<real_t>& point2
    )
{
    return std::sqrt(distance_squared(point1, point2));
}
