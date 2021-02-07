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
#include "render/Pipeline.hpp"
#include "render/Material.hpp"
#include "Buffer.hpp"
#include "BufferPool.hpp"
#include "render/CommandBucket.hpp"

namespace donkey
{
  namespace render
  {

    Pipeline::Pipeline(
      Window* window,
      gl::Driver* driver,
      ResourceManager* resource_manager) :
      window_(window),
      render_context_(window->get_render_context()),
      driver_(driver),
      gpu_resource_manager_(driver_->get_resource_manager()),
      resource_manager_(resource_manager)
    {
    }

    Pipeline::~Pipeline()
    {
    }

    void Pipeline::render_geometry_(
      size_t pass_num,
      const RenderPass& render_pass,
      const StackVector<MeshNode>& mesh_nodes,
      const CameraNode& camera_node,
      const CameraNode* last_camera_node,
      const StackVector<DirectionalLightNode>& light_nodes,
      CommandBucket& render_commands,
      ResourceManager* resource_manager,
      GpuResourceManager* gpu_resource_manager)
    {
      for (const MeshNode& mesh_node : mesh_nodes)
      {
        if (camera_node.pass_num != pass_num)
          continue;

        if (render_pass.lighting)
        {
          for (const DirectionalLightNode& light_node : light_nodes)
          {
            render_mesh_node(render_pass,
              mesh_node,
              camera_node,
              last_camera_node,
              &light_node,
              render_commands,
              resource_manager,
              gpu_resource_manager);
          }
        }
        else
        {
          render_mesh_node(
            render_pass,
            mesh_node,
            camera_node,
            last_camera_node,
            nullptr,
            render_commands,
            resource_manager,
            gpu_resource_manager);
        }
      }
    }

    void Pipeline::execute_pass_(
      size_t pass_num,
      const RenderPass& render_pass,
      const StackFramePacket& frame_packet,
      const CameraNode* last_camera_node,
      const StackVector<DirectionalLightNode>& light_nodes,
      CommandBucket& render_commands,
      ResourceManager* resource_manager,
      GpuResourceManager* gpu_resource_manager)
    {
      render_commands.bind_framebuffer(render_pass.framebuffer_id);
      render_commands.set_depth_test(render_pass.depth_test);
      render_commands.set_blending(render_pass.blending);

      const CameraNode& camera_node = frame_packet.get_camera_node();
      assert(camera_node.pass_num == pass_num);
      render_commands.set_viewport(camera_node.viewport_position,
        camera_node.viewport_size);
      render_commands.clear_framebuffer(render_pass.clear_color);

      render_geometry_(
        pass_num,
        render_pass,
        frame_packet.get_mesh_nodes(),
        camera_node,
        last_camera_node,
        light_nodes,
        render_commands,
        resource_manager,
        gpu_resource_manager);
    }

    void Pipeline::render(StackFramePacket* gbuffer_frame_packet,
      CommandBucket& render_commands)
    {
      const CameraNode* last_camera_node = &(gbuffer_frame_packet->get_camera_node());
      signpost_start(1, 0, 0, 0, 0);
      for (size_t i = 0; i < render_passes_.size(); ++i)
      {
        const StackFramePacket* frame_packet;
        if (render_passes_[i].frame_packet)
          frame_packet = render_passes_[i].frame_packet;
        else
          frame_packet = gbuffer_frame_packet;
        execute_pass_(
          i,
          render_passes_[i],
          *frame_packet,
          last_camera_node,
          gbuffer_frame_packet->get_directional_light_nodes(),
          render_commands,
          resource_manager_,
          &gpu_resource_manager_);
        last_camera_node = &(frame_packet->get_camera_node());
      }
      signpost_end(1, 0, 0, 0, 0);
    }

    void Pipeline::add_render_pass(const RenderPass& render_pass)
    {
      render_passes_.push_back(render_pass);
    }

    void Pipeline::add_render_pass(RenderPass&& render_pass)
    {
      render_passes_.push_back(render_pass);
    }

  }
}
