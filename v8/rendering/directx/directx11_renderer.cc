#include "v8/pch_hdr.h"
#include "v8/base/debug_helpers.h"
#include "v8/base/pointer_policies.h"
#include "v8/base/win32_utils.h"
#include "v8/rendering/directx/directx11_renderer.h"

using namespace v8::base;

struct v8::rendering_system::renderer::implementation_details {
private :
    NO_CC_ASSIGN(implementation_details);

public :
    scoped_ptr<IDXGIFactory, com_storage>           dxgi_factory_;
    scoped_ptr<IDXGISwapChain, com_storage>         dxgi_swapchain_;
    scoped_ptr<ID3D11Device, com_storage>           d3d11device_;
    scoped_ptr<ID3D11DeviceContext, com_storage>    d3d11device_ctx_;
    scoped_ptr<ID3D11RenderTargetView, com_storage> rendertargetview_;
    scoped_ptr<ID3D11Texture2D, com_storage>        depthstencil_texture_;
    scoped_ptr<ID3D11DepthStencilView, com_storage> depthstencilview_;
    DXGI_MODE_DESC                                  mode_info_;
    int                                             width_;
    int                                             height_;

    implementation_details() : width_(0), height_(0) {}

    void release_resources_before_buffer_resize();

    bool initialize(HWND window, int width, int height, bool fullscreen);

    bool create_render_target_view();

    bool create_depth_stencil_buffer_and_views();

    bool bind_to_output_merger_stage();
};

void 
v8::rendering_system::renderer::implementation_details::
release_resources_before_buffer_resize() {
    d3d11device_ctx_->OMSetRenderTargets(0, nullptr, nullptr);
    scoped_pointer_reset(depthstencilview_);
    scoped_pointer_reset(depthstencil_texture_);
    scoped_pointer_reset(rendertargetview_);
}

bool v8::rendering_system::renderer::implementation_details::initialize(
    HWND window, 
    int width, 
    int height, 
    bool fullscreen
    )
{
    assert(!dxgi_swapchain_);
    assert(!d3d11device_);
    assert(!d3d11device_ctx_);

    scoped_ptr<IDXGIFactory, com_storage> dxgiFactory(win32::create_dxgi_factory());
    if (!dxgiFactory)
        return false;

    std::vector<win32::unique_adaptor_handle_t> graphicAdapters(
        win32::enum_graphic_adapters(scoped_pointer_get(dxgiFactory)));
    if (graphicAdapters.empty())
        return false;

    bool modeFound = false;
    size_t i = 0;
    for (i = 0; i < graphicAdapters.size() && !modeFound; ++i) {
        std::vector<win32::unique_output_handle_t> adapterOutputs(
            win32::enum_adapter_outputs(scoped_pointer_get(graphicAdapters[i])));
        if (adapterOutputs.empty())
            continue;

        for (size_t j = 0; j < adapterOutputs.size() && !modeFound; ++j) {
            auto modes_list = win32::enum_output_modes(
                scoped_pointer_get(adapterOutputs[j]), DXGI_FORMAT_R8G8B8A8_UNORM);
            DXGI_MODE_DESC req_mode = {
                width, 
                height, 
                { 0, 0 }, 
                DXGI_FORMAT_R8G8B8A8_UNORM,
                DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED,
                DXGI_MODE_SCALING_UNSPECIFIED
            };

            modeFound = adapterOutputs[j]->FindClosestMatchingMode(
                &req_mode, &mode_info_, nullptr) == S_OK;
        }
    }

    if (!modeFound)
        return false;

    DXGI_SWAP_CHAIN_DESC swp_chain_desc = {
        mode_info_,
        { 1, 0 },
        DXGI_USAGE_RENDER_TARGET_OUTPUT,
        1,
        window,
        !fullscreen,
        DXGI_SWAP_EFFECT_DISCARD,
        DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH
    };

    UINT creation_flags = D3D11_CREATE_DEVICE_DEBUG;
    HRESULT ret_code;
    CHECK_D3D(
        &ret_code,
        ::D3D11CreateDeviceAndSwapChain(
            scoped_ptr_get(graphicAdapters[i - 1]),
            D3D_DRIVER_TYPE_UNKNOWN, 
            nullptr,
            creation_flags,
            nullptr,
            0,
            D3D11_SDK_VERSION,
            &swp_chain_desc,
            scoped_pointer_get_impl(dxgi_swapchain_),
            scoped_pointer_get_impl(d3d11device_),
            nullptr,
            scoped_ptr_get_ptr_ptr(d3d11device_ctx_)
            ));

    if (FAILED(ret_code))
        return false;

    return create_render_target_view() 
           && create_depth_stencil_buffer_and_views()
           && bind_to_output_merger_stage();
}

bool v8::rendering_system::renderer::implementation_details::
    create_render_target_view() {
    assert(!rendertargetview_);
    scoped_ptr<ID3D11Texture2D, com_storage> texture_resource;
    HRESULT ret_code;
    CHECK_D3D(
		&ret_code,
        dxgi_swapchain_->GetBuffer(
            0, __uuidof(ID3D11Texture2D), 
			reinterpret_cast<void**>(scoped_pointer_get_impl(texture_resource))));

	CHECK_D3D(
		&ret_code,
		d3d11device_->CreateRenderTargetView(
			scoped_pointer_get(texture_resource),
			nullptr,
			scoped_pointer_get_impl(rendertargetview_)));

	return ret_code == S_OK;
}

