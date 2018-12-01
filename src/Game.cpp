#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "common.hpp"
#include "render/ResourceManager.hpp"
#include "render/Material.hpp"
#include "Game.hpp"
#include "Buffer.hpp"
#include "BufferPool.hpp"

Game::Game():
  window_("Pipelined rendering demo", 1280, 720),
  renderer_(window_),
  render_context_(window_.get_ancillary_context())
{
  window_.make_current(render_context_);
  initialize_resources_(window_.get_width(), window_.get_height());
  BufferPool::add_instance();
  BufferPool::add_instance();
}

Game::~Game()
{
}

void Game::initialize_resources_(int width, int height)
{
  render::ResourceManager& resource_manager = renderer_.get_resource_manager();

  glDisable(GL_STENCIL_TEST);
  glEnable(GL_DEPTH_TEST);

  // load textures
  uint32_t quad_diffuse =
    resource_manager.load_texture_from_file("textures/quad-diffuse.png");
  uint32_t quad_normal =
    resource_manager.load_texture_from_file("textures/quad-normal.png");

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
    "shaders/deferred-gbuffer-pass-vs.glsl",
    "shaders/deferred-gbuffer-pass-fs.glsl");
  uint32_t light_program_id = resource_manager.load_gpu_program_from_file(
    "shaders/simple-vs.glsl",
    "shaders/deferred-light-pass-fs.glsl");

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
  scene_.create_perspective_camera_node(0, 45.0f, ratio, 0.1f, 100.0f,
      glm::vec3(0.0f, 0.0f, 3.0f),
      glm::vec3(0.0f, 0.0f, 0.0f),
      glm::tvec2<int>(0, 0),
      glm::tvec2<GLsizei>(width, height));
  scene_.create_mesh_node(0,
      glm::vec3(0.0f, 0.0f, 0.0f),
      glm::vec3(0.0f, 0.0f, 0.0f),
      quad_id, quad_material_id);

  // create second pass' scene nodes
  scene_.create_mesh_node(1,
      glm::vec3(0.0f, 0.0f, 0.0f),
      glm::vec3(0.0f, 0.0f, 0.0f),
      screen_mesh_id, rt1_material_id);

  scene_.create_ortho_camera_node(1,
      glm::vec3(0.0f, 0.0f, 0.0f),
      glm::vec3(0.0f, 0.0f, 0.0f),
      glm::tvec2<int>(0, 0),
      glm::tvec2<GLsizei>(width, height));

  uint32_t gbuffer_id = resource_manager.create_framebuffer(albedo_rt_id,
      normal_rt_id, depth_rt_id);
  // register first render pass
  renderer_.add_render_pass({gbuffer_id,
      GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT,
      glm::vec3(0.0f, 0.0f, 0.0f),
      true,
      false});
  // register second render pass
  renderer_.add_render_pass({0,
      GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT,
      glm::vec3(0.0f, 0.0f, 0.0f),
      false,
      true});
}

void Game::prepare_frame_packet()
{
  Buffer& buffer = BufferPool::get_push_head();
  buffer.reset();
  void* ptr = buffer.allocate(sizeof(render::FramePacket));
  new (ptr) render::FramePacket(
      scene_.get_mesh_nodes(),
      scene_.get_camera_nodes());
  renderer_.frame_count++;
  BufferPool::next_push_head();
  renderer_.condition_variable.notify_one();
}

void Game::notify_exit()
{
  renderer_.notify_exit();
}

void Game::update(Duration elapsed_time)
{
  while (renderer_.frame_count - renderer_.render_frame_index > 1);
  const float rotation_speed = 50.0f;
  for (SceneNode& node: scene_.get_mesh_nodes())
  {
    if (node.pass_num == 0)
    {
      float angle = elapsed_time.count() * rotation_speed;
      node.angles.y += angle;
    }
  }
}
