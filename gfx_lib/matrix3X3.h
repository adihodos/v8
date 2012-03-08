#pragma once

#include <cassert>
#include "gfx_misc.h"
#include "vector2.h"
#include "vector3.h"

namespace gfx {

/**
 * \class   
 *
 * \brief   Represents a 3 by 3 matrix, stored in row major format.
 */
template<typename real_t>
class matrix3X3 {
private :

    int index_at(int row, int col) const {
        return (row - 1) * 3 + col - 1;
    }

public:
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
    typedef matrix3X3<real_t>   matrix3X3_t;

    union {
        struct {
            float a11_, a12_, a13_;
            float a21_, a22_, a23_;
            float a31_, a32_, a33_;
        };
        float elements_[9];
    };

    static const matrix3X3_t zero;  ///< The zero matrix. */

    static const matrix3X3_t identity;  ///< The identity matrix. */

    /**
     * \fn    static const matrix3X3 matrix3X3::skew_symmetric(float a12, float a13, float a23)
     *
     * \brief Return a skew symmetric matrix. A skew symmetric matrix has
     *        A(i,j) = 0 when i = j and A(i, j) = -A(j, i) when i <> j.
     *
     */
    static matrix3X3<real_t> skew_symmetric(real_t u0, real_t u1, real_t u2) {
        return matrix3X3(
            real_t(0),  -u2,        u1,
            u2,         real_t(0),  -u0,
            -u1,        u0,         real_t(0)
        );
    }

    /**
     * \fn  static matrix3X3<real_t> matrix3X3::translation(real_t tx, real_t ty)
     *
     * \brief   Returns a matrix that can be used to translate affine points
     * 			in R3.
     */
    static matrix3X3<real_t> translation(real_t tx, real_t ty) {
        return matrix3X3(
            real_t(1), real_t(0), tx,
            real_t(0), real_t(1), ty,
            real_t(0), real_t(0), real_t(1)
            );
    }

    /**
     * \fn  static matrix3X3<real_t> matrix3X3::translation(const gfx::vector2<real_t>& tv)
     *
     * \brief   Returns a translation matrix, with the translating factors
     * 			specified by a vector2 object.
     */
    static matrix3X3<real_t> translation(const gfx::vector2<real_t>& tv) {
        return matrix3X3<real_t>::translation(tv.x_, tv.y_, tv.z_);
    }

    /**
     * \fn  static matrix3X3<real_t> matrix3X3::scale(real_t x, real_t y)
     *
     * \brief   Returns a matrix suitable for scaling an affine vector in R2.
     */
    static matrix3X3<real_t> scale(real_t x, real_t y) {
        return matrix3X3_t(
            x,          real_t(0),  real_t(0),
            real_t(0),  y,          real_t(0),
            real_t(0),  real_t(0),  real_t(1)
            );
    }

    /**
     * \fn  static const matrix3X3<real_t> matrix3X3::scale(real_t sx, real_t sy, real_t sz)
     *
     * \brief   Returns a scaling matrix for a vector in R3.
     */
    static matrix3X3<real_t> scale(real_t sx, real_t sy, real_t sz) {
        return matrix3X3_t(
            sx,         real_t(0),  real_t(0),
            real_t(0),  sy,         real_t(0),
            real_t(0),  real_t(0),  sz
            );
    }

    static matrix3X3<real_t> scale(const vector3<real_t>& sv) {
        return matrix3X3<real_t>::scale(sv.x_, sv.y_, sv.z_);
    }  

    /**
     * \fn  static const matrix3X3<real_t> matrix3X3::euler_xyz(real_t rx, real_t ry, real_t rz)
     *
     * \brief   Returns a rotation matrix, using Euler angles in xyz format.
     */
    static matrix3X3<real_t> euler_xyz(real_t rx, real_t ry, real_t rz);

