#include <GL/gl3w.h>
#include "ResourceManager.hpp"
#include "TextureMaterialSlot.hpp"

TextureMaterialSlot::TextureMaterialSlot(int location, uint32_t texture_id,
  int texture_unit):
  location(location),
  texture_id(texture_id),
  texture_unit(texture_unit)
{
}

void TextureMaterialSlot::bind(const ResourceManager& resource_manager)
{
  GLuint texture = resource_manager.get_texture(texture_id);
  glUniform1i(location, texture_unit);
  glActiveTexture(GL_TEXTURE0 + texture_unit);
  glBindTexture(GL_TEXTURE_2D, texture);
}
