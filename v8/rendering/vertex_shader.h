#pragma once

#if defined(RENDER_SYSTEM_DIRECTX_11) || defined(RENDER_SYSTEM_DIRECTX10)
#include "v8/rendering/directx/directx_vertex_shader.h"
#elif defined(RENDER_SYSTEM_OPENGL)
#else
#error "No rendering system defined"
#endif