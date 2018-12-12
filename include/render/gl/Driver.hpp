#pragma once

#include <vector>
#include "render/CommandBucket.hpp"

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
    static void draw_elements_(const Command& command);
};

}
}
