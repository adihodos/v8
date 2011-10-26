/*
 * gfx_matrix.cc
 *
 *  Created on: Oct 6, 2011
 *      Author: adi.hodos
 */

#include "pch_hdr.h"
#include "matrix4X4.h"
#include "vector3.h"
#include "vector4.h"

const gfx::matrix4X4
gfx::matrix4X4::null(
    0.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 0.0f
    );

const gfx::matrix4X4
gfx::matrix4X4::identity(
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f
    );

gfx::matrix4X4::matrix4X4(
    float a11, float a12, float a13, float a14,
    float a21, float a22, float a23, float a24,
    float a31, float a32, float a33, float a34,
    float a41, float a42, float a43, float a44
    )
{
  a11_ = a11; a12_ = a12; a13_ = a13; a14_ = a14;
  a21_ = a21; a22_ = a22; a23_ = a23; a24_ = a24;
  a31_ = a31; a32_ = a32; a33_ = a33; a34_ = a34;
  a41_ = a41; a42_ = a42; a43_ = a43; a44_ = a44;
}

gfx::matrix4X4&
gfx::matrix4X4::operator +=(
    const gfx::matrix4X4& rhs
    ) {
  a11_ += rhs.a11_; a12_ += rhs.a12_; a13_ += rhs.a13_; a14_ += rhs.a14_;
  a21_ += rhs.a21_; a22_ += rhs.a22_; a23_ += rhs.a23_; a24_ += rhs.a24_;
  a31_ += rhs.a31_; a32_ += rhs.a32_; a33_ += rhs.a33_; a34_ += rhs.a34_;
  a41_ += rhs.a41_; a42_ += rhs.a42_; a43_ += rhs.a43_; a44_ += rhs.a44_;
  return *this;
}

gfx::matrix4X4&
gfx::matrix4X4::operator -=(
    const gfx::matrix4X4& rhs
    ) {
  a11_ -= rhs.a11_; a12_ -= rhs.a12_; a13_ -= rhs.a13_; a14_ -= rhs.a14_;
  a21_ -= rhs.a21_; a22_ -= rhs.a22_; a23_ -= rhs.a23_; a24_ -= rhs.a24_;
  a31_ -= rhs.a31_; a32_ -= rhs.a32_; a33_ -= rhs.a33_; a34_ -= rhs.a34_;
  a41_ -= rhs.a41_; a42_ -= rhs.a42_; a43_ -= rhs.a43_; a44_ -= rhs.a44_;
  return *this;
}

gfx::matrix4X4&
gfx::matrix4X4::operator *=(float k) {
  a11_ *= k; a12_ *= k; a13_ *= k; a14_ *= k;
  a21_ *= k; a22_ *= k; a23_ *= k; a24_ *= k;
  a31_ *= k; a32_ *= k; a33_ *= k; a34_ *= k;
  a41_ *= k; a42_ *= k; a43_ *= k; a44_ *= k;
  return *this;
}

gfx::matrix4X4&
gfx::matrix4X4::operator /=(float scalar) {
  assert(!is_zero(scalar));
  float k  = (1.0f / scalar);

  a11_ *= k; a12_ *= k; a13_ *= k; a14_ *= k;
  a21_ *= k; a22_ *= k; a23_ *= k; a24_ *= k;
  a31_ *= k; a32_ *= k; a33_ *= k; a34_ *= k;
  a41_ *= k; a42_ *= k; a43_ *= k; a44_ *= k;
  return *this;
}

float
gfx::matrix4X4::determinant() const {
  return a11_ * (
      a22_ * a33_ * a44_	+
      a32_ * a43_ * a24_	+
      a42_ * a23_ * a34_	-
      a24_ * a33_ * a42_	-
      a34_ * a43_ * a22_	-
      a44_ * a23_ * a32_
      )
      +
    a12_ * (
      a21_ * a33_ * a44_ 	+
      a31_ * a43_ * a24_	+
      a41_ * a23_ * a34_	-
      a24_ * a33_ * a41_	-
      a34_ * a43_ * a21_	-
      a44_ * a23_ * a31_
      )
      -
    a13_ * (
      a21_ * a32_ * a44_ 	+
      a31_ * a42_ * a24_	+
      a41_ * a22_ * a34_	-
      a24_ * a32_ * a41_ 	-
      a34_ * a42_ * a21_	-
      a44_ * a22_ * a31_
      )
      +
    a14_ * (
      a21_ * a32_ * a43_	+
      a31_ * a42_ * a23_	+
      a41_ * a22_ * a33_	-
      a23_ * a32_ * a41_	-
      a33_ * a42_ * a21_	-
      a43_ * a22_ * a31_
      );
}

