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

template<typename T>
struct fp_test {
    static const bool result = false;
};

template<>
struct fp_test<double> {
    static const bool result = true;
};

template<>
struct fp_test<long double> {
    static const bool result = true;
};

namespace details {

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

} // namespace details

template<typename T>
inline
bool
operands_eq(T left, T right) {
    return details::op_eq<T, fp_test<T>::result>::result(left, right);
}

template<typename real_t>
inline
bool
is_zero(real_t value) {
    return operands_eq(real_t(0), value);
}

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

template<typename real_t>
inline 
real_t
inv_sqrt(real_t val) {
    return real_t(1) / std::sqrt(val);
}

template<typename real_t>
struct type_traits;

template<>
struct type_traits<float> {
    static float fabs(float val) {
        return std::fabsf(val);
    }

    static float sqrt(float val) {
        return std::sqrtf(val);
    }

    static float inv_sqrt(float val) {
        return 1.0f / sqrt(val);
    }
};

template<>
struct type_traits<double> {
    static double fabs(double val) {
        return std::fabs(val);
    }

    static double sqrt(double val) {
        return std::sqrt(val);
    }

    static double inv_sqrt(double val) {
        return 1.0f / sqrt(val);
    }

    static double sin(double val) {
        return std::sin(val);
    }

    static double cos(double val) {
        return std::cos(val);
    }

    static double tan(double val) {
        return std::tan(val);
    }
};


} // namespace math

} // namespace gfx