/**
 * \file    matrix4X4.h
 *
 * \brief   4x4 matrix class and associated functions.
 */

#ifndef GFX_LIB_MATRIX4X4_H__
#define GFX_LIB_MATRIX4X4_H__



#include <cassert>
#include <cmath>
#include <cstring>
#include <algorithm>
#include "matrix3X3.h"
#include "vector4.h"
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

/**
 * \class   matrix4X4
 *
 * \brief   A 4x4 homogenous matrix, stored using row major indexing. 
 * 			The matrix is designed so that it multiplies the vector.
 * 			Given a matrix M and a vector V, to transform the vector V by the
 * 			matrix M, one would write V1 = M * V;
 */
class matrix4X4 {
private :
  int index_at(int row, int col) const {
    return (row - 1) * 4 + col - 1;
  }

public :

  union {
    struct {
      float a11_, a12_, a13_, a14_; ///< The first row */
      float a21_, a22_, a23_, a24_; ///< The second row */
      float a31_, a32_, a33_, a34_; ///< The third row */
      float a41_, a42_, a43_, a44_; ///< The fourth row */
    };
    float elements_[16];	///< Access to elements using an array */
  };

  static const matrix4X4	null;   ///< The null matrix */

  static const matrix4X4	identity;   ///< The identity 4x4 matrix*/

  /**
   * \fn    static matrix4X4 matrix4X4::translation(float x, float y, float z)
   *
   * \brief Returns a 4x4 translation matrix.
   *
   */
  static matrix4X4 translation(float x, float y, float z) {
    matrix4X4 mtx(matrix4X4::identity);
    mtx.a14_ = x;
    mtx.a24_ = y;
    mtx.a34_ = z;
    return mtx;
  }

  /**
   * \fn    static void matrix4X4::invert_translation_matrix(matrix4X4* mtx)
   *
   * \brief Invert a translation matrix. (T = -T).
   *
   * \param [in,out]    Pointer to a 4x4 matrix. Must not be null.
   */
  static void	invert_translation_matrix(matrix4X4* mtx) {
    mtx->a14_ = -mtx->a14_;
    mtx->a24_ = -mtx->a24_;
    mtx->a34_ = -mtx->a34_;
  }

  /**
   * \fn    matrix4X4::matrix4X4()
   *
   * \brief Default constructor. Leaves elements uninitialized.
   */
  matrix4X4() {}

  matrix4X4(
      float a11, float a12, float a13, float a14,
      float a21, float a22, float a23, float a24,
      float a31, float a32, float a33, float a34,
      float a41, float a42, float a43, float a44
      );

  /**
   * \fn    matrix4X4::matrix4X4(const float* input, size_t count)
   *
   * \brief Constructor from an array of float values.
   *
   * \param input   Pointer to an array of float values.
   * \param count   Number of entries in the array.
   * 				
   * \remarks If the array has less than 16 elements, the remaining matrix components
   * 		  will be left uninitialized. Values beyond the 16th element of the
   * 		  array will be ignored.
   */
  matrix4X4(const float* input, size_t count) {
    std::memcpy(elements_, input, std::min(_countof(elements_), count));
  }

  /**
   * \fn    float& matrix4X4::operator()(int row, int col)
   *
   * \brief Component read-write access, using the [row][column] syntax.
   *
   * \param row Component's row index (1 based).
   *
   * \param col Component's column index (1 based).
   *
   * \return The value of the specified component.
   */
  float& operator()(int row, int col) {
    return elements_[index_at(row, col)];
  }

  /**
   * \fn    float matrix4X4::operator()(int row, int col) const
   *
   * \brief Component read only access, using the [row][column] syntax.
   * 		
   * \param row Component's row index (1 based).
   * 			
   * \param col Component's column index (1 based).
   *
   * \return The value of the specified component.
   */
  float operator()(int row, int col) const {
    return elements_[index_at(row, col)];
  }

  /**
   * \fn    matrix4X4& matrix4X4::operator+=(const matrix4X4& rhs);
   *
   * \brief Addition assignment operator.
   *
   * \param rhs The right hand side.
   *
   * \return    The result of the operation.
   */
  matrix4X4& operator+=(const matrix4X4& rhs);

  /**
   * \fn    matrix4X4& matrix4X4::operator-=(const matrix4X4& rhs);
   *
   * \brief Subtraction assignment operator.
   *
   * \param rhs The right hand side.
   *
   */
  matrix4X4& operator-=(const matrix4X4& rhs);

  /**
   * \fn    matrix4X4& matrix4X4::operator*=(float k);
   *
   * \brief Multiplication assignment operator.
   *
   * \param k   Scalar to multiply with.
   *
   */
  matrix4X4& operator*=(float k);

