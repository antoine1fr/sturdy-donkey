#pragma once

#include "IMaterialSlot.hpp"

class ResourceManager;

template <class Storage_>
class ScalarMaterialSlot: public IMaterialSlot
{
  public:
    typedef Storage_ Storage;

  public:
    int location;
    Storage storage;

  public:
    ScalarMaterialSlot(int location, const Storage& storage);
    virtual void bind(const ResourceManager& resource_manager);
};

#include "ScalarMaterialSlot.inl"
