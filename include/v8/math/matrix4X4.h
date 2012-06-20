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
#include "v8/base/fundamental_types.h"
#include "v8/base/compiler_warnings.h"
#include "v8/math/math_utils.h"
#include "v8/math/matrix3X3.h"
#include "v8/math/vector4.h"

namespace v8 { namespace math {

/**
 * \class   matrix4X4
 *
 * \brief   A 4x4 matrix, stored using row major indexing. 
 * 			The matrix is designed so that it multiplies the vector on its right
 * 			side. Given a matrix M and a vector V, to transform the vector V by the
 * 			matrix M, one would write V1 = M * V;
 */
template<typename real_t>
class matrix_4X4 {
private :

  int index_at(int row, int col) const {
    return (row - 1) * 4 + col - 1;
  }

public :
    enum { 
        is_floating_point = base::is_floating_point_type<real_t>::Yes
    };

    /**
     * \typedef real_t element_type
     *
     * \brief   Defines an alias representing type of the element.
     */
    typedef real_t              element_type;
    typedef real_t&             reference;
    typedef const real_t&       const_reference;
    typedef matrix_4X4<real_t>   matrix4X4_t;

    MSVC_DISABLE_WARNING_BLOCK_BEGIN(4201)
    union {
        struct {
            real_t a11_, a12_, a13_, a14_; ///< The first row */
            real_t a21_, a22_, a23_, a24_; ///< The second row */
            real_t a31_, a32_, a33_, a34_; ///< The third row */
            real_t a41_, a42_, a43_, a44_; ///< The fourth row */
        };
        real_t elements_[16];	///< Access to elements using an array */
    };
    MSVC_DISABLE_WARNING_BLOCK_END(4201)

    static const matrix_4X4<real_t>	null;   ///< The null matrix */

    static const matrix_4X4<real_t>	identity;   ///< The identity 4x4 matrix*/

    /**
     * \fn    matrix4X4::matrix4X4()
     *
     * \brief Default constructor. Leaves elements uninitialized.
     */
    matrix_4X4() {}

    template<typename real_u>
    matrix_4X4(const matrix_3X3<real_u>& mtx3x3);

    matrix_4X4(
        real_t a11, real_t a12, real_t a13, real_t a14,
        real_t a21, real_t a22, real_t a23, real_t a24,
        real_t a31, real_t a32, real_t a33, real_t a34,
        real_t a41, real_t a42, real_t a43, real_t a44
        );

    /**
     * \fn    matrix4X4::matrix4X4(const real_t* input, size_t count)
     *
     * \brief Constructor from an array of real_t values.
     *
     * \param input   Pointer to an array of real_t values.
     * \param count   Number of entries in the array.
     * 				
     * \remarks If the array has less than 16 elements, the remaining matrix components
     * 		  will be left uninitialized. Values beyond the 16th element of the
     * 		  array will be ignored.
     */
    matrix_4X4(const real_t* input, size_t count) {
        std::memcpy(elements_, 
                    input, 
                    std::min(_countof(elements_), count) * sizeof(real_t));
    }

    /**
     * \fn  matrix4X4::matrix4X4(real_t a11, real_t a22, real_t a33, real_t a44 = real_t(1));
     *
     * \brief   Constructs a diagonal matrix.
     */
    matrix_4X4(real_t a11, real_t a22, real_t a33, real_t a44 = real_t(1));

    /**
     * \fn  matrix4X4::matrix4X4( const math::vector4<real_t>& v1, const math::vector4<real_t>& v2,
     * const math::vector4<real_t>& v3, const math::vector4<real_t>& v4, bool column = true );
     *
     * \brief   Construct from 4 vectors.
     */
    matrix_4X4(
        const math::vector4<real_t>& v1, 
        const math::vector4<real_t>& v2,
        const math::vector4<real_t>& v3,
        const math::vector4<real_t>& v4,
        bool column = true
        );

    /**
     * \fn    real_t& matrix4X4::operator()(int row, int col)
     *
     * \brief Component read-write access, using the [row][column] syntax.
     *
     * \param row Component's row index (1 based).
     *
     * \param col Component's column index (1 based).
     */
    real_t& operator()(int row, int col) {
        return elements_[index_at(row, col)];
    }

