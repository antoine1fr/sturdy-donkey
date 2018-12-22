#pragma once

#include <cstdint>

namespace donkey {
namespace render {

struct Resource
{
  std::uint32_t gpu_resource_id;

  Resource(std::uint32_t id):
    gpu_resource_id(id)
  {
  }
};

}
}
