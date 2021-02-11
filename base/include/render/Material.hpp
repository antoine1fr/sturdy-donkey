/* Copyright (C) 2018 Antoine Luciani
 *
 * This file is part of Sturdy Donkey.
 *
 * Sturdy Donkey is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, version 3.
 *
 * Sturdy Donkey is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * Sturdy Donkey. If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include "render/Resource.hpp"

namespace donkey {
namespace render {

struct Material : Resource {
  std::uint32_t program_id;
  unsigned int position_location;
  unsigned int normal_location;
  unsigned int uv_location;
  unsigned int tangent_location;
  unsigned int bitangent_location;
  unsigned int model_location;
  unsigned int view_location;
  unsigned int projection_location;
  unsigned int gbuffer_projection_inverse_location;
  unsigned int gbuffer_view_location;
  unsigned int gbuffer_projection_params_location;
  unsigned int ambient_location;
  unsigned int light_dir_location;
  unsigned int light_diffuse_location;
  unsigned int light_specular_location;
  unsigned int camera_position_location;

  Material(std::uint32_t id,
           std::uint32_t program_id,
           int position_location,
           int normal_location,
           int uv_location,
           int tangent_location,
           int bitangent_location,
           int model_location,
           int view_location,
           int projection_location,
           int gbuffer_projection_inverse_location,
           int gbuffer_projection_params_location,
           int gbuffer_view_location,
           int ambient_location,
           int light_dir_location,
           int light_diffuse_location,
           int light_specular_location,
           int camera_position_location)
      : Resource(id),
        program_id(program_id),
        position_location(position_location),
        normal_location(normal_location),
        uv_location(uv_location),
        tangent_location(tangent_location),
        bitangent_location(bitangent_location),
        model_location(model_location),
        view_location(view_location),
        projection_location(projection_location),
        gbuffer_projection_inverse_location(
            gbuffer_projection_inverse_location),
        gbuffer_view_location(gbuffer_view_location),
        gbuffer_projection_params_location(gbuffer_projection_params_location),
        ambient_location(ambient_location),
        light_dir_location(light_dir_location),
        light_diffuse_location(light_diffuse_location),
        light_specular_location(light_specular_location),
        camera_position_location(camera_position_location) {}
};

}  // namespace render
}  // namespace donkey
