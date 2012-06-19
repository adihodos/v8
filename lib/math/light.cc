#include "pch_hdr.h"
#include "v8/math/light.h"

v8::math::light::light(
    const v8::math::color& ambient_color, 
    const v8::math::color& diffuse_color, 
    const v8::math::color& specular_color,
    const v8::math::vector3F& direction
    )
    : ambient_color_(ambient_color),
      diffuse_color_(diffuse_color),
      specular_color_(specular_color),
      direction_(direction),
      type_(v8::math::light::light_type_directional) {}

v8::math::light::light(
    const v8::math::color& ambient_color, 
    const v8::math::color& diffuse_color, 
    const v8::math::color& specular_color,
    const v8::math::vector3F& position,
    const v8::math::vector3F& attenuation,
    float max_range
    )
    : ambient_color_(ambient_color),
      diffuse_color_(diffuse_color),
      specular_color_(specular_color),
      type_(v8::math::light::light_type_point),
      position_(position),
      max_range_(max_range),
      attenuation_(attenuation) {}

v8::math::light::light(
    const v8::math::color& ambient_color, 
    const v8::math::color& diffuse_color, 
    const v8::math::color& specular_color,
    const v8::math::vector3F& position,
    const v8::math::vector3F& direction,
    const v8::math::vector3F& attenuation,
    float max_range,
    float spot_power,
    float spot_cone_theta
    )
    : ambient_color_(ambient_color),
      diffuse_color_(diffuse_color),
      specular_color_(specular_color),
      direction_(direction),
      type_(v8::math::light::light_type_spot),
      position_(position),
      max_range_(max_range),
      attenuation_(attenuation),
      spot_cone_theta_(spot_cone_theta),
      spot_power_(spot_power) {}