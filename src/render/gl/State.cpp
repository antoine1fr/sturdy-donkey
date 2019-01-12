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

#include <algorithm>
#include "render/gl/State.hpp"

namespace donkey {
namespace render {
namespace gl {

GLint State::cull_mode_map_[] = {
  GL_FRONT,
  GL_BACK
};

GLint State::blending_factor_map_[] = {
  GL_ZERO,
  GL_ONE,
  GL_SRC_COLOR,
  GL_ONE_MINUS_SRC_COLOR,
  GL_DST_COLOR,
  GL_ONE_MINUS_DST_COLOR,
  GL_SRC_ALPHA,
  GL_ONE_MINUS_SRC_ALPHA,
  GL_DST_ALPHA,
  GL_ONE_MINUS_DST_ALPHA,
  GL_CONSTANT_COLOR,
  GL_ONE_MINUS_CONSTANT_COLOR,
  GL_CONSTANT_ALPHA,
  GL_ONE_MINUS_CONSTANT_ALPHA,
  GL_SRC_ALPHA_SATURATE,
  GL_SRC1_COLOR,
  GL_ONE_MINUS_SRC1_COLOR,
  GL_SRC1_ALPHA,
  GL_ONE_MINUS_SRC1_ALPHA
};

GLint State::blending_function_map_[] = {
  GL_FUNC_ADD,
  GL_FUNC_SUBTRACT,
  GL_FUNC_REVERSE_SUBTRACT,
  GL_MIN,
  GL_MAX
};

State::State(GLint viewport[4], GLint scissor_box[4]):
  blend_equation_rgb(GL_FUNC_ADD),
  blend_equation_alpha(GL_FUNC_ADD),
  blend_source_alpha(GL_ONE),
  blend_source_rgb(GL_ONE),
  blend_destination_alpha(GL_ONE),
  blend_destination_rgb(GL_ONE),
  cull_mode(GL_BACK),
  depth_test_enabled(GL_TRUE),
  stencil_test_enabled(GL_FALSE),
  scissor_test_enabled(GL_FALSE),
  blending_enabled(GL_FALSE),
  face_culling_enabled(GL_FALSE)
{
  std::copy(&(viewport[0]), &(viewport[3]), &(this->viewport[0]));
  std::copy(&(scissor_box[0]), &(scissor_box[3]), &(this->scissor_box[0]));
}

State::State(const render::State& state):
  blend_equation_rgb(blending_function_map_[static_cast<size_t>(state.blend_equation_rgb)]),
  blend_equation_alpha(blending_function_map_[static_cast<size_t>(state.blend_equation_alpha)]),
  blend_source_alpha(blending_factor_map_[static_cast<size_t>(state.blend_source_alpha)]),
  blend_source_rgb(blending_factor_map_[static_cast<size_t>(state.blend_source_rgb)]),
  blend_destination_alpha(blending_factor_map_[static_cast<size_t>(state.blend_destination_alpha)]),
  blend_destination_rgb(blending_factor_map_[static_cast<size_t>(state.blend_destination_rgb)]),
  cull_mode(cull_mode_map_[static_cast<size_t>(state.cull_mode)]),
  depth_test_enabled(static_cast<GLboolean>(state.depth_test_enabled)),
  stencil_test_enabled(static_cast<GLboolean>(state.stencil_test_enabled)),
  scissor_test_enabled(static_cast<GLboolean>(state.scissor_test_enabled)),
  blending_enabled(static_cast<GLboolean>(state.blending_enabled)),
  face_culling_enabled(static_cast<GLboolean>(state.face_culling_enabled))
{
  std::copy(&(state.viewport[0]), &(state.viewport[3]), &(viewport[0]));
  std::copy(&(state.scissor_box[0]), &(state.scissor_box[3]),
      &(scissor_box[0]));
}

}
}
}
