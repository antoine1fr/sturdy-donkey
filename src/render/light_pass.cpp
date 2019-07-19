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

void execute_light_pass(
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
  render_commands.set_depth_test(false);
  render_commands.set_blending(true);

  const CameraNode& camera_node = frame_packet.get_camera_nodes()[0];
  assert(camera_node.pass_num == pass_num);
  render_commands.set_viewport(camera_node.viewport_position,
      camera_node.viewport_size);
  render_commands.clear_framebuffer(render_pass.clear_color);
  const StackVector<MeshNode>& mesh_nodes =
    frame_packet.get_mesh_nodes();
  // 1. accumulate diffuse terms and specular terms
  for (const DirectionalLightNode& light_node: light_nodes)
  {
    render_mesh_node(
        render_pass,
        mesh_nodes[0],
        camera_node,
        last_camera_node,
        &light_node,
        render_commands,
        resource_manager,
        gpu_resource_manager);
  }
  // 2. add ambient term
  render_mesh_node(
      render_pass,
      mesh_nodes[1],
      camera_node,
      last_camera_node,
      nullptr,
      render_commands,
      resource_manager,
      gpu_resource_manager);
  render_commands.set_blending(false);
}

} // namespace render
} // namespace donkey
