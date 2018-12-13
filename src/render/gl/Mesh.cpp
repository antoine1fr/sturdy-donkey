#include "render/gl/Mesh.hpp"

namespace donkey
{
namespace render
{
namespace gl
{

Mesh::Mesh(
      size_t index_count,
      GLuint position_buffer,
      GLuint uv_buffer,
      GLuint index_buffer,
      GLuint vertex_array):
  ::donkey::render::Mesh(index_count),
  position_buffer(position_buffer),
  uv_buffer(uv_buffer),
  index_buffer(index_buffer),
  vertex_array(vertex_array)
{
}

}
}
}
