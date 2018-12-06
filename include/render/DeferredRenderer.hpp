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
    uint32_t light_program_id_;
    uint32_t albedo_rt_id_;
    uint32_t normal_rt_id_;
    uint32_t depth_rt_id_;
    uint32_t gbuffer_id_;
    uint32_t screen_mesh_id_;

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
      void execute_pass_(
          size_t pass_num,
          const RenderPass& render_pass,
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
    void create_light_pass_mesh_(int width, int height);
    void create_gbuffer_(int width, int height);
    uint32_t create_light_pass_material_();
    void create_light_pass_frame_packet_(int width, int height);

  public:
    DeferredRenderer(Window& window);
    ~DeferredRenderer();
    void render();
    ResourceManager& get_resource_manager();
    void add_render_pass(const RenderPass& render_pass);
    void add_render_pass(RenderPass&& render_pass);
    void notify_exit();
    uint32_t get_albedo_rt_id() const;
    uint32_t get_normal_rt_id() const;
    uint32_t get_depth_rt_id() const;
};

}
