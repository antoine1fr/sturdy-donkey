/* Copyright (C) 2018 Antoine Luciani
 *
 * This file is part of Sturdy Donkey.
 *
 * Sturdy Donkey is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, version 3.
 *
 * Sturdy Donkey is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * Sturdy Donkey. If not, see <https://www.gnu.org/licenses/>.
 */

#include "MeshLoader.hpp"

#include <tiny_obj_loader.h>

#include <iostream>
#include <unordered_map>
#include <utility>
#include <vector>

#include "hash.hpp"

namespace {
struct Vertex {
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 uv;
  glm::vec3 tangent;
  glm::vec3 bitangent;
};

bool operator==(const Vertex& lhs, const Vertex& rhs) {
  return (lhs.position == rhs.position && lhs.normal == rhs.normal &&
          lhs.uv == rhs.uv);
}

struct Index {
  int position;
  int normal;
  int uv;
  int tangent;
  int bitangent;
};

bool operator==(const Index& lhs, const Index& rhs) {
  return (lhs.position == rhs.position && lhs.normal == rhs.normal &&
          lhs.uv == rhs.uv);
}
}  // namespace

namespace std {
template <>
struct hash<Vertex> {
  typedef Vertex argument_type;
  typedef std::size_t result_type;

  std::size_t operator()(const Vertex& vertex) const noexcept {
    std::size_t seed = 0;
    donkey::hash_combine(seed, vertex.position);
    donkey::hash_combine(seed, vertex.normal);
    donkey::hash_combine(seed, vertex.uv);
    return seed;
  }
};

template <>
struct hash<Index> {
  typedef Index argument_type;
  typedef std::size_t result_type;

  std::size_t operator()(const Index& index) const noexcept {
    std::size_t seed = 0;
    donkey::hash_combine(seed, index.position);
    donkey::hash_combine(seed, index.normal);
    donkey::hash_combine(seed, index.uv);
    return seed;
  }
};
}  // namespace std

