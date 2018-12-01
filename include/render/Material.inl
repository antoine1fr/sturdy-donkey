#pragma once

#include "ResourceManager.hpp"

namespace render
{

template <class Storage>
void Material::bind_scalar(int location, const Storage& storage) const
{
  ScalarMaterialSlot<Storage> slot(location, storage);
  slot.bind(resource_manager_);
}

}