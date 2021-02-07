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

#include "render/RenderPass.hpp"
#include "render/Window.hpp"
#include "render/gl/Driver.hpp"

namespace donkey {
  namespace render {


    class Pipeline
    {
    private:
      std::vector<RenderPass> render_passes_;
      Window* window_;
      Window::Context render_context_;
      gl::Driver* driver_;
      GpuResourceManager& gpu_resource_manager_;
      ResourceManager* resource_manager_;

      typedef std::list<StackFramePacket> FramePacketList;
      std::list<StackFramePacket> frame_packets_;

    private:
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
      void render_geometry_(
        size_t pass_num,
        const RenderPass& render_pass,
        const StackVector<MeshNode>& mesh_nodes,
        const CameraNode& camera_node,
        const CameraNode* last_camera_node,
        const StackVector<DirectionalLightNode>& light_nodes,
        CommandBucket& render_commands,
        ResourceManager* resource_manager,
        GpuResourceManager* gpu_resource_manager);
      void execute_pass_(
        size_t pass_num,
        const RenderPass& render_pass,
        const StackFramePacket& frame_packet,
        const CameraNode* last_camera_node,
        const StackVector<DirectionalLightNode>& light_nodes,
        CommandBucket& render_commands,
        ResourceManager* resource_manager,
        GpuResourceManager* gpu_resource_manager);

    public:
      Pipeline(
        Window* window,
        gl::Driver* driver,
        ResourceManager* resource_manager);
      ~Pipeline();
      void render(StackFramePacket* frame_packet,
        CommandBucket& render_commands);
      void add_render_pass(const RenderPass& render_pass);
      void add_render_pass(RenderPass&& render_pass);
      uint32_t get_albedo_rt_id() const;
      uint32_t get_normal_rt_id() const;
      uint32_t get_depth_rt_id() const;
    };

  }
}
