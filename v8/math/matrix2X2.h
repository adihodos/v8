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

#include <cmath>
#include <memory.h>
#include "v8/base/fundamental_types.h"
#include "v8/base/compiler_warnings.h"
#include "v8/math/math_utils.h"
#include "v8/math/vector2.h"

namespace v8 { namespace math {

/**
 * \class   matrix_2X2.
 *
 * \brief   A 2x2 matrix, stored in row major format. It follows the convention
 * 			that it multiplies columns vectors, on the right (if M is a matrix
 * 			and V is a vector, then one would write a multiplication like this :
 * 			M * V);
 */
template<typename real_t>
class matrix_2X2 {
public :
    enum {
        is_floating_point = base::is_floating_point_type<real_t>::Yes
    };

    MSVC_DISABLE_WARNING_BLOCK_BEGIN(4201)
    union {
        struct {
            //
            // First row elements.
            real_t  a11_;
            real_t  a12_;
            //
            // Second row elements.
            real_t  a21_;
            real_t  a22_;
        };
        real_t elements_[4];	///< Used for array like access to the elements */
    };
    MSVC_DISABLE_WARNING_BLOCK_END(4201)

    /**
     * \typedef real_t element_type
     *
     * \brief   Defines an alias representing type of the element.
     */
    typedef real_t                  element_type;
    typedef real_t&                 reference;
    typedef const real_t&           const_reference;
    typedef matrix_2X2<real_t>      matrix_2X2_t;

    static const matrix_2X2_t       zero;

    static const matrix_2X2_t       identity;

    /**
     * \fn  matrix_2X2::matrix_2X2()
     *
     * \brief   Default constructor. Leaves elements uninitialized.
     */
    matrix_2X2() {}

    /**
     * \fn  matrix_2X2::matrix_2X2(real_t a11, real_t a12, real_t a21, real_t a22);
     *
     * \brief   Construct with four values.
     */
    matrix_2X2(real_t a11, real_t a12, real_t a21, real_t a22);

    /**
     * \fn  matrix_2X2::matrix_2X2(real_t a11, real_t a22);
     *
     * \brief   Construct a diagonal matrix with two values.
     */
    matrix_2X2(real_t a11, real_t a22);

    /**
     * \fn  matrix_2X2::matrix_2X2( const math::vector2<real_t>& v1, const math::vector2<real_t>& v2,
     * bool column = true );
     *
     * \brief   Construct from two vectors.
     *
     * \param   v1      The first vector.
     * \param   v2      The second vector.
     * \param   column  (optional) If true, the vectors represent columns,
     * 					otherwise rows.
     */
    matrix_2X2(
        const math::vector2<real_t>& v1, 
        const math::vector2<real_t>& v2,
        bool column = true
        );

    /**
     * \fn  matrix_2X2::matrix_2X2(const real_t* input, size_t count);
     *
     * \brief   Construct from an array of values.
     *
     * \param   input   Pointer to the array. Must not be null.
     * \param   count   Number of elements in the array.
     */
    matrix_2X2(const real_t* input, size_t count);

    /**
     * \fn  void matrix_2X2::make_tensor_product( const math::vector2<real_t>& u,
     * const math::vector2<real_t>& v );
     *
     * \brief   Makes the matrix equal to the tensor product of the two vectors.
     * 			Given two vectors u and v, their tensor product is
     * 			u * v ^ T.
     *
     */
    matrix_2X2<real_t>& make_tensor_product(
        const math::vector2<real_t>& u, 
        const math::vector2<real_t>& v
        );

    /**
     * \fn  void matrix_2X2::make_rotation(float theta);
     *
     * \brief   Makes a rotation matrix. Rotation is counter clockwise if theta > 0,
     * 			clockwise if theta < 0.
     */
    matrix_2X2<real_t>& make_rotation(float theta);

    /**
     * \fn  matrix_2X2<real_t>& matrix_2X2::make_skew_symmetric(real_t val);
     *
     * \brief   Makes a skew symmetric matrix, that is (a(i, j) = a(j, i) when i <> j
     * 			and a(i, j) = 0 when a = j).
     */
    matrix_2X2<real_t>& make_skew_symmetric(real_t val);

    matrix_2X2<real_t>& operator+=(const matrix_2X2<real_t>& right);

    matrix_2X2<real_t>& operator-=(const matrix_2X2<real_t>& right);

    matrix_2X2<real_t>& operator*=(real_t k);

    matrix_2X2<real_t>& operator/=(real_t k);

    real_t determinant() const {
        return a11_ * a22_ - a12_ * a21_;
    }

    bool is_invertible() const {
        return !math::is_zero(determinant());
    }

    /**
     * \fn  real_t matrix_2X2::trace() const
     *
     * \brief   Gets the trace of the matrix 
     * 			(sum of elements in the main diagonal).
     */
    real_t trace() const {
        return a11_ + a22_;
    }

    /**
     * \fn  matrix_2X2<real_t>& matrix_2X2::invert();
     *
     * \brief   Inverts the matrix. det(m) must be non zero.
     */
    matrix_2X2<real_t>& invert();

