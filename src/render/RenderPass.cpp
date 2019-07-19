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

#include "render/RenderPass.hpp"

namespace donkey { namespace render {

static void bind_camera_uniforms_(
    CommandBucket& render_commands,
    const Material& material,
    const CameraNode& camera_node)
{
  render_commands.bind_uniform(material.view_location, camera_node.view);
  render_commands.bind_uniform(material.projection_location,
      camera_node.projection);
}

static void bind_light_uniforms_(
    CommandBucket& render_commands,
    const Material& material,
    const glm::mat4& view,
    const DirectionalLightNode* light_node)
{
  // bind light infos
  glm::mat4 rotate_x = glm::rotate(
			glm::mat4(1.0f),
			glm::radians(light_node->angles.x),
      glm::vec3(1.0f, 0.0f, 0.0f));
  glm::mat4 rotate_y = glm::rotate(
			rotate_x,
			glm::radians(light_node->angles.y),
      glm::vec3(0.0f, 1.0f, 0.0f));
  glm::mat4 rotate_z = glm::rotate(
			rotate_y,
			glm::radians(light_node->angles.z),
      glm::vec3(0.0f, 0.0f, 1.0f));
  glm::mat4 model = rotate_z * rotate_y * rotate_x;
  glm::vec4 light_dir_shininess =
    view * model * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);
  light_dir_shininess.w = 10.0f;
  render_commands.bind_uniform(material.light_dir_location,
      light_dir_shininess);
  render_commands.bind_uniform(material.light_diffuse_location,
      light_node->diffuse);
  render_commands.bind_uniform(material.light_specular_location,
      light_node->specular);
}

static void bind_mesh_uniforms_(
    CommandBucket& render_commands,
    const Material& material,
    const MeshNode& mesh_node)
{
  // compute model matrix
  glm::mat4 rotate_x = glm::rotate(
			glm::mat4(1.0f),
			glm::radians(mesh_node.angles.x),
      glm::vec3(1.0f, 0.0f, 0.0f));
  glm::mat4 rotate_y = glm::rotate(
			rotate_x,
			glm::radians(mesh_node.angles.y),
      glm::vec3(0.0f, 1.0f, 0.0f));
  glm::mat4 rotate_z = glm::rotate(
			rotate_y,
			glm::radians(mesh_node.angles.z),
      glm::vec3(0.0f, 0.0f, 1.0f));
  glm::mat4 translate = glm::translate(
			glm::mat4(1.0f),
			mesh_node.position);
  glm::mat4 scale = glm::scale(glm::mat4(1.0f), mesh_node.scale);
  glm::mat4 model = translate * rotate_z * rotate_y * rotate_x * scale;
  render_commands.bind_uniform(material.model_location, model);
}

void render_mesh_node(
    const RenderPass& render_pass,
    const MeshNode& mesh_node,
    const CameraNode& camera_node,
    const CameraNode* last_camera_node,
    const DirectionalLightNode* light_node,
    CommandBucket& render_commands,
    ResourceManager* resource_manager,
    AResourceManager* gpu_resource_manager)
{
  static uint32_t last_material_id = std::numeric_limits<uint32_t>::max();
  const Mesh& mesh = resource_manager->get_mesh(mesh_node.mesh_id);
  const Material& material =
    resource_manager->get_material(mesh_node.material_id);
  const AMaterial& gpu_material =
    gpu_resource_manager->get_material(material.gpu_resource_id);

  if (last_material_id != mesh_node.material_id)
  {
    render_commands.bind_gpu_program(material.program_id);
    gpu_material.bind_slots(render_commands);
    last_material_id = mesh_node.material_id;
  }

  // bind built-in uniforms
  bind_mesh_uniforms_(render_commands, material, mesh_node);
  bind_camera_uniforms_(render_commands, material, camera_node);

  // bind some useful data related to the camera used during gbuffer pass
  if (last_camera_node)
  {
    glm::mat4 projection_inverse = glm::inverse(last_camera_node->projection);
    render_commands.bind_uniform(material.gbuffer_projection_inverse_location,
        projection_inverse);
    render_commands.bind_uniform(material.gbuffer_view_location,
        last_camera_node->view);
    render_commands.bind_uniform(material.gbuffer_projection_params_location,
        glm::vec2(last_camera_node->near_plane, last_camera_node->far_plane));
    // camera position in view-space is always the origin
    render_commands.bind_uniform(material.camera_position_location,
        glm::vec3(0.0f));
    render_commands.bind_uniform(material.ambient_location,
        glm::vec4(0.2f, 0.2f, 0.2f, 1.0f));
    if (light_node) {
      bind_light_uniforms_(render_commands, material, last_camera_node->view,
                           light_node);
    }
  }

  // bind geometry
  render_commands.bind_mesh(mesh.gpu_resource_id, material.position_location,
                            material.normal_location, material.uv_location,
                            material.tangent_location,
                            material.bitangent_location);

  render_commands.draw_elements(mesh.index_count);
}

} // namespace render
} // namespace donkey
