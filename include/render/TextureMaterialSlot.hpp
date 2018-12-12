#pragma once

#include <cstdint>

namespace donkey
{

namespace render
{

class ResourceManager;

class TextureMaterialSlot
{
  public:
    int location;
    uint32_t texture_id;
    int texture_unit;

  public:
    TextureMaterialSlot(int location, uint32_t texture_id, int texture_unit);
    void bind(const ResourceManager& resource_manager);
};

}
}
