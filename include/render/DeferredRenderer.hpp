#pragma once

#include <array>
#include <thread>
#include <condition_variable>
#include <atomic>
#include <SDL.h>
#include <limits>

#include "render/gl/Driver.hpp"
#include "render/FramePacket.hpp"
#include "render/ResourceManager.hpp"
#include "render/RenderPass.hpp"
#include "render/Window.hpp"

namespace donkey {
namespace render {

class DeferredRenderer
{
  private:
    std::vector<RenderPass> render_passes_;
    std::atomic_bool run_;
    Window& window_;
    SDL_GLContext render_context_;
    gl::Driver driver_;
    ResourceManager resource_manager_;
    FramePacket<std::allocator> light_frame_packet_;
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
    void render_mesh_node_(
        const RenderPass& render_pass,
        const MeshNode& mesh_node,
        const CameraNode& camera_node);
    template <template <typename> class Allocator>
      void execute_pass_(
          size_t pass_num,
          const RenderPass& render_pass,
          const FramePacket<Allocator>& frame_packet);
    void bind_light_uniforms_(
        CommandBucket& render_commands,
        const Material& material,
        const glm::mat4& view) const;
    void bind_camera_uniforms_(
        CommandBucket& render_commands,
        const Material& material,
        const CameraNode& camera_node) const;
    void bind_mesh_uniforms_(
        CommandBucket& render_commands,
        const Material& material,
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
    void start_render_thread();
};

template <template <typename> class Allocator>
void DeferredRenderer::execute_pass_(
    size_t pass_num,
    const RenderPass& render_pass,
    const FramePacket<Allocator>& frame_packet)
{
  GLuint framebuffer;
  uint32_t framebuffer_id = render_pass.framebuffer_id;
  if (framebuffer_id == std::numeric_limits<uint32_t>::max())
    framebuffer = 0;
  else
    framebuffer = resource_manager_.get_framebuffer(framebuffer_id);
  glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
  check_gl_framebuffer(GL_FRAMEBUFFER);

  if (render_pass.depth_test)
    glEnable(GL_DEPTH_TEST);
  else
    glDisable(GL_DEPTH_TEST);

  for (auto camera_node: frame_packet.get_camera_nodes())
  {
    if (camera_node.pass_num != pass_num)
      continue;
    glViewport(camera_node.viewport_position.x,
        camera_node.viewport_position.y,
        camera_node.viewport_size.x,
        camera_node.viewport_size.y);
    glClearColor(render_pass.clear_color.x, render_pass.clear_color.y,
        render_pass.clear_color.z, 1.0f);
    if (framebuffer == 0)
    {
      glDrawBuffer(GL_BACK);
      glClear(render_pass.clear_bits);
    }
    else
    {
      std::array<GLenum, 2> targets {{
        GL_COLOR_ATTACHMENT0,
        GL_COLOR_ATTACHMENT1
      }};
      glDrawBuffers(targets.size(), &targets[0]);
      glClear(render_pass.clear_bits);
    }
    for (auto mesh_node: frame_packet.get_mesh_nodes())
    {
      if (mesh_node.pass_num != pass_num)
        continue;
      render_mesh_node_(render_pass, mesh_node, camera_node);
    }
  }
}

}
}
