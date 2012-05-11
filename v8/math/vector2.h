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

#include <cassert>
#include <cmath>
#include "v8/math/math_utils.h"
#include "v8/base/fundamental_types.h"

namespace v8 { namespace math {

/**
 * \class   vector2
 *
 * \brief   A two component vector.
 */
template<typename real_t>
class vector2 {
public:
    enum { 
        is_floating_point = base::is_floating_point_type<real_t>::Yes
    };

    union {
        struct {
            real_t x_;  ///< Component along the x axis */
            real_t y_;  ///< Component along the y axis */
        };
        real_t elements_[2];	///< Used for array like access to elements */
    };

   /**
    * \typedef real_t element_type
    *
    * \brief   Defines an alias representing type of the element.
    */
    typedef real_t          element_type;
    typedef real_t&         reference;
    typedef const real_t&   const_reference;
    typedef vector2<real_t> vector2_t;

    static const vector2_t zero;	///< The zero vector (0, 0)*/

    static const vector2 unit_x;	///< The unit x vector (1, 0) */

    static const vector2 unit_y;	///< The unit y vector (0, 1) */

    /**
     * \fn  vector2::vector2()
     *
     * \brief   Default constructor. Leaves components uninitialized.
     */
    vector2() {}

    /**
     * \fn  vector2::vector2(real_t x, real_t y)
     *
     * \brief   Construct a vector 2 with two given values.
     */
    vector2(real_t x, real_t y) : x_(x), y_(y) {}

    /**
     * \fn  vector2::vector2(const real_t* input_val, size_t count)
     *
     * \brief   Construct from an array of values.
     * \param   input_val   Pointer to an array of values. Must not be null.
     * \param   count       Number of elements in the array.
     */
    vector2(const real_t* input_val, size_t count) {
        std::memcpy(elements_, input_val, 
            std::min(_countof(elements_), count) * sizeof(real_t));
    }

    /**
     * \fn  vector2<real_t>& vector2::operator+=(const vector2<real_t>& rhs)
     *
     * \brief   Addition assignment operator.
     */
    vector2<real_t>& operator+=(const vector2<real_t>& rhs) {
        x_ += rhs.x_;
        y_ += rhs.y_;
        return *this;
      }

    /**
     * \fn  vector2<real_t>& vector2::operator-=(const vector2<real_t>& rhs)
     *
     * \brief   Subtraction assignment operator.
     */
    vector2<real_t>& operator-=(const vector2<real_t>& rhs) {
        x_ -= rhs.x_;
        y_ -= rhs.y_;
        return *this;
    }

    /**
     * \fn  vector2<real_t>& vector2::operator*=(real_t k)
     *
     * \brief   Multiplication assignment operator.
     */
    vector2<real_t>& operator*=(real_t k) {
        x_ *= k;
        y_ *= k;
        return *this;
    }

    /**
     * \fn  vector2<real_t>& vector2::operator/=(real_t k)
     *
     * \brief   Division assignment operator.
     */
    vector2<real_t>& operator/=(real_t k) {
        using namespace internals;
        const real_t kDividend = transform_dividend_for_division<
            real_t, 
            is_floating_point
        >::transform(k);

        typedef divide_helper<real_t, is_floating_point> div_helper_t;

        x_ = div_helper_t::divide(x_, kDividend);
        y_ = div_helper_t::divide(y_, kDividend);
        return *this;
    }

    /**
     * \fn  real_t vector2::sum_components_squared() const
     *
     * \brief   Gets the sum of the components squared (x ^ 2 + y ^ 2).
     */
    real_t sum_components_squared() const {
        return x_ * x_ + y_ * y_;
    }

    /**
     * \fn  real_t vector2::magnitude() const
     *
     * \brief   Gets the magnitude (length) of the vector.
     */
    real_t magnitude() const {
        return std::sqrt(sum_components_squared());
    }

