#include <array>
#include <iostream>

#include "render/gl/Driver.hpp"
#include "render/gl/Mesh.hpp"
#include "render/gl/Texture.hpp"

namespace donkey
{

namespace render
{
namespace gl
{

using namespace std::placeholders;

Driver::Driver():
  render_functions_({
      std::bind(&Driver::bind_mesh_, this, _1),
      std::bind(&Driver::draw_elements_, this, _1),
      std::bind(&Driver::bind_uniform_float_, this, _1),
      std::bind(&Driver::bind_uniform_int_, this, _1),
      std::bind(&Driver::bind_uniform_vec2_, this, _1),
      std::bind(&Driver::bind_uniform_vec3_, this, _1),
      std::bind(&Driver::bind_uniform_vec4_, this, _1),
      std::bind(&Driver::bind_uniform_mat2_, this, _1),
      std::bind(&Driver::bind_uniform_mat3_, this, _1),
      std::bind(&Driver::bind_uniform_mat4_, this, _1),
      std::bind(&Driver::bind_texture_, this, _1),
      std::bind(&Driver::bind_framebuffer_, this, _1),
      std::bind(&Driver::set_viewport_, this, _1),
      std::bind(&Driver::set_depth_test_, this, _1),
      std::bind(&Driver::clear_framebuffer_, this, _1),
      std::bind(&Driver::bind_gpu_program_, this, _1)
  })
{
  assert(gl3wInit() == 0);
  assert(gl3wIsSupported(4, 1) != 0);
  output_debug_info_();
}

void Driver::output_debug_info_() const
{
  const char* string = (const char*)glGetString(GL_VENDOR);
  std::cout << "GL renderer: " << string << '\n';
  std::cout << "GL vendor: " << glGetString(GL_VERSION) << '\n';
  std::cout << "GL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION)
    << '\n';
}

void Driver::execute_commands(const CommandBucket& commands)
{
  for (auto sorted_command: commands.get_commands())
  {
    size_t command_type = sorted_command.sort_key & kCommandTypeMask;
    RenderFunction f = render_functions_[command_type];
    (f)(sorted_command.command);
  }
}

void Driver::bind_mesh_(const Command& command)
{
  assert(command.type == Command::Type::kBindMesh);
  const BindMeshCommand& bind_command =
    static_cast<const BindMeshCommand&>(command);
  const Mesh& mesh =
    resource_manager_.get_mesh(bind_command.mesh_id);
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
  glDrawElements(GL_TRIANGLES, draw_command.count, GL_UNSIGNED_INT,
      nullptr);
}

void Driver::bind_uniform_vec2_(const Command& command)
{
  assert(command.type == Command::Type::kBindUniformVec2);
  const BindUniformVec2Command& bind_command =
    static_cast<const BindUniformVec2Command&>(command);
  glUniform2fv(bind_command.location, 1, &(bind_command.uniform[0]));
}

void Driver::bind_uniform_vec3_(const Command& command)
{
  assert(command.type == Command::Type::kBindUniformVec3);
  const BindUniformVec3Command& bind_command =
    static_cast<const BindUniformVec3Command&>(command);
  glUniform3fv(bind_command.location, 1, &(bind_command.uniform[0]));
}

void Driver::bind_uniform_vec4_(const Command& command)
{
  assert(command.type == Command::Type::kBindUniformVec4);
  const BindUniformVec4Command& bind_command =
    static_cast<const BindUniformVec4Command&>(command);
  glUniform4fv(bind_command.location, 1, &(bind_command.uniform[0]));
}

void Driver::bind_uniform_mat2_(const Command& command)
{
  assert(command.type == Command::Type::kBindUniformMat2);
  const BindUniformMat2Command& bind_command =
    static_cast<const BindUniformMat2Command&>(command);
  glUniformMatrix2fv(bind_command.location, 1, GL_FALSE, &(bind_command.uniform[0][0]));
}

void Driver::bind_uniform_mat3_(const Command& command)
{
  assert(command.type == Command::Type::kBindUniformMat3);
  const BindUniformMat3Command& bind_command =
    static_cast<const BindUniformMat3Command&>(command);
  glUniformMatrix3fv(bind_command.location, 1, GL_FALSE, &(bind_command.uniform[0][0]));
}

void Driver::bind_uniform_mat4_(const Command& command)
{
  assert(command.type == Command::Type::kBindUniformMat4);
  const BindUniformMat4Command& bind_command =
    static_cast<const BindUniformMat4Command&>(command);
  glUniformMatrix4fv(bind_command.location, 1, GL_FALSE, &(bind_command.uniform[0][0]));
}

void Driver::bind_uniform_float_(const Command& command)
{
  assert(command.type == Command::Type::kBindUniformFloat);
  const BindUniformFloatCommand& bind_command =
    static_cast<const BindUniformFloatCommand&>(command);
  glUniform1f(bind_command.location, bind_command.uniform);
}

void Driver::bind_uniform_int_(const Command& command)
{
  assert(command.type == Command::Type::kBindUniformInt);
  const BindUniformIntCommand& bind_command =
    static_cast<const BindUniformIntCommand&>(command);
  glUniform1i(bind_command.location, bind_command.uniform);
}

void Driver::bind_texture_(const Command& command)
{
  assert(command.type == Command::Type::kBindTexture);
  const BindTextureCommand& bind_command =
    static_cast<const BindTextureCommand&>(command);
  unsigned int texture_unit = bind_command.texture_unit;
  const Texture& texture =
    resource_manager_.get_texture(bind_command.texture_id);
  glUniform1i(bind_command.location, texture_unit);
  glActiveTexture(GL_TEXTURE0 + texture_unit);
  glBindTexture(GL_TEXTURE_2D, texture.texture);
}

void Driver::bind_framebuffer_(const Command& command)
{
  assert(command.type == Command::Type::kBindFramebuffer);
  const BindFramebufferCommand& bind_command =
    static_cast<const BindFramebufferCommand&>(command);
  GLuint framebuffer;
  uint32_t framebuffer_id = bind_command.framebuffer_id;
  if (framebuffer_id == std::numeric_limits<uint32_t>::max())
    framebuffer = 0;
  else
    framebuffer = resource_manager_.get_framebuffer(framebuffer_id);
  glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
  check_gl_framebuffer(GL_FRAMEBUFFER);
  if (framebuffer == 0)
  {
    glDrawBuffer(GL_BACK);
  }
  else
  {
    std::array<GLenum, 2> targets {{
      GL_COLOR_ATTACHMENT0,
        GL_COLOR_ATTACHMENT1
    }};
    glDrawBuffers(targets.size(), &targets[0]);
  }
}

void Driver::set_depth_test_(const Command& command)
{
  assert(command.type == Command::Type::kSetDepthTest);
  const SetDepthTestCommand& set_command =
    static_cast<const SetDepthTestCommand&>(command);
  if (set_command.enable == true)
    glEnable(GL_DEPTH_TEST);
  else
    glDisable(GL_DEPTH_TEST);
}

void Driver::set_viewport_(const Command& command)
{
  assert(command.type == Command::Type::kSetViewport);
  const SetViewportCommand& set_command =
    static_cast<const SetViewportCommand&>(command);
  glm::vec2 position = set_command.position;
  glm::vec2 size = set_command.size;
  glViewport(
    position.x,
    position.y,
    size.x,
    size.y);
}

void Driver::clear_framebuffer_(const Command& command)
{
  assert(command.type == Command::Type::kClearFramebuffer);
  const ClearFramebufferCommand& set_command =
    static_cast<const ClearFramebufferCommand&>(command);
  glClearColor(set_command.color.x, set_command.color.y,
      set_command.color.z, 1.0f);
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
}

void Driver::bind_gpu_program_(const Command& command)
{
  assert(command.type == Command::Type::kBindGpuProgram);
  const BindGpuProgramCommand& bind_command =
    static_cast<const BindGpuProgramCommand&>(command);
  const GpuProgram& program =
    resource_manager_.get_gpu_program(bind_command.program_id);
  glUseProgram(program.handle);
}

AResourceManager& Driver::get_resource_manager()
{
  return resource_manager_;
}

}
}
}
