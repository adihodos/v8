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

class ellipse {
public :
  ellipse() {}
  
  ellipse(const vector2F& center, const vector2F& radius)
    : center_pt_(center), radius_(radius) {}
    
  ellipse(float cp_x, float cp_y, float rd_x, float rd_y)
    : center_pt_(cp_x, cp_y), radius_(rd_x, rd_y) {}
    
  const vector2F& get_center_point() const {
    return center_pt_;
  }
  
  const vector2F& get_radius() const {
    return radius_;
  }
  
  void set_center_point(float cp_x, float cp_y) {
    center_pt_.x_ = cp_x;
    center_pt_.y_ = cp_y;
  }
  
  void set_center_point(const vector2F& pt) {
    set_center_point(pt.x_, pt.y_);
  }
  
  void set_radius_x_axis(float rx) {
    radius_.x_ = rx;
  }
  
  void set_radius_y_axis(float ry) {
    radius_.y_ = ry;
  }
  
  void set_radius(float x, float y) {
    set_radius_x_axis(x);
    set_radius_y_axis(y);
  }
  
  void set_radius(const vector2F& rv) {
    set_radius(rv.x_, rv.y_);
  }
private :
  vector2F   center_pt_;
  vector2F   radius_;
};

} // namespace math
} // namespace v8