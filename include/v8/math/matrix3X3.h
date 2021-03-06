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
#include <cassert>
#include "v8/base/fundamental_types.h"
#include "v8/base/compiler_warnings.h"
#include "v8/math/math_constants.h"
#include "v8/math/math_utils.h"
#include "v8/math/vector2.h"
#include "v8/math/vector3.h"

namespace v8 { namespace math {

/**
 * \class   
 *
 * \brief   Represents a 3 by 3 matrix, stored in row major format. 
 * 			It follows the conventions that the matrix goes on the left side
 * 			when multiplying vectors. Also, when concatenating matrices
 * 			representing multiple transformations, the order must be from 
 * 			right to left, eg : if we have a rotation R, followed by a scaling S
 * 			and a reflection RF (R, S, RF) then the matrices need to be concatenated
 * 			like this : FinalTransform = RF * S * R.
 */
template<typename real_t>
class matrix_3X3 {
private :

    int index_at(int row, int col) const {
        return (row - 1) * 3 + col - 1;
    }

public:
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
    typedef matrix_3X3<real_t>  matrix3X3_t;

    MSVC_DISABLE_WARNING_BLOCK_BEGIN(4201)
    union {
        struct {
            //
            // First row
            real_t a11_, a12_, a13_;
            //
            // Second row
            real_t a21_, a22_, a23_;
            //
            // Third row
            real_t a31_, a32_, a33_;
        };
        real_t elements_[9];
    };
    MSVC_DISABLE_WARNING_BLOCK_END(4201)

    static const matrix3X3_t zero;  ///< The zero matrix. */

    static const matrix3X3_t identity;  ///< The identity matrix. */

    /**
     * \fn  matrix3X3::matrix3X3()
     *
     * \brief   Default constructor. Leaves elements uninitialized.
     */
    matrix_3X3() {}

    matrix_3X3(
        real_t a11, real_t a12, real_t a13,
        real_t a21, real_t a22, real_t a23,
        real_t a31, real_t a32, real_t a33
        );

    /**
     * \fn  matrix3X3::matrix3X3(const real_t* input, size_t count);
     *
     * \brief   Constructs from an array of values.
     * 			
     * \param   input   Pointer to an array of values. Must not be null.
     * \param   count   Number of elements in the array.
     */
    matrix_3X3(const real_t* input, size_t count);

    /**
     * \fn  matrix3X3::matrix3X3(float a11, float a22, float a33)
     *
     * \brief   Construct a diagonal matrix, setting A(i,j) = 0 for every i <> j.
     */
    matrix_3X3(real_t a11, real_t a22, real_t a33);

    /**
     * \fn  matrix3X3::matrix3X3( const vector3<real_t>& u, const vector3<real_t>& v,
     * const vector3<real_t>& w, bool column = true );
     *
     * \brief   Construct a matrix from a set of 3 vectors.
     *
     * \param   column  True - vectors represent columns, otherwise rows.
     */
    matrix_3X3(
        const vector3<real_t>& u, 
        const vector3<real_t>& v, 
        const vector3<real_t>& w, 
        bool column = true
        );

    /**
     \brief Set this matrix to be the null matrix.
     */
    inline matrix_3X3<real_t>& make_zero();

    /**
     \brief Set this matrix to be the identity matrix.
     */
    inline matrix_3X3<real_t>& make_identity();

    /**
     * \fn  matrix3X3<real_t>& matrix3X3::make_translation(real_t tx, real_t ty);
     *
     * \brief   Convert matrix to a matrix that can be used to translate affine points
     * 			of the form (x, y, 1).
     */
    matrix_3X3<real_t>& make_translation(real_t tx, real_t ty);

    /**
     * \fn  static matrix3X3<real_t> matrix3X3::translation(const math::vector2<real_t>& tv)
     *
     * \brief   Make a translation matrix, with the translating factors
     * 			specified by a vector2 object. The matrix can be used to translate
     * 			affine points in the form (x, y, 1).
     */
    matrix_3X3<real_t>& make_translation(const math::vector2<real_t>& tv) {
        return make_translation(tv.x_, tv.y_);
    }

    /**
     * \fn    static const matrix3X3 matrix3X3::skew_symmetric(float a12, float a13, float a23)
     *
     * \brief Make a skew symmetric matrix. A skew symmetric matrix has
     *        A(i,j) = 0 when i = j and A(i, j) = -A(j, i) when i <> j.
     */
    matrix_3X3<real_t>& make_skew_symmetric(real_t u0, real_t u1, real_t u2);

