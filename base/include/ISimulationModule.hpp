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

#include "StackAllocator.hpp"
#include "common.hpp"
#include "render/FramePacket.hpp"

namespace donkey {

struct ISimulationModule {
  template <typename T>
  using StackAllocator = render::StackAllocator<T>;
  using FramePacket = render::StackFramePacket;

  virtual ~ISimulationModule() {}
  virtual void update(Duration elapsed_time) = 0;
  virtual void prepare_frame_packet(FramePacket* frame_packet,
                                    StackAllocator<FramePacket>& allocator) = 0;
};

}  // namespace donkey