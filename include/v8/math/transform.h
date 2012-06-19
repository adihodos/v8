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

#include "v8/math/matrix3X3.h"
#include "v8/math/matrix4X4.h"
#include "v8/math/vector3.h"

namespace v8 { namespace math {

template<typename real_t>
class transform {
private :
    vector3<real_t>                 translation_component_;
    real_t                          scale_factor_component_;
    matrix_3X3<real_t>              matrix_component_;
    mutable matrix_4X4<real_t>      transform_matrix_;
    mutable bool                    cache_valid_;
    bool                            is_identity_;
    bool                            is_rotation_reflection_;
    bool                            is_scale_;

    void compute_transform_matrix() const;

    void reset_row4() const;
    
public :
    transform();

    transform(const vector3<real_t>&);

    transform(const matrix_3X3<real_t>&, bool is_rotation_or_reflection = true);

    transform(float scale_factor);

    transform(
        const matrix_3X3<real_t>& matrix_component, 
        bool matrix_is_rotation_or_reflection,
        const vector3<real_t>& translation_component, 
        float scale_component
        );

    inline bool is_identity() const;

    inline bool is_rotation_or_reflection() const;

    inline bool is_scaling() const;

    transform<real_t>& make_identity();

    transform<real_t>& make_non_scaling();
    
    void set_scale_component(float scale_factor);

    inline real_t get_scale_component() const;

    void set_matrix_component_rotate_reflect(const math::matrix_3X3<real_t>&);

    void set_matrix_component(const math::matrix_3X3<real_t>&);

    inline const math::matrix_3X3<real_t>& get_matrix_component() const;

    void set_translation_component(const math::vector3<real_t>&);

    inline const math::vector3<real_t>& get_translation_component() const;

    const math::matrix_4X4<real_t>& get_transform_matrix() const;

    void compute_inverse(math::matrix_4X4<real_t>* inv) const;

    transform<real_t>& invert();

    transform<real_t>& operator*=(const math::transform<real_t>& rhs);
};

template<typename real_t>
inline
math::transform<real_t>
operator*(const math::transform<real_t>&, const math::transform<real_t>&);

typedef transform<float>    transformF;
typedef transform<double>   transformD;

} // namespace math
} // namespace v8

#include "transform.inl"