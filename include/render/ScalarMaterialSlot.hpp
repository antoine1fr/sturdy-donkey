#pragma once

#include "render/CommandBucket.hpp"

namespace donkey
{

namespace render
{

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
    void bind(CommandBucket& render_commands);
};

}
}

#include "ScalarMaterialSlot.inl"