gfx::matrix4X4
gfx::operator *(
    const gfx::matrix4X4& lhs,
    const gfx::matrix4X4& rhs
    )
{
  matrix4X4 res;
  res.a11_ = lhs.a11_ * rhs.a11_ + lhs.a12_ * rhs.a21_ 
             + lhs.a13_ * rhs.a31_ + lhs.a14_ * rhs.a41_;
  res.a12_ = lhs.a11_ * rhs.a12_ + lhs.a12_ * rhs.a22_ 
             + lhs.a13_ * rhs.a32_ + lhs.a14_ * rhs.a42_;
  res.a13_ = lhs.a11_ * rhs.a13_ + lhs.a12_ * rhs.a23_ 
             + lhs.a13_ * rhs.a33_ + lhs.a14_ * rhs.a43_;
  res.a14_ = lhs.a11_ * rhs.a14_ + lhs.a12_ * rhs.a24_ 
             + lhs.a13_ * rhs.a34_ + lhs.a14_ * rhs.a44_;

  res.a21_ = lhs.a21_ * rhs.a11_ + lhs.a22_ * rhs.a21_ 
             + lhs.a23_ * rhs.a31_ + lhs.a24_ * rhs.a41_;
  res.a22_ = lhs.a21_ * rhs.a12_ + lhs.a22_ * rhs.a22_ 
             + lhs.a23_ * rhs.a32_ + lhs.a24_ * rhs.a42_;
  res.a23_ = lhs.a21_ * rhs.a13_ + lhs.a22_ * rhs.a23_ 
             + lhs.a23_ * rhs.a33_ + lhs.a24_ * rhs.a43_;
  res.a24_ = lhs.a21_ * rhs.a14_ + lhs.a22_ * rhs.a24_ 
             + lhs.a23_ * rhs.a34_ + lhs.a24_ * rhs.a44_;

  res.a31_ = lhs.a31_ * rhs.a11_ + lhs.a32_ * rhs.a21_ 
             + lhs.a33_ * rhs.a31_ + lhs.a34_ * rhs.a41_;
  res.a32_ = lhs.a31_ * rhs.a12_ + lhs.a32_ * rhs.a22_ 
             + lhs.a33_ * rhs.a32_ + lhs.a34_ * rhs.a42_;
  res.a33_ = lhs.a31_ * rhs.a13_ + lhs.a32_ * rhs.a23_ 
             + lhs.a33_ * rhs.a33_ + lhs.a34_ * rhs.a43_;
  res.a34_ = lhs.a31_ * rhs.a14_ + lhs.a32_ * rhs.a24_ 
             + lhs.a33_ * rhs.a34_ + lhs.a34_ * rhs.a44_;

  res.a41_ = lhs.a41_ * rhs.a11_ + lhs.a42_ * rhs.a21_ 
             + lhs.a43_ * rhs.a31_ + lhs.a44_ * rhs.a41_;
  res.a42_ = lhs.a41_ * rhs.a12_ + lhs.a42_ * rhs.a22_ 
             + lhs.a43_ * rhs.a32_ + lhs.a44_ * rhs.a42_;
  res.a43_ = lhs.a41_ * rhs.a13_ + lhs.a42_ * rhs.a23_ 
             + lhs.a43_ * rhs.a33_ + lhs.a44_ * rhs.a43_;
  res.a44_ = lhs.a41_ * rhs.a14_ + lhs.a42_ * rhs.a24_ 
             + lhs.a43_ * rhs.a34_ + lhs.a44_ * rhs.a44_;

  return res;
}

gfx::vector3
gfx::operator *(
    const gfx::matrix4X4& mtx,
    const gfx::vector3& vec
    )
{
  return vector3(
      mtx.a11_ * vec.x_ + mtx.a12_ * vec.y_ + mtx.a13_ * vec.z_ + mtx.a14_,
      mtx.a21_ * vec.x_ + mtx.a22_ * vec.y_ + mtx.a23_ * vec.z_ + mtx.a24_,
      mtx.a31_ * vec.x_ + mtx.a32_ * vec.y_ + mtx.a33_ * vec.z_ + mtx.a34_
      );
}

