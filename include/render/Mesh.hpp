#pragma once

#include <cstddef>

namespace donkey
{

namespace render
{

struct Mesh
{
  std::size_t index_count;

  Mesh(std::size_t);
};

}
}
