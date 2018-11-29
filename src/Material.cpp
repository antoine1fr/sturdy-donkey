#include "Material.hpp"
#include "ResourceManager.hpp"

Material::Material(const ResourceManager& resource_manager,
    uint32_t program_id):
  resource_manager_(resource_manager),
  program_id(program_id)
{
}

void Material::bind_slots() const
{
  for (auto slot: slots_)
  {
    GLuint texture = resource_manager_.get_texture(slot.texture_id);
    glUniform1i(slot.location, slot.texture_unit);
    glActiveTexture(GL_TEXTURE0 + slot.texture_unit);
    glBindTexture(GL_TEXTURE_2D, texture);
  }
}

void Material::register_texture_slot(const std::string& name,
    uint32_t texture_id,
    int texture_unit)
{
  const GpuProgram& program = resource_manager_.get_gpu_program(program_id);
  int location = glGetUniformLocation(program.handle, name.c_str());
  texture_slots_.push_back(TextureMaterialSlot(location, texture_id,
    texture_unit));
}
