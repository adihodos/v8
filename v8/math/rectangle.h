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

template<typename real_t>
class rectangle {
public :
    typedef rectangle<real_t>   self_t;

    rectangle() {}

    rectangle(real_t left, real_t top, real_t right, real_t bottom)
        : top_left_(left, top), bot_right_(right, bottom) {}

    template<typename real_u>
    rectangle(real_u left, real_u top, real_u right, real_u bottom)
        : top_left_(left, top), bot_right_(right, bottom) {}

    template<typename real_u>
    rectangle(const rectangle<real_u>& right)
        : top_left_(right.get_top_left()),
          bot_right_(right.get_bottom_right()) {}

    rectangle(const vector2<real_t>& tl, const vector2<real_t>& br)
        : top_left_(tl), bot_right_(br) {}

    template<typename real_u>
    rectangle(const vector2<real_u>& tl, const vector2<real_u>& br)
        : top_left_(tl), bot_right_(br) {}

    real_t width() const {
        return bot_right_.x_ - top_left_.x_;
    }

    real_t height() const {
        return bot_right_.y_ - top_left_.y_;
    }

    const vector2<real_t>& get_top_left() const {
        return top_left_;
    }

    const vector2<real_t>& get_bottom_right() const {
        return bot_right_;
    }

    void set_top_left(real_t x, real_t y) {
        top_left_.x_ = x;
        top_left_.y_ = y;
    }

    void set_top_left(const vector2<real_t>& pt) {
        set_top_left_point(pt.x_, pt.y_);
    }

    void set_bottom_right(real_t x, real_t y) {
        bot_right_.x_ = x;
        bot_right_.y_ = y;
    }

    void set_bottom_right(vector2<real_t>& pt) {
        set_bottom_right_point(pt.x_, pt.y_);
    }

    vector2<real_t> get_centre() const {
        return (bot_right_ + top_left_) / 2;
    }

    template<typename real_u>
    self_t& operator=(const rectangle<real_u>& right) {
        top_left_ = right.get_top_left();
        bot_right_ = right.get_bottom_right();
        return *this;
    }

private :
    vector2<real_t> top_left_;
    vector2<real_t> bot_right_;
};

template<typename R1, typename R2>
inline bool operator==(const rectangle<R1>& left, const rectangle<R2>& right) {
    return left.get_top_left() == right.get_top_left() &&
           left.get_bottom_right() == right.get_bottom_right();
}

template<typename R1, typename R2>
inline bool operator!=(const rectangle<R1>& left, const rectangle<R2>& right) {
    return !(left == right);
}

template<typename real1, typename real2>
inline
bool
point_in_rectangle(
  const vector2<real1>& point,
  const rectangle<real2>& rc
  )
{
  return point.x_ >= rc.get_top_left().x_ &&
         point.x_ <= rc.get_bottom_right().x_ &&
         point.y_ >= rc.get_top_left().y_ &&
         point.y_ <= rc.get_bottom_right().y_;
}

typedef rectangle<float>    rectF;

typedef rectangle<double>   rectD;

typedef rectangle<int>      rectI;

typedef rectangle<short>    rectS;

typedef rectangle<long>     rectL;

} // namespace math
} // namespace v8
