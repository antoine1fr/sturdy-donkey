#include "render/CommandBucket.hpp"

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

DrawElementsCommand::DrawElementsCommand(size_t count, GLenum element_type):
  Command(Type::kDrawElements),
  count(count),
  element_type(element_type)
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

void CommandBucket::draw_elements(size_t count, GLenum element_type)
{
  draw_elements_commands_.push_back(DrawElementsCommand(count, element_type));
  sorted_commands_.push_back({1, draw_elements_commands_.front()});
}

const std::list<SortedCommand>& CommandBucket::get_commands() const
{
  return sorted_commands_;
}

}
