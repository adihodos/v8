//
// Copyright (c) 2011, 2012, Adrian Hodos
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of the author nor the
//       names of its contributors may be used to endorse or promote products
//       derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE AUTHOR OR THE CONTRIBUTORS BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#pragma once

#include "v8/math/vector3.h"
#include "v8/math/vector4.h"
#include "v8/math/matrix4X4.h"

namespace v8 { namespace math {

/**
 * \class   camera
 *
 * \brief   Represents the point of view for an entity, in a 3D scene. 
 * 			The vectors for the camera's frame are always expressed in world 
 * 			space coordinates. The camera uses a left handed coordinate
 * 			system.
 */
class camera {
public :
    enum Projection_Type_t {
        Projection_Perspective,
        Projection_Orthographic,
        Projection_Last
    };

    enum Frustrum_Param_t {
        Frustrum_DMin,
        Frustrum_DMax,
        Frustrum_UMin,
        Frustrum_UMax,
        Frustrum_RMin,
        Frustrum_RMax,
        Frustrum_Params_Max
    };

private :

    /**
     * \brief   The frustrum is defined by 6 parameters :
     *          - dmin : distance to near plane (z axis)
     *          - dmax : distance to far plane (z axis)
     *          - rmin : min x axis value (left) for the projection window
     *          - rmax : max x axis value (right) for the projection window
     *          - umin : min y axis value (bottom) for the projection window
     *          - umax : max y axis value (top) for the projection window
     */
    float               frustrum_params_[6];
    vector3F            view_pos_; ///< The view frame origin, in world coordinates */
    vector3F            view_side_;	///< The side direction vector (x axis) */
    vector3F            view_up_;  ///< The up direction vector (y axis) */
    vector3F            view_dir_; ///< The look direction vector (z axis) */
    matrix_4X4F         view_matrix_;   /*!< Stores the view space transform */
    matrix_4X4F         projection_matrix_; /*!< Stores the projection matrix */
    /*!
     *\brief Stores the product of the projection and view matrices.
     */
    matrix_4X4F         projection_view_matrix_;
    int                 projection_type_;   /*!< Type of the projection (orhthographic/perspective) */

    /**
     * \brief   Constructs the world space to view space transformation matrix.
     * 			Given the view frame vectors, we can derive the world to view 
     * 			matrix in the following way :
     * 			Let M = [v, u, w, t] be the view to world matrix (its columns
     * 			being the view frame vectors).
     * 			We need the	inverse of this matrix. Since this matrix is a rotation (R),
     * 			followed by a translation (T), the inverse of it would be
     * 			[ RT    -RT * T]
     * 			[ 0     1      ].
     * 			R = [vx, ux, wx]
     * 			    [vy, uy, wy]
     * 			    [vz, uz, wz] => RT = [vx, vy, vz]
     * 			                         [ux, uy, uz]
     * 			                         [wx, wy, wz].
     *          T = [tx, ty, tz] => inverse of T = -T = [-tx, -ty, -tz].
     *          Thus, the world to view matrix is :
     *          [ux, uy, uz, -dot(u, t)]
     *          [vx, vy, vz, -dot(v, t)]
     *          [wx, wy, wz, -dot(w, t)]
     *          [0,  0,  0,  1         ].
     */
    void update_view_matrix();

    /**
     * Recompute projection * view matrix.
     */
    inline void update_projection_view_transform();

    /**
     * \brief   Rebuilds the projection matrix after a change in the frustrum
     *          parameter values.
     */
    void handle_frustrum_param_change();

public :

    /**
     * \brief   Default constructor.
     *
     * \remarks The camera is initialized with the following settings :
     * 			- left handed coordinate system  
     * 			- depth range settings for direct 3d -> [0.0f, 1.0f]  
     * 			- view frame origin is at (0, 0, 0, 1)  
     * 			- view frame look direction (0, 0, 1, 0) - z axis
     * 			- view frame up direction (0, 1, 0, 0) - y axis  
     * 			- view frame right direction (1, 0, 0, 0) - x axis  
     * 			- projection matrix is a perspective projection, with the  
     * 			  following settings : near plane 1, far plane 10.000, 
     * 			  aspect ration 1:1, vertical fov 90 degreess.
     */
    camera();

    /**
     * \brief   Sets the four elements of the view frame.
     *
     * \param   origin          The origin point.
     * \param   dir_vector      The look direction vector.
     * \param   up_vector       The up direction vector.
     * \param   right_vector    The right direction vector.
     * 							
     * \remarks This function assumes that dir_vector, up_vector, right_vector
     * 			are three orthonormal vectors.
     */
    camera& set_view_frame(
        const math::vector3F& origin, 
        const math::vector3F& dir_vector, 
        const math::vector3F& up_vector, 
        const math::vector3F& right_vector
        );

    /**
     * \brief   Sets the origin point for the camera's coordinate system.
     */
    inline camera& set_origin(const math::vector3F& origin);

    /**
     * \brief   Sets the vectors that give the directions of the view frame axes.
     *
     * \param   dir_vector      The look direction vector.
     * \param   up_vector       The up direction vector.
     * \param   right_vector    The right direction vector.
     *
     * \remarks This function assumes that dir_vector, up_vector, right_vector
     * 			are three orthonormal vectors. If they are not, the behaviour
     * 			of the function is undefined.
     */
    camera& set_axes(
        const math::vector3F& dir_vector, 
        const math::vector3F& up_vector,
        const math::vector3F& right_vector
        );

