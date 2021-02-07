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

#include <algorithm>
#include "render/Resource.hpp"

namespace donkey {
namespace render {

enum class CullMode
{
  kFront,
  kBack
};

enum class BlendingFunction
{
  kAdd,
  kSubtract,
  kReverseSubtract,
  kMin,
  kMax
};

enum class BlendingFactor
{
  kZero,
  kOne,
  kSourceColor,
  kOneMinusSourceColor,
  kDestinationColor,
  kOneMinusDestinationColor,
  kSourceAlpha,
  kOneMinusSourceAlpha,
  kDestinationAlpha,
  kOneMinusDestinationAlpha,
  kConstantColor,
  kOneMinusConstantColor,
  kConstantAlpha,
  kOneMinusConstantAlpha,
  kSourceAlphaSaturate,
  kSource1Color,
  kOneMinusSource1Color,
  kSource1Alpha,
  kOneMinusSource1Alpha
};

struct State: public Resource
{

  BlendingFunction blend_equation_rgb;
  BlendingFunction blend_equation_alpha;
  BlendingFactor blend_source_alpha;
  BlendingFactor blend_source_rgb;
  BlendingFactor blend_destination_alpha;
  BlendingFactor blend_destination_rgb;
  CullMode cull_mode;
  int viewport[4];
  int scissor_box[4];
  bool depth_test_enabled;
  bool stencil_test_enabled;
  bool scissor_test_enabled;
  bool blending_enabled;
  bool face_culling_enabled;

  State(uint32_t gpu_resource_id):
    Resource(gpu_resource_id),
    blend_equation_rgb(BlendingFunction::kAdd),
    blend_equation_alpha(BlendingFunction::kAdd),
    blend_source_alpha(BlendingFactor::kOne),
    blend_source_rgb(BlendingFactor::kOne),
    blend_destination_alpha(BlendingFactor::kOne),
    blend_destination_rgb(BlendingFactor::kOne),
    cull_mode(CullMode::kBack),
    viewport{0, 0, 0, 0},
    scissor_box{0, 0, 0, 0},
    depth_test_enabled(true),
    stencil_test_enabled(false),
    scissor_test_enabled(false),
    blending_enabled(false),
    face_culling_enabled(false)
  {
  }

  State(uint32_t gpu_resource_id, const State& state):
    Resource(gpu_resource_id),
    blend_equation_rgb(state.blend_equation_rgb),
    blend_equation_alpha(state.blend_equation_alpha),
    blend_source_alpha(state.blend_source_alpha),
    blend_source_rgb(state.blend_source_rgb),
    blend_destination_alpha(state.blend_destination_alpha),
    blend_destination_rgb(state.blend_destination_rgb),
    cull_mode(state.cull_mode),
    viewport{ 0, 0, 0, 0 },
    scissor_box{ 0, 0, 0, 0 },
    depth_test_enabled(state.depth_test_enabled),
    stencil_test_enabled(state.stencil_test_enabled),
    scissor_test_enabled(state.scissor_test_enabled),
    blending_enabled(state.blending_enabled),
    face_culling_enabled(state.face_culling_enabled)
  {
    std::copy(&(state.viewport[0]), &(state.viewport[3]), &(viewport[0]));
    std::copy(&(state.scissor_box[0]), &(state.scissor_box[3]),
        &(scissor_box[0]));
  }
};

}
}
