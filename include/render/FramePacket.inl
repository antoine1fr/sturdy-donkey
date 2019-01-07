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
MeshNode& FramePacket<Allocator>::create_mesh_node(
    uint32_t pass_num,
    const glm::vec3& position,
    const glm::vec3& angles,
    uint32_t mesh_id,
    uint32_t material_id)
{
  mesh_nodes_.push_back({
    pass_num,
    position,
    angles,
    mesh_id,
    material_id
  });
  return mesh_nodes_.front();
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

template <template <typename> class Allocator>
void FramePacket<Allocator>::add_camera_node(CameraNode&& node)
{
  camera_nodes_.push_back(node);
}

}
}
