#pragma once

#include "common.hpp"
#include "ResourceManager.hpp"
#include "Scene.hpp"

class Demo
{
  private:
    uint32_t gbuffer_id;
    ResourceManager resource_manager_;
    Scene scene_;
    std::vector<RenderPass> render_passes_;

  private:
    void render_mesh_node_(const RenderPass& render_pass,
        const MeshNode& mesh_node, const CameraNode& camera_node) const;
    void execute_pass_(size_t pass_num, const RenderPass& render_pass) const;

  public:
    void cleanup();
    void render() const;
    void initialize(int width, int height);
};
