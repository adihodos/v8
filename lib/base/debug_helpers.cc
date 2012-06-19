#include "pch_hdr.h"
#include "v8/base/count_of.h"
#include "v8/base/string_util.h"
#include "v8/base/debug_helpers.h"

void v8::base::debug::string_v_format(
    wchar_t*    dst_str,
    size_t      dst_len,
    const wchar_t* fmt_spec,
    ...
    )
{
    va_list args_ptr;
    va_start(args_ptr, fmt_spec);
    v8::base::vsnwprintf(dst_str, dst_len, fmt_spec, args_ptr);
    va_end(args_ptr);
}

void v8::base::debug::output_debug_string(
    const wchar_t* file,
    int line,
    const wchar_t* fmt_spec,
    ...
    )
{
    wchar_t buff_msg[2048];
    v8::base::snwprintf(buff_msg, count_of_array(buff_msg), 
                        L"\n[File %s, line %d]\n", file, line);
    ::OutputDebugStringW(buff_msg);

    va_list args_ptr;
    va_start(args_ptr, fmt_spec);
    v8::base::vsnwprintf(buff_msg, count_of_array(buff_msg),
                         fmt_spec, args_ptr);
    va_end(args_ptr);
    ::OutputDebugStringW(buff_msg);
}

void v8::base::debug::output_debug_string(
    const char* file,
    int line,
    const char* fmt,
    ...
    )
{
    char buff_msg[2048];
    v8::base::snprintf(buff_msg, count_of_array(buff_msg), 
                       "\n[File %s, line %d]\n", file, line);
    ::OutputDebugStringA(buff_msg);

    va_list args_ptr;
    va_start(args_ptr, fmt);
    v8::base::vsnprintf(buff_msg, count_of_array(buff_msg), fmt, args_ptr);
    va_end(args_ptr);
    ::OutputDebugStringA(buff_msg);
}
