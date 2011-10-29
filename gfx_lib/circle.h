#pragma once

#include "vector2.h"

namespace gfx {

class circle {
public :
  circle() {}
  
  circle(const vector2& centre, float radius) 
    : centre_(centre), radius_(radius) {}
    
  circle(float cx, float cy, float radius)
    : centre_(cx, cy), radius_(radius) {}
  
  const vector2& get_centre_point() const {
    return centre_;
  }
  
  float get_radius() const {
    return radius_;
  }
    
  void set_centre_point(float x, float y) {
    centre_.x_ = x;
    centre_.y_ = y;
  }
  
  void set_centre_point(const vector2& pt) {
    set_centre_point(pt.x_, pt.y_);
  }
  
  void set_radius(float r) {
    radius_ = r;
  }
private :
  vector2   centre_;
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
  const vector2& pt,
  const circle& c
  )
{
  return distance(pt, c.get_centre_point()) <= c.get_radius();
}

} // namespace gfx