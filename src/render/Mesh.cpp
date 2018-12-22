#include "render/Mesh.hpp"

namespace donkey
{

namespace render
{

Mesh::Mesh(std::uint32_t id, size_t index_count):
  Resource(id),
  index_count(index_count)
{
}

}
}