  /**
   * \fn    matrix4X4& matrix4X4::operator/=(float k);
   *
   * \brief Division assignment operator.
   *
   * \param k   The scalar to divide with. Ensure k <> 0.
   *
   */
  matrix4X4& operator/=(float k);

  /**
   * \fn    float matrix4X4::determinant() const;
   *
   * \brief Computes the determinant for this matrix.
   *
   */
  float determinant() const;

  /**
   * \fn    bool matrix4X4::is_invertible() const
   *
   * \brief Query if this matrix4X4 is invertible (that is det(A) is not 0).
   *
   * \return    true if invertible, false if not.
   */
  bool is_invertible() const {
    return !is_zero(determinant());
  }

  /**
   * \fn    matrix4X4& matrix4X4::invert()
   *
   * \brief Inverts the matrix.
   * \remarks Only call this function if is_invertible() returns true.		
   *
   * \return    Reference to the matrix object.
   */
  matrix4X4& invert() {
    *this = inverse_of(*this);
    return *this;
  }

  /**
   * \fn    matrix4X4& matrix4X4::transpose()
   *
   * \brief Transposes the matrix.
   *
   * \return    Reference to the matrix object.
   */
  matrix4X4& transpose() {
    *this = transpose_of(*this);
    return *this;
  }

  /**
   * \fn    matrix4X4& matrix4X4::set_upper3x3(const float* data)
   *
   * \brief Copies the values from an array into the upper 3x3 matrix.
   *
   * \param data    Pointer to an array of float values. Cannot be null.
   * 				Must contain at least 9 elements.
   */
  matrix4X4& set_upper3x3(const float* data);

  /**
   * \fn    matrix4X4& matrix4X4::set_upper3x3(const matrix3X3& src)
   *
   * \brief Sets the contents of the upper 3x3 matrix.
   *
   * \param src Source 3x3 matrix for the assignment
   */
  matrix4X4& set_upper3x3(const matrix3X3& src) {
      return set_upper3x3(src.elements_);
  }

  /**
   * \fn    void matrix4X4::get_upper3x3(float* data) const
   *
   * \brief Gets the contents of the upper 3x3 matrix.
   *
   * \param [in,out]    data    Pointer to an array of float values, where the
   * 					data is to be copied. Cannot be null. 
   * 					Must have at least 9 elements.
   */
  void get_upper3x3(float* data) const;

  /**
   * \fn    void matrix4X4::get_upper3x3(matrix3X3* out_mtx) const
   *
   * \brief Gets the contents of the upper 3x3 matrix.
   *
   * \param [in,out]    out_mtx Pointer to a 3x3 matrix that receives the data.
   */
  void get_upper3x3(matrix3X3* out_mtx) const {
      get_upper3x3(out_mtx->elements_);
  }

  /**
   * \fn    matrix4X4& matrix4X4::set_row(int row, const float* row_data)
   *
   * \brief Sets a row's contents.
   *
   * \param row         The row index (1 based).
   * \param row_data    Pointer to source data.
   */
  matrix4X4& set_row(int row, const float* row_data) {
      float* row_ptr = elements_ + (row - 1) * 4;
      for (size_t i = 0; i < 4; ++i)
          row_ptr[i] = row_data[i];
      return *this;
  }

  /**
   * \fn    matrix4X4& matrix4X4::set_row(int row, const vector4& vec)
   *
   * \brief Assigns the contents of a vector4 to a matrix row.
   *
   * \param row The row index (1 based).
   * \param vec Source vector for the data.
   *
   */
  matrix4X4& set_row(int row, const vector4& vec) {
      return set_row(row, vec.elements_);
  }

  /**
   * \fn    matrix4X4& matrix4X4::set_column(int column, const float* data)
   *
   * \brief Sets a column's data.
   *
   * \param column  The column index (1 based).
   * \param data    Pointer to source data.
   */
  matrix4X4& set_column(int column, const float* data) {
      for (size_t i = 0; i < 4; ++i)
          elements_[i * 4 + column - 1] = data[i];
      return *this;
  }

  /**
   * \fn    matrix4X4& matrix4X4::set_column(int column, const vector4& v4)
   *
   * \brief Assigns the contents of a vector4 to a matrix column.
   *
   * \param column  The column index (1 based).
   * \param v4      Source vector.
   *
   */
  matrix4X4& set_column(int column, const vector4& v4) {
      return set_column(column, v4);
  }

  /**
   * \fn    void matrix4X4::get_row(int row, float* data) const
   *
   * \brief Copies the contents of a row.
   *
   * \param row                 The row index (1 based).
   * \param [in,out]    data    Pointer to a float array that will receive the
   * 					row's data. The array size must be at least 4. 
   * 					Can not be null.
   */
  void get_row(int row, float* data) const {
      const float* row_ptr = elements_ + (row - 1) * 4;
      for (size_t i = 0; i < 4; ++i)
          data[i] = row_ptr[i];
  }

