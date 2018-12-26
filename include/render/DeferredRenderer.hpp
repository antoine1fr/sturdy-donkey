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

#include <array>
#include <thread>
#include <condition_variable>
#include <atomic>
#include <SDL.h>
#include <limits>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "render/gl/Driver.hpp"
#include "render/FramePacket.hpp"
#include "render/AResourceManager.hpp"
#include "render/ResourceManager.hpp"
#include "render/RenderPass.hpp"
#include "render/Window.hpp"

namespace donkey {
namespace render {

class DeferredRenderer
{
  private:
    std::vector<RenderPass> render_passes_;
    std::atomic_bool run_;
    Window& window_;
    SDL_GLContext render_context_;
    gl::Driver driver_;
    AResourceManager& gpu_resource_manager_;
    ResourceManager resource_manager_;
    FramePacket<std::allocator> light_frame_packet_;
    FramePacket<std::allocator> albedo_frame_packet_;
    std::thread* render_thread_;
    uint32_t light_program_id_;
    uint32_t albedo_program_id_;
    uint32_t light_rt_id_;
    uint32_t albedo_rt_id_;
    uint32_t normal_rt_id_;
    uint32_t depth_rt_id_;
    uint32_t gbuffer_id_;
    uint32_t light_framebuffer_id_;
    uint32_t screen_mesh_id_;

  public:
    std::condition_variable condition_variable;
    std::mutex mutex;
    std::atomic_uint32_t frame_count;
    std::atomic_uint32_t render_frame_index;

  private:
    template <typename T, template <typename> class Allocator>
      using Vector = typename FramePacket<Allocator>::template Vector<T>;

    void render_mesh_node_(
        const RenderPass& render_pass,
        const MeshNode& mesh_node,
        const CameraNode& camera_node,
        const CameraNode* last_camera_node,
        const DirectionalLightNode* light_node,
        CommandBucket& render_commands);
    template <template <typename> class Allocator>
      void render_geometry_(
          size_t pass_num,
          const RenderPass& render_pass,
          const Vector<MeshNode, Allocator>& mesh_nodes,
          const CameraNode& camera_node,
          const CameraNode* last_camera_node,
          const DirectionalLightNode* light_node,
          CommandBucket& render_commands);
    template <template <typename> class Allocator>
      void execute_gbuffer_pass_(
          size_t pass_num,
          const RenderPass& render_pass,
          const FramePacket<Allocator>& frame_packet,
          CommandBucket& render_commands);
    template <template <typename> class Allocator>
      void execute_light_pass_(
          size_t pass_num,
          const RenderPass& render_pass,
          const FramePacket<std::allocator>& frame_packet,
          const CameraNode* last_camera_node,
          const Vector<DirectionalLightNode, Allocator>& light_nodes,
          CommandBucket& render_commands);
    template <template <typename> class Allocator>
      void execute_albedo_pass_(
          size_t pass_num,
          const RenderPass& render_pass,
          const FramePacket<std::allocator>& frame_packet,
          CommandBucket& render_commands);
    void bind_light_uniforms_(
        CommandBucket& render_commands,
        const Material& material,
        const glm::mat4& view,
        const DirectionalLightNode* light_node) const;
    void bind_camera_uniforms_(
        CommandBucket& render_commands,
        const Material& material,
        const CameraNode& camera_node) const;
    void bind_mesh_uniforms_(
        CommandBucket& render_commands,
        const Material& material,
        const MeshNode& mesh_node) const;
    void create_light_pass_mesh_(int width, int height);
    void create_gbuffer_(int width, int height);
    void create_light_render_target_(int width, int height);
    uint32_t create_light_material_(
        const std::string& vertex_shader_path,
        const std::string& fragment_shader_path);
    uint32_t create_albedo_material_(
        const std::string& vertex_shader_path,
        const std::string& fragment_shader_path);
    void create_light_pass_frame_packet_(int width, int height);
    void create_albedo_pass_frame_packet_(int width, int height);

