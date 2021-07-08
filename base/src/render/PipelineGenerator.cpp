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

#if defined(max)
#undef max
#endif

namespace donkey {
namespace render {
PipelineGenerator::PipelineGenerator(ResourceManager& resource_manager,
                                     GpuResourceManager& gpu_resource_manager,
                                     Pipeline& pipeline,
                                     int window_width,
                                     int window_height)
    : resource_manager_(resource_manager),
      gpu_resource_manager_(gpu_resource_manager),
      pipeline_(pipeline),
      camera_node_(
          donkey::CameraNode(1,
                             glm::vec3(0.0f, 0.0f, 0.0f),
                             glm::vec3(0.0f, 0.0f, 0.0f),
                             glm::tvec2<int>(0, 0),
                             glm::tvec2<GLsizei>(window_width, window_height),
                             0.0f,
                             -1.0f,
                             1.0f,
                             donkey::CameraNode::Type::kOrthographic)),
      screen_mesh_id_(0) {
  create_screen_mesh_(window_width, window_height);
}

void PipelineGenerator::create_screen_mesh_(int window_width,
                                            int window_height) {
  std::vector<float> screen_mesh_positions{0.0f,
                                           0.0f,
                                           0.0f,
                                           0.0f,
                                           static_cast<float>(window_height),
                                           0.0f,
                                           static_cast<float>(window_width),
                                           static_cast<float>(window_height),
                                           0.0f,
                                           static_cast<float>(window_width),
                                           0.0f,
                                           0.0f};
  std::vector<float> screen_mesh_normals{0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                                         0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
  std::vector<float> screen_mesh_uvs{0.0f, 0.0f, 0.0f, 1.0f,
                                     1.0f, 1.0f, 1.0f, 0.0f};
  std::vector<unsigned int> screen_mesh_indices{0, 1, 2, 0, 2, 3};
  screen_mesh_id_ = resource_manager_.create_mesh(
      screen_mesh_positions, screen_mesh_normals, screen_mesh_uvs,
      screen_mesh_normals, screen_mesh_normals, screen_mesh_indices);
}

void PipelineGenerator::register_texture(const std::string& name,
                                         int width,
                                         int height,
                                         pixel::Format format,
                                         pixel::InternalFormat internal_format,
                                         pixel::ComponentType component_type) {
  uint32_t id = resource_manager_.create_texture(
      width, height, format, internal_format, component_type);
  id_map_[name] = id;
}

void PipelineGenerator::register_pass(
    const std::list<std::string>& input_textures,
    const std::list<std::string>& render_targets,
    const std::string& vertex_shader_path,
    const std::string& fragment_shader_path,
    GLint clear_bits,
    bool depth_test,
    bool lighting,
    bool blending) {
  uint32_t material_id = register_material_(input_textures, vertex_shader_path,
                                            fragment_shader_path);
  uint32_t framebuffer_id = register_framebuffer_(render_targets);
  pipeline_.add_render_pass(camera_node_, screen_mesh_id_, material_id,
                            framebuffer_id, clear_bits, depth_test, lighting,
                            blending);
}

void PipelineGenerator::register_pass(
    const std::list<std::string>& render_targets,
    GLint clear_bits,
    bool depth_test,
    bool lighting,
    bool blending) {
  uint32_t framebuffer_id = register_framebuffer_(render_targets);
  pipeline_.add_render_pass(framebuffer_id, clear_bits, depth_test, lighting,
                            blending);
}

uint32_t PipelineGenerator::register_material_(
    const std::list<std::string>& input_textures,
    const std::string& vertex_shader_path,
    const std::string& fragment_shader_path) {
  ResourceManager::Id ambient_program_id_ = resource_manager_.load_gpu_program_from_file(
      vertex_shader_path, fragment_shader_path);
  std::uint32_t id = resource_manager_.create_material(ambient_program_id_);
  const render::Material& material = resource_manager_.get_material(id);
  render::AMaterial& gpu_material =
      gpu_resource_manager_.get_material(material.gpu_resource_id);
  int texture_unit = 0;

  for (auto& texture_name : input_textures) {
    uint32_t texture_id = id_map_[texture_name];
    const Texture& texture = resource_manager_.get_texture(texture_id);
    gpu_material.register_texture_slot(texture_name, texture.gpu_resource_id,
                                       texture_unit);
    texture_unit += 1;
  }
  return id;
}

uint32_t PipelineGenerator::register_framebuffer_(
    const std::list<std::string>& render_target_names) {
  if (render_target_names.size() == 0)
    return std::numeric_limits<uint32_t>::max();

  std::list<const Texture*> render_targets;
  for (auto& name : render_target_names) {
    uint32_t id = id_map_.at(name);
    const Texture& texture = resource_manager_.get_texture(id);
    render_targets.push_back(&texture);
  }
  uint32_t framebuffer_id =
      gpu_resource_manager_.create_framebuffer(render_targets);
  return framebuffer_id;
}
}  // namespace render
}  // namespace donkey