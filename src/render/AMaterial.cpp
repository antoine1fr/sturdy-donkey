#include "render/AMaterial.hpp"
#include "render/gl/ResourceManager.hpp"

namespace donkey
{

namespace render
{

AMaterial::AMaterial(const AResourceManager& resource_manager,
    uint32_t program_id):
  resource_manager_(static_cast<const gl::ResourceManager&>(resource_manager)),
  program_id(program_id)
{
}

#define BIND_SLOTS(x) \
  for (auto slot: x ## _slots_) \
  { \
    slot.bind(render_commands); \
  }

void AMaterial::bind_slots(CommandBucket& render_commands) const
{
  BIND_SLOTS(texture);
  BIND_SLOTS(float)
  BIND_SLOTS(vec2)
  BIND_SLOTS(vec3)
  BIND_SLOTS(vec4)
  BIND_SLOTS(mat2)
  BIND_SLOTS(mat2)
  BIND_SLOTS(mat4)
  BIND_SLOTS(int)
}

#undef BIND_SLOTS

}
}
