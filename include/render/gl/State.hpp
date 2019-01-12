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
#include "render/State.hpp"

namespace donkey {
namespace render {
namespace gl {

struct State
{
  GLenum blend_equation_rgb;
  GLenum blend_equation_alpha;
  GLenum blend_source_alpha;
  GLenum blend_source_rgb;
  GLenum blend_destination_alpha;
  GLenum blend_destination_rgb;
  GLenum cull_mode;
  GLint viewport[4];
  GLint scissor_box[4];
  GLboolean depth_test_enabled;
  GLboolean stencil_test_enabled;
  GLboolean scissor_test_enabled;
  GLboolean blending_enabled;
  GLboolean face_culling_enabled;

  State(GLint viewport[4], GLint scissor_box[4]);
  State(const render::State& state);

  private:
  static GLint cull_mode_map_[2];
  static GLint blending_factor_map_[19];
  static GLint blending_function_map_[5];
};

}
}
}
