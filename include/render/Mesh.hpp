#pragma once

namespace render
{

struct Mesh
{
  GLuint position_buffer;
  GLuint uv_buffer;
  GLuint index_buffer;
  size_t index_count;
  GLenum index_type;
  GLuint vertex_array;
};

}