    /**
     * \fn  static matrix3X3<real_t> matrix3X3::scale(real_t x, real_t y)
     *
     * \brief   Makes a matrix suitable for scaling an affine vector of the
     * 			form (x, y, 0).
     */
    matrix_3X3<real_t>& make_scale(real_t x, real_t y);

    /**
     * \fn  static const matrix3X3<real_t> matrix3X3::scale(real_t sx, real_t sy, real_t sz)
     *
     * \brief   Returns a scaling matrix for a vector in R3.
     */
    matrix_3X3<real_t>& make_scale(real_t sx, real_t sy, real_t sz);

    matrix_3X3<real_t>& make_scale(const vector3<real_t>& sv) {
        return make_scale(sv.x_, sv.y_, sv.z_);
    }  

    /**
     * \fn  static const matrix3X3<real_t> matrix3X3::euler_xyz(real_t rx, real_t ry, real_t rz)
     *
     * \brief   Makes a rotation matrix, using Euler angles in xyz format.
     */
    matrix_3X3<real_t>& make_euler_xyz(real_t rx, real_t ry, real_t rz);

    /**
     * \fn  static const matrix3X3<real_t> matrix3X3::rotation_x(real_t theta)
     *
     * \brief   Makes a matrix for a rotation around the x axis, with
     * 			an angle of theta radians.
     * 			
     * \param   theta   Angle of rotation, in radians.
     */
    matrix_3X3<real_t>& make_rotation_x(real_t theta);

    /**
     * \fn  static const matrix3X3<real_t> matrix3X3::rotation_y(real_t theta)
     *
     * \brief   Returns a matrix for a rotation around the y axis, with
     * 			an angle of theta radians.
     * 			
     * \param   theta   Angle of rotation, in radians.
     */
    matrix_3X3<real_t>& make_rotation_y(real_t theta);

    /**
     * \fn  static const matrix3X3<real_t> matrix3X3::rotation_z(real_t theta)
     *
     * \brief   Returns a matrix for a rotation around the y axis, with
     * 			an angle of theta radians.
     * 			
     * \param   theta   Angle of rotation, in radians.
     */
    matrix_3X3<real_t>& make_rotation_z(real_t theta);

    /**
     * \fn  static matrix3X3<real_t> matrix3X3::axis_angle(const vector3<real_t>& axisv,
     * real_t theta);
     *
     * \brief   Make a rotation matrix out of an angle-axis representation.
     *
     * \param   axisv   Normalized vector that gives the rotation axis.
     * \param   theta   Rotation angle, in radians.
     */
    matrix_3X3<real_t>& axis_angle(
        const vector3<real_t>& axisv, real_t theta
        );

    /**
     * \fn  static matrix3X3 matrix3X3::rotation(const vector3<real_t>& v1,
     * const vector3<real_t>& v2);
     *
     * \brief   Given 2 vectors v1 and v2, build a matrix that will rotate v1 into v2.
     *          If v1 and v2 are parallel, the function will return a  matrix representing
     *          a rotation of 0 radians around v1.
     */
    matrix_3X3<real_t>& rotation(
        const vector3<real_t>& v1, 
        const vector3<real_t>& v2
        );

    /**
     * \fn  static matrix3X3<real_t> matrix3X3::planar_reflection(const vector3<real_t>& pn);
     *
     * \brief   Return a reflection matrix around a plane's normal.
     *
     * \param   pn  Normalized vector representing the plane's normal.
     */
    matrix_3X3<real_t>& planar_reflection(
        const vector3<real_t>& pn
        );

    /**
     * \brief Extract Euler angles from a rotation matrix, in format xyz.
     * \param[out]  angles Pointer to an array of values that will receive the
     *              angles of rotation.
     */
    void extract_euler_xyz(
        real_t* angles
        ) const;

    /**
     * \brief Extract axis of rotation and rotation angle from a rotation matrix.
     * \param[in,out] rot_axis  Pointer to a vector3 object that receives a unit
     *                          vector, representing the rotation axis.
     * \param[in,out] rot_angle Pointer to a value that receives the rotation 
     *                          angle.
     */
    void extract_axis_angle(
        vector3<real_t>* rot_axis, 
        real_t* rot_angle
        ) const;

