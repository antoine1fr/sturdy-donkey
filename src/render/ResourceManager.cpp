#include "render/ResourceManager.hpp"

namespace donkey {
namespace render {

ResourceManager::ResourceManager(AResourceManager& gpu_resource_manager):
  gpu_resource_manager_(gpu_resource_manager)
{
}

const GpuProgram& ResourceManager::get_gpu_program(uint32_t id) const
{
  return gpu_programs_[id];
}

const Material& ResourceManager::get_material(uint32_t id) const
{
  return materials_[id];;
}

const Mesh& ResourceManager::get_mesh(uint32_t id) const
{
  return meshes_[id];
}

const Texture& ResourceManager::get_texture(uint32_t id) const
{
  return textures_[id];
}

void ResourceManager::cleanup()
{
  gpu_resource_manager_.cleanup();
}

uint32_t ResourceManager::load_texture_from_file(const std::string& path)
{
  uint32_t id = gpu_resource_manager_.load_texture_from_file(path);
  textures_.push_back(Texture(id));
  return textures_.size() - 1;
}

uint32_t ResourceManager::load_gpu_program_from_file(
    const std::string& vs_path,
    const std::string& fs_path)
{
  std::uint32_t id = gpu_resource_manager_.load_gpu_program_from_file(
      vs_path, fs_path);
  gpu_programs_.push_back(GpuProgram(id));
  return gpu_programs_.size() - 1;
}

uint32_t ResourceManager::create_material(uint32_t gpu_program)
{
  std::uint32_t id = gpu_resource_manager_.create_material(gpu_program);
  const AMaterial& gpu_material = gpu_resource_manager_.get_material(id);
  materials_.push_back(Material(
    id,
    gpu_program,
    gpu_material.position_location,
    gpu_material.uv_location,
    gpu_material.model_location,
    gpu_material.view_location,
    gpu_material.projection_location,
    gpu_material.projection_inverse_location,
    gpu_material.projection_params_location,
    gpu_material.ambient_location,
    gpu_material.light_dir_location,
    gpu_material.light_diffuse_location,
    gpu_material.light_specular_location,
    gpu_material.camera_position_location));
  return materials_.size() - 1;
}

uint32_t ResourceManager::create_mesh(
    const std::vector<float>& positions,
    const std::vector<float>& uvs,
    const std::vector<uint32_t>& indices)
{
  uint32_t id = gpu_resource_manager_.create_mesh(positions, uvs, indices);
  meshes_.push_back(Mesh(id, indices.size()));
  return meshes_.size() - 1;
}

uint32_t ResourceManager::create_texture(
    std::size_t width,
    std::size_t height,
    pixel::Format format,
    pixel::InternalFormat internal_format,
    pixel::ComponentType component_type)
{
  uint32_t id = gpu_resource_manager_.create_texture(width, height, format,
      internal_format, component_type);
  textures_.push_back(Texture(id));
  return textures_.size() - 1;
}

}
}
