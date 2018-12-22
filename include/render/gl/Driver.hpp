#pragma once

#include <functional>
#include <vector>

#include "render/CommandBucket.hpp"

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

  public:
    Driver();
    void execute_commands(const CommandBucket& commands);

  private:
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
    void draw_elements_(const Command& command);
};

}
}
}
