/**
 * \file    camera.h
 *
 * \brief   Camera class and related utilities
 */
#pragma once

#include "gfx_misc.h"
#include "vector3.h"
#include "vector4.h"
#include "matrix4X4.h"

namespace gfx {

/**
 * \class   camera
 *
 * \brief   Represents the point of view for an entity, in a 3D scene. 
 * 			The vectors for the camera's frame are always in world space coordinates.
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
     * \fn  void camera::update_view_matrix() const;
     *
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
     * \fn  camera::camera()
     *
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
     * \fn  camera& camera::set_view_frame( const gfx::vector4F& origin,
     * const gfx::vector4F& dir_vector, const gfx::vector4F& up_vector,
     * const gfx::vector4F& right_vector );
     *
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
        const gfx::vector4F& origin, 
        const gfx::vector4F& dir_vector, 
        const gfx::vector4F& up_vector, 
        const gfx::vector4F& right_vector
        );

    /**
     * \fn  camera& camera::set_origin(const gfx::vector4F& origin);
     *
     * \brief   Sets the origin point.
     */
    inline camera& set_origin(const gfx::vector4F& origin);

    /**
     * \fn  camera& camera::set_axes( const gfx::vector4F& dir_vector, const gfx::vector4F& up_vector,
     * const gfx::vector4F& right_vector );
     *
     * \brief   Sets the vectors that give the directions of the view frame axes.
     *
     * \param   dir_vector      The look direction vector.
     * \param   up_vector       The up direction vector.
     * \param   right_vector    The right direction vector.
     *
     * \remarks This function assumes that dir_vector, up_vector, right_vector
     * 			are three orthonormal vectors.
     */
    camera& set_axes(
        const gfx::vector4F& dir_vector, 
        const gfx::vector4F& up_vector,
        const gfx::vector4F& right_vector
        );

    /**
     * \fn  camera& camera::look_at( const gfx::vector3& origin, const gfx::vector3& world_up,
     * const gfx::vector3& target );
     *
     * \brief   Derive the view frame, given the three parameters.
     *
     * \param   origin      The camera's origin point (world space coords).
     * \param   world_up    The world up direction vector (world space coords).
     * \param   target      The point the camera looks at (world space coords).
     * 						
     */
    camera& look_at(
        const gfx::vector3F& origin, 
        const gfx::vector3F& world_up, 
        const gfx::vector3F& target
        );

    /**
     * \fn  inline projection_type camera::get_projection_type() const;
     *
     * \brief   Gets the projection type (orthographic/projection).
     */
    inline projection_type get_projection_type() const;

    /**
     * \fn  inline const gfx::vector4F& camera::get_origin() const;
     *
     * \brief   Gets the origin point of the view frame.
     *
     */
    inline const gfx::vector4F& get_origin() const;

    /**
     * \fn  inline const gfx::vector4F& camera::get_direction_vector() const;
     *
     * \brief   Gets the direction vector of the camera.
     *
     */
    inline const gfx::vector4F& get_direction_vector() const;

    /**
     * \fn  inline const gfx::vector4F& camera::get_up_vector() const;
     *
     * \brief   Gets the up direction vector of the camera.
     */
    inline const gfx::vector4F& get_up_vector() const;

    /**
     * \fn  inline const gfx::vector4F& camera::get_right_vector() const;
     *
     * \brief   Gets the right direction vector.
     *
     */
    inline const gfx::vector4F& get_right_vector() const;

    /**
     * \fn  inline const gfx::matrix4X4F& camera::get_view_transform() const;
     *
     * \brief   Gets the view transform matrix.
     *
     */
    inline const gfx::matrix_4X4F& get_view_transform() const;

    /**
     * \fn  inline const gfx::matrix4X4F& camera::get_projection_transform() const;
     *
     * \brief   Gets the projection transform matrix.
     *
     */
    inline const gfx::matrix_4X4F& get_projection_transform() const;

    /**
     * \fn  void camera::set_projection_matrix( const gfx::matrix4X4F& proj, projection_type ptype );
     *
     * \brief   Sets the projection matrix.
     *
     * \param   proj    A 4x4 matrix representing a projection.
     * \param   ptype   The projection type (orthographic/perspective)
     */
    void set_projection_matrix(
        const gfx::matrix_4X4F& proj, 
        projection_type ptype
        );
};

} // namespace gfx

#include "camera.inl"
