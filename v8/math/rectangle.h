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

class rectangle {
public :
  rectangle() {}
  
  rectangle(float left, float top, float right, float bottom)
    : top_left_(left, top), bot_right_(right, bottom) {}
    
  rectangle(const vector2F& tl, const vector2F& br)
    : top_left_(tl), bot_right_(br) {}

  float width() const {
    return bot_right_.x_ - top_left_.x_;
  }
  
  float height() const {
    return bot_right_.y_ - top_left_.y_;
  }
  
  const vector2F& get_top_left_point() const {
    return top_left_;
  }
  
  const vector2F& get_bottom_right_point() const {
    return bot_right_;
  }
  
  void set_top_left_point(float x, float y) {
    top_left_.x_ = x;
    top_left_.y_ = y;
  }
  
  void set_top_left_point(const vector2F& pt) {
    set_top_left_point(pt.x_, pt.y_);
  }
  
  void set_bottom_right_point(float x, float y) {
    bot_right_.x_ = x;
    bot_right_.y_ = y;
  }
  
  void set_bottom_right_point(vector2F& pt) {
    set_bottom_right_point(pt.x_, pt.y_);
  }
  
  vector2F get_centre_point() const {
    return (bot_right_ + top_left_) / 2;
  }
private :
  vector2F top_left_;
  vector2F bot_right_;
};

inline
bool
point_in_rectangle(
  const vector2F& point,
  const rectangle& rc
  )
{
  return point.x_ >= rc.get_top_left_point().x_ &&
         point.x_ <= rc.get_bottom_right_point().x_ &&
         point.y_ >= rc.get_top_left_point().y_ &&
         point.y_ <= rc.get_bottom_right_point().y_;
}

} // namespace math
} // namespace v8