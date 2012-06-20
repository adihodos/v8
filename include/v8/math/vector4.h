//
// Copyright (c) 2011, 2012, Adrian Hodos
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of the author nor the
//       names of its contributors may be used to endorse or promote products
//       derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE AUTHOR OR THE CONTRIBUTORS BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#pragma once

#include <memory.h>
#include <cassert>
#include <cmath>
#include <vector>
#include "v8/base/fundamental_types.h"
#include "v8/base/compiler_warnings.h"
#include "v8/math/math_utils.h"
#include "v8/math/vector3.h"

namespace v8 { namespace math {

/**
 * \class   vector4
 *
 * \brief   A four component tuple in the form (x, y, z, w),
 * 			that is used to represent an affine vector/affine point/ 
 * 			homogeneous point.
 * 			It is up to the user of the class to make the distinction 
 * 			in code. An affine vector has the w component set to 0. An affine
 * 			point has the w component set to 1. And finally, a homogeneous point
 * 			has a w component with a value different than 1.
 */
template<typename real_t>
class vector4 {
private :
    enum { 
        is_floating_point = base::is_floating_point_type<real_t>::Yes
    };
public :
    /**
      * \typedef real_t element_type
      *
      * \brief   Defines an alias representing type of the element.
      */
    typedef real_t          element_type;
    typedef real_t&         reference;
    typedef const real_t&   const_reference;
    typedef vector4<real_t> vector4_t;

    MSVC_DISABLE_WARNING_BLOCK_BEGIN(4201)
    union {
        struct {
            real_t x_;
            real_t y_;
            real_t z_;
            real_t w_;
        };
        real_t elements_[4];   ///< Array like access to the vector's elements */
    };
    MSVC_DISABLE_WARNING_BLOCK_END(4201)

    static const vector4_t	zero;

    static const vector4_t	unit_x;

    static const vector4_t	unit_y;

    static const vector4_t	unit_z;

    static const vector4_t  unit_w;

   /**
    * \fn    vector4::vector4()
    *
    * \brief Default constructor. Leaves elements uninitialized.
    *
    */
    vector4() {}

    /**
     * \fn  vector4::vector4(float x, float y, float z, float w)
     *
     * \brief   Constructs a vector4 with the specified values.
     */
    vector4(real_t x, real_t y, real_t z, real_t w) 
        : x_(x), y_(y), z_(z), w_(w) {}

    /**
     * \fn  inline vector4::vector4(const real_t* inputs, size_t count);
     *
     * \brief   Constructs a vector4 from an array of existing values.
     * \param   inputs  Pointer to an array of elements.
     * \param   count   Number of elements in the array
     */
    inline 
    vector4(
        const real_t* inputs, 
        size_t count
        );

    template<typename Convertible_Type>
    vector4(const vector4<Convertible_Type>& other)
        : x_(other.x_), y_(other.y_), z_(other.z_), w_(other.w_) {}

    /**
     * \fn    static inline vector4 vector4::as_affine_point(const math::vector3& pt)
     *
     * \brief Constructs a vector4 representing an affine point (w = 1).
     */
    static 
    inline 
    vector4<real_t> 
    as_affine_point(
        const math::vector3<real_t>& pt
        );

    /**
     * \fn  static inline vector4<real_t> vector4::as_affine_vector( const math::vector3<real_t>& v );
     *
     * \brief   Constructs a vector4 representing an affine vector (w = 0).
     */
    static 
    inline 
    vector4<real_t> 
    as_affine_vector(
        const math::vector3<real_t>& v
        );

    /**
     * \fn  static inline vector4<real_t> vector4::as_homogeneous_point( const math::vector3<real_t>& pt,
     * real_t w );
     *
     * \brief   Constructs a vector4 representing a homogeneous point 
     * 			(w <> 0 and generally not 1).
     * \param   w   Value for the w component.
     */
    static
    inline
    vector4<real_t>
    as_homogeneous_point(
        const math::vector3<real_t>& pt,
        real_t w
        );

