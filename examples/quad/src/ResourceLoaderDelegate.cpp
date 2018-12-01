#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <render/ResourceManager.hpp>
#include <render/Material.hpp>

#include "ResourceLoaderDelegate.hpp"

void ResourceLoaderDelegate::load(render::Window& window, Scene& scene,
    render::DeferredRenderer& renderer)
{
  render::ResourceManager& resource_manager = renderer.get_resource_manager();
  int width = window.get_width();
  int height = window.get_height();

  glDisable(GL_STENCIL_TEST);
  glEnable(GL_DEPTH_TEST);

  // load textures
  uint32_t quad_diffuse =
    resource_manager.load_texture_from_file("../../textures/quad-diffuse.png");
  uint32_t quad_normal =
    resource_manager.load_texture_from_file("../../textures/quad-normal.png");

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
  uint32_t quad_id = resource_manager.create_mesh(positions, uvs, indices);

  // create a quad covering the whole screen, for the second pass
  std::vector<float> screen_mesh_positions {
    0.0f, 0.0f, 0.0f,
    0.0f, static_cast<float>(height), 0.0f,
    static_cast<float>(width), static_cast<float>(height), 0.0f,
    static_cast<float>(width), 0.0f, 0.0f
  };
  std::vector<float> screen_mesh_uvs {
    0.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 1.0f,
    1.0f, 0.0f
  };
  std::vector<unsigned int> screen_mesh_indices {0, 1, 2, 0, 2, 3};
  uint32_t screen_mesh_id =
    resource_manager.create_mesh(screen_mesh_positions, screen_mesh_uvs,
        indices);

  // load gpu programs
  uint32_t gbuffer_program_id = resource_manager.load_gpu_program_from_file(
    "../../shaders/deferred-gbuffer-pass-vs.glsl",
    "../../shaders/deferred-gbuffer-pass-fs.glsl");
  uint32_t light_program_id = resource_manager.load_gpu_program_from_file(
    "../../shaders/simple-vs.glsl",
    "../../shaders/deferred-light-pass-fs.glsl");

  // create gbuffer
  uint32_t albedo_rt_id =
    resource_manager.create_texture(width, height, GL_RGBA8, GL_RGBA,
        GL_UNSIGNED_BYTE);
  uint32_t normal_rt_id =
    resource_manager.create_texture(width, height, GL_RGBA8, GL_RGBA,
        GL_UNSIGNED_BYTE);
  uint32_t depth_rt_id =
    resource_manager.create_texture(width, height, GL_DEPTH_COMPONENT24,
        GL_DEPTH_COMPONENT, GL_FLOAT);

  // create materials
  uint32_t quad_material_id;
  {
    render::Material material(resource_manager, gbuffer_program_id);
    material.register_texture_slot("diffuse_texture", quad_diffuse, 0);
    material.register_texture_slot("normal_map", quad_normal, 1);
    quad_material_id =
      resource_manager.register_material(std::move(material));
  }

  uint32_t rt1_material_id;
  {
    render::Material material(resource_manager, light_program_id);
    material.register_texture_slot("albedo_tex", albedo_rt_id, 0);
    material.register_texture_slot("normals_tex", normal_rt_id, 1);
    material.register_texture_slot("depth_tex", depth_rt_id, 2);
    rt1_material_id =
      resource_manager.register_material(std::move(material));
  }

  // create first pass' scene nodes
  float ratio = static_cast<float>(width) / static_cast<float>(height);
  scene.create_perspective_camera_node(0, 45.0f, ratio, 0.1f, 100.0f,
      glm::vec3(0.0f, 0.0f, 3.0f),
      glm::vec3(0.0f, 0.0f, 0.0f),
      glm::tvec2<int>(0, 0),
      glm::tvec2<GLsizei>(width, height));
  scene.create_mesh_node(0,
      glm::vec3(0.0f, 0.0f, 0.0f),
      glm::vec3(0.0f, 0.0f, 0.0f),
      quad_id, quad_material_id);

  // create second pass' scene nodes
  scene.create_mesh_node(1,
      glm::vec3(0.0f, 0.0f, 0.0f),
      glm::vec3(0.0f, 0.0f, 0.0f),
      screen_mesh_id, rt1_material_id);

  scene.create_ortho_camera_node(1,
      glm::vec3(0.0f, 0.0f, 0.0f),
      glm::vec3(0.0f, 0.0f, 0.0f),
      glm::tvec2<int>(0, 0),
      glm::tvec2<GLsizei>(width, height));

  uint32_t gbuffer_id = resource_manager.create_framebuffer(albedo_rt_id,
      normal_rt_id, depth_rt_id);
  // register first render pass
  renderer.add_render_pass({gbuffer_id,
      GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT,
      glm::vec3(0.0f, 0.0f, 0.0f),
      true,
      false});
  // register second render pass
  renderer.add_render_pass({0,
      GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT,
      glm::vec3(0.0f, 0.0f, 0.0f),
      false,
      true});
}
