#pragma once

#include <cmath>
#include <cstdint>

#include "v8/base/count_of.h"
#include "v8/base/fundamental_types.h"
#include "v8/math/math_utils.h"
#include "v8/math/vector2.h"

namespace v8 { namespace math {

/**
 * \brief A two by three matrix for applying linear/affine
 *      transformations to R^2 vectors/points. The matrix is
 *      stored in row major format. It follows the convention
 *      that it multiplies column vectors, on the right side.
 *      When concatenating matrices representings a sequence
 *      of transformations, the matrices must be multiplied from
 *      last to first. So the sequence S1, S2, S3, ... S(n - 1), Sn
 *      must be concatenated as :
 *      Sn * S(n-1) * S(n -2) * ... * S3 * S2 * S1.
 */
template<typename real_t>
class matrix_2X3 {
public :
    enum {
        is_floating_point = base::is_floating_point_type<real_t>::Yes
    };

    union {
        struct {
            real_t a11_; real_t a12_; real_t a13_;
            real_t a21_; real_t a22_; real_t a23_;
        };
        real_t elements_[6];
    };

    /**
     * \brief Alias for the type of elements of the matrix.
     */
    typedef real_t  element_type;

    static const matrix_2X3<real_t> zero;

    static const matrix_2X3<real_t> identity;

    /**
     * \brief Default constructor, leaves elements uninitialized.
     */
    matrix_2X3() {}

    /**
     * \brief Construct with six explicit values.
     */
    inline matrix_2X3(
        real_t a11, real_t a12, real_t a13,
        real_t a21, real_t a22, real_t a23
        );

    /**
     * \brief Construct from a matrix with convertible element type.
     */
    template<typename real_u>
    inline matrix_2X3(const matrix_2X3<real_u>& other);

    /**
     * \brief Construct from an array of existing values.
     * \param[in] data Pointer to existing data.
     * \param[in] count Number of entries in the data array.
     */
    inline matrix_2X3(const real_t* data, uint32_t count);

    /**
     * \brief Indexing operator.
     * \param[in] row Row of the accessed element, 
     *              using <b>1 based indexing</b>.
     * \param[in] col Column of the accessed element,
     *              using <b>1 based indexing</b>.
     */
    real_t& operator()(uint32_t row, uint32_t col) {
        return elements_[(row - 1) * 3 + (col - 1)];
    }

    /**
     * \brief Indexing operator.
     * \param[in] row Row of the accessed element, 
     *              using <b>1 based indexing</b>.
     * \param[in] col Column of the accessed element,
     *              using <b>1 based indexing</b>.
     */
    const real_t& operator()(uint32_t row, uint32_t col) const {
        return elements_[(row - 1) * 3 + (col - 1)];
    }

    template<typename real_u>
    inline matrix_2X3<real_t>& operator=(
        const matrix_2X3<real_u>& convertible
        );

    template<typename real_u>
    inline matrix_2X3<real_t>& operator+=(
        const matrix_2X3<real_u>& conv
        );

    template<typename real_u>
    inline matrix_2X3<real_t>& operator-=(
        const matrix_2X3<real_u>& conv
        );

    template<typename real_u>
    inline matrix_2X3<real_t>& operator*=(
        real_u conv_scalar
        );

    template<typename real_u>
    inline matrix_2X3<real_t>& operator/=(
        real_u conv_scalar
        );

    /**
     * \brief Set this matrix to the null matrix.
     */
    inline matrix_2X3<real_t>& make_zero();

    /**
     * \brief Set this matrix to the identity matrix.
     */
    inline matrix_2X3<real_t>& make_identity();

    /**
     * \brief Builds a rotation matrix.
     * \param[in] theta Angle of rotation, <b>in radians</b>.
     * \remarks Rotation is computed counter-clockwise.
     */
    inline matrix_2X3<real_t>& make_rotate(float theta);

    /**
     * \brief Builds a scaling matrix.
     * \param[in] sx Scale factor along the X axis.
     * \param[in] sy Scale factor along the Y axis.
     */
    inline matrix_2X3<real_t>& make_scale(real_t sx, real_t sy);

    /**
     * \brief Builds a scaling matrix from a vector2 object.
     * \param[in] sv Two component vector, containing scale
     *              factors along the X and Y axes.
     */
    matrix_2X3<real_t>& make_scale(const vector2<real_t>& sv) {
        return make_scale(sv.x_, sv.y_);
    }

