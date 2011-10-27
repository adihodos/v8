/*
 * gfx_matrix.h
 *
 *  Created on: Oct 6, 2011
 *      Author: adi.hodos
 */

#ifndef GFX_LIB_MATRIX4X4_H__
#define GFX_LIB_MATRIX4X4_H__

#include <cassert>
#include <cmath>
#include <cstring>
#include <algorithm>
#include "gfx_misc.h"

namespace gfx {

class vector3;
class vector4;

class matrix4X4;

inline
matrix4X4
transpose_of(
  const matrix4X4&
  );

matrix4X4
inverse_of(
  const matrix4X4&
  );

class matrix4X4 {
private :
  int index_at(int row, int col) const {
    return (row - 1) * 4 + col - 1;
  }
public :
  union {
    struct {
      float a11_, a12_, a13_, a14_;
      float a21_, a22_, a23_, a24_;
      float a31_, a32_, a33_, a34_;
      float a41_, a42_, a43_, a44_;
    };
    float elements_[16];
  };

  static const matrix4X4	null;

  static const matrix4X4	identity;

  static matrix4X4 translation(float x, float y, float z) {
    matrix4X4 mtx(matrix4X4::identity);
    mtx.a14_ = x;
    mtx.a24_ = y;
    mtx.a34_ = z;
    return mtx;
  }

  static void	invert_translation_matrix(matrix4X4* mtx) {
    mtx->a14_ = -mtx->a14_;
    mtx->a24_ = -mtx->a24_;
    mtx->a34_ = -mtx->a34_;
  }

  matrix4X4() {}

  matrix4X4(
      float a11, float a12, float a13, float a14,
      float a21, float a22, float a23, float a24,
      float a31, float a32, float a33, float a34,
      float a41, float a42, float a43, float a44
      );

  matrix4X4(const float* input, size_t count) {
    std::memcpy(elements_, input, std::min(_countof(elements_), count));
  }

  float& operator()(int row, int col) {
    return elements_[index_at(row, col)];
  }

  float operator()(int row, int col) const {
    return elements_[index_at(row, col)];
  }

  matrix4X4& operator+=(const matrix4X4& rhs);

  matrix4X4& operator-=(const matrix4X4& rhs);

  matrix4X4& operator*=(float k);

  matrix4X4& operator/=(float k);

  float determinant() const;

  bool is_invertible() const {
    return !is_zero(determinant());
  }

  matrix4X4& invert() {
    *this = inverse_of(*this);
    return *this;
  }

  matrix4X4& transpose() {
    *this = transpose_of(*this);
    return *this;
  }
};

inline
matrix4X4
operator+(
    const matrix4X4& lhs,
    const matrix4X4& rhs
    )
{
  matrix4X4 result(lhs);
  result += rhs;
  return result;
}

inline
matrix4X4
operator-(
    const matrix4X4& lhs,
    const matrix4X4& rhs
    )
{
  matrix4X4 result(lhs);
  result -= rhs;
  return result;
}

inline
matrix4X4
operator-(
    const matrix4X4& mtx
    )
{
  return matrix4X4(
      -mtx.a11_, -mtx.a12_, -mtx.a13_, -mtx.a14_,
      -mtx.a21_, -mtx.a22_, -mtx.a23_, -mtx.a24_,
      -mtx.a31_, -mtx.a32_, -mtx.a33_, -mtx.a34_,
      -mtx.a41_, -mtx.a42_, -mtx.a43_, -mtx.a44_
      );
}

matrix4X4
operator*(
    const gfx::matrix4X4& lhs,
    const gfx::matrix4X4& rhs
    );

vector3
operator*(
    const matrix4X4& mtx,
    const vector3& vec
    );

vector4
operator*(
  const matrix4X4& mtx,
  const vector4& vec
  );

inline
matrix4X4
operator*(
    float k,
    const matrix4X4& mtx
    )
{
  matrix4X4 result(mtx);
  result *= k;
  return result;
}

inline
matrix4X4
operator*(
    const matrix4X4& mtx,
    float k
    )
{
  return k * mtx;
}

inline
matrix4X4
operator/(
    const matrix4X4& mtx,
    float k
    )
{
  assert(!is_zero(k));

  matrix4X4 result(mtx);
  result *= (1.0f / k);
  return result;
}

inline
matrix4X4
transpose_of(
    const matrix4X4& mtx
    )
{
  return matrix4X4(
    mtx.a11_, mtx.a21_, mtx.a31_, mtx.a41_,
    mtx.a12_, mtx.a22_, mtx.a32_, mtx.a42_,
    mtx.a13_, mtx.a23_, mtx.a33_, mtx.a43_,
    mtx.a14_, mtx.a24_, mtx.a34_, mtx.a44_
    );
}

matrix4X4
inverse_of(
    const matrix4X4&
    );

matrix4X4
adjoint_of(
  const matrix4X4&
  );

} // ns gfx

#endif /* GFX_MATRIX_H_ */