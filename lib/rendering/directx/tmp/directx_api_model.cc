#include "v8/pch_hdr.h"
#include "v8/base/debug_helpers.h"
#include "v8/base/scoped_handle.h"
#include "v8/base/crt_handle_policies.h"
#include "v8/rendering/directx/directx_api_model.h"

v8::rendering_system::blob_pointer_t
v8::rendering_system::compile_shader_from_file( 
    const char* file_name, 
    const char* function_name, 
    const char* profile, 
    unsigned int compile_flags
    )
{
    using namespace base;
    scoped_handle<crt_file_handle> shader_file(::fopen(file_name, "r"));
    if (!shader_file)
        return blob_pointer_t();

    std::string source_code;
    char buff[1024];
    while (fgets(buff, _countof(buff), scoped_handle_get(shader_file)))
        source_code.append(buff);

    blob_pointer_t blob_bytecode;
    blob_pointer_t blob_msg;
    HRESULT ret_code;
    CHECK_D3D(
        &ret_code,
        D3DCompile(source_code.c_str(), source_code.size(), file_name,
                   nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, profile,
                   compile_flags, 0U, 
                   scoped_pointer_get_impl(blob_bytecode),
                   scoped_pointer_get_impl(blob_msg)));

  if (FAILED(ret_code)) {
      OUTPUT_DBG_MSGA(
          "Failed to compile shader, error = %s", 
          blob_msg->GetBufferPointer() ? 
              reinterpret_cast<const char*>(blob_msg->GetBufferPointer()) 
              : "unknown");
  }
  return blob_bytecode;
}