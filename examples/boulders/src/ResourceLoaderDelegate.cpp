#include <algorithm>
#include <functional>
#include <random>
#include <string>
#include <utility>
#include <vector>
#include <unordered_map>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include <hash.hpp>
#include <render/Material.hpp>
#include "ResourceLoaderDelegate.hpp"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

void ResourceLoaderDelegate::load(
    donkey::render::Window& window,
    donkey::Scene& scene,
    donkey::render::DeferredRenderer& renderer)
{
  donkey::render::ResourceManager& resource_manager =
    renderer.get_resource_manager();
  donkey::render::AResourceManager& gpu_resource_manager =
    renderer.get_gpu_resource_manager();
  int width = window.get_width();
  int height = window.get_height();

  glDisable(GL_STENCIL_TEST);
  glEnable(GL_DEPTH_TEST);

  // load textures
  uint32_t boulder_diffuse_id =
    resource_manager.load_texture_from_file("../../resources/textures/pbr/rusted_iron/albedo.png");
  uint32_t boulder_normal_id =
    resource_manager.load_texture_from_file("../../resources/textures/pbr/rusted_iron/normal.png");

  // create a quad mesh
  uint32_t boulder_mesh_id = load_mesh_(resource_manager,
      "../../resources/objects/rock/rock.obj");

  // load gpu programs
  uint32_t gbuffer_program_id = resource_manager.load_gpu_program_from_file(
    "../../shaders/gbuffer-pass.vert.glsl",
    "../../shaders/gbuffer-pass.frag.glsl");

  // create materials
  uint32_t boulder_material_id;
  {
    boulder_material_id = resource_manager.create_material(gbuffer_program_id);
    const donkey::render::Material& material =
      resource_manager.get_material(boulder_material_id);
    donkey::render::AMaterial& gpu_material =
      gpu_resource_manager.get_material(material.gpu_resource_id);
    const donkey::render::Texture& diffuse =
      resource_manager.get_texture(boulder_diffuse_id);
    const donkey::render::Texture& normal =
      resource_manager.get_texture(boulder_normal_id);
    gpu_material.register_texture_slot("diffuse_texture",
        diffuse.gpu_resource_id, 0);
    gpu_material.register_texture_slot("normal_map",
        normal.gpu_resource_id, 1);
  }

  // create first pass' scene nodes
  float ratio = static_cast<float>(width) / static_cast<float>(height);
  scene.create_perspective_camera_node(0, 45.0f, ratio, 0.1f, 1000.0f,
      glm::vec3(0.0f, 0.0f, 100.0f),
      glm::vec3(0.0f, 0.0f, 0.0f),
      glm::tvec2<int>(0, 0),
      glm::tvec2<GLsizei>(width, height));

  std::random_device random_device;
  std::default_random_engine random_engine(random_device());
  std::uniform_real_distribution<float> distribution(-100.0f, 100.0f);
  for (size_t i = 0; i < 4000; ++i)
  {
    float x = distribution(random_engine);
    float y = distribution(random_engine);
    float z = distribution(random_engine);
    scene.create_mesh_node(0,
        glm::vec3(x, y, z),
        glm::vec3(0.0f, 0.0f, 0.0f),
        boulder_mesh_id, boulder_material_id);
  }
}