    template<typename Convertible_Type>
    inline 
    vector4<real_t>& 
    operator=(
        const vector4<Convertible_Type>& other
        ) {
        x_ = other.x_; y_ = other.y_; z_ = other.z_; w_ = other.w_;
        return *this;
    }

    /**
     * \fn  inline vector4<real_t>& vector4::operator+=( const vector4<real_t>& rhs );
     *
     * \brief   Addition assignment operator.
     * 			
     * \remarks This operation applies to affine vectors. It can be applied
     * 			to affine points, if the following holds :
     * 			Let P, P0, .. Pn be affine points in R4. Then
     * 			P can be written as P = P0 * a0 + P1 * a1 + ... + Pn * an,
     * 			only if (a0 + a1 + ... + an) = 1. So if you know that the right
     * 			hand operand is one of the members in the set 
     * 			{ Pi * ai | 0 <= i <= n and sum(i = 0..n) ai = 1 }
     * 			then it can be applied to vector4 objects representing affine
     * 			points.
     */
    template<typename Convertible_Type>
    inline
    vector4<real_t>& 
    operator+=(
        const vector4<Convertible_Type>& rhs
        );

    /**
     * \fn  inline vector4<real_t>& vector4::operator-=( const vector4<real_t>& rhs );
     *
     * \brief   Subtraction assignment operator.
     *
     * \remarks This operation is valid for vectors and affine points. 
     * 			Substracting two affine points result in an affine vector.
     */
    template<typename Convertible_Type>
    inline
    vector4<real_t>& 
    operator-=(
        const vector4<Convertible_Type>& rhs
        );

    /**
     * \fn  inline vector4<real_t>& vector4::operator*=( real_t k );
     *
     * \brief   Multiplication assignment operator.
     */
    template<typename Convertible_Type>
    inline
    vector4<real_t>& 
    operator*=(
        Convertible_Type k
        );

    /**
     * \fn  inline vector4<real_t>& vector4::operator/=( real_t k );
     *
     * \brief   Division assignment operator.
     */
    template<typename Convertible_Type>
    inline
    vector4<real_t>&
    operator/=(
        Convertible_Type k
        );

    /**
     * \fn  inline float vector4::sum_components_squared() const;
     *
     * \brief   Returns the sumation of the squares of the components.
     *
     * \remarks This function is valid only if the vector4 object represents
     * 			an affine vector. It return value is equal to
     * 			x ^ 2 + y ^ 2 + z ^ 2.
     */
    inline
    real_t
    sum_components_squared() const;

    /**
     * \fn  inline float vector4::magnitude() const;
     *
     * \brief   Gets the magnitude(length) of the vector.
     *
     * \remarks This function is valid only if the vector4 object represents
     * 			an affine vector.
     */
    inline
    real_t
    magnitude() const;

