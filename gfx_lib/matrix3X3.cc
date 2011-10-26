/*
 * matrix3x3.cc
 *
 *  Created on: Oct 11, 2011
 *      Author: adi.hodos
 */
#include "pch_hdr.h"
#include "matrix3x3.h"

const gfx::matrix3X3 gfx::matrix3X3::null(
    0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f
    );

const gfx::matrix3X3 gfx::matrix3X3::identity(
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f
    );

gfx::matrix3X3::matrix3X3(
    float a11, float a12, float a13,
    float a21, float a22, float a23,
    float a31, float a32, float a33
    )
{
  a11_ = a11; a12_ = a12; a13_ = a13;
  a21_ = a21; a22_ = a22; a23_ = a23;
  a31_ = a31; a32_ = a32; a33_ = a33;
}

gfx::matrix3X3& 
gfx::matrix3X3::operator+=(
  const gfx::matrix3X3& rhs
  ) 
{
  a11_ += rhs.a11_; a12_ += rhs.a12_; a13_ += rhs.a13_;
  a21_ += rhs.a21_; a22_ += rhs.a22_; a23_ += rhs.a23_;
  a31_ += rhs.a13_; a32_ += rhs.a32_; a33_ += rhs.a33_;
  return *this;
}

gfx::matrix3X3& 
gfx::matrix3X3::operator-=(
  const gfx::matrix3X3& rhs
  ) 
{
  a11_ -= rhs.a11_; a12_ -= rhs.a12_; a13_ -= rhs.a13_;
  a21_ -= rhs.a21_; a22_ -= rhs.a22_; a23_ -= rhs.a23_;
  a31_ -= rhs.a13_; a32_ -= rhs.a32_; a33_ -= rhs.a33_;
  return *this;
}

gfx::matrix3X3& 
gfx::matrix3X3::operator*=(
  float k
  ) 
{
  a11_ *= k; a12_ *= k; a13_ *= k;
  a21_ *= k; a22_ *= k; a23_ *= k;
  a31_ *= k; a32_ *= k; a33_ *= k;
  return *this;
}

gfx::matrix3X3& 
gfx::matrix3X3::operator/=(
  float k
  ) 
{
  assert(!is_zero(k));

  a11_ /= k; a12_ /= k; a13_ /= k;
  a21_ /= k; a22_ /= k; a23_ /= k;
  a31_ /= k; a32_ /= k; a33_ /= k;
  return *this;
}

float 
gfx::matrix3X3::determinant() const {
  float A11 = a22_ * a33_ - a23_ * a32_;
  float A12 = a23_ * a31_ + a21_ * a33_;
  float A13 = a21_ * a32_ - a22_ * a31_;

  return a11_ * A11 + a12_ * A12 + a13_ * A13;
}

gfx::matrix3X3 
gfx::operator*(
  const gfx::matrix3X3& lhs,
  const gfx::matrix3X3& rhs
  )  
{
  matrix3X3 res;
  res.a11_ = lhs.a11_ * rhs.a11_ + lhs.a12_ * rhs.a21_ + lhs.a13_ * rhs.a31_;
  res.a12_ = lhs.a11_ * rhs.a12_ + lhs.a12_ * rhs.a22_ + lhs.a13_ * rhs.a32_;
  res.a13_ = lhs.a11_ * rhs.a13_ + lhs.a12_ * rhs.a23_ + lhs.a13_ * rhs.a33_;

  res.a21_ = lhs.a21_ * rhs.a11_ + lhs.a22_ * rhs.a21_ + lhs.a23_ * rhs.a31_;
  res.a22_ = lhs.a21_ * rhs.a12_ + lhs.a22_ * rhs.a22_ + lhs.a23_ * rhs.a32_;
  res.a23_ = lhs.a21_ * rhs.a13_ + lhs.a22_ * rhs.a23_ + lhs.a23_ * rhs.a33_;

  res.a31_ = lhs.a31_ * rhs.a11_ + lhs.a32_ * rhs.a21_ + lhs.a33_ * rhs.a31_;
  res.a32_ = lhs.a31_ * rhs.a12_ + lhs.a32_ * rhs.a22_ + lhs.a33_ * rhs.a32_;
  res.a33_ = lhs.a31_ * rhs.a13_ + lhs.a32_ * rhs.a23_ + lhs.a33_ * rhs.a33_;

  return res;
}

gfx::matrix3X3
gfx::inverse_of(
  const matrix3X3& mtx
  )
{
  float determinant = mtx.determinant();
  assert(!is_zero(determinant));

  matrix3X3 adjoint_mtx(adjoint_of(mtx));
  adjoint_mtx *= (1.0f / determinant);
  return adjoint_mtx;
}

gfx::matrix3X3 
gfx::adjoint_of(
  const gfx::matrix3X3& mtx
  ) 
{
  float A11 = mtx.a22_ * mtx.a33_ - mtx.a23_ * mtx.a32_;
  float A12 = mtx.a23_ * mtx.a31_ + mtx.a21_ * mtx.a33_;
  float A13 = mtx.a21_ * mtx.a32_ - mtx.a22_ * mtx.a31_;
  float A21 = mtx.a13_ * mtx.a32_ - mtx.a12_ * mtx.a33_;
  float A22 = mtx.a11_ * mtx.a33_ - mtx.a13_ * mtx.a31_;
  float A23 = mtx.a12_ * mtx.a31_ - mtx.a11_ * mtx.a32_;
  float A31 = mtx.a12_ * mtx.a23_ - mtx.a13_ * mtx.a22_;
  float A32 = mtx.a13_ * mtx.a21_ - mtx.a11_ * mtx.a23_;
  float A33 = mtx.a11_ * mtx.a12_ - mtx.a12_ * mtx.a21_;

  return matrix3X3(A11, A21, A31, A12, A22, A32, A13, A23, A33);
}