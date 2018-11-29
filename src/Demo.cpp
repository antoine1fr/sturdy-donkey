#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <array>
#include "common.hpp"
#include "Demo.hpp"
#include "Material.hpp"

void Demo::initialize(int width, int height)
{
  glDisable(GL_STENCIL_TEST);
  glEnable(GL_DEPTH_TEST);

  // load textures
  uint32_t quad_diffuse =
    resource_manager_.load_texture_from_file("textures/quad-diffuse.png");
  uint32_t quad_normal =
    resource_manager_.load_texture_from_file("textures/quad-normal.png");

  // create a quad mesh
  std::vector<float> positions {
    -0.5f, -0.5f, 0.0f,
    -0.5f, 0.5f, 0.0f,
    0.5f, 0.5f, 0.0f,
    0.5f, -0.5f, 0.0f
  };
  std::vector<float> uvs {
    0.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 1.0f,
    1.0f, 0.0f
  };
  std::vector<unsigned int> indices {0, 1, 2, 0, 2, 3};
  uint32_t quad_id = resource_manager_.create_mesh(positions, uvs, indices);

  // create a quad covering the whole screen, for the second pass
  std::vector<float> screen_mesh_positions {
    0.0f, 0.0f, 0.0f,
    0.0f, static_cast<float>(height) / 2.0f, 0.0f,
    static_cast<float>(width) / 2.0f, static_cast<float>(height) / 2.0f, 0.0f,
    static_cast<float>(width) / 2.0f, 0.0f, 0.0f
  };
  std::vector<float> screen_mesh_uvs {
    0.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 1.0f,
    1.0f, 0.0f
  };
  std::vector<unsigned int> screen_mesh_indices {0, 1, 2, 0, 2, 3};
  uint32_t screen_mesh_id =
    resource_manager_.create_mesh(screen_mesh_positions, screen_mesh_uvs,
        indices);

  // load gpu programs
  uint32_t gbuffer_program_id = resource_manager_.load_gpu_program_from_file(
    "shaders/deferred-gbuffer-pass-vs.glsl",
    "shaders/deferred-gbuffer-pass-fs.glsl");
  uint32_t simple_program_id = resource_manager_.load_gpu_program_from_file(
    "shaders/simple-vs.glsl",
    "shaders/simple-fs.glsl");

  // create gbuffer
  uint32_t albedo_rt_id =
    resource_manager_.create_texture(width, height, GL_RGBA8, GL_RGBA,
        GL_UNSIGNED_BYTE);
  uint32_t normal_rt_id =
    resource_manager_.create_texture(width, height, GL_RGBA8, GL_RGBA,
        GL_UNSIGNED_BYTE);
  uint32_t depth_rt_id =
    resource_manager_.create_texture(width, height, GL_DEPTH_COMPONENT24,
        GL_DEPTH_COMPONENT, GL_FLOAT);
  gbuffer_id = resource_manager_.create_framebuffer(albedo_rt_id,
      normal_rt_id, depth_rt_id);

  // create materials
  uint32_t quad_material_id;
  {
    Material material(resource_manager_, gbuffer_program_id);
    material.register_texture_slot("diffuse_texture", quad_diffuse, 0);
    material.register_texture_slot("normal_map", quad_normal, 1);
    quad_material_id =
      resource_manager_.register_material(std::move(material));
  }

  uint32_t rt1_material_id;
  {
    Material material(resource_manager_, simple_program_id);
    material.register_texture_slot("diffuse_texture", albedo_rt_id, 0);
    rt1_material_id =
      resource_manager_.register_material(std::move(material));
  }

  uint32_t rt2_material_id;
  {
    Material material(resource_manager_, simple_program_id);
    material.register_texture_slot("diffuse_texture", normal_rt_id, 0);
    rt2_material_id =
      resource_manager_.register_material(std::move(material));
  }

  uint32_t rt3_material_id;
  {
    Material material(resource_manager_, simple_program_id);
    material.register_texture_slot("diffuse_texture", depth_rt_id, 0);
    rt3_material_id =
      resource_manager_.register_material(std::move(material));
  }

  // create first pass' scene nodes
  float ratio = static_cast<float>(width) / static_cast<float>(height);
  scene_.create_perspective_camera_node(0, 45.0f, ratio, 0.1f, 100.0f,
      glm::vec3(0.0f, 0.0f, 3.0f),
      glm::vec3(0.0f, 0.0f, 0.0f),
      glm::tvec2<int>(0, 0),
      glm::tvec2<GLsizei>(800, 720));
  scene_.create_mesh_node(0,
      glm::vec3(0.0f, 0.0f, 0.0f),
      glm::vec3(0.0f, 0.0f, 0.0f),
      quad_id, quad_material_id);

  // create second pass' scene nodes
  scene_.create_mesh_node(1,
      glm::vec3(0.0f, 0.0f, 0.0f),
      glm::vec3(0.0f, 0.0f, 0.0f),
      screen_mesh_id, rt1_material_id);

  scene_.create_mesh_node(1,
      glm::vec3(static_cast<float>(width) / 2.0f,
        0.0f,
        0.0f),
      glm::vec3(0.0f, 0.0f, 0.0f),
      screen_mesh_id, rt2_material_id);

  scene_.create_mesh_node(1,
      glm::vec3(0.0f, static_cast<float>(height) / 2.0f, 0.0f),
      glm::vec3(0.0f, 0.0f, 0.0f),
      screen_mesh_id, rt3_material_id);

  scene_.create_ortho_camera_node(1,
      glm::vec3(0.0f, 0.0f, 0.0f),
      glm::vec3(0.0f, 0.0f, 0.0f),
      glm::tvec2<int>(0, 0),
      glm::tvec2<GLsizei>(width, height));

  // register first render pass
  render_passes_.push_back({gbuffer_id,
      GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT,
      glm::vec3(0.0f, 0.0f, 0.0f),
      true});
  // register second render pass
  render_passes_.push_back({0,
      GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT,
      glm::vec3(0.0f, 0.0f, 0.0f),
      false});
}

