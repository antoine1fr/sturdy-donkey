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
void GBufferPass::register_input_textures(Pipeline& pipeline, int width, int height) {
  pipeline.register_texture("albedo_texture", width, height,
                            pixel::Format::kRGBA, pixel::InternalFormat::kRGBA8,
                            pixel::ComponentType::kUnsignedByte);
  pipeline.register_texture("normals_texture", width, height,
                            pixel::Format::kRGBA, pixel::InternalFormat::kRGBA8,
                            pixel::ComponentType::kFloat);
  pipeline.register_texture(
      "depth_texture", width, height, pixel::Format::kDepthComponent,
      pixel::InternalFormat::kDepthComponent24, pixel::ComponentType::kFloat);
}

void GBufferPass::register_pass(Pipeline& pipeline) {
  framebuffer_id_ = pipeline.register_pass(
      {"albedo_texture", "normals_texture", "depth_texture"});
}

void GBufferPass::prepare_frame_packet(const ResourceManager& resource_manager,
                                       const Scene& scene,
                                       FramePacket& packet) {
  glm::vec3 clear_color = glm::vec3(0.0f, 0.0f, 0.0f);
  const Framebuffer& framebuffer =
      resource_manager.get_framebuffer(framebuffer_id_);
  const GpuProgram& gpu_program =
      resource_manager.get_gpu_program(gpu_program_id_);
  const GpuBuffer& pass_uniform_buffer =
      resource_manager.get_gpu_buffer(pass_uniform_buffer_id_);
  const GpuBuffer& object_uniform_buffer =
      resource_manager.get_gpu_buffer(object_uniform_buffer_id_);
  const Mesh& mesh = resource_manager.get_mesh(mesh_id_);
  FramePacket::UniformBlock pass_uniform_block =
      FramePacket::UniformBlock{gpu_program.pass_uniform_block_id,
                                pass_uniform_buffer.gpu_resource_id,
                                {}};
  FramePacket::Vector<FramePacket::RenderObject> render_objects;

  render_objects.reserve(scene.get_mesh_nodes().size());
  fill_up_render_objects(resource_manager, gpu_program, object_uniform_buffer,
                         scene, render_objects);
  FramePacket::Pass pass =
      FramePacket::Pass{gpu_program.gpu_resource_id,
                        pass_uniform_buffer.gpu_resource_id,
                        object_uniform_buffer.gpu_resource_id,
                        framebuffer.gpu_resource_id,
                        FramePacket::ClearFlag::kColorBuffer |
                            FramePacket::ClearFlag::kDepthBuffer,
                        clear_color,
                        pass_uniform_block,
                        render_objects,
                        true,
                        false};
  packet.passes.push_back(pass);
}

void GBufferPass::fill_up_render_objects(
    const ResourceManager& resource_manager,
    const GpuProgram& gpu_program,
    const GpuBuffer& object_uniform_buffer,
    const Scene& scene,
    Vector<FramePacket::RenderObject> render_objects) {
  for (const auto& mesh_node : scene.get_mesh_nodes()) {
    const Mesh& mesh = resource_manager.get_mesh(mesh_node.mesh_id);
    FramePacket::UniformBlock mesh_uniform_block =
        FramePacket::UniformBlock{gpu_program.object_uniform_block_id,
                                  object_uniform_buffer.gpu_resource_id,
                                  {}};
    render_objects.push_back(
        {mesh.gpu_resource_id, {}, {}, {}, {}, mesh_uniform_block});
  }
}
}  // namespace render
}  // namespace donkey