    /**
     * \fn  static const matrix3X3<real_t> matrix3X3::tensor_product( const vector3<real_t>& u,
     * const vector3<real_t>& w );
     *
     * \brief   Return a matrix that is the tensor product of the two input vectors.
     * 			If u(u0, u1, u2) and v(v0, v1, v2) are two vectors
     * 			then the tensor product is u * v^T.
     */
    matrix_3X3<real_t>& tensor_product(
        const vector3<real_t>& u, 
        const vector3<real_t>& w
        );

    real_t& operator()(int row, int col) {
        return elements_[index_at(row, col)];
    }

    real_t operator()(int row, int col) const {
        return elements_[index_at(row, col)];
    }

    matrix_3X3<real_t>& operator+=(const matrix_3X3<real_t>& rhs);

    matrix_3X3<real_t>& operator-=(const matrix_3X3<real_t>& rhs);

    matrix_3X3<real_t>& operator*=(real_t k);

    matrix_3X3<real_t>& operator/=(real_t k);

    /**
     * \fn  real_t matrix3X3::determinant() const;
     *
     * \brief   Compute the determinant for this matrix.
     */
    real_t determinant() const;

    /**
     * \fn  bool matrix3X3::is_invertible() const
     *
     * \brief   Query if this matrix3X3 is invertible. A matrix B is invertible 
     * 			only if det(B) != 0.
     */
    bool is_invertible() const {
        return !math::is_zero(determinant());
    }

    /**
     * \fn  matrix3X3<real_t>& matrix3X3::invert()
     *
     * \brief   Inverts the matrix.
     */
    matrix_3X3<real_t>& invert();

    /**    
     \brief Copy the inverse of this matrix, if it has one.    
     \param [in,out]    mtx Pointer to a matrix_3X3 that receives the inverse of
        this matrix. Must not be null.
     */
    void get_inverse(math::matrix_3X3<real_t>* mtx) const;

    /**
     * \fn  matrix3X3<real_t>& matrix3X3::transpose();
     *
     * \brief   Transpose the matrix and return a reference to it.
     */
    matrix_3X3<real_t>& transpose();

    /**
     \brief Gets the transpose of this matrix
        
     \param [in,out]    mtx Pointer to a matrix_3X3 that receives the transpose 
        of this matrix. Must not be null.
     */
    inline void get_transpose(math::matrix_3X3<real_t>* mtx) const;

    /**
     \brief Gets the adjoint matrix.
        
     \param [in,out]    mtx Pointer to a matrix_3X3 that receives the adjoint
        of this matrix.
     */
    void get_adjoint(math::matrix_3X3<real_t>* mtx) const;

    /**
     * \fn  real_t matrix3X3::trace() const
     *
     * \brief   Return the trace of the matrix (sum of elements in the main diagonal).
     */
    real_t trace() const {
        return a11_ + a22_ + a33_;
    }

    /**
     * \fn  matrix3X3& matrix3X3::ortho_normalize();
     *
     * \brief   Make a matrix whose columns represent ortho-normalized vectors.
     *          Let M = [v0, v1, v2]. This function will output a matrix 
     *          M1 = [q0, q1, q2], such that q0, q1, q2 are orthonormal vectors.
     *
     *          q0 = v0 / || v0 ||;
     *          q1 = (v1 - proj(v1, q0)) / || v1 - proj(v1, q0) ||;
     *          q2 = (v2 - proj(v2, q0) - proj(v2, q1)) / 
     *              || (v2 - proj(v2, q0) - proj(v2, q1)) ||;
     */
    matrix_3X3<real_t>& ortho_normalize();

    matrix_3X3<real_t>& set_row(int row, real_t a1, real_t a2, real_t a3);

    /**
     * \fn  matrix3X3<real_t>& matrix3X3::set_row(int row, const real_t* data_ptr);
     *
     * \brief   Sets the contents of a row from an array of values.
     * 			
     * \param   row         The row index (1 based).
     * \param   data_ptr    Pointer to the array. Must not be null.
     */
    matrix_3X3<real_t>& set_row(int row, const real_t* data_ptr);

    /**
     * \fn  matrix3X3<real_t>& matrix3X3::set_row(int row, const math::vector3<real_t>& vec);
     *
     * \brief   Sets contents of the specified row equal to the elements of a vector.
     * 			
     * \param   row Index of the row to set, using 1 based indexing.
     * \param   Source vector to copy elements from.
     */
    matrix_3X3<real_t>& set_row(int row, const math::vector3<real_t>& vec) {
        set_row(row, vec.elements_);
    }
  
