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

#include <list>
#include <string>
#include <unordered_map>

#include "render/GpuResourceManager.hpp"
#include "render/Pipeline.hpp"
#include "render/ResourceManager.hpp"
#include "render/pixel.hpp"

namespace donkey {
namespace render {
class PipelineGenerator {
 public:
  PipelineGenerator(ResourceManager& resource_manager,
                    GpuResourceManager& gpu_resource_manager,
                    Pipeline& pipeline,
                    int window_width,
                    int window_height);
  void register_texture(const std::string& name,
                        int width,
                        int height,
                        pixel::Format format,
                        pixel::InternalFormat internal_format,
                        pixel::ComponentType component_type);
  void register_pass(const std::list<std::string>& input_textures,
                     const std::list<std::string>& render_targets,
                     const std::string& vertex_shader_path,
                     const std::string& fragment_shader_path,
                     GLint clear_bits,
                     bool depth_test,
                     bool lighting,
                     bool blending);
  void register_pass(const std::list<std::string>& render_targets,
                     GLint clear_bits,
                     bool depth_test,
                     bool lighting,
                     bool blending);

 private:
  void create_screen_mesh_(int window_width, int window_height);
  uint32_t register_material_(const std::list<std::string>& input_textures,
                              const std::string& vertex_shader_path,
                              const std::string& fragment_shader_path);
  uint32_t register_framebuffer_(const std::list<std::string>& render_targets);

 private:
  std::unordered_map<std::string, uint32_t> id_map_;
  ResourceManager& resource_manager_;
  GpuResourceManager& gpu_resource_manager_;
  Pipeline& pipeline_;
  donkey::CameraNode camera_node_;
  uint32_t screen_mesh_id_;
};
}  // namespace render
}  // namespace donkey