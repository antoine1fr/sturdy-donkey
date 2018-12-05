#pragma once

#include <thread>
#include <condition_variable>
#include <atomic>
#include <SDL.h>

#include "render/FramePacket.hpp"
#include "render/ResourceManager.hpp"
#include "render/RenderPass.hpp"
#include "render/Window.hpp"

namespace render
{

class DeferredRenderer
{
  private:
    ResourceManager resource_manager_;
    std::vector<RenderPass> render_passes_;
    std::atomic_bool run_;
    Window& window_;
    SDL_GLContext render_context_;
    std::thread* render_thread_;

  public:
    std::condition_variable condition_variable;
    std::mutex mutex;
    std::atomic_uint32_t frame_count;
    std::atomic_uint32_t render_frame_index;

  private:
    void render_mesh_node_(const RenderPass& render_pass,
        const MeshNode& mesh_node, const CameraNode& camera_node,
        const glm::vec3& camera_direction) const;
    template <template <typename> class Allocator>
      void execute_pass_(size_t pass_num, const RenderPass& render_pass,
          const FramePacket<Allocator>& frame_packet) const;
    void bind_light_uniforms_(const Material& material,
        const glm::mat4& view) const;
    void bind_camera_uniforms_(
        const Material& material,
        const CameraNode& camera_node,
        const glm::vec3& camera_direction) const;
    void bind_mesh_uniforms_(const Material& material,
        const MeshNode& mesh_node) const;
    void output_debug_info_() const;

  public:
    DeferredRenderer(Window& window);
    ~DeferredRenderer();
    void render();
    ResourceManager& get_resource_manager();
    void add_render_pass(const RenderPass& render_pass);
    void add_render_pass(RenderPass&& render_pass);
    void notify_exit();
};

}
