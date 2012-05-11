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

#include <cassert>
#include "v8/math/color.h"
#include "v8/math/vector3.h"

namespace v8 { namespace math {

/**
 \class light

 \brief Abstract representation of a light source.
 */
class light {
public :
  enum light_type {
    light_type_directional,
    light_type_point,
    light_type_spot
  };

private :

    /**
     \summary The ambient color that this light emits.
     */
    math::color      ambient_color_;

    /**
     \summary The diffuse color that this light emits.
     */
    math::color      diffuse_color_;

    /**
     \summary The specular color that this light source emits.
     */
    math::color      specular_color_;

    /**
     \summary The direction vector. Only valid for directional or spot
        lights.
     */
    math::vector3F   direction_;

    /**
     \summary The type of this light source (directional, point or
        spot).
     */
    int             type_;

    /**
     \summary The position of the light source. Irrelevant for
        directional(parallel) lights.
     */
    math::vector3F   position_;

    /**
     \summary The maximum range. If any objects is beyond this range, 
        it will not receive any light from this light source. Valid
        only for point/spot lights.
     */
    float           max_range_;

    /**
     \summary Stores the attenuation factors for the lighting
        equation (valid only for point/spot lights).
     */
    math::vector3F   attenuation_;        

    /**
     \summary Defines the maximum angle for the spotlight's cone. 
          Expressed in radians.
     */
    float           spot_cone_theta_;

    /**
     \summary The spot power factor (intensity).
     */
    float           spot_power_;   

public :

  /**
   \fn  light::light()
  
   \brief Default constructor, leaves object un-initialized.
   */
  light() {}

  /**
   \brief Constructs a directional(parallel) light.
  
   \param ambient_color   The ambient color.
   \param diffuse_color   The diffuse color.
   \param specular_color  The specular color.
   \param direction       The direction of the rays.
   */
  light(
    const math::color& ambient_color, 
    const math::color& diffuse_color, 
    const math::color& specular_color,
    const math::vector3F& direction
    );

  /**
   \brief Constructs a point light.
  
   \param ambient_color   The ambient color.
   \param diffuse_color   The diffuse color.
   \param specular_color  The specular color.
   \param position        The position of the light source.
   \param attenuation     The attenuation factors.
   \param max_range       The maximum range for the light source.
   */
  light(
    const math::color& ambient_color, 
    const math::color& diffuse_color, 
    const math::color& specular_color,
    const math::vector3F& position,
    const math::vector3F& attenuation,
    float max_range
    );

  /**  
   \brief Constructs a spot light.
  
   \param ambient_color   The ambient color.
   \param diffuse_color   The diffuse color.
   \param specular_color  The specular color.
   \param position        The position.
   \param direction       The direction vector of the spot light.
   \param attenuation     The attenuation factors.
   \param spot_power      The spot power (intensity).
   \param spot_cone_theta The angle of the spot's cone.
   */
  light(
    const math::color& ambient_color, 
    const math::color& diffuse_color, 
    const math::color& specular_color,
    const math::vector3F& position,
    const math::vector3F& direction,
    const math::vector3F& attenuation,
    float max_range,
    float spot_power,
    float spot_cone_theta
    );

  int get_type() const {
    return type_;
  }

  void set_type(light_type type) {
    type_ = type;
  }

  const math::color& get_ambient_color() const {
    return ambient_color_;
  }

  void set_ambient_color(const math::color& color) {
    ambient_color_ = color;
  }

  const math::color& get_diffuse_color() const {
    return diffuse_color_;
  }

  void set_diffuse_color(const math::color& diffuse_color) {
    diffuse_color_ = diffuse_color;
  }

  const math::color& get_specular_color() const {
    return specular_color_;
  }

  void set_specular_color(const math::color& specular_color) {
    specular_color_ = specular_color;
  }

  void set_direction(const math::vector3F& light_dir) {
    assert(type_ == light_type_directional || type_ == light_type_spot);
    direction_ = light_dir;
  }

  const math::vector3F& get_direction() const {
    assert(type_ == light_type_directional || type_ == light_type_spot);
    return direction_;
  }

  void set_position(const math::vector3F& light_pos) {
    assert(type_ != light_type_directional);
    position_ = light_pos;
  }

  const math::vector3F& get_position() const {
    assert(type_ != light_type_directional);
    return position_;
  }

  void set_attenuation_factors(float a, float b, float c) {
    assert(type_ != light_type_directional);
    attenuation_.x_ = a;
    attenuation_.y_ = b;
    attenuation_.z_ = c;
  }

  const math::vector3F& get_attenuation_factors() const {
    assert(type_ != light_type_directional);
    return attenuation_;
  }

  void set_max_range(float max_range) {
    assert(type_ == light_type_point || type_ == light_type_spot);
    max_range_ = max_range;
  }

  float get_max_range() const {
    assert(type_ == light_type_point || type_ == light_type_spot);
    return max_range_;
  }

  void set_spot_power(float spot_pow) {
    assert(type_ == light_type_spot);
    spot_power_ = spot_pow;
  }

  float get_spot_power() const {
    assert(type_ == light_type_spot);
    return spot_power_;
  }

  void set_spot_cone_angle(float theta) {
    assert(type_ == light_type_spot);
    spot_cone_theta_ = theta;
  }

  float get_spot_cone_angle() const {
    assert(type_ == light_type_spot);
    return spot_cone_theta_;
  }
};

} // namespace math
} // namespace v8