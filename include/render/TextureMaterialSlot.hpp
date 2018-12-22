#pragma once

#include <cstdint>
#include "render/CommandBucket.hpp"

namespace donkey
{

namespace render
{

class TextureMaterialSlot
{
  public:
    int location;
    uint32_t texture_id;
    int texture_unit;

  public:
    TextureMaterialSlot(int location, uint32_t texture_id, int texture_unit);
    void bind(CommandBucket& render_commands);
};

}
}
