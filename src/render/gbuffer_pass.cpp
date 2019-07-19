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

#include "render/RenderPass.hpp"

namespace donkey {
namespace render {

static void render_geometry_(
    size_t pass_num,
    const RenderPass& render_pass,
    const StackVector<MeshNode>& mesh_nodes,
    const CameraNode& camera_node,
    const CameraNode* last_camera_node,
    const DirectionalLightNode* light_node,
    CommandBucket& render_commands,
    ResourceManager* resource_manager,
    AResourceManager* gpu_resource_manager)
{
  for (auto mesh_node: mesh_nodes)
  {
    if (camera_node.pass_num != pass_num)
      continue;
    render_mesh_node(
        render_pass,
        mesh_node,
        camera_node,
        last_camera_node,
        light_node,
        render_commands,
        resource_manager,
        gpu_resource_manager);
  }
}

void execute_gbuffer_pass(
    size_t pass_num,
    const RenderPass& render_pass,
    const StackFramePacket& frame_packet,
    const CameraNode* last_camera_node,
    const StackVector<DirectionalLightNode>& light_nodes,
    CommandBucket& render_commands,
    ResourceManager* resource_manager,
    AResourceManager* gpu_resource_manager)
{
  render_commands.bind_framebuffer(render_pass.framebuffer_id);

  render_commands.set_depth_test(true);

  const CameraNode& camera_node = frame_packet.get_camera_nodes()[0];
  assert(camera_node.pass_num == pass_num);
  render_commands.set_viewport(camera_node.viewport_position,
      camera_node.viewport_size);
  render_commands.clear_framebuffer(render_pass.clear_color);
  render_geometry_(
      pass_num,
      render_pass,
      frame_packet.get_mesh_nodes(),
      camera_node,
      nullptr,
      nullptr,
      render_commands,
      resource_manager,
      gpu_resource_manager);
}

  
} // namespace render
} // namespace donkey