    /**
     * \fn  static const matrix3X3<real_t> matrix3X3::rotation_x(real_t theta)
     *
     * \brief   Returns a matrix for a rotation around the x axis, with
     * 			an angle of theta radians.
     * 			
     * \param   theta   Angle of rotation, in radians.
     */
    static matrix3X3<real_t> rotation_x(real_t theta) {
        real_t sin_theta = std::sinf(theta);
        real_t cos_theta = std::cosf(theta);
        return matrix3X3_t(
            real_t(1), real_t(0), real_t(0),
            real_t(0), cos_theta, -sin_theta,
            real_t(0), sin_theta, cos_theta
        );
    }

    /**
     * \fn  static const matrix3X3<real_t> matrix3X3::rotation_y(real_t theta)
     *
     * \brief   Returns a matrix for a rotation around the y axis, with
     * 			an angle of theta radians.
     * 			
     * \param   theta   Angle of rotation, in radians.
     */
    static matrix3X3<real_t> rotation_y(real_t theta) {
        real_t sin_theta = std::sinf(theta);
        real_t cos_theta = std::cosf(theta);
        return matrix3X3_t(
            cos_theta, real_t(0), sin_theta,
            real_t(0), real_t(1), real_t(0),
            -sin_theta, real_t(0), cos_theta
        );
    }

    /**
     * \fn  static const matrix3X3<real_t> matrix3X3::rotation_z(real_t theta)
     *
     * \brief   Returns a matrix for a rotation around the y axis, with
     * 			an angle of theta radians.
     * 			
     * \param   theta   Angle of rotation, in radians.
     */
    static matrix3X3<real_t> rotation_z(real_t theta) {
        real_t sin_theta = std::sinf(theta);
        real_t cos_theta = std::cosf(theta);
        return matrix3X3_t(
            cos_theta, -sin_theta, real_t(0),
            sin_theta, cos_theta, real_t(0),
            real_t(0), real_t(0), real_t(1)
        );
    }

    /**
     * \fn  static matrix3X3<real_t> matrix3X3::axis_angle(const vector3<real_t>& axisv,
     * real_t theta);
     *
     * \brief   Make a rotation matrix out of an angle-axis representation.
     *
     * \param   axisv   Normalized vector that gives the rotation axis.
     * \param   theta   Rotation angle, in radians.
     */
    static matrix3X3<real_t> axis_angle(const vector3<real_t>& axisv, real_t theta);

    /**
     * \fn  static matrix3X3 matrix3X3::rotation(const vector3<real_t>& v1,
     * const vector3<real_t>& v2);
     *
     * \brief   Given 2 vectors v1 and v2, build a matrix that will rotate v1 into v2.
     *          If v1 and v2 are parallel, the function will return a  matrix representing
     *          a rotation of 0 radians around v1.
     */
    static matrix3X3<real_t> rotation(
        const vector3<real_t>& v1, 
        const vector3<real_t>& v2
        );

    /**
     * \fn  static matrix3X3<real_t> matrix3X3::planar_reflection(const vector3<real_t>& pn);
     *
     * \brief   Return a reflection matrix around a plane's normal.
     *
     * \author  Adi.hodos
     * \date    3/8/2012
     *
     * \param   pn  Normalized vector representing the plane's normal.
     */
    static matrix3X3<real_t> planar_reflection(const vector3<real_t>& pn);

    /**
     * \fn  static const matrix3X3<real_t> matrix3X3::tensor_product( const vector3<real_t>& u,
     * const vector3<real_t>& w );
     *
     * \brief   Return a matrix that is the tensor product of the two input vectors.
     * 			If u(u0, u1, u2) and v(v0, v1, v2) are two vectors
     * 			then the tensor product is u * v^T.
     */
    static const matrix3X3<real_t> tensor_product(
        const vector3<real_t>& u, 
        const vector3<real_t>& w
        );

    /**
     * \fn  matrix3X3::matrix3X3()
     *
     * \brief   Default constructor. Leaves elements uninitialized.
     */
    matrix3X3() {}