  public:
    DeferredRenderer(Window& window);
    ~DeferredRenderer();
    void render();
    ResourceManager& get_resource_manager();
    AResourceManager& get_gpu_resource_manager();
    void add_render_pass(const RenderPass& render_pass);
    void add_render_pass(RenderPass&& render_pass);
    void notify_exit();
    uint32_t get_albedo_rt_id() const;
    uint32_t get_normal_rt_id() const;
    uint32_t get_depth_rt_id() const;
    void start_render_thread();
};

template <template <typename> class Allocator>
void DeferredRenderer::render_geometry_(
    size_t pass_num,
    const RenderPass& render_pass,
    const Vector<MeshNode, Allocator>& mesh_nodes,
    const CameraNode& camera_node,
    const CameraNode* last_camera_node,
    const DirectionalLightNode* light_node,
    CommandBucket& render_commands)
{
  for (auto mesh_node: mesh_nodes)
  {
    if (camera_node.pass_num != pass_num)
      continue;
    render_mesh_node_(
        render_pass,
        mesh_node,
        camera_node,
        last_camera_node,
        light_node,
        render_commands);
  }
}

template <template <typename> class Allocator>
void DeferredRenderer::execute_gbuffer_pass_(
    size_t pass_num,
    const RenderPass& render_pass,
    const FramePacket<Allocator>& frame_packet,
    CommandBucket& render_commands)
{
  render_commands.bind_framebuffer(render_pass.framebuffer_id);

  render_commands.set_depth_test(true);

  const CameraNode& camera_node = frame_packet.get_camera_nodes()[0];
  assert(camera_node.pass_num == pass_num);
  render_commands.set_viewport(camera_node.viewport_position,
      camera_node.viewport_size);
  render_commands.clear_framebuffer(render_pass.clear_color);
  render_geometry_<Allocator>(
      pass_num,
      render_pass,
      frame_packet.get_mesh_nodes(),
      camera_node,
      nullptr,
      nullptr,
      render_commands);
}

template <template <typename> class Allocator>
void DeferredRenderer::execute_light_pass_(
    size_t pass_num,
    const RenderPass& render_pass,
    const FramePacket<std::allocator>& frame_packet,
    const CameraNode* last_camera_node,
    const Vector<DirectionalLightNode, Allocator>& light_nodes,
    CommandBucket& render_commands)
{
  render_commands.bind_framebuffer(render_pass.framebuffer_id);
  render_commands.set_depth_test(false);
  render_commands.set_blending(true);

  const CameraNode& camera_node = frame_packet.get_camera_nodes()[0];
  assert(camera_node.pass_num == pass_num);
  render_commands.set_viewport(camera_node.viewport_position,
      camera_node.viewport_size);
  render_commands.clear_framebuffer(render_pass.clear_color);
  const Vector<MeshNode, std::allocator>& mesh_nodes =
    frame_packet.get_mesh_nodes();
  // 1. accumulate diffuse terms and specular terms
  for (const DirectionalLightNode& light_node: light_nodes)
  {
    render_mesh_node_(
        render_pass,
        mesh_nodes[0],
        camera_node,
        last_camera_node,
        &light_node,
        render_commands);
  }
  // 2. add ambient term
  render_mesh_node_(
      render_pass,
      mesh_nodes[1],
      camera_node,
      last_camera_node,
      nullptr,
      render_commands);
  render_commands.set_blending(false);
}

template <template <typename> class Allocator>
void DeferredRenderer::execute_albedo_pass_(
    size_t pass_num,
    const RenderPass& render_pass,
    const FramePacket<std::allocator>& frame_packet,
    CommandBucket& render_commands)
{
  render_commands.bind_framebuffer(render_pass.framebuffer_id);
  render_commands.set_depth_test(false);

  const CameraNode& camera_node = frame_packet.get_camera_nodes()[0];
  assert(camera_node.pass_num == pass_num);
  render_commands.set_viewport(camera_node.viewport_position,
      camera_node.viewport_size);
  render_commands.clear_framebuffer(render_pass.clear_color);
  const Vector<MeshNode, std::allocator>& mesh_nodes =
    frame_packet.get_mesh_nodes();
  render_mesh_node_(
      render_pass,
      mesh_nodes[0],
      camera_node,
      nullptr,
      nullptr,
      render_commands);
}

}
}
