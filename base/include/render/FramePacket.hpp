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

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <vector>

#include "Scene.hpp"
#include "StackVector.hpp"
#include "common.hpp"

namespace donkey {
namespace render {
struct FramePacket {
  template <typename T>
  using Vector = StackVector<T, 4>;

  enum ClearFlag {
    kColorBuffer = 0,
    kDepthBuffer = 1
  };

  struct UniformBlock {
    uint32_t block_id;
    uint32_t buffer_id;
    Vector<uint8_t> data;
  };

  struct VertexAttribute {
    enum class Type {
      kFloat
    };

    unsigned int location;
    int size;
    Type type;
  };

  struct RenderObject {
    uint32_t mesh_id;
    Vector<float> vertices;
    Vector<uint32_t> indices;
    Vector<VertexAttribute> vertex_attributes;
    Vector<uint32_t> texture_ids;
    UniformBlock uniform_block;
  };

  struct Pass {
    uint32_t gpu_program_id;
    uint32_t pass_uniform_buffer_id;
    uint32_t object_uniform_buffer_id;
    uint32_t framebuffer_id;
    uint32_t clear_bits;
    glm::vec3 clear_color;
    UniformBlock uniform_block;
    Vector<RenderObject> render_objects;
    bool depth_test;
    bool blending;
  };

  Vector<Pass> passes;
};
}  // namespace render
}  // namespace donkey
