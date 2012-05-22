#pragma once

#include <cstdio>

namespace v8 { namespace base {

struct crt_file_handle {
    typedef FILE*   handle_t;

    static FILE* null_handle() {
        return nullptr;
    }

    static void dispose(FILE* fp) {
        if (fp)
            ::fclose(fp);
    }
};

} // namespace base
} // namespace v8