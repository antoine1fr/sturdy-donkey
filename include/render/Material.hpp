#pragma once

#include "render/Resource.hpp"

namespace donkey {
namespace render {

struct Material: Resource
{
  std::uint32_t program_id;
  int position_location;
  int uv_location;
  int model_location;
  int view_location;
  int projection_location;
  int projection_inverse_location;
  int projection_params_location;
  int ambient_location;
  int light_dir_location;
  int light_diffuse_location;
  int light_specular_location;
  int camera_position_location;

  Material(
      std::uint32_t id,
      std::uint32_t program_id,
      int position_location,
      int uv_location,
      int model_location,
      int view_location,
      int projection_location,
      int projection_inverse_location,
      int projection_params_location,
      int ambient_location,
      int light_dir_location,
      int light_diffuse_location,
      int light_specular_location,
      int camera_position_location):
    Resource(id),
    program_id(program_id),
    position_location(position_location),
    uv_location(uv_location),
    model_location(model_location),
    view_location(view_location),
    projection_location(projection_location),
    projection_inverse_location(projection_inverse_location),
    projection_params_location(projection_params_location),
    ambient_location(ambient_location),
    light_dir_location(light_dir_location),
    light_diffuse_location(light_diffuse_location),
    light_specular_location(light_specular_location),
    camera_position_location(camera_position_location)
  {
  }

};

}
}
