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

#include "Scene.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace donkey {

MeshNode& Scene::create_mesh_node(uint32_t pass_num,
                                  const glm::vec3& position,
                                  const glm::vec3& angles,
                                  const glm::vec3& scale,
                                  uint32_t mesh_id,
                                  uint32_t material_id) {
  mesh_nodes_.push_front(
      {pass_num, position, angles, scale, mesh_id, material_id});
  return mesh_nodes_.front();
}

DirectionalLightNode& Scene::create_directional_light_node(
    uint32_t pass_num,
    const glm::vec3& position,
    const glm::vec3& angles,
    const glm::vec4& diffuse,
    const glm::vec4& specular) {
  directional_light_nodes_.push_front(
      {pass_num, position, angles, diffuse, specular});
  return directional_light_nodes_.front();
}

CameraNode& Scene::create_perspective_camera_node(
    uint32_t pass_num,
    float fov,
    float near_plane,
    float far_plane,
    const glm::vec3& position,
    const glm::vec3& angles,
    const glm::tvec2<int> viewport_position,
    const glm::tvec2<GLsizei> viewport_size) {
  camera_nodes_.push_front(
      CameraNode(pass_num, position, angles, viewport_position, viewport_size,
                 fov, near_plane, far_plane, CameraNode::Type::kPerspective));
  return camera_nodes_.front();
}

CameraNode& Scene::create_ortho_camera_node(
    uint32_t pass_num,
    const glm::vec3& position,
    const glm::vec3& angles,
    const glm::tvec2<int> viewport_position,
    const glm::tvec2<GLsizei> viewport_size) {
  camera_nodes_.push_front(
      CameraNode(pass_num, position, angles, viewport_position, viewport_size,
                 0.0f, -1.0f, 1.0f, CameraNode::Type::kOrthographic));
  return camera_nodes_.front();
}

const std::list<MeshNode>& Scene::get_mesh_nodes() const {
  return mesh_nodes_;
}

const std::list<CameraNode>& Scene::get_camera_nodes() const {
  return camera_nodes_;
}

const std::list<DirectionalLightNode>& Scene::get_directional_light_nodes()
    const {
  return directional_light_nodes_;
}

std::list<MeshNode>& Scene::get_mesh_nodes() {
  return mesh_nodes_;
}

std::list<CameraNode>& Scene::get_camera_nodes() {
  return camera_nodes_;
}

std::list<DirectionalLightNode>& Scene::get_directional_light_nodes() {
  return directional_light_nodes_;
}

}  // namespace donkey
