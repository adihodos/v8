/**
 * \file    vector4.h
 *
 * \brief   Declares the vector 4 class and related functions.
 */
#ifndef GFX_LIB_VECTOR4_H__
#define GFX_LIB_VECTOR4_H__

#include <cassert>
#include <cmath>
#include <cstring>
#include <algorithm>
#include <vector>
#include "vector3.h"
#include "gfx_misc.h"

namespace gfx {

/**
 * \class   vector4
 *
 * \brief   A four component vector.
 *
 */
class vector4 {
public :
  union {
    struct {
      float x_;
      float y_;
      float z_;
      float w_;
    };
    float   elements_[4];   ///< Array like access to the vector's elements */
  };

  static const vector4	null;

  static const vector4	unit;

  static const vector4	unit_x;

  static const vector4	unit_y;

  static const vector4	unit_z;

  static const vector4  unit_w;

  /**
   * \fn    vector4::vector4()
   *
   * \brief Default constructor. Leaves elements uninitialized.
   *
   */
  vector4() {}

  static inline vector4 from_point(const gfx::vector3& pt) {
      return vector4(pt.x_, pt.y_, pt.z_, 1.0f);
  }

  static inline vector4 from_vector(const gfx::vector3& v) {
      return vector4(v.x_, v.y_, v.z_, 0.0f);
  }

  vector4(float x, float y, float z, float w)
    : x_(x), y_(y), z_(z), w_(w) {}

  vector4(const float* input, size_t count) {
    std::memcpy(elements_, input, std::min(_countof(elements_), count));
  }

  vector4& operator+=(const vector4& rhs) {
    x_ += rhs.x_; y_ += rhs.y_; z_ += rhs.z_;
    return *this;
  }

  vector4& operator-=(const vector4& rhs) {
    x_ -= rhs.x_; y_ -= rhs.y_; z_ -= rhs.z_;
    return *this;
  }

  vector4& operator*=(float k) {
    x_ *= k; y_ *= k; z_ *= k;
    return *this;
  }

  vector4& operator/=(float k) {
    x_ /= k; y_ /= k; z_ /= k;
    return *this;
  }

  float sum_components_squared() const {
    return x_ * x_ + y_ * y_ + z_ * z_;
  }

  float magnitude() const {
    return std::sqrt(sum_components_squared());
  }

  vector4& normalize() {
    float mag(magnitude());
    if (is_zero(mag)) {
      x_ = y_ = z_ = 0.0f;
    } else {
      x_ /= mag;
      y_ /= mag;
      z_ /= mag;
    }
    return *this;
  }
};

inline
bool
operator==(
    const vector4& lhs,
    const vector4& rhs
    ) {
  return is_zero(lhs.x_ - rhs.x_) && is_zero(lhs.y_ - rhs.y_) &&
         is_zero(lhs.z_ - rhs.z_);
}

inline
bool
operator!=(
    const vector4& lhs,
    const vector4& rhs
    ) {
  return !(lhs == rhs);
}

inline
vector4
operator-(const vector4& vec) {
  return vector4(-vec.x_, -vec.y_, -vec.z_, -vec.w_);
}

inline
vector4
operator+(const vector4& lhs, const vector4& rhs) {
  return vector4(lhs.x_ + rhs.x_, lhs.y_ + rhs.y_, lhs.z_ + rhs.z_, 0.0f);
}

inline
vector4
operator-(const vector4& lhs, const vector4& rhs) {
  return vector4(lhs.x_ - rhs.x_, lhs.y_ - rhs.y_, lhs.z_ - rhs.z_, lhs.w_ - rhs.z_);
}

inline
vector4
operator*(
    float k,
    const vector4& vec
    ) {
  vector4 res(vec);
  res *= k;
  return res;
}

inline
vector4
operator*(
    const vector4& vec,
    float k
    ) {
  return k * vec;
}

inline
vector4
operator/(
    const vector4& vec,
    float k
    ) {
  vector4 res(vec);
  res /= k;
  return res;
}

inline
float
dot_product(
    const vector4& v1,
    const vector4& v2
    )
{
  return v1.x_ * v2.x_ + v1.y_ * v2.y_ + v1.z_ * v2.z_;
}

/*!
 * @brief Project vecP onto vecQ.
 */
inline
vector4
projection(
    const vector4& vecP,
    const vector4& vecQ
    ) {
  return (dot_product(vecP, vecQ) / vecQ.sum_components_squared()) * vecQ;
}

inline
bool
ortho_test(
    const vector4& v1,
    const vector4& v2
    )
{
  return is_zero(dot_product(v1, v2));
}

inline
float
angle_of(
    const vector4& lhs,
    const vector4& rhs
    )
{
  return std::acos(dot_product(lhs, rhs) / (lhs.magnitude() * rhs.magnitude()));
}

inline
vector4
normalized_from(const vector4& vec) {
  vector4 res(vec);
  res.normalize();
  return res;
}

/*!
 * @brief Given a set of input vectors, produce a set of ortho normal vectors.
 * @param in Set of input vectors.
 * @param out Set of output vectors.
 * @remarks Running time is O(2nk^2), n = number of elements in the input vector.
 * 			Uses the modified Gram-Schmidt algorithm.
 */
void
mgs_ortho_normalize(
    const std::vector<vector4>& in,
    std::vector<vector4>& out
    );

} // namespace gfx

#endif /* GFX_LIB_VECTOR4_H__ */
