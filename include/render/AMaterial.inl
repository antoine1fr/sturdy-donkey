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

#include "render/GpuResourceManager.hpp"

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
