#include "render/gl/Mesh.hpp"

namespace donkey
{
namespace render
{
namespace gl
{

Mesh::Mesh(
      GLuint position_buffer,
      GLuint uv_buffer,
      GLuint index_buffer,
      GLuint vertex_array):
  position_buffer(position_buffer),
  uv_buffer(uv_buffer),
  index_buffer(index_buffer),
  vertex_array(vertex_array)
{
}

}
}
}
