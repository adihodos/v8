#pragma once

#include <stdio.h>
#include <stdarg.h>
#include <wchar.h>

namespace v8 { namespace base {

inline int 
vsnprintf(char* buffer, size_t size, const char* fmt, va_list args_ptr) {
    return ::vsnprintf(buffer, size, fmt, args_ptr);
}

inline int
vsnwprintf(wchar_t* buff, size_t size, const wchar_t* fmt, va_list args_ptr) {
    return ::vsnwprintf(buff, size, fmt, args_ptr);
}

inline int 
snprintf(char* buffer, size_t buff_size, const char* format, ...) {
    va_list ap;
    va_start(ap, format);
    int count = v8::base::vsnprintf(buffer, buff_size, format, ap);
    va_end(ap);
    return count;
}

inline int
snwprintf(wchar_t* buffer, size_t buff_size, const wchar_t* fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    int count = v8::base::vsnwprintf(buffer, buff_size, fmt, ap);
    va_end(ap);
    return count;
}

} // namespace base
} // namespace v8

