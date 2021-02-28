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

#include <iostream>
#pragma warning(push)
#pragma warning(disable : 26812 26819)
#include <SDL_image.h>
#pragma warning(pop)
#include "render/ResourceManager.hpp"

namespace donkey {
namespace render {

ResourceManager::ResourceManager(GpuResourceManager& gpu_resource_manager)
    : gpu_resource_manager_(gpu_resource_manager) {}

const Framebuffer& ResourceManager::get_framebuffer(uint32_t id) const {
  return framebuffers_[id];
}

const GpuProgram& ResourceManager::get_gpu_program(uint32_t id) const {
  return gpu_programs_[id];
}

const Material& ResourceManager::get_material(uint32_t id) const {
  return materials_[id];
  ;
}

const Mesh& ResourceManager::get_mesh(uint32_t id) const {
  return meshes_[id];
}

const Texture& ResourceManager::get_texture(uint32_t id) const {
  return textures_[id];
}

const GpuBuffer& ResourceManager::get_gpu_buffer(uint32_t id) const {
  return gpu_buffers_[id];
}

void ResourceManager::cleanup() {
  gpu_resource_manager_.cleanup();
}

// uint32_t ResourceManager::load_texture_from_file(const std::string& path)
//{
//  uint32_t id = gpu_resource_manager_.load_texture_from_file(path);
//  textures_.push_back(Texture(id));
//  return textures_.size() - 1;
//}

uint32_t ResourceManager::load_texture_from_file(const std::string& path) {
  std::cout << "Loading texture from file: " << path << '\n';
  SDL_Surface* original_surface = IMG_Load(path.c_str());
  SDL_Surface* img_surface =
      SDL_ConvertSurfaceFormat(original_surface, SDL_PIXELFORMAT_RGBA32, 0);
  SDL_FreeSurface(original_surface);
  SDL_Surface* mirror_surface = create_mirror_surface_(img_surface);
  uint32_t id = load_texture_from_memory(
      reinterpret_cast<uint8_t*>(mirror_surface->pixels), mirror_surface->w,
      mirror_surface->h);
  SDL_FreeSurface(img_surface);
  SDL_FreeSurface(mirror_surface);
  return id;
}

SDL_Surface* ResourceManager::create_mirror_surface_(SDL_Surface* surface) {
  int width = surface->w;
  int height = surface->h;
  SDL_Surface* new_surface = SDL_CreateRGBSurfaceWithFormat(
      0, width, height, 32, SDL_PIXELFORMAT_RGBA32);
  uint32_t* src_pixels = reinterpret_cast<uint32_t*>(surface->pixels);
  uint32_t* dst_pixels = reinterpret_cast<uint32_t*>(new_surface->pixels);

  SDL_LockSurface(new_surface);
  SDL_LockSurface(surface);
  for (size_t y = 0; y < height; ++y) {
    for (size_t x = 0; x < width; ++x) {
      uint32_t* src_ptr = src_pixels + y * width + x;
      uint32_t* dst_ptr = dst_pixels + ((height - y - 1) * width) + x;
      *dst_ptr = *src_ptr;
    }
  }
  SDL_UnlockSurface(surface);
  SDL_UnlockSurface(new_surface);
  return new_surface;
}

uint32_t ResourceManager::load_texture_from_memory(uint8_t* pixels,
                                                   int width,
                                                   int height) {
  uint32_t id =
      gpu_resource_manager_.load_texture_from_memory(pixels, width, height);
  textures_.push_back(Texture(id, pixel::Format::kRGBA,
                              pixel::InternalFormat::kRGBA8,
                              pixel::ComponentType::kByte));
  return static_cast<uint32_t>(textures_.size()) - 1;
}

uint32_t ResourceManager::load_gpu_program_from_file(
    const std::string& vs_path,
    const std::string& fs_path) {
  std::uint32_t id =
      gpu_resource_manager_.load_gpu_program_from_file(vs_path, fs_path);
  std::hash<std::string> hash;
  gpu_programs_.push_back(GpuProgram(id));
  GpuProgram& program = gpu_programs_.back();
  program.object_uniform_block_id = hash("object_uniform_block");
  program.pass_uniform_block_id = hash("pass_uniform_block");
  return static_cast<uint32_t>(gpu_programs_.size()) - 1;
}

uint32_t ResourceManager::create_framebuffer(
    const std::list<uint32_t>& texture_ids) {
  std::list<const Texture*> textures;
  for (auto texture_id : texture_ids) {
    const Texture& texture = get_texture(texture_id);
    textures.push_back(&texture);
  }
  gpu_resource_manager_.create_framebuffer(textures);
}

uint32_t ResourceManager::create_material(uint32_t gpu_program) {
  std::uint32_t id = gpu_resource_manager_.create_material(gpu_program);
  const AMaterial& gpu_material = gpu_resource_manager_.get_material(id);
  materials_.push_back(Material(
      id, gpu_program, gpu_material.position_location,
      gpu_material.normal_location, gpu_material.uv_location,
      gpu_material.tangent_location, gpu_material.bitangent_location,
      gpu_material.model_location, gpu_material.view_location,
      gpu_material.projection_location,
      gpu_material.gbuffer_projection_inverse_location,
      gpu_material.gbuffer_projection_params_location,
      gpu_material.gbuffer_view_location, gpu_material.ambient_location,
      gpu_material.light_dir_location, gpu_material.light_diffuse_location,
      gpu_material.light_specular_location,
      gpu_material.camera_position_location));
  return static_cast<uint32_t>(materials_.size()) - 1;
}

uint32_t ResourceManager::create_mesh(const std::vector<float>& vertices,
                                      const std::vector<uint32_t>& indices) {
  uint32_t id = gpu_resource_manager_.create_mesh(vertices, indices);
  meshes_.push_back(Mesh(id, indices.size()));
  return static_cast<uint32_t>(meshes_.size()) - 1;
}

uint32_t ResourceManager::create_texture(std::size_t width,
                                         std::size_t height,
                                         pixel::Format format,
                                         pixel::InternalFormat internal_format,
                                         pixel::ComponentType component_type) {
  uint32_t id = gpu_resource_manager_.create_texture(
      width, height, format, internal_format, component_type);
  textures_.push_back(Texture(id, format, internal_format, component_type));
  return static_cast<uint32_t>(textures_.size()) - 1;
}

uint32_t ResourceManager::create_gpu_buffer() {
  uint32_t id = gpu_resource_manager_.create_buffer();
  gpu_buffers_.push_back(GpuBuffer(id));
  return static_cast<uint32_t>(gpu_buffers_.size()) - 1;
}

uint32_t ResourceManager::create_state(const State& state) {
  uint32_t id = gpu_resource_manager_.create_state(state);
  states_.push_back(State(id, state));
  return static_cast<uint32_t>(states_.size()) - 1;
}

}  // namespace render
}  // namespace donkey