    /**
     * \fn  inline vector4<real_t>& vector4::normalize();
     *
     * \brief   Normalizes the vector, that is v = v / ||v||.
     *
     * \remarks Only valid if the object represents a vector.
     */
    inline
    vector4<real_t>& 
    normalize();
};

template<typename real_t>
const vector4<real_t> vector4<real_t>::zero(real_t(0), real_t(0), real_t(0), real_t(0));

template<typename real_t>
const vector4<real_t> vector4<real_t>::unit_x(real_t(1), real_t(0), real_t(0), real_t(0));

template<typename real_t>
const vector4<real_t> vector4<real_t>::unit_y(real_t(0), real_t(1), real_t(0), real_t(0));

template<typename real_t>
const vector4<real_t> vector4<real_t>::unit_z(real_t(0), real_t(0), real_t(1), real_t(0));

template<typename real_t>
const vector4<real_t> vector4<real_t>::unit_w(real_t(0), real_t(0), real_t(0), real_t(1));

/**
 * \fn  template<typename real_t> inline bool operator==( const math::vector4<real_t>& lhs,
 * const math::vector4<real_t>& rhs );
 *
 * \brief   Equality operator.
 */
template<typename real_t>
inline
bool
operator==(
    const math::vector4<real_t>& lhs,
    const math::vector4<real_t>& rhs
    );

/**
 * \fn  template<typename real_t> inline bool operator!=( const math::vector4<real_t>& lhs,
 * const math::vector4<real_t>& rhs );
 *
 * \brief   Inequality operator.
 */
template<typename real_t>
inline
bool
operator!=(
    const math::vector4<real_t>& lhs,
    const math::vector4<real_t>& rhs
    );

/**
 * \fn  template<typename real_t> inline math::vector4<real_t> operator-( const math::vector4<real_t>& vec );
 *
 * \brief   Negation operator.
 */
template<typename real_t>
inline
math::vector4<real_t>
operator-(
    const math::vector4<real_t>& vec
    );

/**
 * \fn  template<typename real_t> inline math::vector4<real_t> operator+( const math::vector4<real_t>& lhs,
 * const math::vector4<real_t>& rhs )
 *
 * \brief   Addition operator.
 *
 * \see     vector4<real_t>::operator+=(const math::vector4<real_t>&)
 */
template<typename real_t>
inline
math::vector4<real_t>
operator+(
    const math::vector4<real_t>& lhs,
    const math::vector4<real_t>& rhs
    );

/**
 * \fn  template<typename real_t> inline math::vector4<real_t> operator-( const math::vector4<real_t>& lhs,
 * const math::vector4<real_t>& rhs );
 *
 * \brief   Subtraction operator.
 */
template<typename real_t>
inline
math::vector4<real_t>
operator-(
    const math::vector4<real_t>& lhs,
    const math::vector4<real_t>& rhs
    );

/**
 * \fn  template<typename real_t> inline math::vector4<real_t> operator*( float k,
 * const math::vector4<real_t>& vec, );
 *
 * \brief   Scalar multiplication operator.
 */
template<typename real_t, typename Convertible_Type>
inline
math::vector4<real_t>
operator*(
    Convertible_Type k,
    const math::vector4<real_t>& vec
    );

/**
 * \fn  template<typename real_t> inline math::vector4<real_t> operator*( const math::vector4<real_t>& vec,
 * float k );
 *
 * \brief   Scalar multiplication operator.
 */
template<typename real_t, typename Convertible_Type>
inline
math::vector4<real_t>
operator*(
    const math::vector4<real_t>& vec,
    Convertible_Type k
    );

/**
 * \fn  template<typename real_t> inline math::vector4<real_t> operator/( const math::vector4<real_t>& vec,
 * float k );
 *
 * \brief   Scalar division operator.
 */
template<typename real_t, typename Convertible_Type>
inline
math::vector4<real_t>
operator/(
    const math::vector4<real_t>& vec,
    Convertible_Type k
    );

/**
 * \fn  template<typename real_t> inline math::vector4<real_t> normalized_from( const math::vector4<real_t>& vec );
 *
 * \brief   Return the normalized form of a vector.
 */
template<typename real_t>
inline
math::vector4<real_t>
normalized_from(
    const math::vector4<real_t>& vec
    );
    
/**
 * \fn  template<typename real_t> inline real_t dot_product( const math::vector4<real_t>& lhs,
 * const math::vector4<real_t>& rhs );
 *
 * \brief   Computes the dot product of two vectors.
 */
template<typename real_t>
inline
real_t
dot_product(
    const math::vector4<real_t>& lhs, 
    const math::vector4<real_t>& rhs
    );

/**
 * \typedef vector4<float> vector4F
 *
 * \brief   Defines an alias representing a vector4 having simple precision
 * 			floating point components.
 */
typedef vector4<float>      vector4F;

/**
 * \typedef vector4<double> vector4D
 *
 * \brief   Defines an alias representing a vector4 having double precision
 * 			floating point components.
 */
typedef vector4<double>     vector4D;

} // namespace math
} // namespace v8

#include "vector4.inl"
