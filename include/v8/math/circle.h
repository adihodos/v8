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

#include "v8/math/vector2.h"

namespace v8 { namespace math {

class circle {
public :
  circle() {}
  
  circle(const vector2F& centre, float radius) 
    : centre_(centre), radius_(radius) {}
    
  circle(float cx, float cy, float radius)
    : centre_(cx, cy), radius_(radius) {}
  
  const vector2F& get_centre_point() const {
    return centre_;
  }
  
  float get_radius() const {
    return radius_;
  }
    
  void set_centre_point(float x, float y) {
    centre_.x_ = x;
    centre_.y_ = y;
  }
  
  void set_centre_point(const vector2F& pt) {
    set_centre_point(pt.x_, pt.y_);
  }
  
  void set_radius(float r) {
    radius_ = r;
  }
private :
  vector2F   centre_;
  float     radius_;
};

inline
bool
intersect_test(
  const circle& c1,
  const circle& c2
  )
{
  return distance(c1.get_centre_point(), c2.get_centre_point()) <= 
         c1.get_radius() + c2.get_radius();
}

inline
bool
point_in_circle(
  const vector2F& pt,
  const circle& c
  )
{
  return distance(pt, c.get_centre_point()) <= c.get_radius();
}

} // namespace math
} // namespace v8