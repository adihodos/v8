#ifndef GFX_LIB_PLANE_H__
#define GFX_LIB_PLANE_H__

#include "gfx_misc.h"
#include "vector3.h"

namespace gfx {
  
class plane {
public :
  vector3 normal_;
  float   offset_;
  
  plane() {}
  
  plane(const vector3& normal, float offset = 0.0f) 
    : normal_(normal_), offset_(offset) {}
    
  plane(float A, float B, float C, float D) : normal_(A, B, C), offset_(D) {}
  
  static plane from_point_and_normal(const vector3& pt, const vector3& normal)
    : normal_(normal) {
      offset_ = -(normal_.x_ * pt.x_ + normal_.y_ * pt.y_ + normal_.z_ * pt.z_);
  }
  
  static plane from_point_and_parallel_directions(
    const vector3& pt, const vector3& dir1, const vector3& dir2
  )
  {
    return plane::from_point_and_normal(pt, cross_product(dir1, dir2));    
  }
};

inline
float
distance_from_point_to_plane(
  const vector3& pt,
  const plane& p
)
{
  float t = -(dot_product(p.normal_, pt) + p.offset_) / 
              p.normal_.sum_components_squared(); 
    
  vector3 intercept(p.normal_ * t + pt);
  return distance(pt, intercept);
}
  
} // namespace gfx

#endif // !GFX_LIB_PLANE_H__