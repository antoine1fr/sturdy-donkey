/* Copyright (C) 2018 Antoine Luciani
 *
 * This file is part of Sturdy Donkey.
 *
 * Sturdy Donkey is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, version 3.
 *
 * Sturdy Donkey is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * Sturdy Donkey. If not, see <https://www.gnu.org/licenses/>.
 */

#include "render/AMaterial.hpp"

#include "render/gl/ResourceManager.hpp"

namespace donkey {

namespace render {

AMaterial::AMaterial(uint32_t program_id) : program_id(program_id) {}

#define BIND_SLOTS(x)            \
  for (auto slot : x##_slots_) { \
    slot.bind(render_commands);  \
  }

void AMaterial::bind_slots(CommandBucket& render_commands) const {
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

}  // namespace render
}  // namespace donkey