    /**
     * \brief   Derive the view frame, given the three parameters. This function
     * 			is similar to OpenGL's gluLookAt() and Direct3D's 
     * 			D3DXMatrixLookAtLH().
     *
     * \param   origin      The camera's origin point (world space coords).
     * \param   world_up    The world up direction vector (world space coords).
     * \param   target      The point the camera looks at (world space coords).
     * \remarks Since the camera uses a left handed coodinate system, 
     * 			the vectors representing the axes of the camera's coordinate 
     * 			system are derived with the following method :
     * 			D = (target - origin) / || target - origin ||
     * 			R = (world_up x D) / || world_up x D ||
     * 			U = D x R
     */
    camera& look_at(
        const math::vector3F& origin, 
        const math::vector3F& world_up, 
        const math::vector3F& target
        );

    /**
     * \brief   Sets the frustrum's parameters and builds the corresponding
     *          projection matrix.
     * \param   dmin            Minimum depth value (near plane).
     * \param   dmax            Maximum depth value (far plane).
     * \param   umin            Minimum height of the projection window.
     * \param   umax            Maximum height of the projection window.
     * \param   rmin            Minumum width of the projection window.
     * \param   rmax            Maximum height of the projection window.
     * \param   projection_type (optional) Type of the projection.
     * \remarks The matrix for a perspective projection is built this way :
     *          Let umax = U, umin = u, rmax = R, rmin = r, dmax = D, dmin = d.
     *          [   2*d/(R - r)     0           -(R + r)/(R - r) 0             ]
     *          [   0               2*d/(U - u) -(U + u)/(U - u) 0             ]
     *          [   0               0           D/(D - d)        -D*d/(D - d)  ]
     *          [   0               0           1                0             ]
     *
     *          The matrix for an ortho projection is built this way :
     *          [ 2/(R - r) 0           0           -(R + r)/(R - r) ]
     *          [ 0         2/(U - u)   0           -(U + u)/(U - u) ]
     *          [ 0         0           1/(D - d)   -d/(D - d)       ]
     *          [ 0         0           0           0                ]
     */
    void set_frustrum(
        float dmin, 
        float dmax, 
        float umin, 
        float umax, 
        float rmin, 
        float rmax,
        int projection_type = Projection_Perspective
        );

    /**
     * \brief   Sets a symmetric frustrum, where (rmin = -rmax, umin = -umax) and
     *          builds a perspective projection matrix.
     * \param   fov_angle       The vertical field of view angle, 
     *                          expressed in <b>degrees</b>.
     * \param   aspect_ratio    The aspect ratio.
     * \param   dmin            Minimum depth (near plane).
     * \param   dmax            Maximum depth (far plane).
     */
    void set_symmetric_frustrum(
        float fov_angle, 
        float aspect_ratio, 
        float dmin, 
        float dmax
        );

    /**
     * \brief   Retrieves a pointer to the array storing the frustrum's paramteres.
     *          The parameters can be easily accessed by using the members of
     *          the Frustrum_Param_t enumeration.
     * \see     camera::Frustrum_Param_t enum.
     */

    inline const float* get_frustrum() const;

    /**
     * \brief   Gets the minimum depth of the frustrum.
     */
    inline float get_dmin() const;

    /**
     * \brief   Gets the maximum depth of the frustrum.
     */
    inline float get_dmax() const;

    /**
     * \brief   Gets the minimum height of the frustrum.
     */
    inline float get_umin() const;

    /**
     * \brief   Gets the maximum height of the frustrum.
     */
    inline float get_umax() const;

    /**
     * \brief   Gets the minimum width of the frustrum.
     */
    inline float get_rmin() const;

    /**
     * \brief   Gets the maximum height of the frustrum.
     */
    inline float get_rmax() const;

    /**
     * \brief   Gets the origin point of the view frame.
     *
     */
    inline const math::vector3F& get_origin() const;

    /**
     * \brief   Gets the look direction vector of the camera.
     *
     */
    inline const math::vector3F& get_direction_vector() const;

    /**
     * \brief   Gets the up direction vector of the camera.
     */
    inline const math::vector3F& get_up_vector() const;

    /**
     * \brief   Gets the right direction vector.
     */
    inline const math::vector3F& get_right_vector() const;

    /**
     * \brief   Gets the view transform matrix.
     */
    inline const math::matrix_4X4F& get_view_transform() const;

    /**
     * Returns the projection transform matrix.
     */
    inline const math::matrix_4X4F& get_projection_transform() const;

    /**
     * \brief   Allows the user to set a custom projection matrix.
     * \param   mtx     The projection matrix.
     */
    inline void set_projection_matrix(
        const matrix_4X4F& mtx
        );

    /**
     * \brief   Sets a projection matrix using the four vectors as columns.
     * \param   first_col   The first column vector.
     * \param   second_col  The second column vector.
     * \param   third_col   The third column vector.
     * \param   fourth_col  The fourth column vector.
     * \param   type        The projection type.
     */
    void set_projection_matrix(
        const vector4F& first_col, 
        const vector4F& second_col,
        const vector4F& third_col,
        const vector4F& fourth_col
        );

    /**
     * Return the type of the projection matrix.
     */
    inline int get_projection_type() const;

    /**
     * \brief   Sets the projection's type.
     */
    inline void set_projection_type(int proj_type);

    /**
     * Return the product of P (projection matrix) * V (view matrix).
     */
    inline const math::matrix_4X4F& get_projection_wiew_transform() const;
};

} // namespace math
} // namespace v8

#include "camera.inl"
