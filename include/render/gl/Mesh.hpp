#pragma once

#include <GL/gl3w.h>
#include "render/Mesh.hpp"

namespace donkey
{
namespace render
{
namespace gl
{

struct Mesh: ::donkey::render::Mesh
{
  GLuint position_buffer;
  GLuint uv_buffer;
  GLuint index_buffer;
  GLuint vertex_array;

  Mesh(
      size_t index_count,
      GLuint position_buffer,
      GLuint uv_buffer,
      GLenum index_type,
      GLuint vertex_array);
};

}
}
}
