#pragma once

#include <cstdio>
#include "v8/base/handle_traits.h"

namespace v8 { namespace base {

struct crt_file_handle : public handle_traits_base<FILE*> {
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
