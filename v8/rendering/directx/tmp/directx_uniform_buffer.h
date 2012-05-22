#pragma once

#include "v8/base/misc.h"
#if defined(RENDER_SYSTEM_DIRECTX10)
#include "v8/rendering/directx/directx10_uniform_buffer_base.h"
#elif defined(RENDER_SYSTEM_DIRECTX11)
#endif

namespace v8 { namespace rendering_system {

template<typename T>
class uniform_buffer : public uniform_buffer_base {
private :
    NO_CC_ASSIGN(uniform_buffer);
    T                                                         uniform_data_;  
public :
    uniform_buffer(const char* name) : uniform_buffer_base(name) {
        static_assert(((sizeof(T) % 16) == 0) && 
                      "For Direct3D, the size of the constant buffer "
                      "must be a multiple of 16");
    }

    uniform_buffer(const char* name, const T& uniform_data) 
        : uniform_buffer_base(name),
          uniform_data_(uniform_data) {
        static_assert(((sizeof(T) % 16) == 0) && 
                      "For Direct3D, the size of the constant buffer "
                      "must be a multiple of 16");
    }

    T& get_buffered_data() {
        return uniform_data_;
    }

    const T& get_buffered_data() const {
        return uniform_data_;
    }

    bool initialize(renderer* r) {
        return uniform_buffer_base::initialize(r, sizeof(T), sizeof(T));
    }

    void sync_with_gpu(renderer* r) {
        return uniform_buffer_base::sync_with_gpu(r, &uniform_data_);
    }
};

} // namespace rendering_system
} // namespace v8
