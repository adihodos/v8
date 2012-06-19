#pragma once

#include <D3D10.h>
#include <D3D11.h>
#include "v8/rendering/directx/api_model.h"

namespace v8 { namespace rendering_system {
    
template<>
struct api_model<directx_api_version_11> {
    typedef ID3D11Buffer            buffer_t;
    typedef ID3D11VertexShader      vertex_shader_t;
    typedef ID3D11PixelShader       pixel_shader_t;
    typedef ID3D11ShaderReflection  shader_reflection_t;
    typedef ID3D10Blob              blob_t;
};

typedef api_model<directx_api_version_11> directx_api_model;

} //namespace rendering_system
} //namespace v8
