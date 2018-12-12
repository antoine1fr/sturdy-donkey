#include "render/gl/Driver.hpp"

namespace render
{
namespace gl
{

const std::vector<Driver::RenderFunction> Driver::render_functions_({
  &Driver::bind_mesh_,
  &Driver::draw_elements_
});

void Driver::execute_commands(const CommandBucket& commands)
{
  for (auto sorted_command: commands.get_commands())
  {
    size_t command_type = sorted_command.sort_key & kCommandTypeMask;
    RenderFunction f = render_functions_[command_type];
    (*f)(sorted_command.command);
  }
}

void Driver::bind_mesh_(const Command& command)
{
  assert(command.type == Command::Type::kBindMesh);
  const BindMeshCommand& bind_command =
    static_cast<const BindMeshCommand&>(command);
  const Mesh& mesh = bind_command.mesh;
  int position_location = bind_command.position_location;
  int uv_location = bind_command.uv_location;

  glBindVertexArray(mesh.vertex_array);
  // vertex positions
  glBindBuffer(GL_ARRAY_BUFFER, mesh.position_buffer);
  glVertexAttribPointer(position_location, 3, GL_FLOAT, GL_FALSE, 0,
      nullptr);
  glEnableVertexAttribArray(position_location);
  // vertex UVs
  glBindBuffer(GL_ARRAY_BUFFER, mesh.uv_buffer);
  glVertexAttribPointer(uv_location, 2, GL_FLOAT, GL_FALSE, 0,
      nullptr);
  glEnableVertexAttribArray(uv_location);
  // indices
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.index_buffer);
}

void Driver::draw_elements_(const Command& command)
{
  assert(command.type == Command::Type::kDrawElements);
  const DrawElementsCommand& draw_command =
    static_cast<const DrawElementsCommand&>(command);
  glDrawElements(GL_TRIANGLES, draw_command.count, draw_command.element_type,
      nullptr);
}

}
}
