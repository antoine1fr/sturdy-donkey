#pragma once

#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

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

  SceneNode(uint32_t pass_num,
      const glm::vec3& position,
      const glm::vec3& angles):
    pass_num(pass_num),
    position(position),
    angles(angles)
  {
  }

  SceneNode(const ::donkey::SceneNode& node):
    pass_num(node.pass_num),
    position(node.position),
    angles(node.angles)
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

  CameraNode(const ::donkey::CameraNode& node):
    SceneNode(node),
    projection(node.projection),
    view(node.view),
    viewport_position(node.viewport_position),
    viewport_size(node.viewport_size),
    near_plane(node.near_plane),
    far_plane(node.far_plane)
  {
  }
};

template <template <typename> class Allocator>
class FramePacket
{
  public:
    template <typename T>
      using Vector = std::vector<T, Allocator<T>>;

  private:
    Vector<MeshNode> mesh_nodes_;
    Vector<CameraNode> camera_nodes_;

  public:
    FramePacket();
    FramePacket(std::list<::donkey::MeshNode> mesh_nodes,
        std::list<::donkey::CameraNode> camerea_nodes);

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

    const Vector<MeshNode>& get_mesh_nodes() const;
    const Vector<CameraNode>& get_camera_nodes() const;
    Vector<MeshNode>& get_mesh_nodes();
    Vector<CameraNode>& get_camera_nodes();

    void sort_mesh_nodes();

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

}
}

#include "FramePacket.inl"
