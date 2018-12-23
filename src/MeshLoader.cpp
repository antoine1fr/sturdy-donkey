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

#include <iostream>
#include <unordered_map>
#include <utility>
#include <vector>

#include "hash.hpp"
#include "MeshLoader.hpp"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

namespace
{
  struct Vertex
  {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 uv;
  };

  bool operator == (const Vertex& lhs, const Vertex& rhs)
  {
    return (lhs.position == rhs.position
        && lhs.normal == rhs.normal
        && lhs.uv == rhs.uv);
  }

  struct Index
  {
    int position;
    int normal;
    int uv;
  };

  bool operator == (const Index& lhs, const Index& rhs)
  {
    return (lhs.position == rhs.position
        && lhs.normal == rhs.normal
        && lhs.uv == rhs.uv);
  }
}

namespace std
{
  template <>
    struct hash<Vertex>
    {
      typedef Vertex argument_type;
      typedef std::size_t result_type;

      std::size_t operator () (const Vertex& vertex) const noexcept
      {
        std::size_t seed = 0;
        donkey::hash_combine(seed, vertex.position);
        donkey::hash_combine(seed, vertex.normal);
        donkey::hash_combine(seed, vertex.uv);
        return seed;
      }
    };

  template <>
    struct hash<Index>
    {
      typedef Index argument_type;
      typedef std::size_t result_type;

      std::size_t operator () (const Index& index) const noexcept
      {
        std::size_t seed = 0;
        donkey::hash_combine(seed, index.position);
        donkey::hash_combine(seed, index.normal);
        donkey::hash_combine(seed, index.uv);
        return seed;
      }
    };
}

namespace donkey {

uint32_t MeshLoader::load(
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
  std::vector<float> normals;
  std::vector<float> uvs;
  consolidate_indices_(
      attributes,
      shapes[0].mesh.indices,
      indices,
      positions,
      normals,
      uvs);
  return resource_manager.create_mesh(positions, uvs, indices);
}

void MeshLoader::consolidate_indices_(
    const tinyobj::attrib_t& attributes,
    const std::vector<tinyobj::index_t>& tinyobj_indices,
    std::vector<uint32_t>& indices,
    std::vector<float>& positions,
    std::vector<float>& normals,
    std::vector<float>& uvs) const
{
  // 1. expand vertices
  std::vector<Vertex> vertices;
  vertices.reserve(tinyobj_indices.size());
  for (const tinyobj::index_t& index: tinyobj_indices)
  {
    uint32_t vertex_index = index.vertex_index * 3;
    uint32_t normal_index = index.normal_index * 3;
    uint32_t texcoord_index = index.texcoord_index * 2;
    vertices.push_back({
      {
        attributes.vertices[vertex_index + 0],
        attributes.vertices[vertex_index + 1],
        attributes.vertices[vertex_index + 2]
      },
      {
        attributes.normals[normal_index + 0],
        attributes.normals[normal_index + 1],
        attributes.normals[normal_index + 2]
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
  std::unordered_map<Index, uint32_t> index_map;
  for (const tinyobj::index_t& old_index: tinyobj_indices)
  {
    uint32_t old_vertex_index = old_index.vertex_index * 3;
    uint32_t old_normal_index = old_index.normal_index * 3;
    uint32_t old_texcoord_index = old_index.texcoord_index * 2;
    Vertex vertex = {
      {
        attributes.vertices[old_vertex_index + 0],
        attributes.vertices[old_vertex_index + 1],
        attributes.vertices[old_vertex_index + 2]
      },
      {
        attributes.normals[old_normal_index + 0],
        attributes.normals[old_normal_index + 1],
        attributes.normals[old_normal_index + 2]
      },
      {
        attributes.texcoords[old_texcoord_index + 0],
        attributes.texcoords[old_texcoord_index + 1]
      }
    };
    uint32_t new_index = (index.find(vertex))->second;
    Index key = {
      old_index.vertex_index,
      old_index.normal_index,
      old_index.texcoord_index
    };
    index_map[key] = new_index;
  }

  // 5. copy indices in correct order
  indices.reserve(tinyobj_indices.size());
  for (const tinyobj::index_t& old_index: tinyobj_indices)
  {
    indices.push_back(index_map[{
      old_index.vertex_index,
      old_index.normal_index,
      old_index.texcoord_index
    }]);
  }

  // 6. copy vertex attributes in correct order
  positions.resize(index.size() * 3);
  normals.resize(index.size() * 3);
  uvs.resize(index.size() * 2);
  for (const Vertex& vertex: vertices)
  {
    // find the vertex' id in the index
    uint32_t i = (index.find(vertex))->second;
    positions[i * 3] = vertex.position.x;
    positions[(i * 3) + 1] = vertex.position.y;
    positions[(i * 3) + 2] = vertex.position.z;
    normals[i * 3] = vertex.normal.x;
    normals[(i * 3) + 1] = vertex.normal.y;
    normals[(i * 3) + 2] = vertex.normal.z;
    uvs[i * 2] = vertex.uv.x;
    uvs[(i * 2) + 1] = vertex.uv.y;
  }

  // debug traces
  std::cout << "\tpositions: " << positions.size() << '\n';
  std::cout << "\tnormals: " << normals.size() << '\n';
  std::cout << "\tuvs: " << uvs.size() << '\n';
  std::cout << "\tindices: " << indices.size() << '\n';
}

}
