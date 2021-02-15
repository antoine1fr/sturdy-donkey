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

#include "GBufferPass.hpp"

namespace donkey {
namespace render {
void GBufferPass::register_pass(Pipeline& pipeline, int width, int height) {
  pipeline.register_texture("albedo_texture", width, height,
                            pixel::Format::kRGBA, pixel::InternalFormat::kRGBA8,
                            pixel::ComponentType::kUnsignedByte);
  pipeline.register_texture("normals_texture", width, height,
                            pixel::Format::kRGBA, pixel::InternalFormat::kRGBA8,
                            pixel::ComponentType::kFloat);
  pipeline.register_texture(
      "depth_texture", width, height, pixel::Format::kDepthComponent,
      pixel::InternalFormat::kDepthComponent24, pixel::ComponentType::kFloat);
  pipeline.register_pass(
      {"albedo_texture", "normals_texture", "depth_texture"});
}

void GBufferPass::prepare_frame_packet(render::FramePacket&) {
  assert(false);
}
}  // namespace render
}  // namespace donkey
