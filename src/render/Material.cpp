#include "render/Material.hpp"
#include "render/ResourceManager.hpp"

// TODO: find a pure c++ (i.e. no preprocessor) implementation that is as
// concise and doesn't use RTTIs.

#define DEFINE_REGISTER_SCALAR_SLOT(x,y) \
  void Material::register_scalar_slot(const std::string& name, const x& storage) \
  { \
    const GpuProgram& program = resource_manager_.get_gpu_program(program_id); \
    int location = glGetUniformLocation(program.handle, name.c_str()); \
    y ## _slots_.push_back(ScalarMaterialSlot<x>(location, storage)); \
  }

namespace donkey
{

namespace render
{

Material::Material(const ResourceManager& resource_manager,
    uint32_t program_id):
  resource_manager_(resource_manager),
  program_id(program_id)
{
  const GpuProgram& program = resource_manager_.get_gpu_program(program_id);
  model_location = glGetUniformLocation(program.handle, "model");
  view_location = glGetUniformLocation(program.handle, "view");
  projection_location = glGetUniformLocation(program.handle, "projection");
  projection_inverse_location = glGetUniformLocation(program.handle,
    "projection_inverse");
  projection_params_location = glGetUniformLocation(program.handle,
    "projection_params");
  light_dir_location = glGetUniformLocation(program.handle,
    "light_dir");
  light_diffuse_location = glGetUniformLocation(program.handle,
    "light_diffuse");
  light_specular_location = glGetUniformLocation(program.handle,
    "light_specular");
  ambient_location = glGetUniformLocation(program.handle,
    "ambient");
  camera_position_location = glGetUniformLocation(program.handle,
    "camera_position");
}

DEFINE_REGISTER_SCALAR_SLOT(float, float)
DEFINE_REGISTER_SCALAR_SLOT(glm::vec2, vec2)
DEFINE_REGISTER_SCALAR_SLOT(glm::vec3, vec3)
DEFINE_REGISTER_SCALAR_SLOT(glm::vec4, vec4)
DEFINE_REGISTER_SCALAR_SLOT(glm::mat2, mat2)
DEFINE_REGISTER_SCALAR_SLOT(glm::mat3, mat3)
DEFINE_REGISTER_SCALAR_SLOT(glm::mat4, mat4)
DEFINE_REGISTER_SCALAR_SLOT(int, int)

#define BIND_SLOTS(x) \
  for (auto slot: x ## _slots_) \
  { \
    slot.bind(resource_manager_, render_commands); \
  }

void Material::bind_slots(CommandBucket& render_commands) const
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

}
}
