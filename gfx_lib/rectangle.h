#pragma once

#include "gfx_misc.h"
#include "vector2.h"

namespace gfx {

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

} // namespace gfx