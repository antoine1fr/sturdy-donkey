#pragma once

#include "render/Resource.hpp"

namespace donkey {
namespace render {

struct GpuProgram: Resource
{
  GpuProgram(std::uint32_t id):
    Resource(id)
  {
  }
};

}
}
