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
#include "render/DeferredRenderer.hpp"
#include "render/Window.hpp"

namespace donkey
{

class GameManager
{
  private:
    render::Window* window_;
    render::gl::Driver* driver_;
    render::DeferredRenderer* renderer_;
    IResourceLoaderDelegate& resource_loader_;
    std::atomic_bool run_;

  private:
    void wait_render_thread_() const;
    void prepare_frame_packet_(Game& game);

  public:
    template <typename T>
    using StackAllocator = render::DeferredRenderer::StackAllocator<T>;
    using FramePacket = render::DeferredRenderer::StackFramePacket;

    GameManager(IResourceLoaderDelegate& resource_loader);
    ~GameManager();
    void run();
    void render_loop();
    void simulation_loop();
};

}
