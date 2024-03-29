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
#include <tuple>

#if defined(MSVC)
# pragma warning(push)
# pragma warning(disable : 26812 26819)
#endif

#include <SDL_image.h>

#if defined(MSVC)
# pragma warning(pop)
#endif

#include "render/ResourceManager.hpp"

namespace std
{
  template<>
  struct hash<std::tuple<std::string, std::string>>
  {
    std::size_t operator() (const std::tuple<std::string, std::string>& tuple) const noexcept
      {
        std::hash<std::string> hash {};
        std::size_t h1 = hash(std::get<0>(tuple));
        std::size_t h2 = hash(std::get<1>(tuple));
        return h1 ^ (h2 << 1);
      }
  };
}

namespace donkey {
namespace render {

ResourceManager::ResourceManager(GpuResourceManager& gpu_resource_manager)
    : gpu_resource_manager_(gpu_resource_manager) {}

const GpuProgram& ResourceManager::get_gpu_program(Id id) const {
  return gpu_programs_.at(id);
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

ResourceManager::Id ResourceManager::load_gpu_program_from_file(
    const std::string& vs_path,
    const std::string& fs_path) {
  using Key = std::tuple<std::string, std::string>;
  std::hash<Key> hash {};
  size_t final_id = hash(std::make_tuple(vs_path, fs_path));
  if (gpu_programs_.find(final_id) == gpu_programs_.end())
  {
    std::uint32_t id =
      gpu_resource_manager_.load_gpu_program_from_file(vs_path, fs_path);
    gpu_programs_.emplace(std::make_pair(final_id, GpuProgram(id)));
  }
  return final_id;
}

uint32_t ResourceManager::create_material(Id cpu_side_gpu_program_id) {
  const GpuProgram& cpu_side_gpu_program = gpu_programs_.at(cpu_side_gpu_program_id);
  std::uint32_t gpu_program_id = gpu_resource_manager_.create_material(cpu_side_gpu_program.gpu_resource_id);
  const AMaterial& gpu_material = gpu_resource_manager_.get_material(gpu_program_id);
  materials_.push_back(Material(
      gpu_program_id, cpu_side_gpu_program_id, gpu_material.position_location,
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

uint32_t ResourceManager::create_mesh(const std::vector<float>& positions,
                                      const std::vector<float>& normals,
                                      const std::vector<float>& uvs,
                                      const std::vector<float>& tangents,
                                      const std::vector<float>& bitangents,
                                      const std::vector<uint32_t>& indices) {
  uint32_t id = gpu_resource_manager_.create_mesh(
      positions, normals, uvs, tangents, bitangents, indices);
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

uint32_t ResourceManager::create_state(const State& state) {
  uint32_t id = gpu_resource_manager_.create_state(state);
  states_.push_back(State(id, state));
  return static_cast<uint32_t>(states_.size()) - 1;
}

}  // namespace render
}  // namespace donkey
