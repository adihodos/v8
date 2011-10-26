/*
 * matrix3x3.h
 *
 *  Created on: Oct 11, 2011
 *      Author: adi.hodos
 */

#ifndef GFX_LIB_MATRIX3X3_H_
#define GFX_LIB_MATRIX3X3_H_

#include <cassert>
#include "gfx_misc.h"
#include "vector2.h"
#include "vector3.h"

namespace gfx {

class matrix3X3;

inline
matrix3X3
transpose_of(
  const matrix3X3&
  );

matrix3X3
inverse_of(
  const matrix3X3&
  );

class matrix3X3 {
public:
  union {
    struct {
      float a11_, a12_, a13_;
      float a21_, a22_, a23_;
      float a31_, a32_, a33_;
    };
    float elements_[9];
  };

  static const matrix3X3 null;

  static const matrix3X3 identity;

  static matrix3X3 translation(float x0, float y0) {
    return matrix3X3(
        1.0f, 0.0f, x0,
        0.0f, 1.0f, y0,
        0.0f, 0.0f, 1.0f
        );
  }

  static matrix3X3 translation(const vector2& org) {
    return matrix3X3::translation(org.x_, org.y_);
  }

  static matrix3X3 rotation(float theta, float x_org = 0.0f, float y_org = 0.0f) {
    float sin_theta = std::sin(deg2rads(theta));
    float cos_theta = std::cos(deg2rads(theta));
    /*
     * x = x0 + (x - x0) cost - (y - y0) sint
     * y = y0 + (x - x0) sint + (y - y0) cost
     */
    return matrix3X3(
        cos_theta, -sin_theta, 	x_org * (1.0f - cos_theta) + y_org * sin_theta,
        sin_theta,  cos_theta, 	y_org * (1.0f - cos_theta) + x_org * sin_theta,
        0.0f, 		0.0f, 		1.0f
        );
  }

  static matrix3X3 rotation(float theta, const vector2& org) {
    return matrix3X3::rotation(theta, org.x_, org.y_);
  }

  static matrix3X3 scale(float sx, float sy, float x_org = 0.0f, float y_org = 0.0f) {
    /*
     * x = x0 + (x - x0) sx = x0 + xsx - x0sx = sxx + x0(1-sx)
     */
    return matrix3X3(
        sx, 0.0f, (1.0f - sx) * x_org,
        0.0f, sy, (1.0f - sy) * y_org,
        0.0f, 0.0f, 1.0f
        );
  }

  static matrix3X3 scale(float sx, float sy, const vector2& org) {
    return matrix3X3::scale(sx, sy, org.x_, org.y_);
  }

  matrix3X3() {}

  matrix3X3(
      float a11, float a12, float a13,
      float a21, float a22, float a23,
      float a31, float a32, float a33
      );

  matrix3X3(const float* input, size_t count) {
    std::memcpy(elements_, input, std::min(_countof(elements_), count));
  }

  matrix3X3& operator+=(const matrix3X3& rhs);

  matrix3X3& operator-=(const matrix3X3& rhs);

  matrix3X3& operator*=(float k);

  matrix3X3& operator/=(float k);

  float determinant() const;

  bool is_invertible() const {
    return !is_zero(determinant());
  }

  matrix3X3& invert() {
    *this = inverse_of(*this);
    return *this;
  }

  matrix3X3& transpose() {
    *this = transpose_of(*this);
    return *this;
  }
};

inline
matrix3X3
operator+(
  const matrix3X3& lhs, 
  const matrix3X3& rhs
  ) 
{
  matrix3X3 result(lhs);
  result += rhs;
  return result;
}

inline
matrix3X3
operator-(
  const matrix3X3& lhs, 
  const matrix3X3& rhs
  ) 
{
  matrix3X3 result(lhs);
  result -= rhs;
  return result;
}

inline
matrix3X3
operator-(
  const matrix3X3& mtx
  ) 
{
  matrix3X3 result(mtx);
  result *= -1.0f;
  return result;
}

matrix3X3
operator*(
  const matrix3X3& lhs, 
  const matrix3X3& rhs
  );

inline
matrix3X3
operator*(
  float k, 
  const matrix3X3& mtx
  ) 
{
  matrix3X3 result(mtx);
  result *= k;
  return result;
}

inline
matrix3X3
operator*(
  const matrix3X3& mtx, 
  float k
  ) 
{
  return k * mtx;
}

inline
vector2
operator*(
  const matrix3X3& mtx, 
  const vector2& vec
  ) 
{
  return vector2(
      mtx.a11_ * vec.x_ + mtx.a12_ * vec.y_ + mtx.a13_,
      mtx.a21_ * vec.x_ + mtx.a22_ * vec.y_ + mtx.a23_
      );
}

inline
vector3
operator*(
  const matrix3X3& mtx,
  const vector3& vec
  )
{
  return vector3(
    mtx.a11_ * vec.x_ + mtx.a12_ * vec.y_ + mtx.a13_ * vec.z_,
    mtx.a21_ * vec.x_ + mtx.a22_ * vec.y_ + mtx.a23_ * vec.z_,
    mtx.a31_ * vec.x_ + mtx.a32_ * vec.y_ + mtx.a33_ * vec.z_
    );
}

inline
matrix3X3
transpose_of(const matrix3X3& mtx) {
  return matrix3X3(
      mtx.a11_, mtx.a21_, mtx.a31_,
      mtx.a12_, mtx.a22_, mtx.a32_,
      mtx.a13_, mtx.a23_, mtx.a33_
      );
}

matrix3X3
adjoint_of(
  const matrix3X3&
  );

} /* namespace gfx */
#endif /* MATRIX3X3_H_ */
