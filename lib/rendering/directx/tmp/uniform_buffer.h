#pragma once

#if defined(RENDER_SYSTEM_DIRECTX_11) || defined(RENDER_SYSTEM_DIRECTX10)
#include "v8/rendering/directx/directx_uniform_buffer.h"
#elif defined(RENDER_SYSTEM_OPENGL)
#else
#error "No rendering system defined"
#endif