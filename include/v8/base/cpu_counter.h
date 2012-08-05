#pragma once

#include "v8/config/config.h"

#if defined(MSVC_BUILD_SYSTEM) || defined(MINGW_BUILD_SYSTEM)
#include "v8/base/cpu_counter_win.h"
#else
#error Undefined build system.
#endif

