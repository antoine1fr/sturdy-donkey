#pragma once

#include <functional>
#include <vector>

#include "render/CommandBucket.hpp"
#include "render/gl/ResourceManager.hpp"

namespace donkey
{

namespace render
{
namespace gl
{

class Driver
{
  private:
    enum
    {
      kCommandTypeMask = 0xff
    };

    typedef std::function<void (const Command&)> RenderFunction;
    const std::vector<RenderFunction> render_functions_;

    ResourceManager resource_manager_;

  public:
    Driver();
    void execute_commands(const CommandBucket& commands);
    AResourceManager& get_resource_manager();

  private:
    void output_debug_info_() const;
    void bind_mesh_(const Command& command);
    void bind_uniform_float_(const Command& command);
    void bind_uniform_int_(const Command& command);
    void bind_uniform_vec2_(const Command& command);
    void bind_uniform_vec3_(const Command& command);
    void bind_uniform_vec4_(const Command& command);
    void bind_uniform_mat2_(const Command& command);
    void bind_uniform_mat3_(const Command& command);
    void bind_uniform_mat4_(const Command& command);
    void bind_texture_(const Command& command);
    void bind_framebuffer_(const Command& command);
    void bind_gpu_program_(const Command& command);
    void draw_elements_(const Command& command);
    void set_viewport_(const Command& command);
    void set_depth_test_(const Command& command);
    void clear_framebuffer_(const Command& command);
};

}
}
}
