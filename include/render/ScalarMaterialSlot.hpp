#pragma once

namespace render
{

class ResourceManager;

template <class Storage_>
class ScalarMaterialSlot
{
  public:
    typedef Storage_ Storage;

  public:
    int location;
    Storage storage;

  public:
    ScalarMaterialSlot(int location, const Storage& storage);
    void bind(const ResourceManager& resource_manager);
};

}

#include "ScalarMaterialSlot.inl"