    /**
     * \fn    real_t matrix4X4::operator()(int row, int col) const
     *
     * \brief Component read only access, using the [row][column] syntax.
     * 		
     * \param row Component's row index (1 based).
     * 			
     * \param col Component's column index (1 based).
     *
     * \return The value of the specified component.
     */
    real_t operator()(int row, int col) const {
        return elements_[index_at(row, col)];
    }

    /**
     * \fn    matrix4X4& matrix4X4::operator+=(const matrix4X4& rhs);
     *
     * \brief Addition assignment operator.
     *
     * \param rhs The right hand side.
     *
     * \return    The result of the operation.
     */
    matrix_4X4<real_t>& operator+=(const matrix_4X4<real_t>& rhs);

    /**
     * \fn    matrix4X4& matrix4X4::operator-=(const matrix4X4& rhs);
     *
     * \brief Subtraction assignment operator.
     *
     * \param rhs The right hand side.
     *
     */
    matrix_4X4<real_t>& operator-=(const matrix_4X4<real_t>& rhs);

    /**
     * \fn    matrix4X4& matrix4X4::operator*=(real_t k);
     *
     * \brief Multiplication assignment operator.
     *
     * \param k   Scalar to multiply with.
     *
     */
    matrix_4X4<real_t>& operator*=(real_t k);

    /**
     * \fn    matrix4X4& matrix4X4::operator/=(real_t k);
     *
     * \brief Division assignment operator.
     *
     * \param k   The scalar to divide with. Ensure k <> 0.
     *
     */
    matrix_4X4<real_t>& operator/=(real_t k);

    /**
     * \fn    real_t matrix4X4::determinant() const;
     *
     * \brief Computes the determinant for this matrix.
     *
     */
    real_t determinant() const;

    /**
     * \fn    bool matrix4X4::is_invertible() const
     *
     * \brief Query if this matrix4X4 is invertible (that is det(A) is not 0).
     *
     * \return    true if invertible, false if not.
     */
    bool is_invertible() const {
        return !math::operands_eq(real_t(0), determinant());
    }

    /**
     * \fn    matrix4X4& matrix4X4::invert()
     *
     * \brief Inverts the matrix.
     * \remarks Only call this function if is_invertible() returns true.		
     *
     * \return    Reference to the matrix object.
     */
    matrix_4X4<real_t>& invert();

    inline void get_inverse(math::matrix_4X4<real_t>* mx) const;

    /**
     * \fn    matrix4X4& matrix4X4::transpose()
     *
     * \brief Transposes the matrix.
     *
     * \return    Reference to the matrix object.
     */
    matrix_4X4<real_t>& transpose();

    void get_transpose(math::matrix_4X4<real_t>* mx) const;

    void get_adjoint(math::matrix_4X4<real_t>* mx) const;

   /**
    * \fn    matrix4X4& matrix4X4::set_upper3x3(const real_t* data)
    *
    * \brief Copies the values from an array into the upper 3x3 matrix.
    *
    * \param data    Pointer to an array of real_t values. Cannot be null.
    * 				Must contain at least 9 elements.
    */
    template<typename real_u>
    matrix_4X4<real_t>& set_upper3x3(const real_u* data);

   /**
    * \fn    matrix4X4& matrix4X4::set_upper3x3(const matrix3X3& src)
    *
    * \brief Sets the contents of the upper 3x3 matrix.
    *
    * \param src Source 3x3 matrix for the assignment
    */
    template<typename real_u>
    matrix_4X4<real_t>& set_upper3x3(const matrix_3X3<real_u>& src) {
        return set_upper3x3(src.elements_);
    }

   /**
    * \fn    void matrix4X4::get_upper3x3(real_t* data) const
    *
    * \brief Gets the contents of the upper 3x3 matrix.
    *
    * \param [in,out]    data    Pointer to an array of real_t values, where the
    * 					data is to be copied. Cannot be null. 
    * 					Must have at least 9 elements.
    */
    template<typename real_u>
    void get_upper3x3(real_u* data) const;

