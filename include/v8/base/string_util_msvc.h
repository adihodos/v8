#pragma once

#include <stdio.h>
#include <stddef.h>
#include <stdarg.h>

namespace v8 { namespace base {

//
// See this http://stackoverflow.com/questions/2915672/snprintf-and-visual-studio-2010
inline int 
vsnprintf(char* buffer, size_t size, const char* fmt, va_list args_ptr) {
    int count = -1;

    if (size) {
        count = _vsnprintf_s(buffer, size, _TRUNCATE, fmt, args_ptr);
    } 
    if (count == -1)
        count = _vscprintf(fmt, args_ptr);

    return count;
}

inline int
vsnwprintf(wchar_t* buff, size_t size, const wchar_t* fmt, va_list args_ptr) {
    int count = -1;

    if (size)
        count = _vsnwprintf_s(buff, size, _TRUNCATE, fmt, args_ptr);
    if (count == -1)
        count = _vscwprintf(fmt, args_ptr);

    return count;
}

inline int 
snprintf(char* buffer, size_t buff_size, const char* format, ...) {
    va_list args_ptr;
    va_start(args_ptr, format);
    int count = v8::base::vsnprintf(buffer, buff_size, format, args_ptr);
    va_end(args_ptr);
    return count;
}

inline int
snwprintf(wchar_t* buffer, size_t buff_size, const wchar_t* fmt, ...) {
    va_list args_ptr;
    va_start(args_ptr, fmt);
    int count = v8::base::vsnwprintf(buffer, buff_size, fmt, args_ptr);
    va_end(args_ptr);
    return count;
}

} // namespace base
} // namespace v8
