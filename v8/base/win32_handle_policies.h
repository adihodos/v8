#pragma once

#include <Windows.h>

namespace v8 { namespace base {

/**
 *\brief Management policy for file handles returned by CreateHandle().
 */
struct win32_file_handle {
    static HANDLE null_handle() {
        return INVALID_HANDLE_VALUE;
    }

    static void dispose(HANDLE file_handle) {
        if (file_handle != INVALID_HANDLE_VALUE)
            ::CloseHandle(file_handle);
    }
};

} // namespace base
} // namespace v8
