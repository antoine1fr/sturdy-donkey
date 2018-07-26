#pragma once

#include <GL/gl3w.h>
#include <vector>

struct MaterialSlot
{
  GLint location;
  uint32_t texture_id;
  int texture_unit;
};

class ResourceManager;

class Material
{
  private:
    std::vector<MaterialSlot> slots_;
    const ResourceManager& resource_manager_;

  public:
    uint32_t program_id;

  public:
    Material(const ResourceManager& resource_manager, uint32_t program_id);
    void bind_slots() const;
    void register_slot(const std::string& name,
        uint32_t texture_id,
        int texture_unit);
};
