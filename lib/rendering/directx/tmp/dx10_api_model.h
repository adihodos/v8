#pragma once

#include <d3d10.h>
#include "v8/rendering/directx/api_model.h"

namespace v8 { namespace rendering_system {

template<>
struct api_model<directx_api_version_10> {
    typedef ID3D10Buffer            buffer_t;
    typedef ID3D10VertexShader      vertex_shader_t;
    typedef ID3D10PixelShader       pixel_shader_t;
    typedef ID3D10ShaderReflection  shader_reflection_t;
    typedef ID3D10Blob              blob_t;
};

typedef api_model<directx_api_version_10> directx_api_model;

} //namespace rendering_system
} //namespace v8
