#pragma once

#include <cmath>
#include "math.h"

namespace gfx {

/**
 * \namespace   math {
 *
 * \brief   Contains constants and various mathematical stuff.
 */
namespace math {

const float kEpsilon = 0.00000001f; ///< The epsilon value, used for testing with zero. */

const float kPi = 3.14159265f;  ///< The pi constant. */

const float kPiOver180 = kPi / 180.0f;

const float k180OverPi = 180.0f / kPi;

/**
 * \class   zero_test 
 *
 * \brief   Test if a value is equal to zero. This is needed since the test
 * 			for an integer value is different from the test for a floating
 * 			point value.
 */
template<typename real_t, bool is_floating_point = false>
struct zero_test {
    static bool result(real_t value) {
        return value == real_t(0);
    }
};

/**
 * \class   zero_test
 *
 * \brief   Template specialization, used to test floating point values for zero.
 */
template<typename real_t>
struct zero_test<real_t, true> {
    static bool result(real_t value) {
        return std::fabs(value) <= kEpsilon;
    }
};

template<typename ty, bool is_floating_point = false>
struct op_eq {
    static bool result(const ty left, const ty right) {
        return left == right;
    }
};

template<typename ty>
struct op_eq<ty, true> {
    static bool result(const ty left, const ty right) {
        return std::fabs(left - right) <= kEpsilon;
    }
};

/**
 * \fn  template<typename real_t> inline real_t to_degrees( real_t radians )
 *
 * \brief   Converts from radians to degrees.
 */
template<typename real_t>
inline
real_t
to_degrees(
    real_t radians
    )
{
    return radians * k180OverPi;
}

/**
 * \fn  template<typename real_t> inline real_t to_radians( real_t degrees )
 *
 * \brief   Converts from degrees to radians.
 */
template<typename real_t>
inline
real_t
to_radians(
    real_t degrees
    )
{
    return degrees * kPiOver180;
}

} // namespace math

} // namespace gfx