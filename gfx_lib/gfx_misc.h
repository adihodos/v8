/*
 * gfx_misc.h
 *
 *  Created on: Oct 5, 2011
 *      Author: adi.hodos
 */

#pragma once

#include <cassert>
#include <cmath>

namespace gfx {

const float EPSILON = 0.000001f;

const float PI = 3.14159265f;

template<typename T>
inline T clamp(const T& val, const T& min, const T& max) {
  return val <= min ? min : (val >= max ? max : val);
}

inline
float
deg2rads(
    float degs
    )
{
  return (PI * degs) / 180.0f;
}

inline
float
rads2degs(
    float rads
    )
{
  return (rads * 180.0f) / PI;
}

inline
bool
is_zero(
    float val
    )
{
  return std::fabs(val) <= EPSILON;
}

struct gfx_countof_helper {
  template<typename T, std::size_t size>
  static std::size_t array_size(T (&)[size]) {
    return size;
  }
};

#ifndef _countof
#define _countof(array) \
  gfx_countof_helper::array_size(array)
#endif

#ifndef DISABLE_WARNING_BLOCK_BEGIN
#define DISABLE_WARNING_BLOCK_BEGIN(warning_id)  \
  __pragma(warning(push)) \
  __pragma(warning(disable : warning_id))
#endif

#ifndef DISABLE_WARNING_BLOCK_END
#define DISABLE_WARNING_BLOCK_END(warning_id) \
  __pragma(warning(pop))
#endif

} // ns gfx
