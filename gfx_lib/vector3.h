/**
 * \file    vector3.h
 *
 * \brief   Declares the vector 3 class and related functions.
 */
#pragma once

#include <cassert>
#include <cstring>
#include <algorithm>
#include "details.h"
#include "gfx_misc.h"
#include "gfx_math.h"

namespace gfx {

/**
 * \class   vector3
 *
 * \brief   Represents a vector/point in R3. It is up to the user of the class
 * 			to make the distinction in code. A vector has a direction, 
 * 			a length(magnitude)	and a way. A point has only the property of 
 * 			location. So operations	like length(), normalize(), addition, etc
 * 			are not valid for points. Yet many operations are common, so rather
 * 			then creating a point class and duplicating much of the code, it is 
 * 			up to the user to choose the meaning and apply the correct 
 * 			operations in code.
 */
template<typename real_t>
class vector3 {
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
    typedef real_t          element_type;
    typedef real_t&         reference;
    typedef const real_t&   const_reference;
    typedef vector3<real_t> vector3_t;

    union {
        struct {
            real_t x_;
            real_t y_;
            real_t z_;
        };
        real_t elements_[3];	///< Allows array like access to the elements */
    };

    static const vector3_t zero;  ///< The null vector (0, 0, 0) of corresponding type */

    static const vector3_t unit_x;	///< The unit vector along the x axis (1, 0, 0) */

    static const vector3_t unit_y;	///< The unit vector along the y axis (0, 1, 0) */
  
    static const vector3_t unit_z;	///< The unit vector along the z axis (0, 0, 1) */

    /**
     * \fn  vector3::vector3()
     *
     * \brief   Default constructor. Leaves components uninitialized.
     */
    vector3() {}

    /**
     * \fn  vector3::vector3(real_t x, real_t y, real_t z)
     *
     * \brief   Construct from 3 components.
     */
    vector3(real_t x, real_t y, real_t z) : x_(x), y_(y), z_(z) {}

    /**
     * \fn  inline vector3::vector3(const real_t* input, size_t count);
     *
     * \brief   Construct from an array of values.
     *
     * \param   input   Pointer to an array of values.
     * \param   count   Number of elements in the array.
     */
    vector3(const real_t* input, size_t count);

    /**
     * \fn  inline vector3<real_type>& vector3::operator+=(const vector3<real_type>& rhs);
     *
     * \brief   Addition assignment operator.
     */
    inline vector3<real_t>& operator+=(const vector3<real_t>& rhs);

    /**
     * \fn  inline vector3<real_t>& vector3::operator-=(const vector3<real_t>& rhs);
     *
     * \brief   Subtraction assignment operator.
     */
    inline vector3<real_t>& operator-=(const vector3<real_t>& rhs);

    /**
     * \fn  inline vector3<real_t>& vector3::operator*=(float k);
     *
     * \brief   Scalar multiplication assignment operator.
     */
    inline vector3<real_t>& operator*=(real_t k);

    /**
     * \fn  inline vector3<real_t>& vector3::operator/=(float k);
     *
     * \brief   Scalar division assignment operator.
     */
    inline vector3<real_t>& operator/=(real_t k);

    /**
     * \fn  inline float vector3::sum_components_squared() const;
     *
     * \brief   Returns the sum of the squares of the vector's components,
     * 			that is x^2 + y^2 + z^2 .
     */
    inline real_t sum_components_squared() const;

    /**
     * \fn  inline float vector3::magnitude() const;
     *
     * \brief   Computes the magnitude of the vector.
     */
    inline real_t magnitude() const;

