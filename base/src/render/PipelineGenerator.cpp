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

#include "render/PipelineGenerator.hpp"

namespace donkey {
  namespace render {
    PipelineGenerator::PipelineGenerator(ResourceManager& resource_manager,
      GpuResourceManager& gpu_resource_manager):
      resource_manager_(resource_manager),
      gpu_resource_manager_(gpu_resource_manager)
    {
    }

    void PipelineGenerator::register_texture(const std::string& name,
      int width,
      int height,
      pixel::Format format,
      pixel::InternalFormat internal_format,
      pixel::ComponentType component_type)
    {
      uint32_t id =
        resource_manager_.create_texture(width, height, format,
          internal_format, component_type);
      id_map_[name] = id;
    }

    void PipelineGenerator::register_pass(const std::list<std::string>& input_textures,
      const std::list<std::string>& render_targets,
      const std::string& vertex_shader_path,
      const std::string& fragment_shader_path)
    {
      register_material_(input_textures, vertex_shader_path, fragment_shader_path);
      register_framebuffer_(render_targets);
      std::vector<uint32_t> render_target_ids;
      render_target_ids.reserve(render_targets.size());
      for (auto& name : input_textures)
      {
        uint32_t id = id_map_.at(name);
        render_target_ids.push_back(id);
      }
    }

    uint32_t PipelineGenerator::register_material_(const std::list<std::string>& input_textures,
      const std::string& vertex_shader_path,
      const std::string& fragment_shader_path)
    {
      uint32_t ambient_program_id_ = resource_manager_.load_gpu_program_from_file(
        vertex_shader_path,
        fragment_shader_path);
      std::uint32_t id = resource_manager_.create_material(ambient_program_id_);
      const render::Material& material = resource_manager_.get_material(id);
      render::AMaterial& gpu_material = gpu_resource_manager_.get_material(
        material.gpu_resource_id);
      int texture_unit = 0;

      for (auto& texture_name : input_textures)
      {
        uint32_t texture_id = id_map_[texture_name];
        const Texture& texture = resource_manager_.get_texture(texture_id);
        gpu_material.register_texture_slot(texture_name, texture.gpu_resource_id,
          texture_unit);
        texture_unit += 1;
      }
      return id;
    }

    uint32_t PipelineGenerator::register_framebuffer_(const std::list<std::string>& render_target_names)
    {
      std::list<const Texture*> render_targets;
      for (auto& name : render_target_names)
      {
        uint32_t id = id_map_.at(name);
        const Texture& texture = resource_manager_.get_texture(id);
        render_targets.push_back(&texture);
      }
      uint32_t framebuffer_id = gpu_resource_manager_.create_framebuffer(render_targets);
      return framebuffer_id;
    }
  }
}