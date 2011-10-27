#ifndef GFX_LIB_PCH_HDR_H__
#define GFX_LIB_PCH_HDR_H__

#include <cassert>
#include <cmath>
#include <cstdlib>

#include <algorithm>
#include <functional>
#include <iterator>
#include <vector>

#if defined(D2D_SUPPORT__)
#pragma message("Direct2D support is enabled!")

#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <Windows.h>
#include <d2d1.h>
#include <d2d1helper.h>
#endif

#endif // GFX_LIB_PCH_HDR_H__