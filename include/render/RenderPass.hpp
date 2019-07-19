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

#include "render/FramePacket.hpp"
#include "render/CommandBucket.hpp"
#include "ResourceManager.hpp"

namespace donkey
{

namespace render
{

struct RenderPass;

typedef void (*ExecuteRenderPass)(
  size_t pass_num,
  const RenderPass& render_pass,
  const StackFramePacket& frame_packet,
  const CameraNode* last_camera_node,
  const StackVector<DirectionalLightNode>& light_nodes,
  CommandBucket& render_commands,
  ResourceManager* resource_manager,
  AResourceManager* gpu_resource_manager);


struct RenderPass
{
  uint32_t framebuffer_id;
  GLint clear_bits;
  glm::vec3 clear_color;
  bool depth_test;
  bool lighting; // loop through lights if true
  ExecuteRenderPass execute;
};

void render_mesh_node(
  const RenderPass& render_pass,
  const MeshNode& mesh_node,
  const CameraNode& camera_node,
  const CameraNode* last_camera_node,
  const DirectionalLightNode* light_node,
  CommandBucket& render_commands,
  ResourceManager* resource_manager,
  AResourceManager* gpu_resource_manager);

void execute_light_pass(
  size_t pass_num,
  const RenderPass& render_pass,
  const StackFramePacket& frame_packet,
  const CameraNode* last_camera_node,
  const StackVector<DirectionalLightNode>& light_nodes,
  CommandBucket& render_commands,
  ResourceManager* resource_manager,
  AResourceManager* gpu_resource_manager);

void execute_albedo_pass(
  size_t pass_num,
  const RenderPass& render_pass,
  const StackFramePacket& frame_packet,
  const CameraNode* last_camera_node,
  const StackVector<DirectionalLightNode>& light_nodes,
  CommandBucket& render_commands,
  ResourceManager* resource_manager,
  AResourceManager* gpu_resource_manager);

void execute_gbuffer_pass(
  size_t pass_num,
  const RenderPass& render_pass,
  const StackFramePacket& frame_packet,
  const CameraNode* last_camera_node,
  const StackVector<DirectionalLightNode>& light_nodes,
  CommandBucket& render_commands,
  ResourceManager* resource_manager,
  AResourceManager* gpu_resource_manager);

}
}