    /**
     * \fn    void matrix3X3::get_row(int row, float* data) const
     *
     * \brief Copies the contents of a row.
     *
     * \param row       The row index (1 based).
     * \param [in,out]  data    Pointer to a float array that will receive the
     * 					row's data. The array size must be at least 3. 
     * 					Can not be null.
     */
    void get_row(int row, real_t* ptr) const;

    /**
     * \fn  void matrix3X3::get_row(int row, math::vector3<real_t>* v3)
     *
     * \brief   Copies the contents of a row into a vector3.
     * 			
     * \param   row         The row index (1 based).
     * \param [in,out]  v3  Pointer to a vector3 object that receives the data.
     * 					Cannot be null.
     */
    void get_row(int row, math::vector3<real_t>* v3) {
        get_row(row, v3->elements_);
    }

    matrix_3X3<real_t>& set_column(int column, real_t c1, real_t c2, real_t c3);

    /**
     * \fn  matrix3X3<real_t>& matrix3X3::set_column(int column, const real_t* data_ptr)
     *
     * \brief Sets a column's data.
     *
     * \param column  The column index (1 based).
     * \param data_ptr  Pointer to source data. Must not be null.
     */
    matrix_3X3<real_t>& set_column(int column, const real_t* data_ptr);

    /**
     * \fn  matrix3X3<real_t>& matrix3X3::set_column(int column, const math::vector3<real_t>& vec)
     *
     * \brief   Assigns the contents of a vector3 object to a column.
     *
     * \param   column  The column index(1 based).
     * \param   vec     The source vector for the assignment.
     */
    matrix_3X3<real_t>& set_column(int column, const math::vector3<real_t>& vec) {
        return set_column(column, vec.elements_);
    }

    /**
     * \fn  void matrix3X3::get_column(int column, real_t* data_ptr);
     *
     * \brief   Copies the contents of a column into an array.
     * 			
     * \param   column              The column index(1 based).
     * \param [in,out]  data_ptr    Pointer to an array of values. Must be non
     * 					null and have at least 3 elements.
     */
    void get_column(int column, real_t* data_ptr);

    /**
     * \fn  void matrix3X3::get_column(int column, math::vector3<real_t>* v3)
     *
     * \brief   Copies the contents of a column into a vector3 object.
     * \param   column      The column index(1 based).
     * \param [in,out]  v3  Pointer to a vector3 object. Must be non null.
     */
    void get_column(int column, math::vector3<real_t>* v3) {
        get_column(column, v3->elements_);
    }

    /**
     * \fn  math::vector2<real_t>* matrix3X3::transform_point(math::vector2* p) const
     *
     * \brief   Transforms a 2d affine point and returns a pointer to it.
     *
     * \param [in,out]  p   Pointer to the point. Must not be null.
     */
    math::vector2<real_t>* transform_point(math::vector2<real_t>* p) const {
        p->x_ = a11_ * p->x_ + a12_ * p->y_ + a13_;
        p->y_ = a21_ * p->x_ + a22_ * p->y_ + a23_;
        return p;
    }

