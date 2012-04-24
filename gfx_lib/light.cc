#include "pch_hdr.h"
#include "light.h"

gfx::light::light(
    const gfx::color& ambient_color, 
    const gfx::color& diffuse_color, 
    const gfx::color& specular_color,
    const gfx::vector3F& direction
    )
    : ambient_color_(ambient_color),
      diffuse_color_(diffuse_color_),
      specular_color_(specular_color),
      direction_(direction),
      type_(gfx::light::light_type_directional) {}

gfx::light::light(
    const gfx::color& ambient_color, 
    const gfx::color& diffuse_color, 
    const gfx::color& specular_color,
    const gfx::vector3F& position,
    const gfx::vector3F& attenuation,
    float max_range
    )
    : ambient_color_(ambient_color),
      diffuse_color_(diffuse_color_),
      specular_color_(specular_color),
      type_(gfx::light::light_type_point),
      position_(position),
      max_range_(max_range),
      attenuation_(attenuation) {}

gfx::light::light(
    const gfx::color& ambient_color, 
    const gfx::color& diffuse_color, 
    const gfx::color& specular_color,
    const gfx::vector3F& position,
    const gfx::vector3F& direction,
    const gfx::vector3F& attenuation,
    float max_range,
    float spot_power,
    float spot_cone_theta
    )
    : ambient_color_(ambient_color),
      diffuse_color_(diffuse_color_),
      specular_color_(specular_color),
      direction_(direction),
      type_(gfx::light::light_type_spot),
      position_(position_),
      max_range_(max_range),
      attenuation_(attenuation),
      spot_power_(spot_power),
      spot_cone_theta_(spot_cone_theta) {}