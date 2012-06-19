#pragma once

#include "v8/base/fundamental_types.h"
#include "v8/math/math_utils.h"

namespace v8 { namespace math 

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
        real_t elements_[9];
    };
}; 

} // namespace math
} // namespace v8