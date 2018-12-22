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
    void bind_uniform_float(const Command& command);
    void bind_uniform_int(const Command& command);
    void bind_uniform_vec2(const Command& command);
    void bind_uniform_vec3(const Command& command);
    void bind_uniform_vec4(const Command& command);
    void bind_uniform_mat2(const Command& command);
    void bind_uniform_mat3(const Command& command);
    void bind_uniform_mat4(const Command& command);
    void bind_texture(const Command& command);
    void draw_elements_(const Command& command);
};

}
}
}
