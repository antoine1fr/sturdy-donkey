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

#include "render/CommandBucket.hpp"

namespace donkey
{

namespace render
{

Command::Command(Type type):
  type(type)
{
}

BindMeshCommand::BindMeshCommand(
    uint32_t mesh_id,
    int position_location,
    int normal_location,
    int uv_location,
    int tangent_location,
    int bitangent_location):
  Command(Type::kBindMesh),
  mesh_id(mesh_id),
  position_location(position_location),
  normal_location(normal_location),
  uv_location(uv_location),
  tangent_location(tangent_location),
  bitangent_location(bitangent_location)
{
}

BindUniformFloatCommand::BindUniformFloatCommand(int location, float uniform):
  Command(Type::kBindUniformFloat),
  location(location),
  uniform(uniform)
{
}

BindUniformIntCommand::BindUniformIntCommand(int location, int uniform):
  Command(Type::kBindUniformInt),
  location(location),
  uniform(uniform)
{
}

BindUniformVec2Command::BindUniformVec2Command(int location,
    const glm::vec2& uniform):
  Command(Type::kBindUniformVec2),
  location(location),
  uniform(uniform)
{
}

BindUniformVec3Command::BindUniformVec3Command(int location,
    const glm::vec3& uniform):
  Command(Type::kBindUniformVec3),
  location(location),
  uniform(uniform)
{
}

BindUniformVec4Command::BindUniformVec4Command(int location,
    const glm::vec4& uniform):
  Command(Type::kBindUniformVec4),
  location(location),
  uniform(uniform)
{
}

BindUniformMat2Command::BindUniformMat2Command(int location,
    const glm::mat2& uniform):
  Command(Type::kBindUniformMat2),
  location(location),
  uniform(uniform)
{
}

BindUniformMat3Command::BindUniformMat3Command(int location,
    const glm::mat3& uniform):
  Command(Type::kBindUniformMat3),
  location(location),
  uniform(uniform)
{
}

BindUniformMat4Command::BindUniformMat4Command(int location,
    const glm::mat4& uniform):
  Command(Type::kBindUniformMat4),
  location(location),
  uniform(uniform)
{
}

BindTextureCommand::BindTextureCommand(
    int location,
    unsigned int texture_unit,
    uint32_t texture_id):
  Command(Type::kBindTexture),
  location(location),
  texture_unit(texture_unit),
  texture_id(texture_id)
{
}

DrawElementsCommand::DrawElementsCommand(size_t count):
  Command(Type::kDrawElements),
  count(count)
{
}

BindFramebufferCommand::BindFramebufferCommand(
    uint32_t framebuffer_id):
  Command(Type::kBindFramebuffer),
  framebuffer_id(framebuffer_id)
{
}

SetViewportCommand::SetViewportCommand(const glm::tvec2<int>& position,
    const glm::tvec2<std::size_t>& size):
  Command(Type::kSetViewport),
  position(position),
  size(size)
{
}

SetDepthTestCommand::SetDepthTestCommand(bool enable):
  Command(Type::kSetDepthTest),
  enable(enable)
{
}

SetBlendingCommand::SetBlendingCommand(bool enable):
  Command(Type::kSetBlending),
  enable(enable)
{
}

ClearFramebufferCommand::ClearFramebufferCommand(const glm::vec3& color):
  Command(Type::kClearFramebuffer),
  color(color)
{
}

BindGpuProgramCommand::BindGpuProgramCommand(
    uint32_t program_id):
  Command(Type::kBindGpuProgram),
  program_id(program_id)
{
}

SetStateCommand::SetStateCommand(uint32_t state_id):
  Command(Type::kSetState),
  state_id(state_id)
{
}

void CommandBucket::bind_mesh(
    uint32_t mesh_id,
    int position_location,
    int normal_location,
    int uv_location,
    int tangent_location,
    int bitangent_location)
{
  bind_mesh_commands_.push_back(BindMeshCommand(
    mesh_id,
    position_location,
    normal_location,
    uv_location,
    tangent_location,
    bitangent_location
  ));
  sorted_commands_.push_back({0, bind_mesh_commands_.back()});
}

void CommandBucket::draw_elements(size_t count)
{
  draw_elements_commands_.push_back(DrawElementsCommand(count));
  sorted_commands_.push_back({1, draw_elements_commands_.back()});
}

const std::list<SortedCommand>& CommandBucket::get_commands() const
{
  return sorted_commands_;
}

void CommandBucket::bind_uniform(int location, float uniform)
{
  bind_float_commands_.push_back(BindUniformFloatCommand(location, uniform));
  sorted_commands_.push_back({
    make_sort_key_(Command::Type::kBindUniformFloat),
    bind_float_commands_.back()
  });
}

void CommandBucket::bind_uniform(int location, int uniform)
{
  bind_int_commands_.push_back(BindUniformIntCommand(location, uniform));
  sorted_commands_.push_back({
    make_sort_key_(Command::Type::kBindUniformInt),
    bind_int_commands_.back()
  });
}

void CommandBucket::bind_uniform(int location, const glm::vec2& uniform)
{
  bind_vec2_commands_.push_back(BindUniformVec2Command(location, uniform));
  sorted_commands_.push_back({
    make_sort_key_(Command::Type::kBindUniformVec2),
    bind_vec2_commands_.back()
  });
}

void CommandBucket::bind_uniform(int location, const glm::vec3& uniform)
{
  bind_vec3_commands_.push_back(BindUniformVec3Command(location, uniform));
  sorted_commands_.push_back({
    make_sort_key_(Command::Type::kBindUniformVec3),
    bind_vec3_commands_.back()
  });
}

void CommandBucket::bind_uniform(int location, const glm::vec4& uniform)
{
  bind_vec4_commands_.push_back(BindUniformVec4Command(location, uniform));
  sorted_commands_.push_back({
    make_sort_key_(Command::Type::kBindUniformVec4),
    bind_vec4_commands_.back()
  });
}

void CommandBucket::bind_uniform(int location, const glm::mat2& uniform)
{
  bind_mat2_commands_.push_back(BindUniformMat2Command(location, uniform));
  sorted_commands_.push_back({
    make_sort_key_(Command::Type::kBindUniformMat2),
    bind_mat4_commands_.back()
  });
}

void CommandBucket::bind_uniform(int location, const glm::mat3& uniform)
{
  bind_mat3_commands_.push_back(BindUniformMat3Command(location, uniform));
  sorted_commands_.push_back({
    make_sort_key_(Command::Type::kBindUniformMat3),
    bind_mat4_commands_.back()
  });
}

void CommandBucket::bind_uniform(int location, const glm::mat4& uniform)
{
  bind_mat4_commands_.push_back(BindUniformMat4Command(location, uniform));
  sorted_commands_.push_back({
    make_sort_key_(Command::Type::kBindUniformMat4),
    bind_mat4_commands_.back()
  });
}

uint64_t CommandBucket::make_sort_key_(Command::Type type)
{
  return static_cast<uint64_t>(type);
}

void CommandBucket::bind_texture(
    int location,
    unsigned int texture_unit,
    uint32_t texture_id)
{
  bind_texture_commands_.push_back(BindTextureCommand(
    location,
    texture_unit,
    texture_id));
  sorted_commands_.push_back({
    make_sort_key_(Command::Type::kBindTexture),
    bind_texture_commands_.back()
  });
}

void CommandBucket::bind_framebuffer(
    uint32_t framebuffer_id)
{
  bind_framebuffer_commands_.push_back(BindFramebufferCommand(
    framebuffer_id));
  sorted_commands_.push_back({
    make_sort_key_(Command::Type::kBindFramebuffer),
    bind_framebuffer_commands_.back()
  });
}

void CommandBucket::set_depth_test(bool enable)
{
  set_depth_test_commands_.push_back(SetDepthTestCommand(enable));
  sorted_commands_.push_back({
    make_sort_key_(Command::Type::kSetDepthTest),
    set_depth_test_commands_.back()
  });
}

void CommandBucket::set_blending(bool enable)
{
  set_blending_commands_.push_back(SetBlendingCommand(enable));
  sorted_commands_.push_back({
    make_sort_key_(Command::Type::kSetBlending),
    set_blending_commands_.back()
  });
}

void CommandBucket::set_viewport(const glm::tvec2<int>& position, 
    const glm::tvec2<std::size_t>& size)
{
  set_viewport_commands_.push_back(SetViewportCommand(position, size));
  sorted_commands_.push_back({
    make_sort_key_(Command::Type::kSetViewport),
    set_viewport_commands_.back()
  });
}

void CommandBucket::clear_framebuffer(const glm::vec3& color)
{
  clear_framebuffer_commands_.push_back(ClearFramebufferCommand(color));
  sorted_commands_.push_back({
    make_sort_key_(Command::Type::kClearFramebuffer),
    clear_framebuffer_commands_.back()
  });
}

void CommandBucket::bind_gpu_program(
    uint32_t program_id)
{
  bind_gpu_program_commands_.push_back(BindGpuProgramCommand(
    program_id));
  sorted_commands_.push_back({
    make_sort_key_(Command::Type::kBindGpuProgram),
    bind_gpu_program_commands_.back()
  });
}

void CommandBucket::set_state(uint32_t state_id)
{
  set_state_commands_.push_back(SetStateCommand(state_id));
  sorted_commands_.push_back({
    make_sort_key_(Command::Type::kSetState),
    set_state_commands_.back()
  });
}

}
}
