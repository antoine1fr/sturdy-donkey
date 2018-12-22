#pragma once

#include <cstddef>
#include "render/Resource.hpp"

namespace donkey {
namespace render {

struct Texture: public Resource
{
  Texture(uint32_t id):
    Resource(id)
  {
  }
};

}
}