    /**
     * \brief Builds a translation matrix.
     * \param[in] tx Translation component along the <b>X axis</b>.
     * \param[in] ty Translation component along the <b>Y axis</b>.
     */
    inline matrix_2X3<real_t>& make_translate(real_t tx, real_t ty);

    /**
     * \brief Builds a translation matrix from a vector2 object.
     * \param[in] sv Two component vector, containing translation
     *          factors along the X and Y axes.
     */
    matrix_2X3<real_t>& make_translate(const vector2<real_t>& sv) {
        return make_translate(sv.x_, sv.y_);
    }

    /**
     * \brief Builds a reflection matrix across a line with the direction
     *      vector given by <b>dir</b>.
     * \param[in] dir Direction vector of the reflection line.
     * \remarks The formula for the reflection is derived in the following way:
     *          let N be the unit vector orthogonal to the dir vector
     *          [ 1 - 2 * N.x * N.x     -2 * N.x * N.y     0 ]
     *          [ -2 * N.x * N.y        1 - 2 * N.y * N.y  0 ]
     */
    inline matrix_2X3<real_t>& make_reflect(const vector2<real_t>& dir);

    /**
     * \brief Builds a reflection matrix across a line that makes an angle
     *          of <b>theta radians</b> with the X axis.
     * \param[in] theta Angle <b>in radians</b> of the reflection line
     *          with the X axis.
     * \remarks The reflection matrix is build with the following formula :
     *          [ cos(2 * theta)    sin(2 * theta)  ]
     *          [ sin(2 * theta)    -cos(2 * theta) ]
     */          
    inline matrix_2X3<real_t>& make_reflect(real_t theta);

    /**
     * \brief Builds a reflection matrix across a line with the
     *        specified slope.
     * \param[in] slope Slope of the reflection line.
     * \remarks The reflection matrix is build with the following formula :
     *          [ (1 - slope^2) / (1 + slope^2)     2 * slope / (1 + slope^2) ]
     *          [ 2 * slope / ( 1 + slope^2)    (slope^2 - 1) / (1 + slope^2) ]
     */          
    inline matrix_2X3<real_t>& make_reflect_slope(real_t slope);

    /**
     * \brief Transform a 2D vector (translation has no effect).
     * \param[in] vec Pointer to a 2D vector object.
     */
    template<typename real_u>
    inline matrix_2X3<real_t>& transform_vector(vector2<real_u>* vec);

    /**
     * \brief Transform a 2D point (applies translation).
     * \param[in] pt Pointer to a vector2 object, representing a point.
     */
    template<typename real_u>
    inline matrix_2X3<real_t>& transform_point(vector2<real_u>* pt);
}; 

template<typename real_t>
const matrix_2X3<real_t> 
matrix_2X3<real_t>::zero(real_t(0), real_t(0), real_t(0),
                         real_t(0), real_t(0), real_t(0));

template<typename real_t>
const matrix_2X3<real_t>
matrix_2X3<real_t>::identity(real_t(1), real_t(0), real_t(0),
                             real_t(0), real_t(1), real_t(0));

template<typename real_t>
inline bool operator==(
    const matrix_2X3<real_t>& lhs,
    const matrix_2X3<real_t>& rhs
    );

template<typename real_t>
inline bool operator!=(
    const matrix_2X3<real_t>& lhs,
    const matrix_2X3<real_t>& rhs
    );

template<typename real_t>
inline matrix_2X3<real_t> operator+(
    const matrix_2X3<real_t>& lhs,
    const matrix_2X3<real_t>& rhs
    );

template<typename real_t>
inline matrix_2X3<real_t> operator-(
    const matrix_2X3<real_t>& lhs,
    const matrix_2X3<real_t>& rhs
    );

template<typename real_t, typename real_u>
inline matrix_2X3<real_t> operator*(
    real_u scalar,
    const matrix_2X3<real_t>& mtx
    );

template<typename real_t, typename real_u>
inline matrix_2X3<real_t> operator*(
    const matrix_2X3<real_t>& mtx,
    real_u scalar
    );

template<typename real_t, typename real_u>
inline matrix_2X3<real_t> operator/(
    const matrix_2X3<real_t>& mtx,
    real_u scalar
    );

} // namespace math
} // namespace v8

