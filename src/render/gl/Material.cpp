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

#include <GL/gl3w.h>

#include "render/gl/Material.hpp"
#include "render/gl/GpuProgram.hpp"
#include "render/gl/ResourceManager.hpp"

namespace donkey {
namespace render {
namespace gl {

Material::Material(
    const AResourceManager& resource_manager,
    uint32_t program_id):
  AMaterial(resource_manager, program_id)
{
  const GpuProgram& program = resource_manager_.get_gpu_program(program_id);
  position_location = glGetAttribLocation(program.handle, "position");
  uv_location = glGetAttribLocation(program.handle, "uv");
  model_location = glGetUniformLocation(program.handle, "model");
  view_location = glGetUniformLocation(program.handle, "view");
  projection_location = glGetUniformLocation(program.handle, "projection");
  projection_inverse_location = glGetUniformLocation(program.handle,
    "projection_inverse");
  projection_params_location = glGetUniformLocation(program.handle,
    "projection_params");
  light_dir_location = glGetUniformLocation(program.handle,
    "light_dir");
  light_diffuse_location = glGetUniformLocation(program.handle,
    "light_diffuse");
  light_specular_location = glGetUniformLocation(program.handle,
    "light_specular");
  ambient_location = glGetUniformLocation(program.handle,
    "ambient");
  camera_position_location = glGetUniformLocation(program.handle,
    "camera_position");
}

// TODO: find a pure c++ (i.e. no preprocessor) implementation that is as
// concise and doesn't use RTTIs.

#define DEFINE_REGISTER_SLOT(x,y,z) \
  void Material::register_ ## z ##_slot(const std::string& name, const x& storage) \
  { \
    const GpuProgram& program = resource_manager_.get_gpu_program(program_id); \
    int location = glGetUniformLocation(program.handle, name.c_str()); \
    y ## _slots_.push_back(ScalarMaterialSlot<x>(location, storage)); \
  }

DEFINE_REGISTER_SLOT(float, float, float);
DEFINE_REGISTER_SLOT(glm::vec2, vec2, vector2);
DEFINE_REGISTER_SLOT(glm::vec3, vec3, vector3);
DEFINE_REGISTER_SLOT(glm::vec4, vec4, vector4);
DEFINE_REGISTER_SLOT(glm::mat2, mat2, matrix2);
DEFINE_REGISTER_SLOT(glm::mat3, mat3, matrix3);
DEFINE_REGISTER_SLOT(glm::mat4, mat4, matrix4);
DEFINE_REGISTER_SLOT(int, int, int);

#undef DEFINE_REGISTER_SLOT

void Material::register_texture_slot(const std::string& name,
    uint32_t texture_id,
    int texture_unit)
{
  const GpuProgram& program = resource_manager_.get_gpu_program(program_id);
  int location = glGetUniformLocation(program.handle, name.c_str());
  texture_slots_.push_back(TextureMaterialSlot(location, texture_id,
    texture_unit));
}

}
}
}