bool v8::rendering_system::renderer::implementation_details::
    create_depth_stencil_buffer_and_views() {
    assert(!depthstencilview_);
    assert(!depthstencil_texture_);

    D3D11_TEXTURE2D_DESC texture_desc = {
        mode_info_.Width,
        mode_info_.Height,
        0,
        1,
        DXGI_FORMAT_D32_FLOAT_S8X24_UINT,
        { 1, 0 },
        D3D11_USAGE_DEFAULT,
        D3D11_BIND_DEPTH_STENCIL,
        0,
        0
    };

    HRESULT ret_code;
    CHECK_D3D(
        &ret_code,
        d3d11device_->CreateTexture2D(
            &texture_desc, nullptr, 
            scoped_pointer_get_impl(depthstencil_texture_)));

    if (FAILED(ret_code))
        return false;

    CHECK_D3D(
        &ret_code,
        d3d11device_->CreateDepthStencilView(
            scoped_pointer_get(depthstencil_texture_),
            nullptr,
            scoped_pointer_get_impl(depthstencilview_)));

    return ret_code == S_OK;
}

bool v8::rendering_system::renderer::implementation_details
    ::bind_to_output_merger_stage() {
   ID3D11RenderTargetView* render_target_views[] = { 
        scoped_pointer_get(rendertargetview_) 
    };

    d3d11device_ctx_->OMSetRenderTargets(_countof(render_target_views), 
                                         render_target_views, 
                                         scoped_pointer_get(depthstencilview_));

    D3D11_VIEWPORT view_port = {
        0, 0,
        static_cast<float>(width_), 
        static_cast<float>(height_),
        0.0f, 1.0f
    };
    d3d11device_ctx_->RSSetViewports(1, &view_port);
    return true;
}

v8::rendering_system::renderer::renderer()
    : impl_(new implementation_details()) {}

v8::rendering_system::renderer::~renderer() {
    if (impl_->dxgi_swapchain_)
        impl_->dxgi_swapchain_->SetFullscreenState(false, nullptr);
}

bool v8::rendering_system::renderer::initialize(
    HWND window, 
    int width, 
    int height, 
    bool fullscreen /* = false */
    )
{
    return impl_->initialize(window, width, height, fullscreen);
}

bool v8::rendering_system::renderer::resize_buffers(int width, int height) {
    impl_->release_resources_before_buffer_resize();

    impl_->mode_info_.Width = static_cast<UINT>(width);
    impl_->mode_info_.Height = static_cast<UINT>(height);
    HRESULT ret_code;
    CHECK_D3D(
        &ret_code,
        impl_->dxgi_swapchain_->ResizeBuffers(
            0, width, height, DXGI_FORMAT_UNKNOWN, 0));
    if (FAILED(ret_code))
        return false;

    return impl_->create_render_target_view() 
           && impl_->create_depth_stencil_buffer_and_views()
           && impl_->bind_to_output_merger_stage();
}

int v8::rendering_system::renderer::get_width() const {
    return impl_->width_;
}

int v8::rendering_system::renderer::get_height() const {
    return impl_->height_;
}

void v8::rendering_system::renderer::clear_backbuffer(
    const float color[4]
    )
{
    impl_->d3d11device_ctx_->ClearRenderTargetView(
        scoped_pointer_get(impl_->rendertargetview_), color); 
}

void v8::rendering_system::renderer::clear_depth_stencil(
    unsigned int cf,
    float depth, 
    unsigned char stencil
    )
{
    unsigned d3d_clear_flag = D3D11_CLEAR_FLAG(0);
    if (cf & clear_depth)
        d3d_clear_flag |= D3D11_CLEAR_DEPTH;

    if (cf & clear_stencil)
        d3d_clear_flag |= D3D11_CLEAR_STENCIL;

    impl_->d3d11device_ctx_->ClearDepthStencilView(
        scoped_pointer_get(impl_->depthstencilview_),
        d3d_clear_flag, depth, stencil
        );
}

v8::rendering_system::renderer::presentation_result
v8::rendering_system::renderer::present(
    v8::rendering_system::renderer::presentation_flags pf
    ) 
{
    HRESULT ret_code = impl_->dxgi_swapchain_->Present(0, pf);
    switch (ret_code) {
    case S_OK :
        return present_ok;
        break;

    case DXGI_STATUS_OCCLUDED :
        return present_window_occluded;
        break;

    default :
        break; 
    }

    return present_error;
}

ID3D11Device* v8::rendering_system::renderer::get_device() const {
    return scoped_pointer_get(impl_->d3d11device_);
}

ID3D11DeviceContext* v8::rendering_system::renderer::get_device_context() const {
    return scoped_pointer_get(impl_->d3d11device_ctx_);
}

v8::base::scoped_ptr<ID3D11Buffer, v8::base::com_storage>
v8::rendering_system::renderer::create_buffer( 
    usage_type use_type, 
    bind_flag binding_type, 
    size_t buffer_size, 
    size_t stride_size,
    const void* initial_data,
    unsigned initial_data_size
    )
{
    D3D11_BUFFER_DESC buffer_desc = {
        buffer_size,
        (D3D11_USAGE) use_type,
        binding_type,

    }
}