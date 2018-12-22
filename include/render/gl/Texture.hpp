#pragma once

#include <GL/gl3w.h>
#include "render/Texture.hpp"

namespace donkey {
namespace render {
namespace gl {

struct Texture
{
  GLuint texture;

  Texture(GLuint texture);
};

}
}
}