    matrix3X3(
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
    matrix3X3(const real_t* input, size_t count);

    /**
     * \fn  matrix3X3::matrix3X3(float a11, float a22, float a33)
     *
     * \brief   Construct a diagonal matrix, setting A(i,j) = 0 for every i <> j.
     */
    matrix3X3(real_t a11, real_t a22, real_t a33);

    /**
     * \fn  matrix3X3::matrix3X3( const vector3<real_t>& u, const vector3<real_t>& v,
     * const vector3<real_t>& w, bool column = true );
     *
     * \brief   Construct a matrix from a set of 3 vectors.
     *
     * \param   column  True - vectors represent columns, otherwise rows.
     */
    matrix3X3(
        const vector3<real_t>& u, 
        const vector3<real_t>& v, 
        const vector3<real_t>& w, 
        bool column = true
        );

    real_t& operator()(int row, int col) {
        return elements_[index_at(row, col)];
    }

    real_t operator()(int row, int col) const {
        return elements_[index_at(row, col)];
    }

    matrix3X3<real_t>& operator+=(const matrix3X3<real_t>& rhs);

    matrix3X3<real_t>& operator-=(const matrix3X3<real_t>& rhs);

    matrix3X3<real_t>& operator*=(real_t k);

    matrix3X3<real_t>& operator/=(real_t k);

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
        return !math::op_eq<real_t, is_floating_point>::result(determinant(), real_t(0));
    }

    /**
     * \fn  matrix3X3<real_t>& matrix3X3::invert()
     *
     * \brief   Inverts the matrix.
     */
    matrix3X3<real_t>& invert();

    /**
     * \fn  matrix3X3<real_t>& matrix3X3::transpose();
     *
     * \brief   Transpose the matrix and return a reference to it.
     */
    matrix3X3<real_t>& transpose();

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
    matrix3X3& ortho_normalize();

    /**
     * \fn  matrix3X3<real_t>& matrix3X3::set_row(int row, const real_t* data_ptr);
     *
     * \brief   Sets the contents of a row from an array of values.
     * 			
     * \param   row         The row index (1 based).
     * \param   data_ptr    Pointer to the array. Must not be null.
     */
    matrix3X3<real_t>& set_row(int row, const real_t* data_ptr);

