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

#include "v8/base/fundamental_types.h"
#include "v8/math/math_constants.h"

namespace v8 { namespace math { namespace internals {

template<typename real_type, bool is_floating_point = false>
struct op_eq_helper {
    static bool result(real_type left, real_type right) {
        return left == right;
    }
};

template<typename real_type>
struct op_eq_helper<real_type, true> {
    static bool result(real_type left, real_type right) {
        return fabs(left - right) <= constants::kEpsilon;
    }
};

template<typename real_t, bool is_floating_point = false>
struct transform_dividend_for_division {
    static real_t transform(real_t dividend) {
        return dividend;
    }
};

template<typename real_t>
struct transform_dividend_for_division<real_t, true> {
    static real_t transform(real_t dividend) {
        return real_t(1) / dividend;
    }
};

template<typename real_t, bool is_floating_point = false>
struct divide_helper {
    static real_t divide(real_t divided, real_t dividend) {
        return divided / dividend;
    }
};

template<typename real_t>
struct divide_helper<real_t, true> {
    static real_t divide(real_t divided, real_t inverse_of_dividend) {
        return divided * inverse_of_dividend;
    }
};

} // namespace internals

template<typename T>
inline
bool
operands_eq(T left, T right) {
    return internals::op_eq_helper<
        T, base::is_floating_point_type<T>::Yes
    >::result(left, right);
}

template<typename real_t>
inline
bool
is_zero(real_t value) {
    return fabs(value) <= constants::kEpsilon;
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
    return radians * constants::k180OverPi;
}

/**
 * \brief   Converts from degrees to radians.
 */
template<typename real_t>
inline
real_t
to_radians(
    real_t degrees
    )
{
    return degrees * constants::kPiOver180;
}

template<typename real_t>
inline 
real_t
inv_sqrt(real_t val) {
    return real_t(1) / sqrt(val);
}

template<typename T>
inline T clamp(const T& val, const T& min, const T& max) {
  return (val <= min ? min : (val >= max ? max : val));
}

template<typename Ty>
inline Ty min(const Ty& left, const Ty& right) {
    return left < right ? left : right;
}

template<typename T>
inline T max(const T& left, const T& right) {
    return left > right ? left : right;
}

template<typename ty>
inline void swap(ty& left, ty& right) {
    ty temp = left;
    left = right;
    right = temp;
}

} // namespace math
} // namespace v8