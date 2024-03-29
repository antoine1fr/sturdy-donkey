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

#pragma once

#include <cstdint>

#include "render/CommandBucket.hpp"

namespace donkey {

namespace render {

class TextureMaterialSlot {
 public:
  int location;
  uint32_t texture_id;
  int texture_unit;

 public:
  TextureMaterialSlot(int location, uint32_t texture_id, int texture_unit);
  void bind(CommandBucket& render_commands);
};

}  // namespace render
}  // namespace donkey
