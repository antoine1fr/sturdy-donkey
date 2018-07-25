#pragma once

#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <forward_list>
#include "common.hpp"

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

  CameraNode(uint32_t pass_num,
      const glm::vec3& position,
      const glm::vec3& angles,
      const glm::mat4& projection,
      const glm::mat4& view,
      const glm::tvec2<int> viewport_position,
      const glm::tvec2<GLsizei> viewport_size):
    SceneNode(pass_num, position, angles),
    projection(projection),
    view(view),
    viewport_position(viewport_position),
    viewport_size(viewport_size)
  {
  }
};

class Scene
{
  private:
    std::forward_list<MeshNode> mesh_nodes_;
    std::forward_list<CameraNode> camera_nodes_;

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

    const std::forward_list<MeshNode>& get_mesh_nodes() const;
    const std::forward_list<CameraNode>& get_camera_nodes() const;
};
