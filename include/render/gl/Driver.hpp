#pragma once

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

    typedef void (*RenderFunction)(const Command&);
    static const std::vector<RenderFunction> render_functions_;

  public:
    void execute_commands(const CommandBucket& commands);

  private:
    static void bind_mesh_(const Command& command);
    static void bind_uniform_float(const Command& command);
    static void bind_uniform_int(const Command& command);
    static void bind_uniform_vec2(const Command& command);
    static void bind_uniform_vec3(const Command& command);
    static void bind_uniform_vec4(const Command& command);
    static void bind_uniform_mat2(const Command& command);
    static void bind_uniform_mat3(const Command& command);
    static void bind_uniform_mat4(const Command& command);
    static void bind_texture(const Command& command);
    static void draw_elements_(const Command& command);
};

}
}
}
