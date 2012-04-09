#pragma once

#include "vector2.h"

namespace gfx {

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

} // namespace gfx