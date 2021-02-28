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

#include "render/core_renderer.hpp"

namespace donkey {
namespace render {
static void render_object_(const FramePacket::RenderObject& render_object,
                           CommandBucket& bucket) {
  bucket.bind_uniform_block(render_object.uniform_block);
  bucket.bind_mesh(render_object.mesh_id, render_object.vertex_attributes,
                   render_object.vertices, render_object.indices);
}

static void render_pass_(const ResourceManager& resource_manager,
                         const FramePacket::Pass& pass,
                         CommandBucket& bucket) {
  const Framebuffer& framebuffer =
      resource_manager.get_framebuffer(pass.framebuffer_id);
  for (const auto& render_object : pass.render_objects) {
    bucket.bind_framebuffer(framebuffer.gpu_resource_id);
    bucket.set_blending(pass.blending);
    bucket.set_depth_test(pass.depth_test);
    bucket.set_state(pass.clear_bits);
    bucket.clear_framebuffer(pass.clear_color);
    bucket.bind_uniform_block(uniform_block.gpu_resource_id);
    render_object_(render_object);
  }
}

void render(const FramePacket& frame_packet,
            const ResourceManager& resource_manager,
            CommandBucket& bucket) {
  for (const auto& pass : frame_packet.passes) {
    render_pass_(pass, bucket);
  }
}
}  // namespace render
}  // namespace donkey