    /**
     * \fn  inline vector3& vector3::normalize();
     *
     * \brief   Normalizes the vector (v = v / ||v||);
     */
    inline vector3& normalize();
};

/**
 * \fn  template<typename real_t> inline bool operator==(const gfx::vector3<real_t>& lhs,
 * const gfx::vector3<real_t>& rhs);
 *
 * \brief   Equality operator.
 */
template<typename real_t>
inline
bool
operator==(
    const gfx::vector3<real_t>& lhs, 
    const gfx::vector3<real_t>& rhs
    );

/**
 * \fn  template<typename real_t> inline bool operator!=(const gfx::vector3<real_t>& lhs,
 * const gfx::vector3<real_t>& rhs);
 *
 * \brief   Inequality operator.
 */
template<typename real_t>
inline
bool
operator!=(
    const gfx::vector3<real_t>& lhs, 
    const gfx::vector3<real_t>& rhs
    );

/**
 * \fn  template<typename real_t> inline gfx::vector3<real_t> operator+(const gfx::vector3<real_t>& lhs,
 * const gfx::vector3<real_t>& rhs);
 *
 * \brief   Addition operator.
 */
template<typename real_t>
inline
gfx::vector3<real_t>
operator+(
    const gfx::vector3<real_t>& lhs, 
    const gfx::vector3<real_t>& rhs
    );

/**
 * \fn  template<typename real_t> inline gfx::vector3<real_t> operator-(const gfx::vector3<real_t>& lhs,
 * const vector3<real_t>& rhs);
 *
 * \brief   Subtraction operator.
 */
template<typename real_t>
inline
gfx::vector3<real_t>
operator-(
    const gfx::vector3<real_t>& lhs, 
    const vector3<real_t>& rhs
    );

/**
 * \fn  template<typename real_t> inline gfx::vector3<real_t> operator-(const gfx::vector3<real_t>& vec);
 *
 * \brief   Negation operator.
 */
template<typename real_t>
inline
gfx::vector3<real_t>
operator-(
    const gfx::vector3<real_t>& vec
    );

/**
 * \fn  template<typename real_t> inline gfx::vector3<real_t> operator*(const gfx::vector3<real_t>& vec,
 * float k);
 *
 * \brief   Scalar multiplication operator.
 */
template<typename real_t>
inline
gfx::vector3<real_t>
operator*(
    const gfx::vector3<real_t>& vec, 
    real_t k
    );

/**
 * \fn  template<typename real_t> inline gfx::vector3<real_t> operator*(float k,
 * const gfx::vector3<real_t>& vec);
 *
 * \brief   Scalar multiplication operator.
 */
template<typename real_t>
inline
gfx::vector3<real_t>
operator*(
    real_t k, 
    const gfx::vector3<real_t>& vec
    );

/**
 * \fn  template<typename real_t> inline gfx::vector3<real_t> operator/(const gfx::vector3<real_t>& vec,
 * float k);
 *
 * \brief   Scalar division operator.
 */
template<typename real_t>
inline
gfx::vector3<real_t>
operator/(
    const gfx::vector3<real_t>& vec, 
    real_t k
    );

/**
 * \fn  template<typename real_t> inline float dot_product(const gfx::vector3<real_t>& lhs,
 * const gfx::vector3<real_t>& rhs);
 *
 * \brief   Dot product.
 */
template<typename real_t>
inline
real_t
dot_product(
    const gfx::vector3<real_t>& lhs, 
    const gfx::vector3<real_t>& rhs
    );

/**
 * \fn  template<typename real_t> inline gfx::vector3<real_t> cross_product(const gfx::vector3<real_t>& lhs,
 * const gfx::vector3<real_t>& rhs);
 *
 * \brief   Cross product.
 */
template<typename real_t>
inline
gfx::vector3<real_t>
cross_product(
    const gfx::vector3<real_t>& lhs, 
    const gfx::vector3<real_t>& rhs
    );

/**
 * \fn  template<typename real_t> inline bool ortho_test( const gfx::vector3<real_t>& lhs,
 * const gfx::vector3<real_t>& rhs );
 *
 * \brief   Tests if vectors are orthogonal.
 */
template<typename real_t>
inline
bool
ortho_test(
    const gfx::vector3<real_t>& lhs, 
    const gfx::vector3<real_t>& rhs
    );

/**
 * \fn  template<typename real_t> inline float angle_of( const gfx::vector3<real_t>& lhs,
 * const gfx::vector3<real_t>& rhs );
 *
 * \brief   Computes the angle between the two vectors.
 */
template<typename real_t>
inline
float
angle_of(
    const gfx::vector3<real_t>& lhs, 
    const gfx::vector3<real_t>& rhs
    );

/**
 * \fn  template<typename real_t> inline gfx::vector3<real_t> project_vector_on_vector( const gfx::vector3<real_t>& lhs,
 * const gfx::vector3<real_t>& rhs );
 *
 * \brief   Projects a vector on another vector.
 *
 * \param   lhs The projected vector.
 * \param   rhs The projection vector.
 * \remarks The formula for the projection of a vector P on a vector Q is :
 * 			((dot(P, Q) / ||Q||^2) * Q.
 * \return  A vector, representing the projection of lhs on rhs.
 */
template<typename real_t>
inline
gfx::vector3<real_t>
project_vector_on_vector(
    const gfx::vector3<real_t>& lhs, 
    const gfx::vector3<real_t>& rhs
    );

/**
 * \fn  template<typename real_t> inline gfx::vector3<real_t> project_vector_on_unit_vector( const gfx::vector3<real_t>& lhs,
 * const gfx::vector3<real_t>& rhs );
 *
 * \brief   Project vector on a unit vector.
 *
 * \param   lhs The projected vector.
 * \param   rhs The projection vector.
 * \remarks Use this function if you know that the projection vector has 
 * 			unit length. The formula is dot(lhs, rhs) * rhs.
 * \return  A vector, representing the projection of lhs on rhs.
 */
template<typename real_t>
inline
gfx::vector3<real_t>
project_vector_on_unit_vector(
    const gfx::vector3<real_t>& lhs, 
    const gfx::vector3<real_t>& rhs
    );

/**
 * \fn  template<typename real_t> inline gfx::vector3<real_t> normal_of( const gfx::vector3<real_t>& vec );
 *
 * \brief   Returns the normal of the given vector.
 *
 */
template<typename real_t>
inline
gfx::vector3<real_t>
normal_of(
  const gfx::vector3<real_t>& vec
);

/**
 * \fn  template<typename real_t> inline float triple_scalar_product( const gfx::vector3<real_t>& v1,
 * const gfx::vector3<real_t>& v2, const gfx::vector3<real_t>& v3 );
 *
 * \brief   Returns the triple scalar product.
 *
 * \remarks The formula for the triple scalar product of 3 vectors u, v, w is
 * 			dot(u, cross(v, w)).
 */
template<typename real_t>
inline
real_t
triple_scalar_product(
    const gfx::vector3<real_t>& v1, 
    const gfx::vector3<real_t>& v2, 
    const gfx::vector3<real_t>& v3
);

/**
 * \fn  template<typename real_t> inline gfx::vector3<real_t> triple_vector_product( const gfx::vector3<real_t>& v1,
 * const gfx::vector3<real_t>& v2, const gfx::vector3<real_t>& v3 );
 *
 * \brief   Returns the triple vector product.
 * \remarks The formula for the triple vector product of three vectors u, v, w
 * 			is cross(u, cross(v, w)).
 */
template<typename real_t>
inline
gfx::vector3<real_t>
triple_vector_product(
    const gfx::vector3<real_t>& v1, 
    const gfx::vector3<real_t>& v2, 
    const gfx::vector3<real_t>& v3
);

/**
 * \fn  template<typename real_t> inline float distance_squared( const gfx::vector3<real_t>& point1,
 * const gfx::vector3<real_t>& point2 );
 *
 * \brief   Returns the square of the distance between two points in R3.
 * \param   point1  The first point.
 * \param   point2  The second point.
 */
template<typename real_t>
inline
real_t
distance_squared(
    const gfx::vector3<real_t>& point1, 
    const gfx::vector3<real_t>& point2
);

/**
 * \fn  template<typename real_t> inline float distance( const gfx::vector3<real_t>& point1,
 * const gfx::vector3<real_t>& point2 );
 *
 * \brief   Returns the distance between two points in R3.
 * \param   point1  The first point.
 * \param   point2  The second point.
 */
template<typename real_t>
inline
real_t
distance(
    const gfx::vector3<real_t>& point1, 
    const gfx::vector3<real_t>& point2
    );

/**
 * \typedef vector3<float> vector3F
 *
 * \brief   Defines an alias representing a vector3 having simple precision 
 * 			floating point components.
 */
typedef vector3<float>  vector3F;

/**
 * \typedef vector3<double> vector3D
 *
 * \brief   Defines an alias representing a vector3 having double precision
 * 			floating point components.
 */
typedef vector3<double> vector3D;

} // namespace gfx

#include "vector3.inl"
