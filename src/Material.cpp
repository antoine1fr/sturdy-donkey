#include "Material.hpp"
#include "ResourceManager.hpp"

// TODO: find a pure c++ (i.e. no preprocessor) implementation that is as
// concise and doesn't use RTTIs.

#define DEFINE_REGISTER_SCALAR(x,y) \
  void Material::register_scalar(const std::string& name, const x& storage) \
  { \
    const GpuProgram& program = resource_manager_.get_gpu_program(program_id); \
    int location = glGetUniformLocation(program.handle, name.c_str()); \
    y ## _slots_.push_back(ScalarMaterialSlot<x>(location, storage)); \
  }

Material::Material(const ResourceManager& resource_manager,
    uint32_t program_id):
  resource_manager_(resource_manager),
  program_id(program_id)
{
  const GpuProgram& program = resource_manager_.get_gpu_program(program_id);
  model_location = glGetUniformLocation(program.handle, "model");
  view_location = glGetUniformLocation(program.handle, "view");
  projection_location = glGetUniformLocation(program.handle, "projection");
}

DEFINE_REGISTER_SCALAR(float, float)
DEFINE_REGISTER_SCALAR(glm::vec2, vec2)
DEFINE_REGISTER_SCALAR(glm::vec3, vec3)
DEFINE_REGISTER_SCALAR(glm::vec4, vec4)
DEFINE_REGISTER_SCALAR(glm::mat2, mat2)
DEFINE_REGISTER_SCALAR(glm::mat3, mat3)
DEFINE_REGISTER_SCALAR(glm::mat4, mat4)
DEFINE_REGISTER_SCALAR(int, int)
DEFINE_REGISTER_SCALAR(glm::tvec2<int>, ivec2)
DEFINE_REGISTER_SCALAR(glm::tvec3<int>, ivec3)
DEFINE_REGISTER_SCALAR(glm::tvec4<int>, ivec4)
DEFINE_REGISTER_SCALAR(unsigned int, uint)
DEFINE_REGISTER_SCALAR(glm::tvec2<unsigned int>, uivec2)
DEFINE_REGISTER_SCALAR(glm::tvec3<unsigned int>, uivec3)
DEFINE_REGISTER_SCALAR(glm::tvec4<unsigned int>, uivec4)

#define BIND_SLOTS(x) \
  for (auto slot: x ## _slots_) \
  { \
    slot.bind(resource_manager_); \
  }

void Material::bind_slots() const
{
  BIND_SLOTS(texture);
  BIND_SLOTS(float)
  BIND_SLOTS(vec2)
  BIND_SLOTS(vec3)
  BIND_SLOTS(vec4)
  BIND_SLOTS(mat2)
  BIND_SLOTS(mat2)
  BIND_SLOTS(mat4)
  BIND_SLOTS(int)
  BIND_SLOTS(ivec2)
  BIND_SLOTS(ivec3)
  BIND_SLOTS(ivec4)
  BIND_SLOTS(uint)
  BIND_SLOTS(uivec2)
  BIND_SLOTS(uivec3)
  BIND_SLOTS(uivec4)
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
