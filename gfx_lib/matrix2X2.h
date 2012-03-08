#pragma once

#include <cmath>
#include <memory.h>
#include "details.h"
#include "math.h"
#include "vector2.h"

namespace gfx {

/**
 * \class   matrix_2X2.
 *
 * \brief   A 2x2 matrix, stored in row major format.
 */
template<typename real_t>
class matrix_2X2 {
public :
    enum {
        is_floating_point = implementation_details::types_eq<real_t, float>::result
                            || implementation_details::types_eq<real_t, double>::result
                            || implementation_details::types_eq<real_t, long double>::result
    };

    union {
        struct {
            real_t  a11_;
            real_t  a12_;
            real_t  a21_;
            real_t  a22_;
        };
        real_t elements_[4];
    };

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
     * \fn  matrix_2X2::matrix_2X2( const gfx::vector2<real_t>& v1, const gfx::vector2<real_t>& v2,
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
        const gfx::vector2<real_t>& v1, 
        const gfx::vector2<real_t>& v2,
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
     * \fn  void matrix_2X2::make_tensor_product( const gfx::vector2<real_t>& u,
     * const gfx::vector2<real_t>& v );
     *
     * \brief   Makes the matrix equal to the tensor product of the two vectors.
     * 			Given two vectors u and v, their tensor product is
     * 			u * v ^ T.
     *
     */
    void make_tensor_product(
        const gfx::vector2<real_t>& u, 
        const gfx::vector2<real_t>& v
        );

    /**
     * \fn  void matrix_2X2::make_rotation(float theta);
     *
     * \brief   Makes a rotation matrix, counter clockwise if theta > 0,
     * 			clockwise if theta < 0.
     */
    void make_rotation(float theta);

    matrix_2X2<real_t>& operator+=(const matrix_2X2<real_t>& right);

    matrix_2X2<real_t>& operator-=(const matrix_2X2<real_t>& right);

    matrix_2X2<real_t>& operator*=(real_t k);

    matrix_2X2<real_t>& operator/=(real_t k);

    real_t determinant() const {
        return a11_ * a22_ - a12_ * a21_;
    }

    bool is_invertible() const {
        return !math::op_eq<real_t, is_floating_point>::result(determinant(), real_t(0));
    }

    real_t trace() const {
        return a11_ + a22_;
    }

    matrix_2X2<real_t>& invert();

    matrix_2X2<real_t>& transpose() {
        implementation_details::swap(a12_, a21_);
        return *this;
    }
};

template<typename real_t>
const matrix_2X2<real_t> matrix_2X2<real_t>::zero(
    real_t(0), real_t(0),
    real_t(0), real_t(0)
    );

template<typename real_t>
const matrix_2X2<real_t> matrix_2X2<real_t>::identity(
    real_t(1), real_t(0),
    real_t(0), real_t(1)
    );

template<typename real_t>
inline
gfx::matrix_2X2<real_t>
gfx::operator+(
    const gfx::matrix_2X2<real_t>& lhs,
    const gfx::matrix_2X2<real_t>& rhs
    );

template<typename real_t>
inline
gfx::matrix_2X2<real_t>
gfx::operator-(
    const gfx::matrix_2X2<real_t>& lhs,
    const gfx::matrix_2X2<real_t>& rhs
    );

template<typename real_t>
gfx::matrix_2X2<real_t>
gfx::operator*(
    const gfx::matrix_2X2<real_t>& lhs,
    const gfx::matrix_2X2<real_t>& rhs
    );

template<typename real_t>
inline
gfx::matrix_2X2<real_t>
gfx::operator*(
    real_t k,
    const gfx::matrix_2X2<real_t>& rhs
    );

template<typename real_t>
inline
gfx::matrix_2X2<real_t>
gfx::operator*(
    const gfx::matrix_2X2<real_t>& lhs,
    real_t k
    );

template<typename real_t>
inline
gfx::vector2<real_t>
operator*(
    const gfx::matrix_2X2<real_t>& mtx,
    const gfx::vector2<real_t>& vec
    );

template<typename real_t>
inline
gfx::matrix_2X2<real_t>
operator/(
    const gfx::matrix_2X2<real_t>& lhs,
    real_t k
    );

template<typename real_t>
gfx::matrix_2X2<real_t>
adjoint_of(const gfx::matrix_2X2<real_t>& mtx);

template<typename real_t>
inline
gfx::matrix_2X2<real_t>
transpose_of(const gfx::matrix_2X2<real_t>& mtx);

template<typename real_t>
inline
gfx::matrix_2X2<real_t>
inverse_of(const gfx::matrix_2X2<real_t>& mtx);

/**
 * \fn  template<typename real_t> gfx::matrix_2X2<real_t> multiply_transpose(const gfx::matrix_2X2<real_t>& other);
 *
 * \brief   Multiply the left matrix with the transpose of the right matrix, 
 * 			that is M * N ^ T.
 */
template<typename real_t>
gfx::matrix_2X2<real_t>
multiply_transpose(
    const gfx::matrix_2X2<real_t>& lhs, 
    const gfx::matrix_2X2<real_t>& rhs
    );

/**
 * \fn  template<typename real_t> gfx::matrix_2X2<real_t> transpose_multiply( const gfx::matrix_2X2<real_t>& lhs,
 * const gfx::matrix_2X2<real_t>& rhs );
 *
 * \brief   Multiply the transpose of the left matrix with the right matrix.
 */
template<typename real_t>
gfx::matrix_2X2<real_t>
transpose_multiply(
    const gfx::matrix_2X2<real_t>& lhs, 
    const gfx::matrix_2X2<real_t>& rhs
    );

/**
 * \fn  template<typename real_t> gfx::matrix_2X2<real_t> transpose_multiply_transpose( const gfx::matrix_2X2<real_t>& lhs,
 * const gfx::matrix_2X2<real_t>& rhs );
 *
 * \brief   Multiply the transpose of the left hand matrix with the transpose
 * 			of the right hand matrix.
 */
template<typename real_t>
gfx::matrix_2X2<real_t>
transpose_multiply_transpose(
    const gfx::matrix_2X2<real_t>& lhs, 
    const gfx::matrix_2X2<real_t>& rhs
    );

typedef matrix_2X2<float>       matrix_2X2F;

typedef matrix_2X2<double>      matrix_2X2D;

} // namespace gfx