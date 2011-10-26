/*
 * vector3.h
 *
 *  Created on: Oct 18, 2011
 *      Author: adi.hodos
 */

#ifndef GFX_LIB_VECTOR3_H__
#define GFX_LIB_VECTOR3_H__

#include <cassert>
#include <cstring>
#include <algorithm>
#include "gfx_misc.h"

namespace gfx {

class vector3 {
public:
  union {
    struct {
      float x_;
      float y_;
      float z_;
    };
    float elements_[3];
  };

  static const vector3 null;

  static const vector3 unit;

  static const vector3 unit_x;

  static const vector3 unit_y;
  
  static const vector3 unit_z;

  vector3() {}

  vector3(float x, float y, float z) : x_(x), y_(y), z_(z) {}

  vector3(const float* input, size_t count) {
    std::memcpy(elements_, input, std::min(_countof(elements_), count));
  }

  vector3& operator+=(const vector3& rhs) {
    x_ += rhs.x_;
    y_ += rhs.y_;
    z_ += rhs.z_;
    return *this;
  }

  vector3& operator-=(const vector3& rhs) {
    x_ -= rhs.x_;
    y_ -= rhs.y_;
    z_ -= rhs.z_;
    return *this;
  }

  vector3& operator*=(float k) {
    x_ *= k;
    y_ *= k;
    z_ *= k;
    return *this;
  }

  vector3& operator/=(float k) {
    assert(!is_zero(k));
    x_ /= k;
    y_ /= k;
    z_ /= k;
    return *this;
  }

  float sum_components_squared() const {
    return x_ * x_ + y_ * y_ + z_ * z_;
  }

  float magnitude() const {
    return std::sqrt(sum_components_squared());
  }

  vector3& normalize() {
    float magn(magnitude());
    if (is_zero(magn)) {
      x_ = y_ = z_ = 0.0f;
    } else {
      x_ /= magn; y_ /= magn; z_ /= magn;
    }
    return *this;
  }

  float* get_elements() {
    return elements_;
  }

  const float* get_elements() const {
    return elements_;
  }
};

inline
bool
operator==(const vector3& lhs, const vector3& rhs) {
    return is_zero(lhs.x_ - rhs.x_) && is_zero(lhs.y_ - rhs.y_) &&
         is_zero(lhs.z_ - rhs.z_);
}

inline
bool
operator!=(const vector3& lhs, const vector3& rhs) {
    return !(lhs == rhs);
}

inline
vector3
operator+(const vector3& lhs, const vector3& rhs) {
    vector3 res(lhs);
    res += rhs;
    return res;
}

inline
vector3
operator-(const vector3& lhs, const vector3& rhs) {
    vector3 res(lhs);
    res -= rhs;
    return res;
}

inline
vector3
operator-(const vector3& vec) {
    return vector3(-vec.x_, -vec.y_, -vec.z_);
}

inline
vector3
operator*(const vector3& vec, float k) {
    vector3 result(vec);
    result *= k;
    return result;
}

inline
vector3
operator*(
  float k, 
  const vector3& vec
) 
{
    return vec * k;
}

inline
vector3
operator/(
  const vector3& vec, 
  float k
) 
{
    vector3 result(vec);
    result /= k;
    return result;
}

inline
float
dot_product(
  const vector3& lhs, 
  const vector3& rhs
) 
{
    return lhs.x_ * rhs.x_ + lhs.y_ * rhs.y_ + lhs.z_ * rhs.z_;
}

inline
vector3
cross_product(
  const vector3& lhs, 
  const vector3& rhs
) 
{
  return vector3(
      lhs.y_ * rhs.z_ - lhs.z_ * rhs.y_,
      lhs.z_ * rhs.x_ - lhs.x_ * rhs.z_,
      lhs.x_ * rhs.y_ - lhs.y_ * rhs.x_
      );
}

inline
bool
ortho_test(
  const vector3& lhs, 
  const vector3& rhs
) 
{
    return is_zero(dot_product(lhs, rhs));
}

inline
float
angle_of(
  const vector3& lhs, 
  const vector3& rhs
) 
{
    return std::acos(dot_product(lhs, rhs) / (lhs.magnitude() * rhs.magnitude()));
}

inline
vector3
projection_of(
  const vector3& lhs, 
  const vector3& rhs
) 
{
    return (dot_product(lhs, rhs) / rhs.sum_components_squared()) * rhs;
}

inline
vector3
normal_of(
  const vector3& vec
) 
{
    vector3 res(vec);
    res.normalize();
    return res;
}

inline
float
triple_scalar_product(
  const vector3& v1, 
  const vector3& v2, 
  const vector3& v3
) 
{
  return dot_product(cross_product(v1, v2), v3);
}

inline
vector3
triple_vector_product(
  const vector3& v1, 
  const vector3& v2, 
  const vector3& v3
) 
{
  return cross_product(cross_product(v1, v2), v3);
}

inline
float
distance_squared(
  const vector3& point1, 
  const vector3& point2
)
{
  float px = point2.x_ - point1.x_;
  float py = point2.y_ - point1.y_;
  float pz = point2.z_ - point1.z_;
  return px * px + py * py + pz * pz;
}

inline
float distance(
  const vector3& point1,
  const vector3& point2
)
{
  return std::sqrt(distance_squared(point1, point2));
}

} /* namespace gfx */
#endif /* GFX_LIB_VECTOR3_H__ */
