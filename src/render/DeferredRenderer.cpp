#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <array>

#include "render/DeferredRenderer.hpp"
#include "render/Material.hpp"

namespace render
{

DeferredRenderer::~DeferredRenderer()
{
  resource_manager_.cleanup();
}

void DeferredRenderer::bind_mesh_uniforms_(const Material& material,
    const MeshNode& mesh_node) const
{
  // compute model matrix
  glm::mat4 model = glm::lookAt(
      glm::vec3(0.0f, 0.0f, 0.0f),
      glm::vec3(0.0f, 0.0f, -1.0f),
      glm::vec3(0.0f, 1.0f, 0.0f));
  glm::mat4 rotate_x = glm::rotate(glm::mat4(1.0f),
      glm::radians(mesh_node.angles.x),
      glm::vec3(1.0f, 0.0f, 0.0f));
  glm::mat4 rotate_y = glm::rotate(glm::mat4(1.0f),
      glm::radians(mesh_node.angles.y),
      glm::vec3(0.0f, 1.0f, 0.0f));
  glm::mat4 rotate_z = glm::rotate(glm::mat4(1.0f),
      glm::radians(mesh_node.angles.z),
      glm::vec3(0.0f, 0.0f, 1.0f));
  glm::mat4 translate = glm::translate(glm::mat4(1.0f), mesh_node.position);
  model = translate * rotate_z * rotate_y * rotate_x * model;
  material.bind_scalar(material.model_location, model);
}

void DeferredRenderer::bind_camera_uniforms_(
    const Material& material,
    const CameraNode& camera_node,
    const glm::vec3& camera_direction) const
{
  glm::mat4 projection_inverse = glm::inverse(camera_node.projection);

  material.bind_scalar(material.view_location, camera_node.view);
  material.bind_scalar(material.projection_location, camera_node.projection);
  material.bind_scalar(material.projection_params_location,
      glm::vec2(camera_node.near_plane, camera_node.far_plane));
  material.bind_scalar(material.projection_inverse_location,
      projection_inverse);
  material.bind_scalar(material.camera_direction_location, camera_direction);
}

void DeferredRenderer::bind_light_uniforms_(const Material& material,
    const glm::mat4& view) const
{
  // bind light infos
  glm::vec4 light_dir = view * glm::vec4(-1.0f, 1.0f, -1.0f, 10.0f);
  material.bind_scalar(material.light_dir_location, light_dir);
  material.bind_scalar(material.ambient_location,
      glm::vec4(0.2f, 0.2f, 0.2f, 1.0f));
  material.bind_scalar(material.light_diffuse_location,
      glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
  material.bind_scalar(material.light_specular_location,
      glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
}

void DeferredRenderer::render_mesh_node_(const RenderPass& render_pass,
    const MeshNode& mesh_node, const CameraNode& camera_node,
    const glm::vec3& camera_direction) const
{
  const Mesh& mesh = resource_manager_.get_mesh(mesh_node.mesh_id);
  const Material& material =
    resource_manager_.get_material(mesh_node.material_id);
  const GpuProgram& program =
    resource_manager_.get_gpu_program(material.program_id);
  glUseProgram(program.handle);

  // bind user-defined uniforms
  material.bind_slots();
  // bind built-in uniforms
  bind_mesh_uniforms_(material, mesh_node);
  bind_camera_uniforms_(material, camera_node, camera_direction);
  bind_light_uniforms_(material, camera_node.view);

  // bind geometry
  glBindVertexArray(mesh.vertex_array);
  // vertex positions
  glBindBuffer(GL_ARRAY_BUFFER, mesh.position_buffer);
  glVertexAttribPointer(program.position_location, 3, GL_FLOAT, GL_FALSE, 0,
      nullptr);
  glEnableVertexAttribArray(program.position_location);
  // vertex UVs
  glBindBuffer(GL_ARRAY_BUFFER, mesh.uv_buffer);
  glVertexAttribPointer(program.uv_location, 2, GL_FLOAT, GL_FALSE, 0,
      nullptr);
  glEnableVertexAttribArray(program.uv_location);
  // indices
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.index_buffer);

  glDrawElements(GL_TRIANGLES, mesh.index_count, mesh.index_type, nullptr);
}

void DeferredRenderer::execute_pass_(size_t pass_num,
    const RenderPass& render_pass, const FramePacket& frame_packet) const
{
  GLuint framebuffer =
    resource_manager_.get_framebuffer(render_pass.framebuffer_id);
  glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
  check_gl_framebuffer(GL_FRAMEBUFFER);

  if (render_pass.depth_test)
    glEnable(GL_DEPTH_TEST);
  else
    glDisable(GL_DEPTH_TEST);

  for (auto camera_node: frame_packet.get_camera_nodes())
  {
    if (camera_node.pass_num != pass_num)
      continue;
    glViewport(camera_node.viewport_position.x,
        camera_node.viewport_position.y,
        camera_node.viewport_size.x,
        camera_node.viewport_size.y);
    glClearColor(render_pass.clear_color.x, render_pass.clear_color.y,
        render_pass.clear_color.z, 1.0f);
    if (framebuffer == 0)
    {
      glDrawBuffer(GL_BACK);
      glClear(render_pass.clear_bits);
    }
    else
    {
      std::array<GLenum, 2> targets {{
        GL_COLOR_ATTACHMENT0,
        GL_COLOR_ATTACHMENT1
      }};
      glDrawBuffers(targets.size(), &targets[0]);
      glClear(render_pass.clear_bits);
    }
    for (auto mesh_node: frame_packet.get_mesh_nodes())
    {
      if (mesh_node.pass_num != pass_num)
        continue;
      glm::vec4 camera_direction =
        camera_node.view * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);
      render_mesh_node_(render_pass, mesh_node, camera_node,
          glm::vec3(camera_direction));
    }
  }
}

void DeferredRenderer::render(const FramePacket& frame_packet) const
{
  for (size_t i = 0; i < render_passes_.size(); ++i)
  {
    execute_pass_(i, render_passes_[i], frame_packet);
  }
}

ResourceManager& DeferredRenderer::get_resource_manager()
{
  return resource_manager_;
}

void DeferredRenderer::add_render_pass(const RenderPass& render_pass)
{
  render_passes_.push_back(render_pass);
}

void DeferredRenderer::add_render_pass(RenderPass&& render_pass)
{
  render_passes_.push_back(render_pass);
}

}
