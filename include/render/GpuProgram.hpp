#pragma once

namespace render
{

struct GpuProgram
{
  GLuint handle;
  GLint model_location;
  GLint view_location;
  GLint projection_location;
  GLint position_location;
  GLint uv_location;
};

}