   /**
    * \fn    void matrix4X4::get_upper3x3(matrix3X3* out_mtx) const
    *
    * \brief Gets the contents of the upper 3x3 matrix.
    *
    * \param [in,out]    out_mtx Pointer to a 3x3 matrix that receives the data.
    */
    template<typename real_u>
    void get_upper3x3(matrix_3X3<real_u>* out_mtx) const {
        get_upper3x3(out_mtx->elements_);
    }

    matrix_4X4<real_t>& set_row(
        int row, real_t r1, real_t r2, real_t r3, real_t r4
        );

   /**
    * \fn    matrix4X4& matrix4X4::set_row(int row, const real_t* row_data)
    *
    * \brief Sets a row's contents.
    *
    * \param row         The row index (1 based).
    * \param row_data    Pointer to source data.
    */
    matrix_4X4<real_t>& set_row(int row, const real_t* row_data);

   /**
    * \fn    matrix4X4& matrix4X4::set_row(int row, const vector4& vec)
    *
    * \brief Assigns the contents of a vector4 to a matrix row.
    *
    * \param row The row index (1 based).
    * \param vec Source vector for the data.
    *
    */
    matrix_4X4<real_t>& set_row(int row, const vector4<real_t>& vec) {
        return set_row(row, vec.elements_);
    }

    matrix_4X4<real_t>& set_column(
        int column, real_t c1, real_t c2, real_t c3, real_t c4
        );

   /**
    * \fn    matrix4X4& matrix4X4::set_column(int column, const real_t* data)
    *
    * \brief Sets a column's data.
    *
    * \param column  The column index (1 based).
    * \param data    Pointer to source data.
    */
    matrix_4X4<real_t>& set_column(int column, const real_t* data);

   /**
    * \fn    matrix4X4& matrix4X4::set_column(int column, const vector4& v4)
    *
    * \brief Assigns the contents of a vector4 to a matrix column.
    *
    * \param column  The column index (1 based).
    * \param v4      Source vector.
    *
    */
    matrix_4X4<real_t>& set_column(int column, const vector4<real_t>& v4) {
        return set_column(column, v4.elements_);
    }

   /**
    * \fn    void matrix4X4::get_row(int row, real_t* data) const
    *
    * \brief Copies the contents of a row.
    *
    * \param row                 The row index (1 based).
    * \param [in,out]    data    Pointer to a real_t array that will receive the
    * 					row's data. The array size must be at least 4. 
    * 					Can not be null.
    */
    void get_row(int row, real_t* data) const;

   /**
    * \fn    void matrix4X4::get_row(int row, vector4* v4) const
    *
    * \brief Copies the contents of a row.
    *
    * \param row             The row index (1 based).
    * \param [in,out]    v4  Pointer to a vector4 object that will receive the 
    * 					    row's contents. Must not be null.
    */
    void get_row(int row, vector4<real_t>* v4) const {
        return get_row(row, v4->elements_);
    }

   /**
    * \fn    void matrix4X4::get_column(int col, real_t* data) const
    *
    * \brief Copies the contents of a column into an array.
    *
    * \param col                 The column index (1 based).
    * \param [in,out]    data    Pointer to a real_t array that will receive the
    * 					column's data. The array size must be at least 4.
    * 					Can not be null.
    */
    inline void get_column(int col, real_t* data) const;

   /**
    * \fn    void matrix4X4::get_column(int col, vector4* v4) const
    *
    * \brief Copies the contents of a column into a vector4 object.
    *
    * \param col             The column index (1 based).
    * \param [in,out]    v4  Pointer to a vector4 object. Cannot be null.
    */
    void get_column(int col, vector4<real_t>* v4) const {
        get_column(col, v4->elements_);
    }

    template<typename R2>
    matrix_4X4<real_t>& transform_affine_vector(vector3<R2>* pvec) const;

    template<typename R2>
    matrix_4X4<real_t>& transform_affine_point(vector3<R2>* point) const;

   /**
    * \fn    matrix4X4& matrix4X4::transform_vector(vector4* pvec) const
    *
    * \brief Transforms an affine (w component is 0) 
    * 		 vector and assigns the result to it.
    *
    * \param [in,out]    pvec    Pointer to the vector4 to be transformed. 
    * 					Must not be null.
    */
    matrix_4X4<real_t>& transform_affine_vector(vector4<real_t>* pvec) const;

