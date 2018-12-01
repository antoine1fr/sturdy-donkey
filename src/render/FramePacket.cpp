#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "render/FramePacket.hpp"

namespace render
{

FramePacket::FramePacket(std::list<::MeshNode> mesh_nodes,
    std::list<::CameraNode> camera_nodes)
{
  copy_nodes_(mesh_nodes, mesh_nodes_);
  copy_nodes_(camera_nodes, camera_nodes_);
}

//void FramePacket::copy_mesh_nodes_(const std::list<::MeshNode>&)
//{
//}

MeshNode& FramePacket::create_mesh_node(uint32_t pass_num,
    const glm::vec3& position,
    const glm::vec3& angles,
    uint32_t mesh_id,
    uint32_t material_id)
{
  mesh_nodes_.push_back({pass_num, position, angles, mesh_id, material_id});
  return mesh_nodes_.front();
}

CameraNode& FramePacket::create_perspective_camera_node(uint32_t pass_num,
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

CameraNode& FramePacket::create_ortho_camera_node(uint32_t pass_num,
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

const StackVector<MeshNode>& FramePacket::get_mesh_nodes() const
{
  return mesh_nodes_;
}

const StackVector<CameraNode>& FramePacket::get_camera_nodes() const
{
  return camera_nodes_;
}

StackVector<MeshNode>& FramePacket::get_mesh_nodes()
{
  return mesh_nodes_;
}

StackVector<CameraNode>& FramePacket::get_camera_nodes()
{
  return camera_nodes_;
}

}