    /**
     * \fn  matrix_2X2<real_t>& matrix_2X2::transpose()
     *
     * \brief   Transposes the matrix.
     */
    matrix_2X2<real_t>& transpose() {
        swap(a12_, a21_);
        return *this;
    }
};

template<typename real_t>
const matrix_2X2<real_t> 
matrix_2X2<real_t>::zero(
    real_t(0), real_t(0),
    real_t(0), real_t(0)
    );

template<typename real_t>
const matrix_2X2<real_t> 
matrix_2X2<real_t>::identity(
    real_t(1), real_t(0),
    real_t(0), real_t(1)
    );

template<typename real_t>
inline
math::matrix_2X2<real_t>
math::operator+(
    const math::matrix_2X2<real_t>& lhs,
    const math::matrix_2X2<real_t>& rhs
    );

template<typename real_t>
inline
math::matrix_2X2<real_t>
math::operator-(
    const math::matrix_2X2<real_t>& lhs,
    const math::matrix_2X2<real_t>& rhs
    );

template<typename real_t>
math::matrix_2X2<real_t>
math::operator*(
    const math::matrix_2X2<real_t>& lhs,
    const math::matrix_2X2<real_t>& rhs
    );

template<typename real_t>
inline
math::matrix_2X2<real_t>
math::operator*(
    real_t k,
    const math::matrix_2X2<real_t>& rhs
    );

template<typename real_t>
inline
math::matrix_2X2<real_t>
math::operator*(
    const math::matrix_2X2<real_t>& lhs,
    real_t k
    );

template<typename real_t>
inline
math::vector2<real_t>
operator*(
    const math::matrix_2X2<real_t>& mtx,
    const math::vector2<real_t>& vec
    );

template<typename real_t>
inline
math::matrix_2X2<real_t>
operator/(
    const math::matrix_2X2<real_t>& lhs,
    real_t k
    );

/**
 * \fn  template<typename real_t> math::matrix_2X2<real_t> adjoint_of(const math::matrix_2X2<real_t>& mtx);
 *
 * \brief   Returns the adjoint of the given matrix. The adjoint matrix is
 * 			the transpose of the cofactor's matrix of matrix mtx.
 */
template<typename real_t>
math::matrix_2X2<real_t>
adjoint_of(const math::matrix_2X2<real_t>& mtx);

/**
 * \fn  template<typename real_t> inline math::matrix_2X2<real_t> transpose_of(const math::matrix_2X2<real_t>& mtx);
 *
 * \brief   Returns the transpose of the given matrix.
 */
template<typename real_t>
inline
math::matrix_2X2<real_t>
transpose_of(const math::matrix_2X2<real_t>& mtx);

/**
 * \fn  template<typename real_t> inline math::matrix_2X2<real_t> inverse_of(const math::matrix_2X2<real_t>& mtx);
 *
 * \brief   Returns the inverse of the given matrix.
 */
template<typename real_t>
inline
math::matrix_2X2<real_t>
inverse_of(const math::matrix_2X2<real_t>& mtx);

/**
 * \fn  template<typename real_t> math::matrix_2X2<real_t> multiply_transpose(const math::matrix_2X2<real_t>& other);
 *
 * \brief   Multiplies the left side matrix with the transpose of the right 
 * 			side matrix, that is LHS * RHS ^ T.
 */
template<typename real_t>
math::matrix_2X2<real_t>
multiply_transpose(
    const math::matrix_2X2<real_t>& lhs, 
    const math::matrix_2X2<real_t>& rhs
    );

/**
 * \fn  template<typename real_t> math::matrix_2X2<real_t> transpose_multiply( const math::matrix_2X2<real_t>& lhs,
 * const math::matrix_2X2<real_t>& rhs );
 *
 * \brief   Multiplies the transpose of the left side matrix with the 
 * 			right side matrix, that is LHS ^ T * RHS.
 */
template<typename real_t>
math::matrix_2X2<real_t>
transpose_multiply(
    const math::matrix_2X2<real_t>& lhs, 
    const math::matrix_2X2<real_t>& rhs
    );

/**
 * \fn  template<typename real_t> math::matrix_2X2<real_t> transpose_multiply_transpose( const math::matrix_2X2<real_t>& lhs,
 * const math::matrix_2X2<real_t>& rhs );
 *
 * \brief   Multiplies the transpose of the left hand matrix with the transpose
 * 			of the right hand matrix, that is LHS ^ T * RHS ^ T.
 */
template<typename real_t>
math::matrix_2X2<real_t>
transpose_multiply_transpose(
    const math::matrix_2X2<real_t>& lhs, 
    const math::matrix_2X2<real_t>& rhs
    );

/**
 * \typedef matrix_2X2<float> matrix_2X2F
 *
 * \brief   Defines an alias representing a matrix_2X2 with simple precision
 * 			components.
 */
typedef matrix_2X2<float>       matrix_2X2F;

/**
 * \typedef matrix_2X2<double> matrix_2X2D
 *
 * \brief   Defines an alias representing a matrix_2X2 with double precision
 * 			components.
 */
typedef matrix_2X2<double>      matrix_2X2D;

} // namespace math
} // namespace v8

#include "matrix2X2.inl"
