#pragma once

#include <d3d11.h>
#include "v8/base/misc.h"
#include "v8/base/scoped_pointer.h"

namespace v8 { namespace rendering_system {

class renderer;

class uniform_buffer_base {
protected :
    unsigned int                                       bind_slot_;
    const char*                                        name_;
    base::scoped_ptr<ID3D11Buffer, base::com_storage>  buff_ptr_;

    bool initialize(renderer* r, size_t buff_size, size_t stride_size);

    void sync_with_gpu(renderer* r, const void* data);

    uniform_buffer_base(const char* name) : name_(name) {}
public :  
    ~uniform_buffer_base() {}

    unsigned int get_binding_slot() const {
        return bind_slot_;
    }

    void set_binding_slot(unsigned int slot) {
        bind_slot_ = slot;
    }

    const char* get_name() const {
        return name_;
    }

    ID3D11Buffer* get_buffer_handle() const {
        return base::scoped_pointer_get(buff_ptr_);
    }
};

} // namespace rendering_system
} // namespace base
