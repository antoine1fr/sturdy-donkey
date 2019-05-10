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

#include <GL/gl3w.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <vector>
#include <string>
#include "TextureMaterialSlot.hpp"
#include "ScalarMaterialSlot.hpp"

namespace donkey {
namespace render {

class AResourceManager;

namespace gl {
  class ResourceManager;
}

class AMaterial
{
  protected:
    std::vector<TextureMaterialSlot> texture_slots_;
    std::vector<ScalarMaterialSlot<float>> float_slots_;
    std::vector<ScalarMaterialSlot<glm::vec2>> vec2_slots_;
    std::vector<ScalarMaterialSlot<glm::vec3>> vec3_slots_;
    std::vector<ScalarMaterialSlot<glm::vec4>> vec4_slots_;
    std::vector<ScalarMaterialSlot<glm::mat2>> mat2_slots_;
    std::vector<ScalarMaterialSlot<glm::mat3>> mat3_slots_;
    std::vector<ScalarMaterialSlot<glm::mat4>> mat4_slots_;
    std::vector<ScalarMaterialSlot<int>> int_slots_;
    const gl::ResourceManager& resource_manager_;

  public:
    uint32_t program_id;
    int position_location;
    int normal_location;
    int uv_location;
    int tangent_location;
    int bitangent_location;
    int model_location;
    int view_location;
    int projection_location;
    int gbuffer_projection_inverse_location;
    int gbuffer_view_location;
    int gbuffer_projection_params_location;
    int ambient_location;
    int light_dir_location;
    int light_diffuse_location;
    int light_specular_location;
    int camera_position_location;

  public:
    AMaterial(const AResourceManager& resource_manager, uint32_t program_id);
    virtual ~AMaterial() {}

    void bind_slots(CommandBucket& render_commands) const;

    template <class Storage>
      void bind_scalar(int location, const Storage& storage,
          CommandBucket& render_commands) const;

    virtual void register_float_slot(
        const std::string& name,
        const float& storage) = 0;

    virtual void register_int_slot(
        const std::string& name,
        const int& storage) = 0;

    virtual void register_vector2_slot(
        const std::string& name,
        const glm::vec2& storage) = 0;

    virtual void register_vector3_slot(
        const std::string& name,
        const glm::vec3& storage) = 0;

    virtual void register_vector4_slot(
        const std::string& name,
        const glm::vec4& storage) = 0;

    virtual void register_matrix2_slot(
        const std::string& name,
        const glm::mat2& storage) = 0;

    virtual void register_matrix3_slot(
        const std::string& name,
        const glm::mat3& storage) = 0;

    virtual void register_matrix4_slot(
        const std::string& name,
        const glm::mat4& storage) = 0;

    virtual void register_texture_slot(
        const std::string& name,
        uint32_t texture_id,
        int texture_unit) = 0;
};

}
}

#include "AMaterial.inl"
