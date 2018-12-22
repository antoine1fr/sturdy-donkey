#pragma once

#include "render/AMaterial.hpp"

namespace donkey {
namespace render {
namespace gl {

class Material: public AMaterial
{
  public:
    Material(
        const AResourceManager& resource_manager,
        uint32_t program_id);
    virtual ~Material()
    {
    }

    virtual void register_float_slot(
        const std::string& name,
        const float& storage);

    virtual void register_int_slot(
        const std::string& name,
        const int& storage);

    virtual void register_vector2_slot(
        const std::string& name,
        const glm::vec2& storage);

    virtual void register_vector3_slot(
        const std::string& name,
        const glm::vec3& storage);

    virtual void register_vector4_slot(
        const std::string& name,
        const glm::vec4& storage);

    virtual void register_matrix2_slot(
        const std::string& name,
        const glm::mat2& storage);

    virtual void register_matrix3_slot(
        const std::string& name,
        const glm::mat3& storage);

    virtual void register_matrix4_slot(
        const std::string& name,
        const glm::mat4& storage);

    virtual void register_texture_slot(
        const std::string& name,
        uint32_t texture_id,
        int texture_unit);
};

}
}
}