void Demo::cleanup()
{
  resource_manager_.cleanup();
}

void Demo::render_mesh_node_(const RenderPass& render_pass,
    const MeshNode& mesh_node, const CameraNode& camera_node) const
{
  const Mesh& mesh = resource_manager_.get_mesh(mesh_node.mesh_id);
  const Material& material =
    resource_manager_.get_material(mesh_node.material_id);
  const GpuProgram& program =
    resource_manager_.get_gpu_program(material.program_id);
  glUseProgram(program.handle);

  material.bind_slots();

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

  // compute the projection's inverse
  glm::mat4 projection_inverse = glm::inverse(camera_node.projection);
  // upload matrices
  material.bind_scalar(material.projection_inverse_location,
      projection_inverse);
  material.bind_scalar(material.model_location, model);
  material.bind_scalar(material.view_location, camera_node.view);
  material.bind_scalar(material.projection_location, camera_node.projection);

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

void Demo::execute_pass_(size_t pass_num, const RenderPass& render_pass) const
{
  GLuint framebuffer =
    resource_manager_.get_framebuffer(render_pass.framebuffer_id);
  glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
  check_gl_framebuffer(GL_FRAMEBUFFER);

  if (render_pass.depth_test)
    glEnable(GL_DEPTH_TEST);
  else
    glDisable(GL_DEPTH_TEST);

  for (auto camera_node: scene_.get_camera_nodes())
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
    for (auto mesh_node: scene_.get_mesh_nodes())
    {
      if (mesh_node.pass_num != pass_num)
        continue;
      render_mesh_node_(render_pass, mesh_node, camera_node);
    }
  }
}

void Demo::render() const
{
  for (size_t i = 0; i < render_passes_.size(); ++i)
  {
    execute_pass_(i, render_passes_[i]);
  }
}