  /**
   * \fn    void matrix4X4::get_row(int row, vector4* v4) const
   *
   * \brief Copies the contents of a row.
   *
   * \param row             The row index (1 based).
   * \param [in,out]    v4  Pointer to a vector4 object that will receive the 
   * 					    row's contents. Must not be null.
   */
  void get_row(int row, vector4* v4) const {
      return get_row(row, v4->elements_);
  }

  /**
   * \fn    void matrix4X4::get_column(int col, float* data) const
   *
   * \brief Copies the contents of a column into an array.
   *
   * \param col                 The column index (1 based).
   * \param [in,out]    data    Pointer to a float array that will receive the
   * 					column's data. The array size must be at least 4.
   * 					Can not be null.
   */
  void get_column(int col, float* data) const {
      for (size_t i = 0; i < 4; ++i)
          data[i] = elements_[i * 4 + col - 1];
  }

  /**
   * \fn    void matrix4X4::get_column(int col, vector4* v4) const
   *
   * \brief Copies the contents of a column into a vector4 object.
   *
   * \param col             The column index (1 based).
   * \param [in,out]    v4  Pointer to a vector4 object. Cannot be null.
   */
  void get_column(int col, vector4* v4) const {
      get_column(col, v4->elements_);
  }

  /**
   * \fn    matrix4X4& matrix4X4::transform_vector(vector4* pvec) const
   *
   * \brief Transforms a vector and assigns the result to it.
   *
   * \param [in,out]    pvec    Pointer to the vector4 to be transformed. 
   * 					Must not be null.
   */
  void transform_vector(vector4* pvec) const;
  
  void transform_point(vector4* point) const;
};

/**
 * \fn  inline matrix4X4 operator+( const matrix4X4& lhs, const matrix4X4& rhs )
 *
 * \brief   Addition operator.
 */
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

/**
 * \fn  inline matrix4X4 operator-( const matrix4X4& lhs, const matrix4X4& rhs )
 *
 * \brief   Subtraction operator.
 */
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

/**
 * \fn  inline matrix4X4 operator-( const matrix4X4& mtx )
 *
 * \brief   Negation operator.
 */
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

/**
 * \fn  matrix4X4 operator*( const gfx::matrix4X4& lhs, const gfx::matrix4X4& rhs );
 *
 * \brief   Multiplication operator.
 */
matrix4X4
operator*(
    const gfx::matrix4X4& lhs,
    const gfx::matrix4X4& rhs
    );

/**
 * \fn  vector4 operator*( const matrix4X4& mtx, const vector4& vec );
 *
 * \brief   Matrix -> vector4 multiplication operator.
 *
 */
vector4
operator*(
  const matrix4X4& mtx,
  const vector4& vec
  );

/**
 * \fn  inline matrix4X4 operator*( float k, const matrix4X4& mtx )
 *
 * \brief   scalar -> matrix multiplication operator.
 */
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

/**
 * \fn  inline matrix4X4 operator*( const matrix4X4& mtx, float k )
 *
 * \brief   matrix -> scalar multiplication operator.
 */
inline
matrix4X4
operator*(
    const matrix4X4& mtx,
    float k
    )
{
  return k * mtx;
}

/**
 * \fn  inline matrix4X4 operator/( const matrix4X4& mtx, float k )
 *
 * \brief   matrix -> scalar division operator.
 */
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

/**
 * \fn  inline matrix4X4 transpose_of( const matrix4X4& mtx )
 *
 * \brief   Compute the transpose of the given mtx.
 */
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

/**
 * \fn  matrix4X4 inverse_of( const matrix4X4& );
 *
 * \brief   Return the inverse of the given matrix.
 */
matrix4X4
inverse_of(
    const matrix4X4&
    );

/**
 * \fn  matrix4X4 adjoint_of( const matrix4X4& );
 *
 * \brief   Return the adjoint matrix of the given matrix.
 */
matrix4X4
adjoint_of(
  const matrix4X4&
  );

/**
 * \fn  matrix4X4 perspective_projection_left_handed( float aspect_ratio, float vertical_fov,
 * float near_plane, float far_plane, float depth_min, float depth_max );
 *
 * \brief   Computes the perspective projection matrix assuming a left handed
 * 			coordinate system.
 *
 * \param   aspect_ratio    The aspect ratio of the window.
 * \param   vertical_fov    The vertical field of view angle, in radians.
 * \param   near_plane      The near plane.
 * \param   far_plane       The far plane.
 * \param   depth_min       The depth minimum.
 * \param   depth_max       The depth maximum.
 *
 */