    /**
     * \fn  matrix4X4<real_t>& matrix4X4::transform_affine_point(vector4<real_t>* point) const;
     *
     * \brief   Transforms an affine point (w component is 1) and assigns 
     * 			the result to it.
     *
     * \param [in,out]  point   If non-null, the point.
     */
    matrix_4X4<real_t>& transform_affine_point(vector4<real_t>* point) const;

    /**
     * \fn  matrix4X4<real_t>& matrix4X4::transform_homogeneous_point(vector4<real_t>& point) const;
     *
     * \brief   Transforms a homogeneous point 
     * 			(w component may be different than 1).
     *
     * \param [in,out]  point   The point.
     */
    matrix_4X4<real_t>& transform_homogeneous_point(vector4<real_t>* point) const;

    /**
     * \fn  matrix4X4 perspective_projection_left_handed( float aspect_ratio, float vertical_fov,
     * float near_plane, float far_plane, float depth_min, float depth_max );
     *
     * \brief   Makes a perspective projection matrix, assuming a left handed
     * 			coordinate system.
     *
     * \param   aspect_ratio    The aspect ratio of the window.
     * \param   vertical_fov    The vertical field of view angle, in radians.
     * \param   near_plane      The near plane.
     * \param   far_plane       The far plane.
     * \param   depth_min       The depth minimum.
     * \param   depth_max       The depth maximum.
     *
     */
    matrix_4X4<real_t>& make_perspective_projection_lh(
        real_t aspect_ratio,
        real_t vertical_fov,
        real_t near_plane,
        real_t far_plane,
        real_t depth_min,
        real_t depth_max
        );

    /**
     * \fn  matrix4X4 perspective_projection_left_handed( float aspect_ratio, float vertical_fov,
     * float near_plane, float far_plane, float depth_min, float depth_max );
     *
     * \brief   Computes the perspective projection matrix assuming a right handed
     * 			coordinate system.
     *
     * \param   aspect_ratio    The aspect ratio of the window.
     * \param   vertical_fov    The vertical field of view angle, in radians.
     * \param   near_plane      The near plane.
     * \param   far_plane       The far plane.
     * \param   depth_min       The depth minimum.
     * \param   depth_max       The depth maximum.
     *
     */
    matrix_4X4<real_t>& make_perspective_projection_rh(
        real_t aspect_ratio,
        real_t vertical_fov,
        real_t near_plane,
        real_t far_plane,
        real_t depth_min,
        real_t depth_max
        );

    /**
     * \fn  matrix4X4 perspective_projection_oblique_left_handed( float near_plane, float far_plane,
     * float wtop, float wbottom, float wleft, float wright, float depth_min, float depth_max );
     *
     * \brief   Returns a matrix describing an oblique perspective projection,
     * 			using a left handed coordinate system.
     *
     * \param   near_plane  The near plane.
     * \param   far_plane   The far plane.
     * \param   wtop        The window top point (y axis).
     * \param   wbottom     The window bottom point (y axis).
     * \param   wleft       The window left point (x axis).
     * \param   wright      The window right point (x axis).
     * \param   depth_min   The minimum depth value.
     * \param   depth_max   The maximum depth value.
     *
     */
    matrix_4X4<real_t>& make_perspective_projection_oblique_lh(
        real_t near_plane,
        real_t far_plane,
        real_t wtop,
        real_t wbottom,
        real_t wleft,
        real_t wright,
        real_t depth_min,
        real_t depth_max
        );

    /**
     * \fn  matrix4X4 orthographic_parallel_projection_left_handed( float near_plane,
     * float far_plane, float wtop, float wbottom, float wleft, float wright, float depth_min,
     * float depth_max );
     *
     * \brief   Returns a matrix representing a parallel orthographic projection,
     * 			suitable for a left handed coordinate system.
     *
     * \param   near_plane  The near plane.
     * \param   far_plane   The far plane.
     * \param   wtop        The window top point (y axis).
     * \param   wbottom     The window bottom point (y axis).
     * \param   wleft       The window left point (x axis).
     * \param   wright      The window right point (x axis).
     * \param   depth_min   The minimum depth value.
     * \param   depth_max   The maximum depth value.
     */
    matrix_4X4<real_t>& make_ortho_parallel_projection_lh(
        real_t near_plane,
        real_t far_plane,
        real_t wtop,
        real_t wbottom,
        real_t wleft,
        real_t wright,
        real_t depth_min,
        real_t depth_max
        );

