#pragma once

#include <DxErr.h>

#ifndef WIDEN2
#define WIDEN2(x) L ## x
#endif

#ifndef WIDEN
#define WIDEN(x) WIDEN2(x)
#endif

#ifndef __WFILE__
#define __WFILE__ WIDEN(__FILE__)
#endif

namespace v8 { namespace base { namespace debug {

void string_v_format(
    wchar_t*        dst_str,
    size_t          dst_len,
    const wchar_t*  fmt_spec,
    ...
    );

void output_debug_string(
    const wchar_t* file,
    int line,
    const wchar_t* fmt_msg,
    ...
    );

void output_debug_string(
    const char* file,
    int line,
    const char* fmt,
    ...
    );

} // namespace debug
} // namespace base
} // namespace v8

#ifndef NOT_REACHED
#define NOT_REACHED() __debugbreak()
#endif

#ifndef NOT_REACHED_MSG
#define NOT_REACHED_MSG(msg, ...)   \
    do {                            \
        v8::base::debug::output_debug_string(__WFILE__, __LINE__, msg, ##__VA_ARGS__); \
        __debugbreak(); \
    } while (0)
#endif

#ifndef NOT_REACHED_MSGA
#define NOT_REACHED_MSGA(msg, ...)   \
    do {                            \
        v8::base::debug::output_debug_string(__FILE__, __LINE__, msg, ##__VA_ARGS__); \
        __debugbreak(); \
    } while (0)
#endif

#ifndef WIN32_CHK_FNCALL
#define WIN32_CHK_FNCALL(ret_code_ptr, func_and_args) \
    do {                            \
        *(ret_code_ptr) = (func_and_args);  \
        if (*(ret_code_ptr) == FALSE) {   \
            v8::base::debug::output_debug_string(__WFILE__, __LINE__, \
                            L"Function %s failed, error %d", \
                            #func_and_args, ::GetLastError()); \
        } \
    } while (0)
#endif

#ifndef CHECK_D3D
#define CHECK_D3D(ret_code_ptr, call_and_args)  \
    do {                                         \
        *(ret_code_ptr) = (call_and_args);              \
        if (FAILED(*(ret_code_ptr))) {                  \
            const wchar_t* errString = ::DXGetErrorString(*(ret_code_ptr)); \
            v8::base::debug::output_debug_string(__WFILE__, __LINE__,\
                                L"Call %s failed, HRESULT %#08x," \
                                L"error string %s", \
                                L#call_and_args, *(ret_code_ptr), \
                                errString ? errString : L"no aditional info"); \
        } \
    } while (0)
#endif

#ifndef OUTPUT_DBG_MSGW
#define OUTPUT_DBG_MSGW(msg, ...)   \
    do {                            \
        v8::base::debug::output_debug_string(__WFILE__, __LINE__, msg, ##__VA_ARGS__);  \
    } while (0)
#endif

#ifndef OUTPUT_DBG_MSGA
#define OUTPUT_DBG_MSGA(fmt, ...)   \
    do {                            \
        v8::base::debug::output_debug_string(__FILE__, __LINE__, fmt, ##__VA_ARGS__);   \
    } while (0)
#endif