    /**
     * \fn  matrix3X3<real_t>& matrix3X3::set_row(int row, const gfx::vector3<real_t>& vec);
     *
     * \brief   Sets contents of the specified row equal to the elements of a vector.
     * 			
     * \param   row Index of the row to set, using 1 based indexing.
     * \param   Source vector to copy elements from.
     */
    matrix3X3<real_t>& set_row(int row, const gfx::vector3<real_t>& vec) {
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
     * \fn  void matrix3X3::get_row(int row, gfx::vector3<real_t>* v3)
     *
     * \brief   Copies the contents of a row into a vector3.
     * 			
     * \param   row         The row index (1 based).
     * \param [in,out]  v3  Pointer to a vector3 object that receives the data.
     * 					Cannot be null.
     */
    void get_row(int row, gfx::vector3<real_t>* v3) {
        get_row(row, v3->elements_);
    }

    /**
     * \fn  matrix3X3<real_t>& matrix3X3::set_column(int column, const real_t* data_ptr)
     *
     * \brief Sets a column's data.
     *
     * \param column  The column index (1 based).
     * \param data_ptr  Pointer to source data. Must not be null.
     */
    matrix3X3<real_t>& set_column(int column, const real_t* data_ptr);

    /**
     * \fn  matrix3X3<real_t>& matrix3X3::set_column(int column, const gfx::vector3<real_t>& vec)
     *
     * \brief   Assigns the contents of a vector3 object to a column.
     *
     * \param   column  The column index(1 based).
     * \param   vec     The source vector for the assignment.
     */
    matrix3X3<real_t>& set_column(int column, const gfx::vector3<real_t>& vec) {
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
     * \fn  void matrix3X3::get_column(int column, gfx::vector3<real_t>* v3)
     *
     * \brief   Copies the contents of a column into a vector3 object.
     * \param   column      The column index(1 based).
     * \param [in,out]  v3  Pointer to a vector3 object. Must be non null.
     */
    void get_column(int column, gfx::vector3<real_t>* v3) {
        get_column(column, v3->elements_);
    }

    /**
     * \fn  gfx::vector2<real_t>* matrix3X3::transform_point(gfx::vector2* p) const
     *
     * \brief   Transforms a 2d affine point and returns a pointer to it.
     *
     * \param [in,out]  p   Pointer to the point. Must not be null.
     */
    gfx::vector2<real_t>* transform_point(gfx::vector2* p) const {
        p->x_ = a11_ * p->x_ + a12_ * p->y_ + a13_;
        p->y_ = a21_ * p->x_ + a22_ * p->y_ + a23_;
        return p;
    }

    /**
     * \fn  gfx::vector2* matrix3X3::transform_vector(gfx::vector2* p) const
     *
     * \brief   Transforms a 2d vector.
     */
    gfx::vector2* transform_vector(gfx::vector2* p) const {
        p->x_ = a11_ * p->x_ + a12_ * p->y_;
        p->y_ = a21_ * p->x_ + a12_ * p->y_;
    }
};

template<typename real_t>
const matrix3X3<real_t> matrix3X3<real_t>::zero(
    real_t(0), real_t(0), real_t(0),
    real_t(0), real_t(0), real_t(0),
    real_t(0), real_t(0), real_t(0)
    );

template<typename real_t>
const matrix3X3<real_t> matrix3X3<real_t>::identity(
    real_t(1), real_t(0), real_t(0),
    real_t(0), real_t(1), real_t(0),
    real_t(0), real_t(0), real_t(1)
    );

//
// static
template<typename real_t>
matrix3X3<real_t>
matrix3X3<real_t>::euler_xyz(
    real_t rx,
    real_t ry,
    real_t rz
    )
{
    const real_t sx = std::sinf(rx);
    const real_t cx = std::cosf(rx);
    const real_t sy = std::sinf(ry);
    const real_t cy = std::cosf(ry);
    const real_t sz = std::sinf(rz);
    const real_t cz = std::cosf(rz);

    return matrix3X3_t(
        cy * cz,                  -cy * sz,                  sy,
        sx * sy * cz + cx * sz,   -sx * sy * sz + cx * cz,  -sx * cy,
        -cx * sy * cz + sx * sz,    cx * sy * sz + sx * cz,   cx * cy
    );
}

//
// static
template<typename real_t>
matrix3X3<real_t>
matrix3X3<real_t>::axis_angle(const vector3<real_t>& axisv, real_t theta) {
    const real_t sin_theta = std::sinf(theta);
    const real_t cos_theta = std::cosf(theta);
    const real_t tval = real_t(1) - cos_theta;

    return matrix3X3(
        //
        // 1st row
        tval * axisv.x_ * axisv.x_ + cos_theta,
        tval * axisv.x_ * axisv.y_ - sin_theta * axisv.z_,
        tval * axisv.x_ * axisv.z_ + sin_theta * axisv.y_,
        //
        // 2nd row
        tval * axisv.x_ * axisv.y_ + sin_theta * axisv.z_,
        tval * axisv.y_ * axisv.y_ + cos_theta,
        tval * axisv.y_ * axisv.z_ - sin_theta * axisv.x_,
        //
        // 3rd row
        tval * axisv.x_ * axisv.z_ - sin_theta * axisv.y_,
        tval * axisv.y_ * axisv.z_ + sin_theta * axisv.x_,
        tval * axisv.z_ * axisv.z_ + cos_theta
        );
}

//
// static
template<typename real_t>
matrix3X3<real_t>
matrix3X3<real_t>::rotation(const vector3<real_t>& v1, const vector3<real_t>& v2) {
    //
    // Compute the angle between the two vectors.
    real_t angle = angle_of(v1, v2);
    if (math::zero_test<real_t, is_floating_point>::result(angle)) {
        //
        // Vectors are parallel, so return a matrix for a rotation around 
        // first vector with 0 radians.
        return axis_angle(v1, 0.0f);
    }

    //
    // Compute the cross product and normalize to get the rotation axis.
    return axis_angle(normal_of(cross_product(v1, v2)), angle);
}

//
// static
template<typename real_t>
matrix3X3<real_t>
matrix3X3<real_t>::planar_reflection(const vector3<real_t>& pn) {
    return matrix3X3_t(
        real_t(1) - real_t(2) * pn.x_ * pn.x_, 
        real_t(-2) * pn.x_ * pn.y_, 
        real_t(-2) * pn.x_ * pn.z_,

        real_t(-2) * pn.y_ * pn.x_, 
        real_t(-2) - real_t(2) * pn.y_ * pn.y_, 
        real_t(-2) * pn.y_ * pn.z_,

        real_t(-2) * pn.z_ * pn.x_, 
        real_t(-2) * pn.z_ * pn.y_, 
        real_t(1) - real_t(2) * pn.z_ * pn.z_
        );
}

//
// static
template<typename real_t>
matrix3X3<real_t>
matrix3X3<real_t>::tensor_product(
    const vector3<real_t>& u, 
    const vector3<real_t>& w
    ) {
    return matrix3X3_t(
            u.x_ * w.x_, u.x_ * w.y_, u.x_ * w.z_,
            u.y_ * w.x_, u.y_ * w.y_, u.y_ * w.z_,
            u.z_ * w.x_, u.z_ * w.y_, u.z_ * w.z_
            );
}

template<typename real_t>
matrix3X3<real_t>::matrix3X3(
    real_t a11, real_t a12, real_t a13,
    real_t a21, real_t a22, real_t a23,
    real_t a31, real_t a32, real_t a33
    )
{
    a11_ = a11; a12_ = a12; a13_ = a13;
    a21_ = a21; a22_ = a22; a23_ = a23;
    a31_ = a31; a32_ = a32; a33_ = a33;
}

template<typename real_t>
matrix3X3<real_t>::matrix3X3(const real_t* input, size_t count) {
    std::memcpy(elements_, input, 
        std::min(_countof(elements_), count) * sizeof(real_t));
}

template<typename real_t>
matrix3X3<real_t>::matrix3X3(real_t a11, real_t a22, real_t a33) 
    : a11_(a11), a22_(a22), a33_(a33) {
    a12_ = a13_ = a21_ = a23_ = a31_ = a32_ = real_t(0);
}

template<typename real_t>
matrix3X3<real_t>::matrix3X3(
    const vector3<real_t>& u, 
    const vector3<real_t>& v, 
    const vector3<real_t>& w, 
    bool column
    ) 
{
    if (column) {
        a11_ = u.x_;
        a12_ = v.x_;
        a13_ = w.x_;
        a21_ = u.y_;
        a22_ = v.y_;
        a23_ = w.y_;
        a31_ = u.z_;
        a32_ = v.z_;
        a33_ = w.z_;
    } else {
        a11_ = u.x_;
        a12_ = u.y_;
        a13_ = u.z_;
        a21_ = v.x_;
        a22_ = v.y_;
        a23_ = v.z_;
        a31_ = w.x_;
        a32_ = w.y_;
        a33_ = w.z_;
    }
}

template<typename real_t>
matrix3X3<real_t>& 
matrix3X3<real_t>::operator+=(const matrix3X3<real_t>& rhs) {
    a11_ += rhs.a11_; a12_ += rhs.a12_; a13_ += rhs.a13_;
    a21_ += rhs.a21_; a22_ += rhs.a22_; a23_ += rhs.a23_;
    a31_ += rhs.a13_; a32_ += rhs.a32_; a33_ += rhs.a33_;
    return *this;
}

template<typename real_t>
matrix3X3<real_t>& 
matrix3X3<real_t>::operator-=(const matrix3X3<real_t>& rhs) {
    a11_ -= rhs.a11_; a12_ -= rhs.a12_; a13_ -= rhs.a13_;
    a21_ -= rhs.a21_; a22_ -= rhs.a22_; a23_ -= rhs.a23_;
    a31_ -= rhs.a13_; a32_ -= rhs.a32_; a33_ -= rhs.a33_;
    return *this;
}

template<typename real_t>
matrix3X3<real_t>& 
matrix3X3<real_t>::operator*=(real_t k) {
    a11_ *= k; a12_ *= k; a13_ *= k;
    a21_ *= k; a22_ *= k; a23_ *= k;
    a31_ *= k; a32_ *= k; a33_ *= k;
    return *this;
}

template<typename real_t>
matrix3X3<real_t>& 
matrix3X3<real_t>::operator/=(real_t k) {
    using namespace implementation_details;
    const real_t dividend = transform_dividend_for_division<
        real_t, 
        is_floating_point
    >::transform(k);

    divide_helper<
        real_t,
        is_floating_point
    > div;

    a11_ = div::divide(a11_, k); 
    a12_ = div::divide(a12_, k); 
    a13_ = div::divide(a13_, k);
    a21_ = div::divide(a21_, k); 
    a22_ = div::divide(a22_, k); 
    a23_ = div::divide(a23_, k);
    a31_ = div::divide(a31_, k);
    a32_ = div::divide(a32_, k); 
    a33_ = div::divide(a33_, k);
    return *this;
}

template<typename real_t>
real_t matrix3X3<real_t>::determinant() const {
    const real_t A11 = a22_ * a33_ - a23_ * a32_;
    const real_t A12 = a23_ * a31_ + a21_ * a33_;
    const real_t A13 = a21_ * a32_ - a22_ * a31_;

    return a11_ * A11 + a12_ * A12 + a13_ * A13;
}

template<typename real_t>
matrix3X3<real_t>& 
matrix3X3<real_t>::invert() {
    real_t det = determinant();

    matrix3X3_t adjoint_mtx(adjoint_of(*this));
    adjoint_mtx /= det;
    return adjoint_mtx;
}

template<typename real_t>
matrix3X3<real_t>& 
matrix3X3<real_t>::transpose() {
    using namespace implementation_details;
    swap(a12_, a21_);
    swap(a13_, a31_);
    swap(a23_, a32_);
    return *this;	
}

template<typename real_t>
matrix3X3<real_t>&
matrix3X3<real_t>::ortho_normalize() {
    //
    // Let M = [v0, v1, v2]. This function will output a matrix M1 = [q0, q1, q2]
    // such that q0, q1, q2 are orthonormal vectors.
    //
    // q0 = v0 / || v0 ||;
    // q1 = (v1 - proj(v1, q0)) / || v1 - proj(v1, q0) ||;
    // q2 = (v2 - proj(v2, q0) - proj(v2, q1)) / || (v2 - proj(v2, q0) - proj(v2, q1)) ||;

    //
    // q0
    using namespace implementation_details;
    real_t norm = transform_dividend_for_division<
        real_t, 
        is_floating_point
    >::transform(std::sqrtf(a11_ * a11_ + a21_ * a21_ + a31_ * a31_));

    divide_helper<
        real_t,
        is_floating_point
    > div;
    a11_ = div::divide(a11_, norm);
    a21_ = div::divide(a21_, norm);
    a31_ = div::divide(a31_, norm);

    //
    // q1
    real_t sum = a11_ * a12_ + a21_ * a22_ + a31_ * a32_;

    a12_ -= a11_ * sum;
    a22_ -= a21_ * sum;
    a32_ -= a31_ * sum;

    inv_norm = transform_dividend_for_division<
        real_t, 
        is_floating_point
    >::transform(std::sqrtf(a12_ * a12_ + a22_ * a22_ + a32_ * a32_));

    a12_ = div::divide(a12_, norm);
    a22_ = div::divide(a22_, norm);
    a32_ = div::divide(a22_, norm);

    //
    // q2
    sum = a11_ * a13_ + a21_ * a23_ + a31_ * a33_;
    real_t sum2 = a12_ * a13_ + a22_ * a23_ + a32_ * a33_;

    a13_ -= (a11_ * sum + a12_ * sum2);
    a23_ -= (a21_ * sum + a22_ * sum2);
    a33_ -= (a31_ * sum + a32_ * sum2);

    norm = transform_dividend_for_division<
        real_t, 
        is_floating_point
    >::transform(std::sqrtf(a13_ * a13_ + a23_ * a23_ + a33_ * a33_));

    a13_ = div::divide(a13_, norm);
    a23_ = div::divide(a23_, norm);
    a33_ = div::divide(a33_, norm);

    return *this;
}

template<typename real_t>
matrix3X3<real_t>&
matrix3X3<real_t>::set_row(int row, const real_t* data_ptr) {
    int row_index = (row - 1) * 3;
    elements_[row_index] = data_ptr[0];
    elements_[row_index + 1] = data_ptr[1];
    elements_[row_index + 2] = data_ptr[2];
    return *this;
}

template<typename real_t>
void matrix3X3<real_t>::get_row(int row, real_t* ptr) const {
    const int row_index = (row - 1) * 3;
    ptr[0] = elements_[row_index];
    ptr[1] = elements_[row_index + 1];
    ptr[2] = elements_[row_index + 2];
}

template<typename real_t>
matrix3X3<real_t>& 
matrix3X3<real_t>::set_column(int column, const real_t* data_ptr) {
    elements_[column - 1] = data_ptr[0];
    elements_[column - 1 + 3] = data_ptr[1];
    elements_[column - 1 + 6] = data_ptr[2];
    return *this;
}

template<typename real_t>
void matrix3X3<real_t>::get_column(int column, real_t* data_ptr) {
    const int c_idx = column - 1;
    data_ptr[0] = elements_[c_idx];
    data_ptr[1] = elements_[3 + c_idx];
    data_ptr[2] = elements_[6 + c_idx];
}

/**
 * \fn  template<typename real_t> inline matrix3X3<real_t> operator+( const matrix3X3<real_t>& lhs,
 * const matrix3X3<real_t>& rhs )
 *
 * \brief   Addition operator.
 */
template<typename real_t>
inline
matrix3X3<real_t>
operator+(
    const matrix3X3<real_t>& lhs, 
    const matrix3X3<real_t>& rhs
    ) 
{
    matrix3X3<real_t> result(lhs);
    result += rhs;
    return result;
}

/**
 * \fn  template<typename real_t> inline matrix3X3<real_t> operator-( const matrix3X3<real_t>& lhs,
 * const matrix3X3<real_t>& rhs )
 *
 * \brief   Subtraction operator.
 */
template<typename real_t>
inline
matrix3X3<real_t>
operator-(
    const matrix3X3<real_t>& lhs, 
    const matrix3X3<real_t>& rhs
  ) 
{
    matrix3X3<real_t> result(lhs);
    result -= rhs;
    return result;
}

/**
 * \fn  template<typename real_t> matrix3X3<real_t> operator-( const matrix3X3<real_t>& mtx )
 *
 * \brief   Unary negation operator.
 */
template<typename real_t>
matrix3X3<real_t>
operator-(
      const matrix3X3<real_t>& mtx
    ) 
{
    return matrix3X3<real_t>(-mtx.a11_, -mtx.a12_, -mtx.a13_,
                             -mtx.a21_, -mtx.a22_, -mtx.a23_,
                             -mtx.a31_, -mtx.a32_, -mtx.a33_
                             );
}

/**
 * \fn  template<typename real_t> matrix3X3<real_t> operator*( const matrix3X3<real_t>& lhs,
 * const matrix3X3<real_t>& rhs )
 *
 * \brief   Multiplication operator of two matrices.
 */
template<typename real_t>
matrix3X3<real_t>
operator*(
    const matrix3X3<real_t>& lhs, 
    const matrix3X3<real_t>& rhs
  )
{
    matrix3X3 res;
    res.a11_ = lhs.a11_ * rhs.a11_ + lhs.a12_ * rhs.a21_ + lhs.a13_ * rhs.a31_;
    res.a12_ = lhs.a11_ * rhs.a12_ + lhs.a12_ * rhs.a22_ + lhs.a13_ * rhs.a32_;
    res.a13_ = lhs.a11_ * rhs.a13_ + lhs.a12_ * rhs.a23_ + lhs.a13_ * rhs.a33_;

    res.a21_ = lhs.a21_ * rhs.a11_ + lhs.a22_ * rhs.a21_ + lhs.a23_ * rhs.a31_;
    res.a22_ = lhs.a21_ * rhs.a12_ + lhs.a22_ * rhs.a22_ + lhs.a23_ * rhs.a32_;
    res.a23_ = lhs.a21_ * rhs.a13_ + lhs.a22_ * rhs.a23_ + lhs.a23_ * rhs.a33_;

    res.a31_ = lhs.a31_ * rhs.a11_ + lhs.a32_ * rhs.a21_ + lhs.a33_ * rhs.a31_;
    res.a32_ = lhs.a31_ * rhs.a12_ + lhs.a32_ * rhs.a22_ + lhs.a33_ * rhs.a32_;
    res.a33_ = lhs.a31_ * rhs.a13_ + lhs.a32_ * rhs.a23_ + lhs.a33_ * rhs.a33_;

    return res;
}

/**
 * \fn  template<typename real_t> inline matrix3X3<real_t> operator*( real_t k,
 * const matrix3X3<real_t>& mtx )
 *
 * \brief   Multiplication operator with scalars.
 *
 */
template<typename real_t>
inline
matrix3X3<real_t>
operator*(
    real_t k, 
    const matrix3X3<real_t>& mtx
  ) 
{
    matrix3X3 result(mtx);
    result *= k;
    return result;
}

/**
 * \fn  template<typename real_t> inline matrix3X3<real_t> operator*( const matrix3X3<real_t>& mtx,
 * real_t k )
 *
 * \brief   Multiplication operator with scalars.
 */
template<typename real_t>
inline
matrix3X3<real_t>
operator*(
    const matrix3X3<real_t>& mtx, 
    real_t k
  ) 
{
    return k * mtx;
}

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
    const matrix3X3<real_t>& mtx,
    const vector3<real_t>& vec
  )
{
  return vector3_t(
    mtx.a11_ * vec.x_ + mtx.a12_ * vec.y_ + mtx.a13_ * vec.z_,
    mtx.a21_ * vec.x_ + mtx.a22_ * vec.y_ + mtx.a23_ * vec.z_,
    mtx.a31_ * vec.x_ + mtx.a32_ * vec.y_ + mtx.a33_ * vec.z_
    );
}

/**
 * \fn  template<typename real_t> inline matrix3X3<real_t> transpose_of(const matrix3X3<real_t>& mtx)
 *
 * \brief   Transpose of the given mtx.
 */
template<typename real_t>
inline
matrix3X3<real_t>
transpose_of(const matrix3X3<real_t>& mtx) {
  return matrix3X3_t(
      mtx.a11_, mtx.a21_, mtx.a31_,
      mtx.a12_, mtx.a22_, mtx.a32_,
      mtx.a13_, mtx.a23_, mtx.a33_
      );
}

/**
 * \fn  template<typename real_t> matrix3X3<real_t> adjoint_of( const matrix3X3<real_t>& )
 *
 * \brief   Returns the adjoint matrix of the given matrix.
 */
template<typename real_t>
matrix3X3<real_t>
adjoint_of(
    const matrix3X3<real_t>&
  )
{
    real_t A11 = mtx.a22_ * mtx.a33_ - mtx.a23_ * mtx.a32_;
    real_t A12 = mtx.a23_ * mtx.a31_ + mtx.a21_ * mtx.a33_;
    real_t A13 = mtx.a21_ * mtx.a32_ - mtx.a22_ * mtx.a31_;
    real_t A21 = mtx.a13_ * mtx.a32_ - mtx.a12_ * mtx.a33_;
    real_t A22 = mtx.a11_ * mtx.a33_ - mtx.a13_ * mtx.a31_;
    real_t A23 = mtx.a12_ * mtx.a31_ - mtx.a11_ * mtx.a32_;
    real_t A31 = mtx.a12_ * mtx.a23_ - mtx.a13_ * mtx.a22_;
    real_t A32 = mtx.a13_ * mtx.a21_ - mtx.a11_ * mtx.a23_;
    real_t A33 = mtx.a11_ * mtx.a12_ - mtx.a12_ * mtx.a21_;

    return matrix3X3_t(A11, A21, A31, A12, A22, A32, A13, A23, A33);
}

typedef matrix3X3<float>    matrix3X3F;

typedef matrix3X3<double>   matrix3X3D;

} /* namespace gfx */
