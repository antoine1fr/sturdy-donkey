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
#include <list>
#include "common.hpp"

namespace donkey
{

struct SceneNode
{
  uint32_t pass_num;
  glm::vec3 position;
  glm::vec3 angles;

  SceneNode(uint32_t pass_num,
      const glm::vec3& position,
      const glm::vec3& angles):
    pass_num(pass_num),
    position(position),
    angles(angles)
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
      uint32_t mesh_id,
      uint32_t material_id):
    SceneNode(pass_num, position, angles),
    mesh_id(mesh_id),
    material_id(material_id)
  {
  }
};

struct CameraNode: public SceneNode
{
  glm::mat4 projection;
  glm::mat4 view;
  glm::tvec2<int> viewport_position;
  glm::tvec2<GLsizei> viewport_size;
  float near_plane;
  float far_plane;

  CameraNode(uint32_t pass_num,
      const glm::vec3& position,
      const glm::vec3& angles,
      const glm::mat4& projection,
      const glm::mat4& view,
      const glm::tvec2<int> viewport_position,
      const glm::tvec2<GLsizei> viewport_size,
      float near_plane,
      float far_plane):
    SceneNode(pass_num, position, angles),
    projection(projection),
    view(view),
    viewport_position(viewport_position),
    viewport_size(viewport_size),
    near_plane(near_plane),
    far_plane(far_plane)
  {
  }
};

class Scene
{
  private:
    std::list<MeshNode> mesh_nodes_;
    std::list<CameraNode> camera_nodes_;

  public:
    MeshNode& create_mesh_node(uint32_t pass_num,
        const glm::vec3& position,
        const glm::vec3& angles,
        uint32_t mesh_id,
        uint32_t material_id);
    CameraNode& create_perspective_camera_node(uint32_t pass_num,
        float fov, float ratio,
        float near_plane, float far_plane, const glm::vec3& position,
        const glm::vec3& angles,
        const glm::tvec2<int> viewport_position,
        const glm::tvec2<GLsizei> viewport_size);
    CameraNode& create_ortho_camera_node(uint32_t pass_num,
        const glm::vec3& position,
        const glm::vec3& angles,
        const glm::tvec2<int> viewport_position,
        const glm::tvec2<GLsizei> viewport_size);

    const std::list<MeshNode>& get_mesh_nodes() const;
    const std::list<CameraNode>& get_camera_nodes() const;
    std::list<MeshNode>& get_mesh_nodes();
    std::list<CameraNode>& get_camera_nodes();
};

}
