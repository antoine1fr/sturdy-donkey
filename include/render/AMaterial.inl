#include "render/AResourceManager.hpp"

namespace donkey
{

namespace render
{

template <class Storage>
void AMaterial::bind_scalar(int location, const Storage& storage,
    CommandBucket& render_commands) const
{
  ScalarMaterialSlot<Storage> slot(location, storage);
  slot.bind(render_commands);
}

}
}
