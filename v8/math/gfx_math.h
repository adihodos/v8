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

namespace v8 { namespace math {


template<typename T>
struct fp_test {
    static const bool result = false;
};

template<>
struct fp_test<float> {
    static const bool result = true;
};

template<>
struct fp_test<double> {
    static const bool result = true;
};

template<>
struct fp_test<long double> {
    static const bool result = true;
};


template<typename real_type>
inline bool equality_test(real_type left, real_type right) {
    return op_eq_helper<real_type, fp_test<real_type>::result>::result(left, right);
}

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
        return fabs(left - right) <= kEpsilon;
    }
};

} // namespace details


template<typename real_t>
struct type_traits;

template<>
struct type_traits<float> {
    static float fabs(float val) {
        return fabs(val);
    }

    static float sqrt(float val) {
        return sqrt(val);
    }

    static float inv_sqrt(float val) {
        return 1.0f / sqrt(val);
    }
};

template<>
struct type_traits<double> {
    static double fabs(double val) {
        return fabs(val);
    }

    static double sqrt(double val) {
        return sqrt(val);
    }

    static double inv_sqrt(double val) {
        return 1.0f / sqrt(val);
    }

    static double sin(double val) {
        return sin(val);
    }

    static double cos(double val) {
        return cos(val);
    }

    static double tan(double val) {
        return tan(val);
    }
};


} // namespace math

} // namespace v8
