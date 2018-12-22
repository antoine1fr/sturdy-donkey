#include <GL/gl3w.h>
#include "render/TextureMaterialSlot.hpp"

namespace donkey
{

namespace render
{

TextureMaterialSlot::TextureMaterialSlot(int location, uint32_t texture_id,
  int texture_unit):
  location(location),
  texture_id(texture_id),
  texture_unit(texture_unit)
{
}

void TextureMaterialSlot::bind(CommandBucket& render_commands)
{
  render_commands.bind_texture(location, texture_unit, texture_id);
}

}
}
