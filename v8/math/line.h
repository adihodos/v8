//
// Copyright (c) 2011, 2012, Adrian Hodos
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of the author nor the
//       names of its contributors may be used to endorse or promote products
//       derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE AUTHOR OR THE CONTRIBUTORS BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#pragma once

#include "v8/math/gfx_misc.h"
#include "v8/math/vector3.h"

namespace v8 { namespace math {
  
class line {
private :
  vector3F origin_;
  vector3F direction_;
  
public :  
  line() {}
  
  line(const vector3F& origin, const vector3F& direction)
    : origin_(origin), direction_(direction) {}
    
  const vector3F& get_origin() const { return origin_; }
  
  void set_origin(const vector3F& origin) { origin_ = origin; }
  
  const vector3F& get_direction() const { return direction_; }
  
  void set_direction(const vector3F& direction) { direction_ = direction; }
};

inline
float
distance_from_point_to_line(
  const vector3F& pt, 
  const line& ln
) 
{
  return cross_product(ln.origin_ - pt, ln.direction_).magnitude() 
         / ln.direction_.magnitude();
}

inline
float
angle_of(
  const line& line1, 
  const line& line2
) 
{
  return angle_of(line1.direction_, line2.direction_);
}
  
} // namespace math
} // namespace v8

