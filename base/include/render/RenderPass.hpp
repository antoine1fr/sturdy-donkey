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

#include "ResourceManager.hpp"
#include "render/CommandBucket.hpp"
#include "render/FramePacket.hpp"

namespace donkey {

namespace render {

struct RenderPass {
  const StackFramePacket* frame_packet;
  uint32_t framebuffer_id;
  GLint clear_bits;
  glm::vec3 clear_color;
  bool depth_test;
  bool lighting;  // loop through lights if true
  bool blending;
};

void render_mesh_node(const RenderPass& render_pass,
                      const MeshNode& mesh_node,
                      const CameraNode& camera_node,
                      const CameraNode* last_camera_node,
                      const DirectionalLightNode* light_node,
                      CommandBucket& render_commands,
                      ResourceManager* resource_manager,
                      GpuResourceManager* gpu_resource_manager);

}  // namespace render
}  // namespace donkey
