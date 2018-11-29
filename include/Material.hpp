#pragma once

#include <GL/gl3w.h>
#include <vector>
#include <string>
#include "TextureMaterialSlot.hpp"


class ResourceManager;

class Material
{
  private:
    std::vector<TextureMaterialSlot> texture_slots_;
    const ResourceManager& resource_manager_;

  public:
    uint32_t program_id;

  public:
    Material(const ResourceManager& resource_manager, uint32_t program_id);
    void bind_slots() const;
    void register_texture_slot(const std::string& name,
        uint32_t texture_id,
        int texture_unit);
};
