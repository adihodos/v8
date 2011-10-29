#pragma once

#if defined(D2D_SUPPORT__)
#include <d2d1.h>
#endif

#include "vector2.h"

namespace gfx {

class ellipse {
public :
  ellipse() {}
  
  ellipse(const vector2& center, const vector2& radius)
    : center_pt_(center), radius_(radius) {}
    
  ellipse(float cp_x, float cp_y, float rd_x, float rd_y)
    : center_pt_(cp_x, cp_y), radius_(rd_x, rd_y) {}
    
#if defined(D2D_SUPPORT__)

  ellipse(const D2D1_ELLIPSE& d2e) 
    : center_pt_(d2e.point), radius_(d2e.radiusX, d2e.radiusY) {}
    
  operator D2D1_ELLIPSE() const {
    return D2D1::Ellipse(center_pt_, radius_.x_, radius_.y_);
  }
  
#endif // D2D_SUPPORT__
    
  const vector2& get_center_point() const {
    return center_pt_;
  }
  
  const vector2& get_radius() const {
    return radius_;
  }
  
  void set_center_point(float cp_x, float cp_y) {
    center_pt_.x_ = cp_x;
    center_pt_.y_ = cp_y;
  }
  
  void set_center_point(const vector2& pt) {
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
  
  void set_radius(const vector2& rv) {
    set_radius(rv.x_, rv.y_);
  }
private :
  vector2   center_pt_;
  vector2   radius_;
};

} // namespace gfx