    inline matrix_4X4<real_t>& make_zero();

    inline matrix_4X4<real_t>& make_identity();
};

template<typename real_t>
const math::matrix_4X4<real_t>
math::matrix_4X4<real_t>::null(
    real_t(0), real_t(0), real_t(0), real_t(0),
    real_t(0), real_t(0), real_t(0), real_t(0),
    real_t(0), real_t(0), real_t(0), real_t(0),
    real_t(0), real_t(0), real_t(0), real_t(0)
    );

template<typename real_t>
const math::matrix_4X4<real_t>
    math::matrix_4X4<real_t>::identity(
    real_t(1), real_t(0), real_t(0), real_t(0),
    real_t(0), real_t(1), real_t(0), real_t(0),
    real_t(0), real_t(0), real_t(1), real_t(0),
    real_t(0), real_t(0), real_t(0), real_t(1)
    );

template<typename real_t>
bool
operator==(
    const math::matrix_4X4<real_t>& lhs,
    const math::matrix_4X4<real_t>& rhs
        );

template<typename real_t>
inline
bool
operator!=(
    const math::matrix_4X4<real_t>& lhs,
    const math::matrix_4X4<real_t>& rhs
        );

/**
 * \fn  inline matrix4X4 operator+( const matrix4X4& lhs, const matrix4X4& rhs )
 *
 * \brief   Addition operator.
 */
template<typename real_t>
inline
matrix_4X4<real_t>
operator+(
    const math::matrix_4X4<real_t>& lhs,
    const math::matrix_4X4<real_t>& rhs
    );

/**
 * \fn  inline matrix4X4 operator-( const matrix4X4& lhs, const matrix4X4& rhs )
 *
 * \brief   Subtraction operator.
 */
template<typename real_t>
inline
matrix_4X4<real_t>
operator-(
    const math::matrix_4X4<real_t>& lhs,
    const math::matrix_4X4<real_t>& rhs
    );

/**
 * \fn  inline matrix4X4 operator-( const matrix4X4& mtx )
 *
 * \brief   Negation operator.
 */
template<typename real_t>
inline
matrix_4X4<real_t>
operator-(
    const math::matrix_4X4<real_t>& mtx
    );

/**
 * \fn  matrix4X4 operator*( const math::matrix4X4& lhs, const math::matrix4X4& rhs );
 *
 * \brief   Multiplication operator.
 */
template<typename real_t>
matrix_4X4<real_t>
operator*(
    const math::matrix_4X4<real_t>& lhs,
    const math::matrix_4X4<real_t>& rhs
    );

/**
 * \fn  inline matrix4X4 operator*( float k, const matrix4X4& mtx )
 *
 * \brief   scalar -> matrix multiplication operator.
 */
template<typename real_t>
inline
matrix_4X4<real_t>
operator*(
    float k,
    const math::matrix_4X4<real_t>& mtx
    );

/**
 * \fn  inline matrix4X4 operator*( const matrix4X4& mtx, float k )
 *
 * \brief   matrix -> scalar multiplication operator.
 */
template<typename real_t>
inline
matrix_4X4<real_t>
operator*(
    const math::matrix_4X4<real_t>& mtx,
    real_t k
    );

/**
 * \fn  inline matrix4X4 operator/( const matrix4X4& mtx, float k )
 *
 * \brief   matrix -> scalar division operator.
 */
template<typename real_t>
inline
math::matrix_4X4<real_t>
operator/(
    const math::matrix_4X4<real_t>& mtx,
    real_t k
    );

typedef matrix_4X4<float>    matrix_4X4F;

typedef matrix_4X4<double>   matrix_4X4D;

} // namespace math
} // namespace v8

#include "matrix4X4.inl"
