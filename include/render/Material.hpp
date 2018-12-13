#pragma once

#include <GL/gl3w.h>
#include <glm/detail/type_vec.hpp>
#include <vector>
#include <string>
#include "TextureMaterialSlot.hpp"
#include "ScalarMaterialSlot.hpp"

#define DECLARE_REGISTER_SCALAR_SLOT(x) \
  void register_scalar_slot(const std::string& name, const x& storage);

namespace donkey
{

namespace render
{

class ResourceManager;

class Material
{
  private:
    std::vector<TextureMaterialSlot> texture_slots_;
    std::vector<ScalarMaterialSlot<float>> float_slots_;
    std::vector<ScalarMaterialSlot<glm::vec2>> vec2_slots_;
    std::vector<ScalarMaterialSlot<glm::vec3>> vec3_slots_;
    std::vector<ScalarMaterialSlot<glm::vec4>> vec4_slots_;
    std::vector<ScalarMaterialSlot<glm::mat2>> mat2_slots_;
    std::vector<ScalarMaterialSlot<glm::mat3>> mat3_slots_;
    std::vector<ScalarMaterialSlot<glm::mat4>> mat4_slots_;
    std::vector<ScalarMaterialSlot<int>> int_slots_;
    const ResourceManager& resource_manager_;

  public:
    uint32_t program_id;
    int model_location;
    int view_location;
    int projection_location;
    int projection_inverse_location;
    int projection_params_location;
    int ambient_location;
    int light_dir_location;
    int light_diffuse_location;
    int light_specular_location;
    int camera_position_location;

  public:
    Material(const ResourceManager& resource_manager, uint32_t program_id);

    void bind_slots() const;

    DECLARE_REGISTER_SCALAR_SLOT(float)
    DECLARE_REGISTER_SCALAR_SLOT(glm::vec2)
    DECLARE_REGISTER_SCALAR_SLOT(glm::vec3)
    DECLARE_REGISTER_SCALAR_SLOT(glm::vec4)
    DECLARE_REGISTER_SCALAR_SLOT(glm::mat2)
    DECLARE_REGISTER_SCALAR_SLOT(glm::mat3)
    DECLARE_REGISTER_SCALAR_SLOT(glm::mat4)
    DECLARE_REGISTER_SCALAR_SLOT(int)

    void register_texture_slot(const std::string& name,
        uint32_t texture_id,
        int texture_unit);

    template <class Storage>
      void bind_scalar(int location, const Storage& storage) const;
};

}
}

#include "Material.inl"
