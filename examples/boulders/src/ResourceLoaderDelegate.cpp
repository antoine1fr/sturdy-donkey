#include <random>
#include <string>
#include <vector>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <render/Material.hpp>
#include "ResourceLoaderDelegate.hpp"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

void ResourceLoaderDelegate::load(render::Window& window, Scene& scene,
    render::DeferredRenderer& renderer)
{
  render::ResourceManager& resource_manager = renderer.get_resource_manager();
  int width = window.get_width();
  int height = window.get_height();

  glDisable(GL_STENCIL_TEST);
  glEnable(GL_DEPTH_TEST);

  // load textures
  uint32_t boulder_diffuse_id =
    resource_manager.load_texture_from_file("../../textures/rock_001_albedo.png");
  uint32_t boulder_normal_id =
    resource_manager.load_texture_from_file("../../textures/rock_001_normal.png");

  // create a quad mesh
  uint32_t boulder_mesh_id = load_mesh_(resource_manager,
      "../../meshes/rock_001.obj");

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
        screen_mesh_indices);

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
  uint32_t boulder_material_id;
  {
    render::Material material(resource_manager, gbuffer_program_id);
    material.register_texture_slot("diffuse_texture", boulder_diffuse_id, 0);
    material.register_texture_slot("normal_map", boulder_normal_id, 1);
    boulder_material_id =
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
  scene.create_perspective_camera_node(0, 45.0f, ratio, 0.1f, 1000.0f,
      glm::vec3(0.0f, 0.0f, 10.0f),
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
    //float z = distribution(random_engine);
    scene.create_mesh_node(0,
        glm::vec3(x, y, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        boulder_mesh_id, boulder_material_id);
  }
}

uint32_t ResourceLoaderDelegate::load_mesh_(
    render::ResourceManager& resource_manager,
    const std::string& path) const
{
  std::cout << "Loading mesh from file: " << path << '\n';

  tinyobj::attrib_t attributes;
  std::vector<tinyobj::shape_t> shapes;
  std::vector<tinyobj::material_t> materials;
  //std::string warning;
  std::string error;

  tinyobj::LoadObj(&attributes, &shapes, &materials, &error,
      path.c_str());
  // TODO: implement support for sub-meshes.
  std::vector<uint32_t> indices;
  std::vector<float> positions;
  std::vector<float> uvs;
  consolidate_indices_(
      attributes,
      shapes[0].mesh.indices,
      indices,
      positions,
      uvs);
  return resource_manager.create_mesh(positions, uvs, indices);
}

void ResourceLoaderDelegate::consolidate_indices_(
    const tinyobj::attrib_t& attributes,
    const std::vector<tinyobj::index_t>& tinyobj_indices,
    std::vector<uint32_t>& indices,
    std::vector<float>& positions,
    std::vector<float>& uvs) const
{
  indices.reserve(tinyobj_indices.size());
  positions = attributes.vertices;
  uvs.reserve(attributes.texcoords.size());
  for (const tinyobj::index_t& index: tinyobj_indices)
  {
    indices.push_back(index.vertex_index);
    uvs.push_back(attributes.texcoords[index.texcoord_index]);
  }
}
