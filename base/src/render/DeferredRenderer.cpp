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

#include "render/DeferredRenderer.hpp"

#include <array>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <limits>

#include "Buffer.hpp"
#include "BufferPool.hpp"
#include "debug.hpp"
#include "render/CommandBucket.hpp"
#include "render/Material.hpp"

namespace donkey {
namespace render {

DeferredRenderer::DeferredRenderer(Window* window,
                                   gl::Driver* driver,
                                   ResourceManager* resource_manager)
    : window_(window),
      driver_(driver),
      gpu_resource_manager_(driver_->get_resource_manager()),
      resource_manager_(resource_manager),
      pipeline_(window, driver, resource_manager),
      pipeline_generator_(*resource_manager_,
                          gpu_resource_manager_,
                          pipeline_,
                          window->get_width(),
                          window->get_height()) {
  int width = window->get_width();
  int height = window->get_height();

  pipeline_generator_.register_texture(
      "albedo_texture", width, height, pixel::Format::kRGBA,
      pixel::InternalFormat::kRGBA8, pixel::ComponentType::kUnsignedByte);
  pipeline_generator_.register_texture(
      "normals_texture", width, height, pixel::Format::kRGBA,
      pixel::InternalFormat::kRGBA8, pixel::ComponentType::kFloat);
  pipeline_generator_.register_texture(
      "depth_texture", width, height, pixel::Format::kDepthComponent,
      pixel::InternalFormat::kDepthComponent24, pixel::ComponentType::kFloat);
  pipeline_generator_.register_texture(
      "light_texture", width, height, pixel::Format::kRGBA,
      pixel::InternalFormat::kRGBA8, pixel::ComponentType::kUnsignedByte);
  pipeline_generator_.register_texture(
      "light_plus_albedo_texture", width, height, pixel::Format::kRGBA,
      pixel::InternalFormat::kRGBA8, pixel::ComponentType::kUnsignedByte);

  // gbuffer pass
  pipeline_generator_.register_pass(
      {"albedo_texture", "normals_texture", "depth_texture"},
      GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT, true, false, false);
  // light accumulation pass
  pipeline_generator_.register_pass(
      {"albedo_texture", "normals_texture", "depth_texture"}, {"light_texture"},
      "shaders/simple.vert.glsl", "shaders/light-pass.frag.glsl",
      GL_COLOR_BUFFER_BIT, false, true, true);
  // albedo pass
  pipeline_generator_.register_pass(
      {"albedo_texture", "light_texture"}, {"light_plus_albedo_texture"},
      "shaders/simple.vert.glsl", "shaders/albedo-pass.frag.glsl",
      GL_COLOR_BUFFER_BIT, false, false, false);
  // ambiant pass
  pipeline_generator_.register_pass(
      {"light_plus_albedo_texture", "depth_texture"}, {},
      "shaders/simple.vert.glsl", "shaders/ambient-pass.frag.glsl",
      GL_COLOR_BUFFER_BIT, false, false, false);
}

DeferredRenderer::~DeferredRenderer() {}

void DeferredRenderer::render(StackFramePacket* frame_packet,
                              CommandBucket& render_commands) {
  pipeline_.render(frame_packet, render_commands);
}

}  // namespace render
}  // namespace donkey
