#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat2x2.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>

namespace render
{

template <class Storage>
ScalarMaterialSlot<Storage>::ScalarMaterialSlot(int location,
  const Storage& storage):
  location(location),
  storage(storage)
{
}

}
