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

namespace donkey {
namespace render {

struct Framebuffer {
  GLuint handle;
  std::vector<GLenum> descriptor;

  Framebuffer(GLuint handle, GLenum color_buffer_count) : handle(handle) {
    descriptor.resize(color_buffer_count);
    for (GLenum i = 0; i < color_buffer_count; i++) {
      descriptor[i] = GL_COLOR_ATTACHMENT0 + i;
    }
  }
};

}  // namespace render
}  // namespace donkey
