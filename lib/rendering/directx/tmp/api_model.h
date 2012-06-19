#pragma once

namespace v8 { namespace rendering_system { 

enum directx_api_version {
    directx_api_version_10,
    directx_api_version_11
};

template<int dx_version>
struct api_model;

} // namespace rendering_system
} // namespace v8
