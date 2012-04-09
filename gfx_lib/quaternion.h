#pragma once

#include <cmath>
#include "details.h"
#include "math.h"
#include "matrix3X3.h"
#include "vector3.h"

namespace gfx {

template<typename real_t>
class quaternion {
public :
    enum { 
        is_floating_point = implementation_details::types_eq<real_t, float>::result 
                            || implementation_details::types_eq<real_t, double>::result 
                            || implementation_details::types_eq<real_t, long double>::result
    };

    /**
     * \typedef real_t element_type
     *
     * \brief   Defines an alias representing type of the element.
     */
    typedef real_t              element_type;
    typedef real_t&             reference;
    typedef const real_t&       const_reference;

    union {
        struct {
            real_t w_;
            real_t x_;
            real_t y_;
            real_t z_;
        };
        real_t elements_[4];
    };

    static const quaternion<real_t>     null;

    static const quaternion<real_t>     identity;

    quaternion();

    quaternion(real_t w, real_t x, real_t y, real_t z);

    quaternion(const real_t* init_data);

    quaternion(float angle, const gfx::vector3<real_t>& axis);

    quaternion(const gfx::vector3<real_t> vec, real_t w = real_t(0));

    quaternion(const gfx::vector3<real_t>& v1, const gfx::vector3<real_t>& v2);

    quaternion<real_t>& make_zero();

    quaternion<real_t>& make_identity();

    quaternion<real_t>& make_from_axis_angle(
        float angle, 
        const gfx::vector3<real_t>& axis
        );

    quaternion<real_t>& make_from_vectors(
        const gfx::vector3<real_t>& v1, 
        const gfx::vector3<real_t>& v2
        );

    quaternion<real_t>& make_from_matrix(
        const gfx::matrix_3X3<real_t>& mtx
        );

    quaternion<real_t>& operator+=(const quaternion<real_t>& rhs);

    quaternion<real_t>& operator-=(const quaternion<real_t>& rhs);

    quaternion<real_t>& operator*=(real_t scalar);

    quaternion<real_t>& operator/=(real_t scalar);

    quaternion<real_t>& normalize();

    quaternion<real_t>& conjugate();

    quaternion<real_t>& invert();

    quaternion<real_t>& to_rotation_matrix(gfx::matrix_3X3<real_t>* mtx) const;

    quaternion<real_t>& to_axis_angle(
        gfx::vector3<real_t>* axis, 
        real_t* angle
        ) const;

    gfx::vector3<real_t> rotate_vector(const gfx::vector3<real_t>& vec);

    inline real_t length_squared() const;

    inline real_t magnitude() const;

    bool is_zero() const;

    bool is_unit() const;

    bool is_identity() const;
};

template<typename real_t>
const quaternion<real_t>
quaternion<real_t>::null(real_t(0), real_t(0), real_t(0), real_t(0));

template<typename real_t>
const quaternion<real_t>
quaternion<real_t>::identity(real_t(1), real_t(0), real_t(0), real_t(0));

template<typename real_t>
inline
real_t
dot_product(
    const gfx::quaternion<real_t>& lhs, 
    const gfx::quaternion<real_t>& rhs
    );

template<typename real_t>
inline
gfx::quaternion<real_t>
inverse_of(
    const gfx::quaternion<real_t> quat
    );

template<typename real_t>
inline
gfx::quaternion<real_t>
normal_of(
    const gfx::quaternion<real_t> quat
    );

template<typename real_t>
inline
gfx::quaternion<real_t>
conjugate_of(
    const gfx::quaternion<real_t> quat
    );

template<typename real_t>
bool
operator==(
    const gfx::quaternion<real_t>& lhs, 
    const gfx::quaternion<real_t>& rhs
    );

template<typename real_t>
inline
bool
operator!=(
    const gfx::quaternion<real_t>& lhs, 
    const gfx::quaternion<real_t>& rhs
    );

template<typename real_t>
inline
gfx::quaternion<real_t>
operator+(
    const gfx::quaternion<real_t>& lhs, 
    const gfx::quaternion<real_t>& rhs
    );

template<typename real_t>
inline
gfx::quaternion<real_t>
operator-(
    const gfx::quaternion<real_t>& lhs, 
    const gfx::quaternion<real_t>& rhs
    );

template<typename real_t>
inline
gfx::quaternion<real_t>
operator-(
    const gfx::quaternion<real_t>& quat
    );

template<typename real_t>
gfx::quaternion<real_t>
operator*(
    const gfx::quaternion<real_t>& lhs, 
    const gfx::quaternion<real_t>& rhs
    );

template<typename real_t>
gfx::quaternion<real_t>
operator*(
    const gfx::quaternion<real_t>& lhs, 
    real_t scalar
    );

template<typename real_t>
inline
gfx::quaternion<real_t>
operator*(
    real_t scalar,
    const gfx::quaternion<real_t>& rhs
    );

template<typename real_t>
gfx::quaternion<real_t>
operator/(
    const gfx::quaternion<real_t>& lhs,
    real_t scalar    
    );

typedef quaternion<float>       quaternionF;

typedef quaternion<double>      quaternionD;

} // namespace gfx

#include "quaternion.inl"