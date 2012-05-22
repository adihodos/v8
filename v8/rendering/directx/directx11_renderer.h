#pragma once

#include <d3d11.h>
#include "v8/base/misc.h"
#include "v8/base/scoped_pointer.h"

namespace v8 { namespace rendering_system {

class renderer {
private :
    NO_CC_ASSIGN(renderer);

    struct implementation_details;
    base::scoped_ptr<implementation_details> impl_;

public :
    renderer();

    ~renderer();

    enum clear_flags {
        clear_depth = 1U << 0,
        clear_stencil = 1U << 1
    };

    enum presentation_flags {
        pf_present_frame,
        pf_present_test_frame
    };

    enum presentation_result {
        present_ok,
        present_window_occluded,
        present_error
    };

    enum usage_type {
        usage_default = D3D11_USAGE_DEFAULT,
        usage_immutable = D3D11_USAGE_IMMUTABLE,
        usage_dynamic = D3D11_USAGE_DYNAMIC,
        usage_staging = D3D11_USAGE_STAGING
    };

    enum bind_flag {
        bind_vertex_buffer = D3D11_BIND_VERTEX_BUFFER,
        bind_index_buffer = D3D11_BIND_INDEX_BUFFER,
        bind_constant_buffer = D3D11_BIND_CONSTANT_BUFFER,
        bind_shader_resource = D3D11_BIND_SHADER_RESOURCE,
        bind_stream_output = D3D11_BIND_STREAM_OUTPUT,
        bind_render_target = D3D11_BIND_RENDER_TARGET,
        bind_depth_stencil = D3D11_BIND_DEPTH_STENCIL,
        bind_unordered_access = D3D11_BIND_UNORDERED_ACCESS
    };

    enum primitive_topology {
        kTopologyUndefined = D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED,
        kTopologyPointList = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST,
        kTopologyLineList = D3D11_PRIMITIVE_TOPOLOGY_LINELIST,
        kTopologyLineStrip = D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP,
        kTopologyTriangleList = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
        kTopologyTriangleStrip = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
        kTopologyLineListAdjacency = D3D11_PRIMITIVE_TOPOLOGY_LINELIST_ADJ,
        kTopologyLineStripAdjacency = D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP_ADJ,
        kTopologyTriangleListAdjancency = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST_ADJ,
        kTopologyTriangleStripAdjacency = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP_ADJ
    };

    bool initialize(HWND window, int width, int height, bool fullscreen = false);

    bool resize_buffers(int width, int height);

    int get_width() const;

    int get_height() const;

    void clear_backbuffer(const float color[4]);

    void clear_depth_stencil(
        unsigned int clr_flags = clear_depth | clear_stencil, 
        float depth = 1.0f, 
        unsigned char stencil = 0xff
        );

    presentation_result present(presentation_flags fp = pf_present_frame);

    ID3D11Device* get_device() const;

    ID3D11DeviceContext* get_device_context() const;

    base::scoped_ptr<ID3D11Buffer, base::com_storage> create_constant_buffer(
        usage_type use_type,
        size_t buffer_size,
        const void* initial_data = nullptr,
        unsigned initial_data_size = 0U
        );
};

} // namespace rendering_system
} // namespace v8
