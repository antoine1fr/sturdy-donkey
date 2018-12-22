/* Copyright (C) 2018 Antoine Luciani
 *
 * This file is part of Sturdy Donkey.
 *
 * Sturdy Donkey is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, version 3.
 *
 * Sturdy Donkey is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * Sturdy Donkey. If not, see <https://www.gnu.org/licenses/>.
 */

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
