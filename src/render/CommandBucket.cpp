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
