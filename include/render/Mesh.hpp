#pragma once

#include <cstddef>
#include "render/Resource.hpp"

namespace donkey {
namespace render {

struct Mesh: Resource
{
  std::size_t index_count;

  Mesh(std::uint32_t id, std::size_t);
};

}
}
