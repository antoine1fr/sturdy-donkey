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

#include <atomic>

#include "Game.hpp"
#include "IResourceLoaderDelegate.hpp"
#include "ISimulationModule.hpp"
#include "render/DeferredRenderer.hpp"
#include "render/ResourceManager.hpp"
#include "render/Window.hpp"

namespace donkey {

class GameManager {
 private:
  render::Window* window_;
  render::gl::Driver* driver_;
  render::DeferredRenderer* renderer_;
  IResourceLoaderDelegate& resource_loader_;
  std::atomic_bool run_;
  std::atomic_size_t simulated_frame_count_;
  std::atomic_size_t rendered_frame_count_;
  render::ResourceManager* resource_manager_;
  std::list<ISimulationModule*> simulation_modules_;

 private:
  size_t wait_for_frame_packet_();
  void wait_render_thread_() const;
  void update_simulation_(Duration elapsed_time);
  void prepare_frame_packet_(Game& game);
  size_t get_rendered_frame_count_() const;
  size_t get_simulated_frame_count_() const;
  size_t get_simulated_frame_count_relaxed_() const;
  void increment_rendered_frame_count_();
  void increment_simulated_frame_count_();

 public:
  template <typename T>
  using StackAllocator = render::StackAllocator<T>;
  using FramePacket = render::StackFramePacket;

  GameManager(IResourceLoaderDelegate& resource_loader);
  ~GameManager();
  void run();
  void render_loop();
  void simulation_loop();
};

}  // namespace donkey