gfx::vector4
gfx::operator*(
  const gfx::matrix4X4& mtx,
  const gfx::vector4& vec
  )
{
  return vector4(
    mtx.a11_ * vec.x_ + mtx.a12_ * vec.y_ + mtx.a13_ * vec.z_ + mtx.a14_ * vec.w_,
    mtx.a21_ * vec.x_ + mtx.a22_ * vec.y_ + mtx.a23_ * vec.z_ + mtx.a24_ * vec.w_,
    mtx.a31_ * vec.x_ + mtx.a32_ * vec.y_ + mtx.a33_ * vec.z_ + mtx.a34_ * vec.w_,
    mtx.a41_ * vec.x_ + mtx.a42_ * vec.y_ + mtx.a43_ * vec.z_ + mtx.a44_ * vec.w_
    );
}

gfx::matrix4X4
gfx::inverse_of(
    const gfx::matrix4X4& mtx
    )
{
  float det = mtx.determinant();
  assert(!is_zero(det));
  gfx::matrix4X4 adjoint_mtx(adjoint_of(mtx));
  adjoint_mtx *= (1.0f / det);
  return adjoint_mtx;
}

gfx::matrix4X4
gfx::adjoint_of(
  const gfx::matrix4X4& mtx
  )
{
  float m1 = mtx.a33_ * mtx.a44_ - mtx.a34_ * mtx.a43_;
  float m2 = mtx.a32_ * mtx.a44_ - mtx.a34_ * mtx.a42_;
  float m3 = mtx.a32_ * mtx.a43_ - mtx.a33_ * mtx.a42_;
  float m4 = mtx.a13_ * mtx.a24_ - mtx.a14_ * mtx.a23_;
  float m5 = mtx.a12_ * mtx.a24_ - mtx.a14_ * mtx.a22_;
  float m6 = mtx.a12_ * mtx.a23_ - mtx.a13_ * mtx.a22_;
  float m7 = mtx.a31_ * mtx.a44_ - mtx.a34_ * mtx.a41_;
  float m8 = mtx.a31_ * mtx.a43_ - mtx.a33_ * mtx.a41_;
  float m9 = mtx.a23_ * mtx.a44_ - mtx.a24_ * mtx.a43_;
  float m10 = mtx.a21_ * mtx.a44_ - mtx.a24_ * mtx.a41_;
  float m11 = mtx.a21_ * mtx.a43_ - mtx.a23_ * mtx.a41_;
  float m12 = mtx.a11_ * mtx.a24_ - mtx.a14_ * mtx.a21_;
  float m13 = mtx.a11_ * mtx.a23_ - mtx.a13_ * mtx.a21_;
  float m14 = mtx.a31_ * mtx.a42_ - mtx.a32_ * mtx.a41_;
  float m15 = mtx.a11_ * mtx.a22_ - mtx.a12_ * mtx.a21_;
  float m16 = mtx.a31_ * mtx.a42_ - mtx.a32_ * mtx.a41_;

  return matrix4X4(
    mtx.a22_ * m1 - mtx.a23_ * m2 + mtx.a24_ * m3,
    -mtx.a12_ * m1 + mtx.a13_ * m2 - mtx.a14_ * m3,
    mtx.a42_ * m4 - mtx.a43_ * m5 + mtx.a44_ * m6,
    -mtx.a32_ * m4 + mtx.a33_ * m5 - mtx.a34_ * m6,

    -mtx.a21_ * m1 + mtx.a23_ * m7 - mtx.a24_ * m8,
    mtx.a11_ * m1 - mtx.a13_ * m7 + mtx.a14_ * m8,
    -mtx.a11_ * m9 + mtx.a13_ * m10 - mtx.a14_ * m11,
    mtx.a31_ * m4 - mtx.a33_ * m12 + mtx.a34_ * m13,

    mtx.a21_ * m2 - mtx.a22_ * m7  + mtx.a24_ * m16,
    -mtx.a11_ * m2 + mtx.a12_ * m7 - mtx.a14_ * m16,
    mtx.a41_ * m5 - mtx.a42_ * m12 + mtx.a44_ * m15,
    -mtx.a31_ * m5 + mtx.a32_ * m12 - mtx.a34_ * m15,

    -mtx.a21_ * m3 + mtx.a22_ * m8 - mtx.a23_ * m14,
    mtx.a11_ * m3 - mtx.a12_ * m8 + mtx.a13_ * m14,
    -mtx.a41_ * m6 + mtx.a42_ * m13 - mtx.a43_ * m15,
    mtx.a31_ * m6 - mtx.a32_ * m13 + mtx.a33_ * m15
    );
}
