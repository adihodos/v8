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

    enum projection_type {
        ptype_perspective,
        ptype_othographic
    };

private :
    vector4F                 view_pos_; ///< The view frame origin, in world coordinates */
    vector4F                 view_side_;	///< The side direction vector (x axis) */
    vector4F                 view_up_;  ///< The up direction vector (y axis) */
    vector4F                 view_dir_; ///< The look direction vector (z axis) */

    mutable bool                viewmatrix_cache_valid_;
    mutable matrix_4X4F         view_matrix_;
    matrix_4X4F                 projection_matrix_;
    projection_type             projection_type_;

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
    void update_view_matrix() const;

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
        const math::vector4F& origin, 
        const math::vector4F& dir_vector, 
        const math::vector4F& up_vector, 
        const math::vector4F& right_vector
        );

    /**
     * \brief   Sets the origin point for the camera's coordinate system.
     */
    inline camera& set_origin(const math::vector4F& origin);

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
        const math::vector4F& dir_vector, 
        const math::vector4F& up_vector,
        const math::vector4F& right_vector
        );

    /**
     * \brief   Derive the view frame, given the three parameters. This function
     * 			is similar to OpenGL's gluLookAt() and Direct3D's 
     * 			D3DXMatrixLookAtLH().
     *
     * \param   origin      The camera's origin point (world space coords).
     * \param   world_up    The world up direction vector (world space coords).
     * \param   target      The point the camera looks at (world space coords).
     * \remarks Since the camera uses a left handle coodinate system, 
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
     * \brief   Gets the projection type (orthographic/projection).
     */
    inline projection_type get_projection_type() const;

    /**
     * \brief   Gets the origin point of the view frame.
     *
     */
    inline const math::vector4F& get_origin() const;

    /**
     * \brief   Gets the direction vector of the camera.
     *
     */
    inline const math::vector4F& get_direction_vector() const;

    /**
     * \brief   Gets the up direction vector of the camera.
     */
    inline const math::vector4F& get_up_vector() const;

    /**
     * \brief   Gets the direction vector.
     */
    inline const math::vector4F& get_right_vector() const;

    /**
     * \brief   Gets the view transform matrix.
     */
    inline const math::matrix_4X4F& get_view_transform() const;

    /**
     * \brief   Gets the projection transform matrix.
     */
    inline const math::matrix_4X4F& get_projection_transform() const;

    /**
     * \brief   Sets the projection matrix.
     *
     * \param   proj    A 4x4 matrix representing a projection.
     * \param   ptype   The projection type (orthographic/perspective)
     */
    void set_projection_matrix(
        const math::matrix_4X4F& proj, 
        projection_type ptype
        );
};

} // namespace gfx
} // namespace v8

#include "camera.inl"
