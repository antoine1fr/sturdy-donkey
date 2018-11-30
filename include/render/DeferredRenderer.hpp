#pragma once

#include "render/FramePacket.hpp"
#include "render/ResourceManager.hpp"
#include "render/RenderPass.hpp"

namespace render
{

class DeferredRenderer
{
  private:
    ResourceManager resource_manager_;
    std::vector<RenderPass> render_passes_;

  private:
    void render_mesh_node_(const RenderPass& render_pass,
        const MeshNode& mesh_node, const CameraNode& camera_node,
        const glm::vec3& camera_direction) const;
    void execute_pass_(size_t pass_num, const RenderPass& render_pass,
        const FramePacket& frame_packet) const;
    void bind_light_uniforms_(const Material& material,
        const glm::mat4& view) const;
    void bind_camera_uniforms_(
        const Material& material,
        const CameraNode& camera_node,
        const glm::vec3& camera_direction) const;
    void bind_mesh_uniforms_(const Material& material,
        const MeshNode& mesh_node) const;

  public:
    ~DeferredRenderer();
    void render(const FramePacket& frame_packet) const;
    ResourceManager& get_resource_manager();
    void add_render_pass(const RenderPass& render_pass);
    void add_render_pass(RenderPass&& render_pass);
};

}
