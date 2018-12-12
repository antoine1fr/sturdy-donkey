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
    resource_manager.load_texture_from_file("../../resources/textures/pbr/rusted_iron/albedo.png");
  uint32_t boulder_normal_id =
    resource_manager.load_texture_from_file("../../resources/textures/pbr/rusted_iron/normal.png");

  // create a quad mesh
  uint32_t boulder_mesh_id = load_mesh_(resource_manager,
      "../../resources/objects/rock/rock.obj");

  // load gpu programs
  uint32_t gbuffer_program_id = resource_manager.load_gpu_program_from_file(
    "../../shaders/deferred-gbuffer-pass-vs.glsl",
    "../../shaders/deferred-gbuffer-pass-fs.glsl");

  // create materials
  uint32_t boulder_material_id;
  {
    render::Material material(resource_manager, gbuffer_program_id);
    material.register_texture_slot("diffuse_texture", boulder_diffuse_id, 0);
    material.register_texture_slot("normal_map", boulder_normal_id, 1);
    boulder_material_id =
      resource_manager.register_material(std::move(material));
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
  // 1. expand vertices
  typedef std::pair<glm::vec3, glm::vec2> Vertex;
  std::vector<Vertex> vertices;
  vertices.reserve(tinyobj_indices.size());
  for (const tinyobj::index_t& index: tinyobj_indices)
  {
    uint32_t vertex_index = index.vertex_index * 3;
    uint32_t texcoord_index = index.texcoord_index * 2;
    vertices.push_back({
      {
        attributes.vertices[vertex_index + 0],
        attributes.vertices[vertex_index + 1],
        attributes.vertices[vertex_index + 2]
      },
      {
        attributes.texcoords[texcoord_index + 0],
        attributes.texcoords[texcoord_index + 1]
      }
    });
  }

  // 2. index vertices
  std::unordered_map<Vertex, uint32_t> index;
  uint32_t max_id = 0;
  for (const Vertex& vertex: vertices)
  {
    if (index.find(vertex) == index.cend())
    {
      index.insert({vertex, max_id});
      max_id += 1;
    }
  }

  // 3. map old indices to new indices
  std::unordered_map<std::pair<uint32_t, uint32_t>, uint32_t> index_map;
  for (const tinyobj::index_t& old_index: tinyobj_indices)
  {
    uint32_t old_vertex_index = old_index.vertex_index * 3;
    uint32_t old_texcoord_index = old_index.texcoord_index * 2;
    Vertex vertex = {
      {
        attributes.vertices[old_vertex_index + 0],
        attributes.vertices[old_vertex_index + 1],
        attributes.vertices[old_vertex_index + 2]
      },
      {
        attributes.texcoords[old_texcoord_index + 0],
        attributes.texcoords[old_texcoord_index + 1]
      }
    };
    uint32_t new_index = (index.find(vertex))->second;
    std::pair<uint32_t, uint32_t> key = {
      old_index.vertex_index,
      old_index.texcoord_index
    };
    index_map[key] = new_index;
  }

  // 5. copy indices in correct order
  indices.reserve(tinyobj_indices.size());
  for (const tinyobj::index_t& old_index: tinyobj_indices)
  {
    indices.push_back(index_map[{old_index.vertex_index,
        old_index.texcoord_index}]);
  }

  // 6. copy vertex attributes in correct order
  positions.resize(index.size() * 3);
  uvs.resize(index.size() * 2);
  for (const Vertex& vertex: vertices)
  {
    // find the vertex' id in the index
    uint32_t i = (index.find(vertex))->second;
    positions[i * 3] = vertex.first.x;
    positions[(i * 3) + 1] = vertex.first.y;
    positions[(i * 3) + 2] = vertex.first.z;
    uvs[i * 2] = vertex.second.x;
    uvs[(i * 2) + 1] = vertex.second.y;
  }

  // debug traces
  std::cout << "\tpositions: " << positions.size() << '\n';
  std::cout << "\tuvs: " << uvs.size() << '\n';
  std::cout << "\tindices: " << indices.size() << '\n';
}
