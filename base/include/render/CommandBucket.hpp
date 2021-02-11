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

#include <glm/mat2x2.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <list>

#include "render/Mesh.hpp"
#include "render/Texture.hpp"

namespace donkey {
namespace render {

class GpuResourceManager;

class Command {
 public:
  enum class Type {
    kBindMesh = 0,
    kDrawElements,
    kBindUniformFloat,
    kBindUniformInt,
    kBindUniformVec2,
    kBindUniformVec3,
    kBindUniformVec4,
    kBindUniformMat2,
    kBindUniformMat3,
    kBindUniformMat4,
    kBindTexture,
    kBindFramebuffer,
    kSetViewport,
    kSetDepthTest,
    kClearFramebuffer,
    kBindGpuProgram,
    kSetBlending,
    kSetState
  };

  Type type;

 protected:
  Command(Type type);
};

struct BindMeshCommand : Command {
  BindMeshCommand(uint32_t mesh_id, unsigned int position_location,
                  unsigned int normal_location, unsigned int uv_location,
                  unsigned int tangent_location,
                  unsigned int bitangent_location);
  uint32_t mesh_id;
  unsigned int position_location;
  unsigned int normal_location;
  unsigned int uv_location;
  unsigned int tangent_location;
  unsigned int bitangent_location;
};

struct BindUniformFloatCommand : Command {
  BindUniformFloatCommand(int location, float uniform);
  int location;
  float uniform;
};

struct BindUniformIntCommand : Command {
  BindUniformIntCommand(int location, int uniform);
  int location;
  int uniform;
};

struct BindUniformVec2Command : Command {
  BindUniformVec2Command(int location, const glm::vec2& uniform);
  int location;
  glm::vec2 uniform;
};

struct BindUniformVec3Command : Command {
  BindUniformVec3Command(int location, const glm::vec3& uniform);
  int location;
  glm::vec3 uniform;
};

struct BindUniformVec4Command : Command {
  BindUniformVec4Command(int location, const glm::vec4& uniform);
  int location;
  glm::vec4 uniform;
};

struct BindUniformMat2Command : Command {
  BindUniformMat2Command(int location, const glm::mat2& uniform);
  int location;
  glm::mat2 uniform;
};

struct BindUniformMat3Command : Command {
  BindUniformMat3Command(int location, const glm::mat3& uniform);
  int location;
  glm::mat3 uniform;
};

struct BindUniformMat4Command : Command {
  BindUniformMat4Command(int location, const glm::mat4& uniform);
  int location;
  glm::mat4 uniform;
};

struct BindTextureCommand : Command {
  BindTextureCommand(int location, unsigned texture_unit, uint32_t texture_id);
  int location;
  unsigned int texture_unit;
  uint32_t texture_id;
};

struct DrawElementsCommand : Command {
  DrawElementsCommand(size_t count);
  size_t count;
};

struct BindFramebufferCommand : Command {
  BindFramebufferCommand(uint32_t framebuffer_id);
  uint32_t framebuffer_id;
};

struct SetViewportCommand : Command {
  SetViewportCommand(const glm::tvec2<int>& position,
                     const glm::tvec2<std::size_t>& size);
  glm::tvec2<int> position;
  glm::tvec2<std::size_t> size;
};

struct SetDepthTestCommand : Command {
  SetDepthTestCommand(bool enable);
  bool enable;
};

struct SetBlendingCommand : Command {
  SetBlendingCommand(bool enable);
  bool enable;
};

struct ClearFramebufferCommand : Command {
  ClearFramebufferCommand(const glm::vec3& color);
  glm::vec3 color;
};

struct BindGpuProgramCommand : Command {
  BindGpuProgramCommand(uint32_t program_id);
  uint32_t program_id;
};

struct SetStateCommand : Command {
  SetStateCommand(uint32_t state_id);
  uint32_t state_id;
};

struct SortedCommand {
  uint64_t sort_key;
  Command& command;
};

class CommandBucket {
 private:
  std::list<SortedCommand> sorted_commands_;
  std::list<BindMeshCommand> bind_mesh_commands_;
  std::list<BindUniformFloatCommand> bind_float_commands_;
  std::list<BindUniformIntCommand> bind_int_commands_;
  std::list<BindUniformVec2Command> bind_vec2_commands_;
  std::list<BindUniformVec3Command> bind_vec3_commands_;
  std::list<BindUniformVec4Command> bind_vec4_commands_;
  std::list<BindUniformMat2Command> bind_mat2_commands_;
  std::list<BindUniformMat3Command> bind_mat3_commands_;
  std::list<BindUniformMat4Command> bind_mat4_commands_;
  std::list<BindTextureCommand> bind_texture_commands_;
  std::list<DrawElementsCommand> draw_elements_commands_;
  std::list<BindFramebufferCommand> bind_framebuffer_commands_;
  std::list<SetDepthTestCommand> set_depth_test_commands_;
  std::list<SetBlendingCommand> set_blending_commands_;
  std::list<SetViewportCommand> set_viewport_commands_;
  std::list<ClearFramebufferCommand> clear_framebuffer_commands_;
  std::list<BindGpuProgramCommand> bind_gpu_program_commands_;
  std::list<SetStateCommand> set_state_commands_;

 private:
  uint64_t make_sort_key_(Command::Type type);

 public:
  void bind_uniform(int location, int uniform);
  void bind_uniform(int location, float uniform);
  void bind_uniform(int location, const glm::vec2& uniform);
  void bind_uniform(int location, const glm::vec3& uniform);
  void bind_uniform(int location, const glm::vec4& uniform);
  void bind_uniform(int location, const glm::mat2& uniform);
  void bind_uniform(int location, const glm::mat3& uniform);
  void bind_uniform(int location, const glm::mat4& uniform);
  void bind_texture(int location, unsigned int texture_unit,
                    uint32_t texture_id);
  void bind_mesh(uint32_t mesh_id, unsigned int position_location,
                 unsigned int normal_location, unsigned int uv_location,
                 unsigned int tangent_location,
                 unsigned int bitangent_location);
  void bind_framebuffer(uint32_t framebuffer_id);
  void bind_gpu_program(uint32_t program_id);
  void draw_elements(size_t count);
  void set_depth_test(bool enable);
  void set_blending(bool enable);
  void set_viewport(const glm::tvec2<int>& position,
                    const glm::tvec2<std::size_t>& size);
  void clear_framebuffer(const glm::vec3& color);
  void set_state(uint32_t state_id);
  const std::list<SortedCommand>& get_commands() const;
};

}  // namespace render
}  // namespace donkey
