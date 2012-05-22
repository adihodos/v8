#include "pch_hdr.h"
#include "v8/base/win32_utils.h"

using namespace v8::base;

void v8::base::win32::varargs_msgbox(
    UINT buttons,
    const wchar_t* title,
    const wchar_t* fmt,
    ...
    )
{
    va_list args_ptr;
    va_start(args_ptr, fmt);

    wchar_t buff_msg[2048];
    ::_vsnwprintf_s(buff_msg, _countof(buff_msg), _TRUNCATE, fmt, args_ptr);
    va_end(args_ptr);
    ::MessageBoxW(nullptr, buff_msg, title, buttons);
}

v8::base::scoped_ptr<IDXGIFactory, v8::base::com_storage>
v8::base::win32::create_dxgi_factory() {
    scoped_ptr<IDXGIFactory, com_storage> factory;
    ::CreateDXGIFactory(
        __uuidof(IDXGIFactory),
        reinterpret_cast<void**>(scoped_pointer_get_impl(factory)));
    return factory;
}

std::vector<v8::base::win32::unique_adaptor_handle_t>
v8::base::win32::enum_graphic_adapters(IDXGIFactory* factory) {
    std::vector<v8::base::win32::unique_adaptor_handle_t> adapters;
    UINT current_index = 0;
    IDXGIAdapter* current_adapter;
    while (factory->EnumAdapters(current_index++, &current_adapter) == S_OK)
        adapters.push_back(unique_adaptor_handle_t(current_adapter));

    return adapters;
}

std::vector<v8::base::win32::unique_output_handle_t>
v8::base::win32::enum_adapter_outputs(IDXGIAdapter* adapter) {
    std::vector<unique_output_handle_t> outputs;
    UINT current_index = 0;
    IDXGIOutput* current_output;
    while (adapter->EnumOutputs(current_index++, &current_output) == S_OK)
        outputs.push_back(unique_output_handle_t(current_output));
    return outputs;
}

std::vector<DXGI_MODE_DESC>
v8::base::win32::enum_output_modes(
    IDXGIOutput* output, 
    DXGI_FORMAT fmt,
    UINT enum_flags /* = 0 */
    ) {
    std::vector<DXGI_MODE_DESC> modes_list;

    UINT item_count = 0;
    output->GetDisplayModeList(fmt, enum_flags, &item_count, nullptr);
    modes_list.resize(item_count);
    output->GetDisplayModeList(fmt, enum_flags, &item_count, &modes_list[0]);
    return modes_list;
}

//const std::vector<DXGI_MODE_DESC>&
//v8::base::win32::graphics_adapter_output::get_modes_for_format( 
//    const format_enum_options& fmt_options 
//    )
//{
//    auto cache_itr = format_and_modes_.find(fmt_options);
//    if (cache_itr != std::end(format_and_modes_))
//        return cache_itr->second;
//
//    std::vector<DXGI_MODE_DESC> modes(enum_output_modes(
//        scoped_pointer_get(output_interface_), 
//        fmt_options.format_, fmt_options.flags_));
//
//    if (!modes.empty()) {
//        format_and_modes_.insert(std::make_pair(fmt_options, modes));
//    }
//    return modes;
//}