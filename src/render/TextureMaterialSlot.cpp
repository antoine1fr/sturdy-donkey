#include <GL/gl3w.h>
#include "render/ResourceManager.hpp"
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

void TextureMaterialSlot::bind(const ResourceManager& resource_manager,
    CommandBucket& render_commands)
{
  const Texture& texture = resource_manager.get_texture(texture_id);
  render_commands.bind_texture(location, texture_unit, texture);
}

}
}
