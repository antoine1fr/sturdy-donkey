#include "render/CommandBucket.hpp"

namespace donkey
{

namespace render
{

Command::Command(Type type):
  type(type)
{
}

BindMeshCommand::BindMeshCommand(const Mesh& mesh, int position_location,
    int uv_location):
  Command(Type::kBindMesh),
  mesh(mesh),
  position_location(position_location),
  uv_location(uv_location)
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
    GLint texture):
  Command(Type::kBindTexture),
  location(location),
  texture_unit(texture_unit),
  texture(texture)
{
}

DrawElementsCommand::DrawElementsCommand(size_t count):
  Command(Type::kDrawElements),
  count(count)
{
}

void CommandBucket::bind_mesh(
    const Mesh& mesh,
    int position_location,
    int uv_location)
{
  bind_mesh_commands_.push_back(BindMeshCommand(
    mesh,
    position_location,
    uv_location
  ));
  sorted_commands_.push_back({0, bind_mesh_commands_.front()});
}

void CommandBucket::draw_elements(size_t count)
{
  draw_elements_commands_.push_back(DrawElementsCommand(count));
  sorted_commands_.push_back({1, draw_elements_commands_.front()});
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
    GLint texture)
{
  bind_texture_commands_.push_back(BindTextureCommand(location, texture_unit,
        texture));
  sorted_commands_.push_back({
    make_sort_key_(Command::Type::kBindTexture),
    bind_texture_commands_.back()
  });
}

}
}