matrix4X4
perspective_projection_left_handed(
    float aspect_ratio,
    float vertical_fov,
    float near_plane,
    float far_plane,
    float depth_min,
    float depth_max
    );  

/**
 * \fn  matrix4X4 perspective_projection_left_handed( float aspect_ratio, float vertical_fov,
 * float near_plane, float far_plane, float depth_min, float depth_max );
 *
 * \brief   Computes the perspective projection matrix assuming a right handed
 * 			coordinate system.
 *
 * \param   aspect_ratio    The aspect ratio of the window.
 * \param   vertical_fov    The vertical field of view angle, in radians.
 * \param   near_plane      The near plane.
 * \param   far_plane       The far plane.
 * \param   depth_min       The depth minimum.
 * \param   depth_max       The depth maximum.
 *
 */
matrix4X4
perspective_projection_right_handed(
    float aspect_ratio,
    float vertical_fov,
    float near_plane,
    float far_plane,
    float depth_min,
    float depth_max
    );

/**
 * \fn  matrix4X4 perspective_projection_oblique_right_handed( float near_plane, float far_plane,
 * float wtop, float wbottom, float wleft, float wright, float depth_min, float depth_max );
 *
 * \brief   Returns a matrix describing an oblique perspective projection,
 * 			using a right handed coordinate system.
 *
 * \param   near_plane  The near plane.
 * \param   far_plane   The far plane.
 * \param   wtop        The window top point (y axis).
 * \param   wbottom     The window bottom point (y axis).
 * \param   wleft       The window left point (x axis).
 * \param   wright      The window right point (x axis).
 * \param   depth_min   The minimum depth value.
 * \param   depth_max   The maximum depth value.
 *
 */
matrix4X4
perspective_projection_oblique_right_handed(
    float near_plane,
    float far_plane,
    float wtop,
    float wbottom,
    float wleft,
    float wright,
    float depth_min,
    float depth_max
    );

/**
 * \fn  matrix4X4 perspective_projection_oblique_left_handed( float near_plane, float far_plane,
 * float wtop, float wbottom, float wleft, float wright, float depth_min, float depth_max );
 *
 * \brief   Returns a matrix describing an oblique perspective projection,
 * 			using a left handed coordinate system.
 *
 * \param   near_plane  The near plane.
 * \param   far_plane   The far plane.
 * \param   wtop        The window top point (y axis).
 * \param   wbottom     The window bottom point (y axis).
 * \param   wleft       The window left point (x axis).
 * \param   wright      The window right point (x axis).
 * \param   depth_min   The minimum depth value.
 * \param   depth_max   The maximum depth value.
 *
 */
matrix4X4
perspective_projection_oblique_left_handed(
    float near_plane,
    float far_plane,
    float wtop,
    float wbottom,
    float wleft,
    float wright,
    float depth_min,
    float depth_max
    );

/**
 * \fn  matrix4X4 orthographic_parallel_projection_left_handed( float near_plane,
 * float far_plane, float wtop, float wbottom, float wleft, float wright, float depth_min,
 * float depth_max );
 *
 * \brief   Returns a matrix representing a parallel orthographic projection,
 * 			suitable for a left handed coordinate system.
 *
 * \param   near_plane  The near plane.
 * \param   far_plane   The far plane.
 * \param   wtop        The window top point (y axis).
 * \param   wbottom     The window bottom point (y axis).
 * \param   wleft       The window left point (x axis).
 * \param   wright      The window right point (x axis).
 * \param   depth_min   The minimum depth value.
 * \param   depth_max   The maximum depth value.
 */
matrix4X4
orthographic_parallel_projection_left_handed(
    float near_plane,
    float far_plane,
    float wtop,
    float wbottom,
    float wleft,
    float wright,
    float depth_min,
    float depth_max
    );


/**
 * \fn  matrix4X4 orthographic_parallel_projection_right_handed( float near_plane,
 * float far_plane, float wtop, float wbottom, float wleft, float wright, float depth_min,
 * float depth_max );
 *
 * \brief   Returns a matrix representing a parallel orthographic projection,
 * 			suitable for a right handed coordinate system.
 *
 * \param   near_plane  The near plane.
 * \param   far_plane   The far plane.
 * \param   wtop        The window top point (y axis).
 * \param   wbottom     The window bottom point (y axis).
 * \param   wleft       The window left point (x axis).
 * \param   wright      The window right point (x axis).
 * \param   depth_min   The minimum depth value.
 * \param   depth_max   The maximum depth value.
 */
matrix4X4
orthographic_parallel_projection_right_handed(
    float near_plane,
    float far_plane,
    float wtop,
    float wbottom,
    float wleft,
    float wright,
    float depth_min,
    float depth_max
    );

} // namespace gfx

#endif /* GFX_MATRIX_H_ */