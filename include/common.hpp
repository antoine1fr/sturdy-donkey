#pragma once

#include <GL/gl3w.h>
#include <glm/vec3.hpp>
#include <vector>
#include <iostream>

#define CHECK_GL_ERROR assert(glGetError() == GL_NO_ERROR)
#define CHECK_GL_FRAMEBUFFER_ERROR(x) \
  assert(glCheckFramebufferStatus(x) != GL_FRAMEBUFFER_COMPLETE)

#define REGISTER_FRAMEBUFFER_ERROR(x) \
  case x: \
    std::cerr << "Framebuffer error: " #x "\n"; \
    assert(false);

inline void check_gl_framebuffer(GLenum target)
{
  GLenum status = glCheckFramebufferStatus(target);
  switch (status)
  {
    case GL_FRAMEBUFFER_COMPLETE:
      break;
    REGISTER_FRAMEBUFFER_ERROR( GL_FRAMEBUFFER_UNDEFINED)
    REGISTER_FRAMEBUFFER_ERROR( GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT)
    REGISTER_FRAMEBUFFER_ERROR( GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT)
    REGISTER_FRAMEBUFFER_ERROR( GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER)
    REGISTER_FRAMEBUFFER_ERROR( GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER)
    REGISTER_FRAMEBUFFER_ERROR( GL_FRAMEBUFFER_UNSUPPORTED)
    REGISTER_FRAMEBUFFER_ERROR( GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE)
    default:
      assert(false);
  }
}

struct GpuProgram
{
  GLuint handle;
  GLuint model_location;
  GLuint view_location;
  GLuint projection_location;
  GLuint position_location;
  GLuint uv_location;
  GLuint diffuse_texture_location;
  GLuint normal_map_location;
};

struct RenderTarget
{
  GLenum format;
  GLsizei width;
  GLsizei height;
};

struct RenderPass
{
  uint32_t framebuffer_id;
  GLint clear_bits;
  glm::vec3 clear_color;
  bool depth_test;
};

struct Material
{
  uint32_t diffuse_texture_id;
  uint32_t normal_map_id;
  uint32_t program_id;
};

struct Mesh
{
  GLuint position_buffer;
  GLuint uv_buffer;
  GLuint index_buffer;
  size_t index_count;
  GLenum index_type;
  GLuint vertex_array;
};
