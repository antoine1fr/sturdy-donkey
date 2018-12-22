#pragma once

namespace donkey {
namespace render {
namespace gl {

struct GpuProgram
{
  GLuint handle;
  int position_location;
  int uv_location;
};

}
}
}