    /**
     * \fn  vector2<real_t>& vector2::normalize()
     *
     * \brief   Normalizes the vector (v = v / ||v||).
     */
    vector2<real_t>& normalize() {
        real_t magn(magnitude());
        if (math::is_zero(magn)) {
            x_ = y_ = real_t(0);
        } else {
            *this /= magn;
        }
        return *this;
    }
};

template<typename real_t>
const vector2<real_t> vector2<real_t>::zero(real_t(0), real_t(0));

template<typename real_t>
const vector2<real_t> vector2<real_t>::unit_x(real_t(1), real_t(0));

template<typename real_t>
const vector2<real_t> vector2<real_t>::unit_y(real_t(0), real_t(1));

/**
 * \fn  template<typename real_t> inline bool operator==(const vector2<real_t>& lhs,
 * const vector2<real_t>& rhs)
 *
 * \brief   Equality operator.
 */
template<typename real_t>
inline
bool
operator==(const vector2<real_t>& lhs, const vector2<real_t>& rhs) {
    return math::operands_eq(lhs.x_, rhs.x_) && 
           math::operands_eq(lhs.y_, rhs.y_);
}

/**
 * \fn  template<typename real_t> inline bool operator!=(const vector2<real_t>& lhs,
 * const vector2<real_t>& rhs)
 *
 * \brief   Inequality operator.
 */
template<typename real_t>
inline
bool
operator!=(const vector2<real_t>& lhs, const vector2<real_t>& rhs) {
    return !(lhs == rhs);
}

/**
 * \fn  template<typename real_t> inline vector2<real_t> operator+(const vector2<real_t>& lhs,
 * const vector2<real_t>& rhs)
 *
 * \brief   Addition operator.
 */
template<typename real_t>
inline
vector2<real_t>
operator+(const vector2<real_t>& lhs, const vector2<real_t>& rhs) {
    math::vector2<real_t> res(lhs);
    res += rhs;
    return res;
}

/**
 * \fn  template<typename real_t> inline vector2<real_t> operator-(const vector2<real_t>& lhs,
 * const vector2<real_t>& rhs)
 *
 * \brief   Subtraction operator.
 */
template<typename real_t>
inline
vector2<real_t>
operator-(const vector2<real_t>& lhs, const vector2<real_t>& rhs) {
    vector2<real_t> res(lhs);
    res -= rhs;
    return res;
}

/**
 * \fn  template<typename real_t> inline vector2<real_t> operator-(const vector2<real_t>& vec)
 *
 * \brief   Negation operator.
 */
template<typename real_t>
inline
vector2<real_t>
operator-(const vector2<real_t>& vec) {
    return vector2<real_t>(-vec.x_, -vec.y_);
}

/**
 * \fn  template<typename real_t> inline vector2<real_t> operator*(const vector2<real_t>& vec,
 * real_t k)
 *
 * \brief   Multiplication operator.
 */
template<typename real_t>
inline
vector2<real_t>
operator*(const vector2<real_t>& vec, real_t k) {
  vector2<real_t> result(vec);
  result *= k;
  return result;
}

/**
 * \fn  template<typename real_t> inline vector2<real_t> operator*(real_t k,
 * const vector2<real_t>& vec)
 *
 * \brief   Multiplication operator.
 */
template<typename real_t>
inline
vector2<real_t>
operator*(real_t k, const vector2<real_t>& vec) {
  return vec * k;
}

/**
 * \fn  template<typename real_t> inline vector2<real_t> operator/(const vector2<real_t>& vec,
 * real_t k)
 *
 * \brief   Division operator.
 */
template<typename real_t>
inline
vector2<real_t>
operator/(const vector2<real_t>& vec, real_t k) {
  vector2<real_t> result(vec);
  result /= k;
  return result;
}

/**
 * \fn  template<typename real_t> inline real_t dot_product(const vector2<real_t>& lhs,
 * const vector2<real_t>& rhs)
 *
 * \brief   Dot product.
 */
template<typename real_t>
inline
real_t
dot_product(const vector2<real_t>& lhs, const vector2<real_t>& rhs) {
  return lhs.x_ * rhs.x_ + lhs.y_ * rhs.y_;
}

/**
 * \fn  template<typename real_t> inline bool ortho_test(const vector2<real_t>& lhs,
 * const vector2<real_t>& rhs)
 *
 * \brief   Tests is two vectors are orthogonal (v1 dot v2 = 0).
 */
template<typename real_t>
inline
bool
ortho_test(const vector2<real_t>& lhs, const vector2<real_t>& rhs) {
    return math::is_zero(dot_product(lhs, rhs));
}

/**
 * \fn  template<typename real_t> inline real_t angle_of(const vector2<real_t>& lhs,
 * const vector2<real_t>& rhs)
 *
 * \brief   Returns the angle of two vectors.
 */
template<typename real_t>
inline
real_t
angle_of(const vector2<real_t>& lhs, const vector2<real_t>& rhs) {
    return std::acos(dot_product(lhs, rhs) / (lhs.magnitude() * rhs.magnitude()));
}

/**
 * \fn  template<typename real_t> inline vector2<real_t> project_vector_on_vector(const vector2<real_t>& lhs,
 * const vector2<real_t>& rhs)
 *
 * \brief   Projects a vector on another vector. Let P, Q be two vectors.
 * 			The projection of P on Q = [dot(P, Q) / ||Q|| ^ 2] * Q.
 */
template<typename real_t>
inline
vector2<real_t>
project_vector_on_vector(const vector2<real_t>& lhs, const vector2<real_t>& rhs) {
    return (dot_product(lhs, rhs) / rhs.sum_components_squared()) * rhs;
}

/**
 * \fn  template<typename real_t> inline vector2<real_t> normal_of(const vector2<real_t>& vec)
 *
 * \brief   Normal of the given vector.
 */
template<typename real_t>
inline
vector2<real_t>
normal_of(const vector2<real_t>& vec) {
    vector2<real_t> res(vec);
    res.normalize();
    return res;
}

/**
 * \fn  template<typename real_t> inline vector2<real_t> orthogonal_vector_from_vector(const vector2<real_t>& vec,
 * bool counter_clockwise = true)
 *
 * \brief   Return a vector that is orthogonal to the input vector.
 * 			
 * \param   vec                 The input vector.
 * \param   counter_clockwise   (optional) True if the vector is computed using
 * 								a counter clockwise rotation, false if using
 * 								a clockwise rotation.
 *
 */
template<typename real_t>
inline
vector2<real_t>
orthogonal_vector_from_vector(const vector2<real_t>& vec, bool counter_clockwise = true) {
    vector2<real_t> result;
    if (counter_clockwise) {
        result.x_ = -vec.y_;
        result.y_ = vec.x_;
    } else {
        result.x_ = vec.y_;
        result.y_ = -vec.x_;
    }
    return result;
}

/**
 * \fn  template<typename real_t> inline real_t distance_squared( const vector2<real_t>& point1,
 * const vector2<real_t>& point2 )
 *
 * \brief   Returns the square of the distance between two points.
 */
template<typename real_t>
inline
real_t
distance_squared(
    const vector2<real_t>& point1,
    const vector2<real_t>& point2
    )
{
    real_t px = point2.x_ - point1.x_;
    real_t py = point2.y_ - point1.y_;
    return px * px + py * py;
}

/**
 * \fn  template<typename real_t> inline real_t distance( const vector2<real_t>& point1,
 * const vector2<real_t>& point2 )
 *
 * \brief   Returns the distance between two points.
 */
template<typename real_t>
inline
real_t
distance(
    const vector2<real_t>& point1,
    const vector2<real_t>& point2
    )
{
    return std::sqrt(distance_squared(point1, point2));
}

/**
 * \typedef vector2<float> vector2F
 *
 * \brief   Defines an alias for a vector2 objects using simple precision 
 * 			components.
 */
typedef vector2<float>      vector2F;

/**
 * \typedef vector2<double> vector2D
 *
 * \brief   Defines an alias for a vector2 objects using double precision
 * 			components.
 */
typedef vector2<double>     vector2D;

} // namespace math
} // namespace v8
