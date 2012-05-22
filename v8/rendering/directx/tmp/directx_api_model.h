#pragma once

#include "v8/base/scoped_pointer.h"
#if defined(RENDER_SYSTEM_DIRECTX11)
#include "v8/rendering/directx/dx11_api_model.h"
#else
#include "v8/rendering/directx/dx10_api_model.h"
#endif

namespace v8 { namespace rendering_system {

typedef base::scoped_ptr<
    directx_api_model::blob_t, base::com_storage
> blob_pointer_t;

blob_pointer_t 
compile_shader_from_file(
    const char* file_name, 
    const char* function_name, 
    const char* profile,
    unsigned int compile_flags
    );

} // namespace rendering system
} // namespace v8
