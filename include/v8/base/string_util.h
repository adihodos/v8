#pragma once

#include "v8/config/config.h"

#if defined(MSVC_BUILD_SYSTEM)
#include "v8/base/string_util_msvc.h"
#elif defined(MINGW_BUILD_SYSTEM)
#include "v8/base/string_util_std.h"
#else
#error Unknown build system.
#endif 