namespace donkey {

uint32_t MeshLoader::load(render::ResourceManager* resource_manager,
                          const std::string& path) const {
  std::cout << "Loading mesh from file: " << path << '\n';

  tinyobj::attrib_t attributes;
  std::vector<tinyobj::shape_t> shapes;
  std::vector<tinyobj::material_t> materials;
  std::string warning;
  std::string error;

  tinyobj::LoadObj(&attributes, &shapes, &materials, &warning, &error,
                   path.c_str());
  // TODO: implement support for sub-meshes.
  std::vector<float> vertices;
  std::vector<uint32_t> indices;
  consolidate_indices_(attributes, shapes[0].mesh.indices, vertices, indices);
  return resource_manager->create_mesh(vertices, indices);
}

void MeshLoader::consolidate_indices_(
    const tinyobj::attrib_t& attributes,
    const std::vector<tinyobj::index_t>& tinyobj_indices,
    std::vector<float>& vertices,
    std::vector<uint32_t>& indices) const {
  // 1. expand vertices
  std::vector<Vertex> expanded_vertices;
  expanded_vertices.reserve(tinyobj_indices.size());
  for (const tinyobj::index_t& index : tinyobj_indices) {
    size_t vertex_index = static_cast<size_t>(index.vertex_index) * 3;
    size_t normal_index = static_cast<size_t>(index.normal_index) * 3;
    size_t texcoord_index = static_cast<size_t>(index.texcoord_index) * 2;
    expanded_vertices.push_back({{attributes.vertices[vertex_index + 0],
                                  attributes.vertices[vertex_index + 1],
                                  attributes.vertices[vertex_index + 2]},
                                 {attributes.normals[normal_index + 0],
                                  attributes.normals[normal_index + 1],
                                  attributes.normals[normal_index + 2]},
                                 {attributes.texcoords[texcoord_index + 0],
                                  attributes.texcoords[texcoord_index + 1]},
                                 glm::vec3(0.0f),
                                 glm::vec3(0.0f)});
  }
  compute_vectors_(expanded_vertices);

  // 2. index vertices
  std::unordered_map<Vertex, uint32_t> index;
  uint32_t max_id = 0;
  for (const Vertex& vertex : expanded_vertices) {
    if (index.find(vertex) == index.cend()) {
      index.insert({vertex, max_id});
      max_id += 1;
    }
  }

  // 3. map old indices to new indices
  std::unordered_map<Index, uint32_t> index_map;
  for (const tinyobj::index_t& old_index : tinyobj_indices) {
    uint32_t old_vertex_index = old_index.vertex_index * 3;
    uint32_t old_normal_index = old_index.normal_index * 3;
    uint32_t old_texcoord_index = old_index.texcoord_index * 2;
    Vertex vertex = {{attributes.vertices[old_vertex_index + 0],
                      attributes.vertices[old_vertex_index + 1],
                      attributes.vertices[old_vertex_index + 2]},
                     {attributes.normals[old_normal_index + 0],
                      attributes.normals[old_normal_index + 1],
                      attributes.normals[old_normal_index + 2]},
                     {attributes.texcoords[old_texcoord_index + 0],
                      attributes.texcoords[old_texcoord_index + 1]},
                     glm::vec3(0.0f),
                     glm::vec3(0.0f)};
    uint32_t new_index = (index.find(vertex))->second;
    Index key = {old_index.vertex_index, old_index.normal_index,
                 old_index.texcoord_index};
    index_map[key] = new_index;
  }

  // 5. copy indices in correct order
  indices.reserve(tinyobj_indices.size());
  for (const tinyobj::index_t& old_index : tinyobj_indices) {
    indices.push_back(
        index_map[{old_index.vertex_index, old_index.normal_index,
                   old_index.texcoord_index, old_index.vertex_index,
                   old_index.vertex_index}]);
  }

  // 6. copy vertex attributes in correct order
  size_t positions_size = index.size() * 3;
  size_t normals_size = index.size() * 3;
  size_t uvs_size = index.size() * 2;
  size_t tangents_size = index.size() * 3;
  size_t bitangents_size = index.size() * 3;
  vertices.resize(positions_size + normals_size + uvs_size + tangents_size +
                  bitangents_size);
  for (const Vertex& vertex : expanded_vertices) {
    // find the vertex' id in the index
    size_t i = 5 * static_cast<size_t>((index.find(vertex))->second);
    vertices[i] = vertex.position.x;
    vertices[i + 1] = vertex.position.y;
    vertices[i + 2] = vertex.position.z;
    vertices[i + 3] = vertex.normal.x;
    vertices[i + 4] = vertex.normal.y;
    vertices[i + 5] = vertex.normal.z;
    vertices[i + 6] = vertex.uv.x;
    vertices[i + 7] = vertex.uv.y;
    vertices[i + 8] = vertex.tangent.x;
    vertices[i + 9] = vertex.tangent.y;
    vertices[i + 10] = vertex.tangent.z;
    vertices[i + 11] = vertex.bitangent.x;
    vertices[i + 12] = vertex.bitangent.y;
    vertices[i + 13] = vertex.bitangent.z;
  }

  // debug traces
  std::cout << "\tvertices: " << expanded_vertices.size() << '\n';
  std::cout << "\tindices: " << indices.size() << '\n';
}

glm::vec3 MeshLoader::compute_tangent_(const glm::vec3& dp1,
                                       const glm::vec3& dp2,
                                       const glm::vec2& duv1,
                                       const glm::vec2& duv2,
                                       float kf) const {
  glm::vec3 tangent;
  tangent.x = kf * (duv2.y * dp1.x - duv1.y * dp2.x);
  tangent.y = kf * (duv2.y * dp1.y - duv1.y * dp2.y);
  tangent.z = kf * (duv2.y * dp1.z - duv1.y * dp2.z);
  return glm::normalize(tangent);
}

glm::vec3 MeshLoader::compute_bitangent_(const glm::vec3& dp1,
                                         const glm::vec3& dp2,
                                         const glm::vec2& duv1,
                                         const glm::vec2& duv2,
                                         float kf) const {
  glm::vec3 bitangent;
  bitangent.x = kf * (-duv2.y * dp1.x + duv1.y * dp2.x);
  bitangent.y = kf * (-duv2.y * dp1.y + duv1.y * dp2.y);
  bitangent.z = kf * (-duv2.y * dp1.z + duv1.y * dp2.z);
  return glm::normalize(bitangent);
}

void MeshLoader::compute_vectors_(std::vector<Vertex>& vertices) const {
  size_t triangle_count = vertices.size() / 3;
  for (size_t i = 0; i < triangle_count; i++) {
    size_t offset = i * 3;
    glm::vec3 p1 = vertices[offset + 0].position;
    glm::vec3 p2 = vertices[offset + 1].position;
    glm::vec3 p3 = vertices[offset + 2].position;
    glm::vec2 uv1 = vertices[offset + 0].uv;
    glm::vec2 uv2 = vertices[offset + 1].uv;
    glm::vec2 uv3 = vertices[offset + 2].uv;
    glm::vec3 dp1 = p2 - p1;
    glm::vec3 dp2 = p3 - p1;
    glm::vec2 duv1 = uv2 - uv1;
    glm::vec2 duv2 = uv3 - uv1;
    float f = 1.0f / (duv1.x * duv2.y - duv2.x * duv1.y);
    glm::vec3 tangent = compute_tangent_(dp1, dp2, duv1, duv2, f);
    glm::vec3 bitangent = compute_bitangent_(dp1, dp2, duv1, duv2, f);
    vertices[offset + 0].tangent = tangent;
    vertices[offset + 1].tangent = tangent;
    vertices[offset + 2].tangent = tangent;
    vertices[offset + 0].bitangent = bitangent;
    vertices[offset + 1].bitangent = bitangent;
    vertices[offset + 2].bitangent = bitangent;
  }
}

}  // namespace donkey
