//
// Copyright (c) 2011, 2012, Adrian Hodos
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of the author nor the
//       names of its contributors may be used to endorse or promote products
//       derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE AUTHOR OR THE CONTRIBUTORS BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#pragma once

#include "v8/base/compiler_warnings.h"

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

/*
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
*/

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
