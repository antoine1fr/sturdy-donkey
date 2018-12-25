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

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <array>
#include <iostream>
#include <limits>

#include "debug.hpp"
#include "render/DeferredRenderer.hpp"
#include "render/Material.hpp"
#include "Buffer.hpp"
#include "BufferPool.hpp"
#include "render/CommandBucket.hpp"

namespace donkey
{
namespace render
{

DeferredRenderer::DeferredRenderer(Window& window):
  run_(true),
  window_(window),
  render_context_(window.get_render_context()),
  gpu_resource_manager_(driver_.get_resource_manager()),
  resource_manager_(gpu_resource_manager_),
  render_thread_(nullptr),
  frame_count(0),
  render_frame_index(0)
{
  window_.make_current(render_context_);
  int width = window.get_width();
  int height = window.get_height();

  create_light_pass_mesh_(width, height);
  create_gbuffer_(width, height);
  create_light_pass_frame_packet_(width, height);
  // register gbuffer pass
  add_render_pass({gbuffer_id_,
      GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT,
      glm::vec3(0.0f, 0.0f, 0.0f),
      true,
      false});
  // register light accumulation pass
  add_render_pass({std::numeric_limits<uint32_t>::max(),
      GL_COLOR_BUFFER_BIT,
      glm::vec3(0.0f, 0.0f, 0.0f),
      false,
      true});
}

DeferredRenderer::~DeferredRenderer()
{
  if (render_thread_)
  {
    render_thread_->join();
    delete render_thread_;
  }
  resource_manager_.cleanup();
}

void DeferredRenderer::create_light_pass_mesh_(int width, int height)
{
  std::vector<float> screen_mesh_positions {
    0.0f, 0.0f, 0.0f,
    0.0f, static_cast<float>(height), 0.0f,
    static_cast<float>(width), static_cast<float>(height), 0.0f,
    static_cast<float>(width), 0.0f, 0.0f
  };
  std::vector<float> screen_mesh_normals {
    0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f
  };
  std::vector<float> screen_mesh_uvs {
    0.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 1.0f,
    1.0f, 0.0f
  };
  std::vector<unsigned int> screen_mesh_indices {0, 1, 2, 0, 2, 3};
  screen_mesh_id_ =
    resource_manager_.create_mesh(
        screen_mesh_positions,
        screen_mesh_normals,
        screen_mesh_uvs,
        screen_mesh_normals,
        screen_mesh_normals,
        screen_mesh_indices);
}

void DeferredRenderer::create_gbuffer_(int width, int height)
{
  albedo_rt_id_ =
    resource_manager_.create_texture(width, height, pixel::Format::kRGBA,
        pixel::InternalFormat::kRGBA8, pixel::ComponentType::kUnsignedByte);
  normal_rt_id_ =
    resource_manager_.create_texture(width, height, pixel::Format::kRGBA,
        pixel::InternalFormat::kRGBA8, pixel::ComponentType::kUnsignedByte);
  depth_rt_id_ =
    resource_manager_.create_texture(width, height,
        pixel::Format::kDepthComponent,
        pixel::InternalFormat::kDepthComponent24,
        pixel::ComponentType::kFloat);
  const Texture& albedo_rt = resource_manager_.get_texture(albedo_rt_id_);
  const Texture& normal_rt = resource_manager_.get_texture(normal_rt_id_);
  const Texture& depth_rt = resource_manager_.get_texture(depth_rt_id_);
  gbuffer_id_ = gpu_resource_manager_.create_framebuffer(
      albedo_rt.gpu_resource_id,
      normal_rt.gpu_resource_id,
      depth_rt.gpu_resource_id);
}

uint32_t DeferredRenderer::create_light_pass_material_()
{
  light_program_id_ = resource_manager_.load_gpu_program_from_file(
    "shaders/simple.vert.glsl",
    "shaders/light-pass.frag.glsl");
  std::uint32_t id = resource_manager_.create_material(light_program_id_);
  const render::Material& material = resource_manager_.get_material(id);
  render::AMaterial& gpu_material = gpu_resource_manager_.get_material(
      material.gpu_resource_id);
  const Texture& albedo_rt = resource_manager_.get_texture(albedo_rt_id_);
  const Texture& normal_rt = resource_manager_.get_texture(normal_rt_id_);
  const Texture& depth_rt = resource_manager_.get_texture(depth_rt_id_);
  gpu_material.register_texture_slot("albedo_tex", albedo_rt.gpu_resource_id,
      0);
  gpu_material.register_texture_slot("normals_tex", normal_rt.gpu_resource_id,
      1);
  gpu_material.register_texture_slot("depth_tex", depth_rt.gpu_resource_id, 2);
  return id;
}

void DeferredRenderer::create_light_pass_frame_packet_(int width, int height)
{
  // create material for light accumulation pass
  uint32_t rt1_material_id = create_light_pass_material_();
  light_frame_packet_.create_mesh_node(1,
      glm::vec3(0.0f, 0.0f, 0.0f),
      glm::vec3(0.0f, 0.0f, 0.0f),
      screen_mesh_id_, rt1_material_id);
  light_frame_packet_.create_ortho_camera_node(1,
      glm::vec3(0.0f, 0.0f, 0.0f),
      glm::vec3(0.0f, 0.0f, 0.0f),
      glm::tvec2<int>(0, 0),
      glm::tvec2<GLsizei>(width, height));
}

void DeferredRenderer::bind_mesh_uniforms_(
    CommandBucket& render_commands,
    const Material& material,
    const MeshNode& mesh_node) const
{
  // compute model matrix
  glm::mat4 rotate_x = glm::rotate(
			glm::mat4(1.0f),
			glm::radians(mesh_node.angles.x),
      glm::vec3(1.0f, 0.0f, 0.0f));
  glm::mat4 rotate_y = glm::rotate(
			rotate_x,
			glm::radians(mesh_node.angles.y),
      glm::vec3(0.0f, 1.0f, 0.0f));
  glm::mat4 rotate_z = glm::rotate(
			rotate_y,
			glm::radians(mesh_node.angles.z),
      glm::vec3(0.0f, 0.0f, 1.0f));
  glm::mat4 translate = glm::translate(
			glm::mat4(1.0f),
			mesh_node.position);
  glm::mat4 model = translate * rotate_z * rotate_y * rotate_x;
  render_commands.bind_uniform(material.model_location, model);
}

void DeferredRenderer::bind_camera_uniforms_(
    CommandBucket& render_commands,
    const Material& material,
    const CameraNode& camera_node) const
{
  glm::mat4 projection_inverse = glm::inverse(camera_node.projection);

  render_commands.bind_uniform(material.view_location, camera_node.view);
  render_commands.bind_uniform(material.projection_location,
      camera_node.projection);
  render_commands.bind_uniform(material.projection_params_location,
      glm::vec2(camera_node.near_plane, camera_node.far_plane));
  render_commands.bind_uniform(material.projection_inverse_location,
      projection_inverse);
  render_commands.bind_uniform(material.camera_position_location,
      camera_node.position);
}

void DeferredRenderer::bind_light_uniforms_(
    CommandBucket& render_commands,
    const Material& material,
    const glm::mat4& view) const
{
  // bind light infos
  glm::vec4 light_dir = view * glm::vec4(-1.0f, 1.0f, -1.0f, 10.0f);
  render_commands.bind_uniform(material.light_dir_location, light_dir);
  render_commands.bind_uniform(material.ambient_location,
      glm::vec4(0.2f, 0.2f, 0.2f, 1.0f));
  render_commands.bind_uniform(material.light_diffuse_location,
      glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
  render_commands.bind_uniform(material.light_specular_location,
      glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
}

void DeferredRenderer::render_mesh_node_(
    const RenderPass& render_pass,
    const MeshNode& mesh_node,
    const CameraNode& camera_node,
    CommandBucket& render_commands)
{
  static uint32_t last_material_id = std::numeric_limits<uint32_t>::max();
  const Mesh& mesh = resource_manager_.get_mesh(mesh_node.mesh_id);
  const Material& material =
    resource_manager_.get_material(mesh_node.material_id);
  const AMaterial& gpu_material =
    gpu_resource_manager_.get_material(material.gpu_resource_id);

  if (last_material_id != mesh_node.material_id)
  {
    render_commands.bind_gpu_program(material.program_id);
    gpu_material.bind_slots(render_commands);
    last_material_id = mesh_node.material_id;
  }

  // bind built-in uniforms
  bind_mesh_uniforms_(render_commands, material, mesh_node);
  bind_camera_uniforms_(render_commands, material, camera_node);
  bind_light_uniforms_(render_commands, material, camera_node.view);

  // bind geometry
  render_commands.bind_mesh(
      mesh.gpu_resource_id,
      material.position_location,
      material.normal_location,
      material.uv_location,
      material.tangent_location,
      material.bitangent_location);

  render_commands.draw_elements(mesh.index_count);
}

void DeferredRenderer::render()
{
  window_.make_current(render_context_);
  while (run_)
  {
    std::unique_lock<std::mutex> frame_lock(mutex);
    condition_variable.wait(frame_lock,
        [this]{return frame_count > render_frame_index;});
    Buffer& buffer = BufferPool::get_pop_head();
    FramePacket<StackAllocator>* gbuffer_frame_packet =
      static_cast<FramePacket<StackAllocator>*>(buffer.ptr());;
    gbuffer_frame_packet->sort_mesh_nodes();

    signpost_start(1, 0, 0, 0, 0);
    CommandBucket render_commands;
    execute_pass_(0, render_passes_[0], *gbuffer_frame_packet,
        render_commands);
    execute_pass_(1, render_passes_[1], light_frame_packet_, render_commands);
    driver_.execute_commands(render_commands);

    window_.swap();
    render_frame_index++;
    BufferPool::next_pop_head();
    signpost_end(1, 0, 0, 0, 0);
  }
}

ResourceManager& DeferredRenderer::get_resource_manager()
{
  return resource_manager_;
}

AResourceManager& DeferredRenderer::get_gpu_resource_manager()
{
  return gpu_resource_manager_;
}

void DeferredRenderer::add_render_pass(const RenderPass& render_pass)
{
  render_passes_.push_back(render_pass);
}

void DeferredRenderer::add_render_pass(RenderPass&& render_pass)
{
  render_passes_.push_back(render_pass);
}

void DeferredRenderer::notify_exit()
{
  run_ = false;
}

uint32_t DeferredRenderer::get_albedo_rt_id() const
{
  return albedo_rt_id_;
}

uint32_t DeferredRenderer::get_normal_rt_id() const
{
  return normal_rt_id_;
}

uint32_t DeferredRenderer::get_depth_rt_id() const
{
  return depth_rt_id_;
}

void DeferredRenderer::start_render_thread()
{
  render_thread_ = new std::thread([this](){ this->render(); });
}

}
}
