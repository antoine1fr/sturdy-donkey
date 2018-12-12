#pragma once

#include <GL/gl3w.h>
#include <glm/vec3.hpp>
#include <vector>
#include <iostream>
#include <chrono>

namespace donkey
{

typedef std::chrono::high_resolution_clock Clock;
typedef std::chrono::duration<float> Duration;

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

}
