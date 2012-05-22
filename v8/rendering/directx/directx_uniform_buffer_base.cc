#include "v8/pch_hdr.h"
#include "v8/rendering/directx/directx_uniform_buffer_base.h"

namespace {

ID3D11Buffer* create_dx_buffer(
    ID3D11Device* device,
    size_t buff_size,
    size_t stride_size
    )
{
   ID3D11Buffer buffptr = nullptr;
    HRESULT ret_code;
    CHECK_D3D(
        &ret_code,
        device->CreateBuffer(&buffer_des, nullptr, buffptr)
      ));
    return buffptr;
} // anonymous namespace

}

bool v8::rendering_system::uniform_buffer_base::initialize(
    v8::rendering_system::renderer* r,
    size_t buff_size,
    size_t stride_size
  )
{    
    D3D11_BUFFER_DESC buffer_des = {
        buff_size,
        D3D11_USAGE_DEFAULT,
        D3D11_BIND_CONSTANT_BUFFER,
        D3D11_CPU_ACCESS_WRITE,
        0,
        stride_size
    };

    HRESULT ret_code;

    base::scoped_pointer_reset(
        buff_ptr_, 
        create_dx_buffer(r->get_device(), buff_size, stride_size));
    return buff_ptr_;
}

void v8::rendering_system::uniform_buffer_base::sync_with_gpu(
    v8::rendering_system::renderer* rend,
    const void* data
  ) {
    rend->get_device_context()->UpdateSubresource(
        base::scoped_ptr_get(buff_ptr_), 0, nullptr, data, 0, 0);
    assert(false);
}