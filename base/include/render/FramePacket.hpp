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

#pragma once

#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include "common.hpp"
#include "Scene.hpp"
#include "StackVector.hpp"

namespace donkey
{

namespace render
{

struct SceneNode
{
  uint32_t pass_num;
  glm::vec3 position;
  glm::vec3 angles;
  glm::vec3 scale;

  SceneNode(uint32_t pass_num,
      const glm::vec3& position,
      const glm::vec3& angles,
      const glm::vec3& scale):
    pass_num(pass_num),
    position(position),
    angles(angles),
    scale(scale)
  {
  }

  SceneNode(const ::donkey::SceneNode& node):
    pass_num(node.pass_num),
    position(node.position),
    angles(node.angles),
    scale(node.scale)
  {
  }
};

struct DirectionalLightNode: public SceneNode
{
  glm::vec4 diffuse;
  glm::vec4 specular;

  DirectionalLightNode(uint32_t pass_num,
      const glm::vec3& position,
      const glm::vec3& angles,
      const glm::vec4& diffuse,
      const glm::vec4& specular):
    SceneNode(pass_num, position, angles, glm::vec3(1.0f)),
    diffuse(diffuse),
    specular(specular)
  {
  }

  DirectionalLightNode(const ::donkey::DirectionalLightNode& node):
    SceneNode(node),
    diffuse(node.diffuse),
    specular(node.specular)
  {
  }
};

struct MeshNode: public SceneNode
{
  uint32_t mesh_id;
  uint32_t material_id;

  MeshNode(uint32_t pass_num,
      const glm::vec3& position,
      const glm::vec3& angles,
      const glm::vec3& scale,
      uint32_t mesh_id,
      uint32_t material_id):
    SceneNode(pass_num, position, angles, scale),
    mesh_id(mesh_id),
    material_id(material_id)
  {
  }

  MeshNode(const ::donkey::MeshNode& node):
    SceneNode(node),
    mesh_id(node.mesh_id),
    material_id(node.material_id)
  {
  }
};

struct CameraNode: public SceneNode
{
  glm::mat4 projection;
  glm::mat4 view;
  glm::tvec2<int> viewport_position;
  glm::tvec2<GLsizei> viewport_size;
  float fov;
  float near_plane;
  float far_plane;

  void make_orthographic_matrices()
  {
    projection =
      glm::orthoRH(
          static_cast<float>(viewport_position.x),
          static_cast<float>(viewport_size.x),
          static_cast<float>(viewport_position.y),
          static_cast<float>(viewport_size.y),
          -1.0f,
          1.0f);
    view = glm::lookAtRH(
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, -1.0f),
        glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 rotate_x = glm::rotate(glm::mat4(1.0f), glm::radians(angles.x),
        glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 rotate_y = glm::rotate(glm::mat4(1.0f), glm::radians(angles.y),
        glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 rotate_z = glm::rotate(glm::mat4(1.0f), glm::radians(angles.z),
        glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 translate = glm::translate(glm::mat4(1.0f), -position);
    view = translate * rotate_z * rotate_y * rotate_x * view;
  }

  void make_perspective_matrices()
  {
    float ratio = static_cast<float>(viewport_size.x)
      / static_cast<float>(viewport_size.y);
    projection = glm::perspectiveRH(
        glm::radians(fov),
        ratio,
        near_plane,
        far_plane);
    view = glm::lookAtRH(
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, -1.0f),
        glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 rotate_x = glm::rotate(glm::mat4(1.0f), glm::radians(angles.x),
        glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 rotate_y = glm::rotate(glm::mat4(1.0f), glm::radians(angles.y),
        glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 rotate_z = glm::rotate(glm::mat4(1.0f), glm::radians(angles.z),
        glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 translate = glm::translate(glm::mat4(1.0f), -position);
    view = translate * rotate_z * rotate_y * rotate_x * view;
  }

  CameraNode(const ::donkey::CameraNode& node):
    SceneNode(node),
    viewport_position(node.viewport_position),
    viewport_size(node.viewport_size),
    fov(node.fov),
    near_plane(node.near_plane),
    far_plane(node.far_plane)
  {
    if (node.type == donkey::CameraNode::Type::kOrthographic)
      make_orthographic_matrices();
    else
      make_perspective_matrices();
  }
};

template <template <typename> class Allocator>
class FramePacket
{
  public:
    template <typename T>
      using Vector = std::vector<T, Allocator<T>>;

  private:
    typedef Allocator<MeshNode> MeshNodeAllocator;
    typedef Allocator<DirectionalLightNode> DirectionalLightNodeAllocator;

  private:
    MeshNodeAllocator mesh_node_allocator_;
    DirectionalLightNodeAllocator directional_light_node_allocator_;
    Vector<MeshNode> mesh_nodes_;
    CameraNode camera_node_;
    Vector<DirectionalLightNode> directional_light_nodes_;

  public:
    FramePacket(const MeshNodeAllocator& allocator, donkey::CameraNode&& camera_node);

    FramePacket(
        std::list<::donkey::MeshNode> mesh_nodes,
        std::list<::donkey::CameraNode> camerea_nodes,
        std::list<::donkey::DirectionalLightNode> directional_light_nodes,
        const MeshNodeAllocator& allocator);

    void set_camera_node(CameraNode&& node);

    MeshNode& create_mesh_node(uint32_t pass_num,
        const glm::vec3& position,
        const glm::vec3& angles,
        const glm::vec3& scale,
        uint32_t mesh_id,
        uint32_t material_id);

    const Vector<MeshNode>& get_mesh_nodes() const;
    const CameraNode& get_camera_node() const;
    const Vector<DirectionalLightNode>& get_directional_light_nodes() const;
    Vector<MeshNode>& get_mesh_nodes();
    Vector<DirectionalLightNode>& get_directional_light_nodes();

    void sort_mesh_nodes();

  public:
    static std::array<FramePacket<Allocator>*, 2> frame_packets;

  private:
    template <typename T, typename U>
      void copy_nodes_(const std::list<T>& source_nodes,
          Vector<U>& destination_nodes)
      {
        destination_nodes.reserve(source_nodes.size());
        for (const T& node: source_nodes)
        {
          destination_nodes.push_back(node);
        }
      }
};

template <typename T>
using StackAllocator = StackAllocator<T, 4>;
typedef FramePacket<StackAllocator> StackFramePacket;
template <typename T>
using StackVector = StackFramePacket::template Vector<T>;

}
}

#include "FramePacket.inl"
