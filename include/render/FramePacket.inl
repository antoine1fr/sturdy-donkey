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

#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>

namespace donkey
{

namespace render
{

template <template <typename> class Allocator>
FramePacket<Allocator>::FramePacket()
{
}

template <template <typename> class Allocator>
FramePacket<Allocator>::FramePacket(
    std::list<::donkey::MeshNode> mesh_nodes,
    std::list<::donkey::CameraNode> camera_nodes,
    std::list<::donkey::DirectionalLightNode> directional_light_nodes)
{
  copy_nodes_(mesh_nodes, mesh_nodes_);
  copy_nodes_(camera_nodes, camera_nodes_);
  copy_nodes_(directional_light_nodes, directional_light_nodes_);
}

//void FramePacket::copy_mesh_nodes_(const std::list<::donkey::MeshNode>&)
//{
//}

template <template <typename> class Allocator>
MeshNode& FramePacket<Allocator>::create_mesh_node(uint32_t pass_num,
    const glm::vec3& position,
    const glm::vec3& angles,
    uint32_t mesh_id,
    uint32_t material_id)
{
  mesh_nodes_.push_back({pass_num, position, angles, mesh_id, material_id});
  return mesh_nodes_.front();
}

template <template <typename> class Allocator>
CameraNode& FramePacket<Allocator>::create_perspective_camera_node(uint32_t pass_num,
    float fov, float ratio,
    float near_plane, float far_plane, const glm::vec3& position,
    const glm::vec3& angles,
    const glm::tvec2<int> viewport_position,
    const glm::tvec2<GLsizei> viewport_size)
{
  glm::mat4 projection = glm::perspectiveRH(glm::radians(fov), ratio, near_plane,
      far_plane);
  glm::mat4 view = glm::lookAtRH(
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
  camera_nodes_.push_back(CameraNode(pass_num, position, angles, projection,
      view, viewport_position, viewport_size, near_plane, far_plane));
  return camera_nodes_.front();
}

template <template <typename> class Allocator>
CameraNode& FramePacket<Allocator>::create_ortho_camera_node(uint32_t pass_num,
    const glm::vec3& position,
    const glm::vec3& angles,
    const glm::tvec2<int> viewport_position,
    const glm::tvec2<GLsizei> viewport_size)
{
  glm::mat4 projection =
    glm::orthoRH(
        static_cast<float>(viewport_position.x),
        static_cast<float>(viewport_size.x),
        static_cast<float>(viewport_position.y),
        static_cast<float>(viewport_size.y),
        -1.0f,
        1.0f);
  glm::mat4 view = glm::lookAtRH(
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
  camera_nodes_.push_back(CameraNode(pass_num, position, angles, projection,
      view, viewport_position, viewport_size, -1.0f, 1.0f));
  return camera_nodes_.front();
}

template <template <typename> class Allocator>
const typename FramePacket<Allocator>::template Vector<MeshNode>& FramePacket<Allocator>::get_mesh_nodes() const
{
  return mesh_nodes_;
}

template <template <typename> class Allocator>
const typename FramePacket<Allocator>::template Vector<CameraNode>& FramePacket<Allocator>::get_camera_nodes() const
{
  return camera_nodes_;
}

template <template <typename> class Allocator>
const typename FramePacket<Allocator>::template Vector<DirectionalLightNode>& FramePacket<Allocator>::get_directional_light_nodes() const
{
  return directional_light_nodes_;
}

template <template <typename> class Allocator>
typename FramePacket<Allocator>::template Vector<MeshNode>& FramePacket<Allocator>::get_mesh_nodes()
{
  return mesh_nodes_;
}

template <template <typename> class Allocator>
typename FramePacket<Allocator>::template Vector<CameraNode>& FramePacket<Allocator>::get_camera_nodes()
{
  return camera_nodes_;
}

template <template <typename> class Allocator>
typename FramePacket<Allocator>::template Vector<DirectionalLightNode>& FramePacket<Allocator>::get_directional_light_nodes()
{
  return directional_light_nodes_;
}

template <template <typename> class Allocator>
void FramePacket<Allocator>::sort_mesh_nodes()
{
  std::sort(mesh_nodes_.begin(), mesh_nodes_.end(),
    [](const MeshNode& lhs, const MeshNode& rhs) {
      return (lhs.material_id < rhs.material_id);
    });
}


}
}
