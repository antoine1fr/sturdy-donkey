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

#include "IRenderPass.hpp"
#include "Scene.hpp"
#include "render/FramePacket.hpp"
#include "render/ResourceManager.hpp"

namespace donkey {
namespace render {

class GBufferPass : public IRenderPass {
 public:
  virtual void register_input_textures(Pipeline& pipeline, int width, int height);
  // virtual void register_pass(Pipeline& pipeline);
  virtual void prepare_frame_packet(const ResourceManager& resource_manager,
                                    const Scene& scene,
                                    render::FramePacket& frame_packet);

 private:
  template <typename T>
  using Vector = FramePacket::Vector<T>;

  void fill_up_render_objects(const ResourceManager& resource_manager,
                              const GpuProgram& gpu_program,
                              const GpuBuffer& object_uniform_buffer,
                              const Scene& scene,
                              Vector<FramePacket::RenderObject> render_objects);

 private:
  uint32_t framebuffer_id_;
  uint32_t gpu_buffer_id_;
  uint32_t gpu_program_id_;
  uint32_t mesh_id_;
  uint32_t object_uniform_buffer_id_;
  uint32_t pass_uniform_buffer_id_;
};

}  // namespace render
}  // namespace donkey
