#pragma once

#include <hash_map>
#include <vector>
#include <DXGI.h>
#include <Windows.h>
#include "v8/base/misc.h"
#include "v8/base/scoped_pointer.h"
#include "v8/base/pointer_policies.h"
#include "v8/math/rectangle.h"

namespace v8 { namespace base { namespace win32 {

//struct format_enum_options {
//    DXGI_FORMAT format_;
//    UINT        flags_;
//
//    format_enum_options(DXGI_FORMAT fmt, UINT flags)
//        : format_(fmt), flags_(flags) {}
//
//    bool operator<(const format_enum_options& right) const {
//        return format_ < right.format_;
//    }
//};
//
//class graphics_adapter_output {
//private :
//    base::scoped_ptr<IDXGIOutput, base::com_storage>    output_interface_;
//    mutable DXGI_OUTPUT_DESC                            output_description_;
//    mutable bool                                        description_cached_;
//    mutable std::hash_map<format_enum_options, std::vector<DXGI_MODE_DESC>> format_and_modes_;
//
//    void cache_description() const {
//        if (description_cached_)
//            return;
//
//        assert(output_interface_);
//        description_cached_ = 
//            (output_interface_->GetDesc(&output_description_) == S_OK);
//    }
//
//public :
//    graphics_adapter_output(IDXGIOutput* output)
//        : output_interface_(output), description_cached_(false) {}
//
//
//    const wchar_t* get_device_name() const {
//        cache_description();
//        return output_description_.DeviceName;
//    }
//
//    math::rectL get_output_bounds() const {
//        cache_description();
//        return math::rectL(
//            output_description_.DesktopCoordinates.left,
//            output_description_.DesktopCoordinates.top,
//            output_description_.DesktopCoordinates.right,
//            output_description_.DesktopCoordinates.bottom);
//    }
//
//    DXGI_MODE_ROTATION get_rotation() const {
//        cache_description();
//        return output_description_.Rotation;
//    }
//
//    HMONITOR get_display_monitor() const {
//        cache_description();
//        return output_description_.Monitor;
//    }
//
//    const std::vector<DXGI_MODE_DESC>& get_modes_for_format(
//        const format_enum_options& fmt_options
//        );
//
//    DXGI_MODE_DESC get_closest_match(
//        unsigned width, 
//        unsigned height, 
//        unsigned refresh_rate,
//        DXGI_FORMAT fmt, 
//        DXGI_MODE_SCANLINE_ORDER scan_order = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED,
//        DXGI_MODE_SCALING scale_mode = DXGI_MODE_SCALING_UNSPECIFIED
//        );
//};
//
//class graphics_adapter {
//private :
//    scoped_ptr<IDXGIAdapter, com_storage>   adapter_;
//    mutable DXGI_ADAPTER_DESC               description_;
//    mutable bool                            description_valid_;
//};

class scoped_dc {
private :
    NO_CC_ASSIGN(scoped_dc);

    HWND    owning_wnd_;
    HDC     devctx_;
public :
    scoped_dc(HWND wnd, HDC devctx) 
        : owning_wnd_(wnd), devctx_(devctx) {}

    ~scoped_dc() {
        if (devctx_)
            ::ReleaseDC(owning_wnd_, devctx_);
    }

    HDC get() const {
        return devctx_;
    }

    bool operator!() const {
        return devctx_ == nullptr;
    }

    HDC release() {
        HDC old_ctx = devctx_;
        devctx_ = nullptr;
        return old_ctx;
    }
};

void varargs_msgbox(
    UINT buttons, 
    const wchar_t* title, 
    const wchar_t* fmt, 
    ...
    );

typedef scoped_ptr<IDXGIFactory, com_storage> unique_dxgifactory_handle_t;

unique_dxgifactory_handle_t create_dxgi_factory();

typedef scoped_ptr<IDXGIAdapter, com_storage> unique_adaptor_handle_t;

/**
 * \brief   Enumerate all the graphics adapter present in a computer.
 *
 * \param [in]  factory Pointer to an IDXGIFactory interface.
 *
 * \return  Vector containing smart pointer to IDXGIAdapter interfaces. 
 */
std::vector<unique_adaptor_handle_t> enum_graphic_adapters(
    IDXGIFactory* factory
    );

typedef scoped_ptr<IDXGIOutput, com_storage> unique_output_handle_t;

/**
 * Enumerate all outputs of a graphics adapter.
 *
 * \param [in  adapter Pointer to an IDXGIAdapter interface. Must not be null.
 *
 * \return  Vector containing smart pointers to IDXGIOutput interfaces,
 * 			representing the outputs of the adapter.
 */
std::vector<unique_output_handle_t> enum_adapter_outputs(
    IDXGIAdapter* adapter
    );

/**
 * Enum all supported modes for the given format and flags, of a certain output.
 *
 * \param [in]  output  Pointer to a IDXGIOutput interfaces.
 * \param   format          Describes the format for which  the list of modes is
 * 							to be retrieved.
 * \param   enum_flags      (optional) Aditional flags to constrain the
 * 							list of modes.
 *
 * \return  Vector object with supported modes for the given combination of
 * 			format and flags.
 */
std::vector<DXGI_MODE_DESC> enum_output_modes(
    IDXGIOutput* output, 
    DXGI_FORMAT format, 
    UINT enum_flags = 0
    );

} // namespace win32
} // namespace base
} // namespace v8
