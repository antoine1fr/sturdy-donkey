#include "render/gl/Driver.hpp"

namespace render
{
namespace gl
{

const std::vector<Driver::RenderFunction> Driver::render_functions_({
  &Driver::bind_mesh_,
  &Driver::draw_elements_,
  &Driver::bind_uniform_float,
  &Driver::bind_uniform_int,
  &Driver::bind_uniform_vec2,
  &Driver::bind_uniform_vec3,
  &Driver::bind_uniform_vec4,
  &Driver::bind_uniform_mat2,
  &Driver::bind_uniform_mat3,
  &Driver::bind_uniform_mat4
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

void Driver::bind_uniform_vec2(const Command& command)
{
  assert(command.type == Command::Type::kBindUniformVec2);
  const BindUniformVec2Command& bind_command =
    static_cast<const BindUniformVec2Command&>(command);
  glUniform2fv(bind_command.location, 1, &(bind_command.uniform[0]));
}

void Driver::bind_uniform_vec3(const Command& command)
{
  assert(command.type == Command::Type::kBindUniformVec3);
  const BindUniformVec3Command& bind_command =
    static_cast<const BindUniformVec3Command&>(command);
  glUniform3fv(bind_command.location, 1, &(bind_command.uniform[0]));
}

void Driver::bind_uniform_vec4(const Command& command)
{
  assert(command.type == Command::Type::kBindUniformVec4);
  const BindUniformVec4Command& bind_command =
    static_cast<const BindUniformVec4Command&>(command);
  glUniform4fv(bind_command.location, 1, &(bind_command.uniform[0]));
}

void Driver::bind_uniform_mat2(const Command& command)
{
  assert(command.type == Command::Type::kBindUniformMat2);
  const BindUniformMat2Command& bind_command =
    static_cast<const BindUniformMat2Command&>(command);
  glUniformMatrix2fv(bind_command.location, 1, GL_FALSE, &(bind_command.uniform[0][0]));
}

void Driver::bind_uniform_mat3(const Command& command)
{
  assert(command.type == Command::Type::kBindUniformMat3);
  const BindUniformMat3Command& bind_command =
    static_cast<const BindUniformMat3Command&>(command);
  glUniformMatrix3fv(bind_command.location, 1, GL_FALSE, &(bind_command.uniform[0][0]));
}

void Driver::bind_uniform_mat4(const Command& command)
{
  assert(command.type == Command::Type::kBindUniformMat4);
  const BindUniformMat4Command& bind_command =
    static_cast<const BindUniformMat4Command&>(command);
  glUniformMatrix4fv(bind_command.location, 1, GL_FALSE, &(bind_command.uniform[0][0]));
}

void Driver::bind_uniform_float(const Command& command)
{
  assert(command.type == Command::Type::kBindUniformFloat);
  const BindUniformFloatCommand& bind_command =
    static_cast<const BindUniformFloatCommand&>(command);
  glUniform1f(bind_command.location, bind_command.uniform);
}

void Driver::bind_uniform_int(const Command& command)
{
  assert(command.type == Command::Type::kBindUniformInt);
  const BindUniformIntCommand& bind_command =
    static_cast<const BindUniformIntCommand&>(command);
  glUniform1i(bind_command.location, bind_command.uniform);
}

}
}
