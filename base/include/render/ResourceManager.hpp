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

#include <list>

#include "render/Framebuffer.hpp"
#include "render/GpuBuffer.hpp"
#include "render/GpuResourceManager.hpp"
#include "render/Material.hpp"
#include "render/State.hpp"
#include "render/UniformBlock.hpp"

struct SDL_Surface;

namespace donkey {
namespace render {

class ResourceManager {
 private:
  GpuResourceManager& gpu_resource_manager_;
  std::vector<Framebuffer> framebuffers_;
  std::vector<GpuProgram> gpu_programs_;
  std::vector<Mesh> meshes_;
  std::vector<Texture> textures_;
  std::vector<Material> materials_;
  std::vector<State> states_;
  std::vector<GpuBuffer> gpu_buffers_;
  std::vector<UniformBlock> uniform_blocks_;

 public:
  ResourceManager(GpuResourceManager& gpu_resource_manager);

  const Framebuffer& get_framebuffer(uint32_t id) const;
  const GpuProgram& get_gpu_program(uint32_t id) const;
  const Material& get_material(uint32_t id) const;
  const Mesh& get_mesh(uint32_t id) const;
  const Texture& get_texture(uint32_t id) const;
  const GpuBuffer& get_gpu_buffer(uint32_t id) const;

  uint32_t register_material(Material&& material);
  void cleanup();
  uint32_t load_texture_from_file(const std::string& path);
  uint32_t load_texture_from_memory(uint8_t* pixels, int width, int height);

  uint32_t load_gpu_program_from_file(const std::string& vs_path,
                                      const std::string& fs_path);

  uint32_t create_framebuffer(const std::list<uint32_t>& texture_ids);
  uint32_t create_material(uint32_t gpu_program);

  uint32_t create_mesh(const std::vector<float>& positions,
                       const std::vector<uint32_t>& indices);

  uint32_t create_texture(std::size_t width,
                          std::size_t height,
                          pixel::Format format,
                          pixel::InternalFormat internal_format,
                          pixel::ComponentType component_type);
  uint32_t create_gpu_buffer();
  GLuint load_texture_(uint8_t* pixels, int width, int height);
  SDL_Surface* create_mirror_surface_(SDL_Surface* surface);

  uint32_t create_state(const State& state);
};

}  // namespace render
}  // namespace donkey
