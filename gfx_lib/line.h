#pragma once

#include "gfx_misc.h"
#include "vector3.h"

namespace gfx {
  
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
  
} // namespace gfx
