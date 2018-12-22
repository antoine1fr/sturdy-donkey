#pragma once

#include <GL/gl3w.h>

namespace donkey
{
namespace render
{
namespace gl
{

struct Mesh
{
  GLuint position_buffer;
  GLuint uv_buffer;
  GLuint index_buffer;
  GLuint vertex_array;

  Mesh(
      GLuint position_buffer,
      GLuint uv_buffer,
      GLenum index_type,
      GLuint vertex_array);
};

}
}
}
