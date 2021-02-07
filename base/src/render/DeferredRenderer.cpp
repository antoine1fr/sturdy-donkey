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

DeferredRenderer::DeferredRenderer(
    Window* window,
    gl::Driver* driver,
    ResourceManager* resource_manager):
  window_(window),
  driver_(driver),
  gpu_resource_manager_(driver_->get_resource_manager()),
  resource_manager_(resource_manager),
  pipeline_(window, driver, resource_manager)
{
  int width = window->get_width();
  int height = window->get_height();
  create_light_pass_mesh_(width, height);
  create_render_targets_(width, height);
  create_render_passes_();
  create_frame_packets_(width, height);
}

DeferredRenderer::~DeferredRenderer()
{
}

void DeferredRenderer::create_render_targets_(int width, int height)
{
  // render targets
  create_gbuffer_(width, height);
  create_light_accu_render_target_(width, height);
  create_albedo_render_target_(width, height);
}

void DeferredRenderer::create_frame_packets_(int width, int height)
{
  // frame packets
  create_light_accu_pass_frame_packet_(width, height);
  create_albedo_pass_frame_packet_(width, height);
  create_ambient_pass_frame_packet_(width, height);
}

void DeferredRenderer::create_render_passes_()
{
  // register gbuffer pass
  pipeline_.add_render_pass({
      nullptr,
      gbuffer_id_,
      GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT,
      glm::vec3(0.0f, 0.0f, 0.0f),
      true,
      false,
      false});
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
    resource_manager_->create_mesh(
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
    resource_manager_->create_texture(width, height, pixel::Format::kRGBA,
        pixel::InternalFormat::kRGBA8, pixel::ComponentType::kUnsignedByte);
  normal_rt_id_ =
    resource_manager_->create_texture(width, height, pixel::Format::kRGBA,
        pixel::InternalFormat::kRGB16F, pixel::ComponentType::kFloat);
  depth_rt_id_ =
    resource_manager_->create_texture(width, height,
        pixel::Format::kDepthComponent,
        pixel::InternalFormat::kDepthComponent24,
        pixel::ComponentType::kFloat);
  const Texture& albedo_rt = resource_manager_->get_texture(albedo_rt_id_);
  const Texture& normal_rt = resource_manager_->get_texture(normal_rt_id_);
  const Texture& depth_rt = resource_manager_->get_texture(depth_rt_id_);
  gbuffer_id_ = gpu_resource_manager_.create_framebuffer(
    depth_rt.gpu_resource_id,
    {albedo_rt.gpu_resource_id,
     normal_rt.gpu_resource_id});
}

void DeferredRenderer::create_light_accu_render_target_(int width, int height)
{
  light_rt_id_ =
    resource_manager_->create_texture(width, height, pixel::Format::kRGBA,
        pixel::InternalFormat::kRGBA8, pixel::ComponentType::kUnsignedByte);
  uint32_t depth_rt_id =
    resource_manager_->create_texture(width, height,
        pixel::Format::kDepthComponent,
        pixel::InternalFormat::kDepthComponent24,
        pixel::ComponentType::kFloat);
  const Texture& light_rt = resource_manager_->get_texture(light_rt_id_);
  const Texture& depth_rt = resource_manager_->get_texture(depth_rt_id);
  light_framebuffer_id_ = gpu_resource_manager_.create_framebuffer(
    depth_rt.gpu_resource_id,
    {light_rt.gpu_resource_id});
}

void DeferredRenderer::create_albedo_render_target_(int width, int height)
{
  light_plus_albedo_rt_id_ =
    resource_manager_->create_texture(width, height, pixel::Format::kRGBA,
        pixel::InternalFormat::kRGBA8, pixel::ComponentType::kUnsignedByte);
  uint32_t depth_rt_id =
    resource_manager_->create_texture(width, height,
        pixel::Format::kDepthComponent,
        pixel::InternalFormat::kDepthComponent24,
        pixel::ComponentType::kFloat);
  const Texture& color_rt = resource_manager_->get_texture(light_plus_albedo_rt_id_);
  const Texture& depth_rt = resource_manager_->get_texture(depth_rt_id);
  albedo_framebuffer_id_ = gpu_resource_manager_.create_framebuffer(
    depth_rt.gpu_resource_id,
    {color_rt.gpu_resource_id});
}

uint32_t DeferredRenderer::create_light_material_(
    const std::string& vertex_shader_path,
    const std::string& fragment_shader_path)
{
  light_program_id_ = resource_manager_->load_gpu_program_from_file(
    vertex_shader_path,
    fragment_shader_path);
  std::uint32_t id = resource_manager_->create_material(light_program_id_);
  const render::Material& material = resource_manager_->get_material(id);
  render::AMaterial& gpu_material = gpu_resource_manager_.get_material(
      material.gpu_resource_id);
  const Texture& albedo_rt = resource_manager_->get_texture(albedo_rt_id_);
  const Texture& normal_rt = resource_manager_->get_texture(normal_rt_id_);
  const Texture& depth_rt = resource_manager_->get_texture(depth_rt_id_);
  gpu_material.register_texture_slot("albedo_tex", albedo_rt.gpu_resource_id,
      0);
  gpu_material.register_texture_slot("normals_tex", normal_rt.gpu_resource_id,
      1);
  gpu_material.register_texture_slot("depth_tex", depth_rt.gpu_resource_id, 2);
  return id;
}

uint32_t DeferredRenderer::create_albedo_material_(
    const std::string& vertex_shader_path,
    const std::string& fragment_shader_path)
{
  albedo_program_id_ = resource_manager_->load_gpu_program_from_file(
    vertex_shader_path,
    fragment_shader_path);
  std::uint32_t id = resource_manager_->create_material(albedo_program_id_);
  const render::Material& material = resource_manager_->get_material(id);
  render::AMaterial& gpu_material = gpu_resource_manager_.get_material(
      material.gpu_resource_id);
  const Texture& albedo_rt = resource_manager_->get_texture(albedo_rt_id_);
  const Texture& light_rt = resource_manager_->get_texture(light_rt_id_);
  gpu_material.register_texture_slot("albedo_texture",
      albedo_rt.gpu_resource_id, 0);
  gpu_material.register_texture_slot("light_texture", light_rt.gpu_resource_id,
      1);
  return id;
}

uint32_t DeferredRenderer::create_ambient_material_(
    const std::string& vertex_shader_path,
    const std::string& fragment_shader_path)
{
  ambient_program_id_ = resource_manager_->load_gpu_program_from_file(
    vertex_shader_path,
    fragment_shader_path);
  std::uint32_t id = resource_manager_->create_material(ambient_program_id_);
  const render::Material& material = resource_manager_->get_material(id);
  render::AMaterial& gpu_material = gpu_resource_manager_.get_material(
      material.gpu_resource_id);
  const Texture& albedo_rt = resource_manager_->get_texture(light_plus_albedo_rt_id_);
  const Texture& depth_rt = resource_manager_->get_texture(depth_rt_id_);
  gpu_material.register_texture_slot("albedo_texture", albedo_rt.gpu_resource_id,
      0);
  gpu_material.register_texture_slot("depth_texture", depth_rt.gpu_resource_id,
      1);
  return id;
}

void DeferredRenderer::create_light_accu_pass_frame_packet_(int width, int height)
{
  // create material for light accumulation pass
  uint32_t light_material_id = create_light_material_(
    "shaders/simple.vert.glsl",
    "shaders/light-pass.frag.glsl");
  frame_packets_.push_back(
    StackFramePacket(
      StackAllocator<MeshNode>(Buffer::Tag::kLightFramePacket, 0),
      donkey::CameraNode(
          1,
          glm::vec3(0.0f, 0.0f, 0.0f),
          glm::vec3(0.0f, 0.0f, 0.0f),
          glm::tvec2<int>(0, 0),
          glm::tvec2<GLsizei>(width, height),
          0.0f,
          -1.0f,
          1.0f,
          donkey::CameraNode::Type::kOrthographic)));
  StackFramePacket& light_frame_packet_ = frame_packets_.back();
  light_frame_packet_.create_mesh_node(1,
      glm::vec3(0.0f, 0.0f, 0.0f),
      glm::vec3(0.0f, 0.0f, 0.0f),
      glm::vec3(1.0f, 1.0f, 1.0f),
      screen_mesh_id_,
      light_material_id);
  pipeline_.add_render_pass({
      &light_frame_packet_,
      light_framebuffer_id_,
      GL_COLOR_BUFFER_BIT,
      glm::vec3(0.0f, 0.0f, 0.0f),
      false,
      true,
      true});
}

void DeferredRenderer::create_ambient_pass_frame_packet_(int width, int height)
{
  // create material for light accumulation pass
  uint32_t ambient_material_id = create_ambient_material_(
    "shaders/simple.vert.glsl",
    "shaders/ambient-pass.frag.glsl");
  frame_packets_.push_back(
    StackFramePacket(
      StackAllocator<MeshNode>(Buffer::Tag::kLightFramePacket, 0),
      donkey::CameraNode(
        3,
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::tvec2<int>(0, 0),
        glm::tvec2<GLsizei>(width, height),
        0.0f,
        -1.0f,
        1.0f,
        donkey::CameraNode::Type::kOrthographic)));
  StackFramePacket& ambient_frame_packet_ = frame_packets_.back();
  ambient_frame_packet_.create_mesh_node(3,
      glm::vec3(0.0f, 0.0f, 0.0f),
      glm::vec3(0.0f, 0.0f, 0.0f),
      glm::vec3(1.0f, 1.0f, 1.0f),
      screen_mesh_id_, ambient_material_id);
  pipeline_.add_render_pass({
      &ambient_frame_packet_,
      std::numeric_limits<uint32_t>::max(),
      GL_COLOR_BUFFER_BIT,
      glm::vec3(0.0f, 0.0f, 0.0f),
      false,
      false,
      false});
}

void DeferredRenderer::create_albedo_pass_frame_packet_(int width, int height)
{
  uint32_t albedo_material_id = create_albedo_material_(
    "shaders/simple.vert.glsl",
    "shaders/albedo-pass.frag.glsl");
  frame_packets_.push_back(
    StackFramePacket(
      StackAllocator<MeshNode>(Buffer::Tag::kAlbedoFramePacket, 0),
      donkey::CameraNode(
        2,
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::tvec2<int>(0, 0),
        glm::tvec2<GLsizei>(width, height),
        0.0f,
        -1.0f,
        1.0f,
        donkey::CameraNode::Type::kOrthographic)));
  StackFramePacket& albedo_frame_packet_ = frame_packets_.back();
  albedo_frame_packet_.create_mesh_node(2,
      glm::vec3(0.0f, 0.0f, 0.0f),
      glm::vec3(0.0f, 0.0f, 0.0f),
      glm::vec3(1.0f, 1.0f, 1.0f),
      screen_mesh_id_, albedo_material_id);
  pipeline_.add_render_pass({
      &albedo_frame_packet_,
      albedo_framebuffer_id_,
      GL_COLOR_BUFFER_BIT,
      glm::vec3(0.0f, 0.0f, 0.0f),
      false,
      false,
      false});
}

void DeferredRenderer::render(StackFramePacket* frame_packet,
  CommandBucket& render_commands)
{
  pipeline_.render(frame_packet, render_commands);
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

}
}