    /**
     * \fn  math::vector2* matrix3X3::transform_vector(math::vector2* p) const
     *
     * \brief   Transforms a 2d vector.
     */
    math::vector2<real_t>* transform_vector(math::vector2<real_t>* p) const {
        p->x_ = a11_ * p->x_ + a12_ * p->y_;
        p->y_ = a21_ * p->x_ + a12_ * p->y_;
        return p;
    }
};

template<typename real_t>
const matrix_3X3<real_t> 
matrix_3X3<real_t>::zero(
    real_t(0), real_t(0), real_t(0),
    real_t(0), real_t(0), real_t(0),
    real_t(0), real_t(0), real_t(0)
    );

template<typename real_t>
const matrix_3X3<real_t> 
matrix_3X3<real_t>::identity(
    real_t(1), real_t(0), real_t(0),
    real_t(0), real_t(1), real_t(0),
    real_t(0), real_t(0), real_t(1)
    );

template<typename real_t>
bool operator==(
    const math::matrix_3X3<real_t>& lhs,
    const math::matrix_3X3<real_t>& rhs
    );

template<typename real_t>
inline bool operator!=(
    const math::matrix_3X3<real_t>& lhs,
    const math::matrix_3X3<real_t>& rhs
    );

/**
 * \fn  template<typename real_t> inline matrix3X3<real_t> operator+( const matrix3X3<real_t>& lhs,
 * const matrix3X3<real_t>& rhs )
 *
 * \brief   Addition operator.
 */
template<typename real_t>
inline
matrix_3X3<real_t>
operator+(
    const matrix_3X3<real_t>& lhs, 
    const matrix_3X3<real_t>& rhs
    );

/**
 * \fn  template<typename real_t> inline matrix3X3<real_t> operator-( const matrix3X3<real_t>& lhs,
 * const matrix3X3<real_t>& rhs )
 *
 * \brief   Subtraction operator.
 */
template<typename real_t>
inline
matrix_3X3<real_t>
operator-(
    const matrix_3X3<real_t>& lhs, 
    const matrix_3X3<real_t>& rhs
    );

/**
 * \fn  template<typename real_t> matrix3X3<real_t> operator-( const matrix3X3<real_t>& mtx )
 *
 * \brief   Unary negation operator.
 */
template<typename real_t>
matrix_3X3<real_t>
operator-(
    const matrix_3X3<real_t>& mtx
    );

/**
 * \fn  template<typename real_t> matrix3X3<real_t> operator*( const matrix3X3<real_t>& lhs,
 * const matrix3X3<real_t>& rhs )
 *
 * \brief   Multiplication operator of two matrices.
 */
template<typename real_t>
matrix_3X3<real_t>
operator*(
    const matrix_3X3<real_t>& lhs, 
    const matrix_3X3<real_t>& rhs
    );

/**
 * \fn  template<typename real_t> inline matrix3X3<real_t> operator*( real_t k,
 * const matrix3X3<real_t>& mtx )
 *
 * \brief   Multiplication operator with scalars.
 *
 */
template<typename real_t>
inline
matrix_3X3<real_t>
operator*(
    real_t k, 
    const matrix_3X3<real_t>& mtx
    );

/**
 * \fn  template<typename real_t> inline matrix3X3<real_t> operator*( const matrix3X3<real_t>& mtx,
 * real_t k )
 *
 * \brief   Multiplication operator with scalars.
 */
template<typename real_t>
inline
matrix_3X3<real_t>
operator*(
    const matrix_3X3<real_t>& mtx, 
    real_t k
    );

/**
 * \fn  template<typename real_t> inline vector3<real_t> operator*( const matrix3X3<real_t>& mtx,
 * const vector3<real_t>& vec )
 *
 * \brief   Multiplication operator with vector3 objects.
 */
template<typename real_t>
inline
vector3<real_t>
operator*(
    const matrix_3X3<real_t>& mtx,
    const vector3<real_t>& vec
    );

/**
 * \fn  template<typename real_t> matrix3X3<real_t> multiply_transpose( const matrix3X3<real_t>& lhs,
 * const matrix3X3<real_t>& rhs );
 *
 * \brief   Multiply the left side matrix with the transpose of the right side
 * 			matrix (M * N ^ T).
 */
template<typename real_t>
matrix_3X3<real_t>
multiply_transpose(
    const matrix_3X3<real_t>& lhs, 
    const matrix_3X3<real_t>& rhs
    );

/**
 * \fn  template<typename real_t> matrix3X3<real_t> transpose_multiply( const matrix3X3<real_t>& lhs,
 * const matrix3X3<real_t>& rhs );
 *
 * \brief   Multiply the transpose of the left side matrix with the
 * 			right side matrix (M ^ T * N).
 */
template<typename real_t>
matrix_3X3<real_t>
transpose_multiply(
    const matrix_3X3<real_t>& lhs, 
    const matrix_3X3<real_t>& rhs
    );

/**
 * \fn  template<typename real_t> matrix3X3<real_t> transpose_multiply_transpose( const matrix3X3<real_t>& lhs,
 * const matrix3X3<real_t>& rhs );
 *
 * \brief   Multiply the transpose of the left side matrix with the transpose of
 * 			the right side matrix.
 */
template<typename real_t>
matrix_3X3<real_t>
transpose_multiply_transpose(
    const matrix_3X3<real_t>& lhs, 
    const matrix_3X3<real_t>& rhs
    );

typedef matrix_3X3<float>    matrix_3X3F;

typedef matrix_3X3<double>   matrix_3X3D;

} // namespace math
} // namespace v8

#include "matrix3X3.inl"
