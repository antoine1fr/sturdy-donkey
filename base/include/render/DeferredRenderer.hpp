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
#include <atomic>
#include <condition_variable>
#include <thread>

#if defined(MSVC)
# pragma warning(push)
# pragma warning(disable : 26812 26819)
#endif

#include <SDL.h>

#if defined(MSVC)
# pragma warning(pop)
#endif

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <limits>

#include "IResourceLoaderDelegate.hpp"
#include "StackVector.hpp"
#include "render/FramePacket.hpp"
#include "render/GpuResourceManager.hpp"
#include "render/Pipeline.hpp"
#include "render/PipelineGenerator.hpp"
#include "render/RenderPass.hpp"
#include "render/ResourceManager.hpp"
#include "render/Window.hpp"
#include "render/gl/Driver.hpp"

namespace donkey {
namespace render {

class DeferredRenderer {
 private:
  Window* window_;
  gl::Driver* driver_;
  GpuResourceManager& gpu_resource_manager_;
  ResourceManager* resource_manager_;

  typedef std::list<StackFramePacket> FramePacketList;
  std::list<StackFramePacket> frame_packets_;

  Pipeline pipeline_;
  PipelineGenerator pipeline_generator_;

 private:
  void bind_light_uniforms_(CommandBucket& render_commands,
                            const Material& material, const glm::mat4& view,
                            const DirectionalLightNode* light_node) const;
  void bind_camera_uniforms_(CommandBucket& render_commands,
                             const Material& material,
                             const CameraNode& camera_node) const;
  void bind_mesh_uniforms_(CommandBucket& render_commands,
                           const Material& material,
                           const MeshNode& mesh_node) const;

 public:
  DeferredRenderer(Window* window, gl::Driver* driver,
                   ResourceManager* resource_manager);
  ~DeferredRenderer();
  void render(StackFramePacket* frame_packet, CommandBucket& render_commands);
};

}  // namespace render
}  // namespace donkey
