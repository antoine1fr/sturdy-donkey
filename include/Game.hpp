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

#include <thread>
#include <SDL.h>

#include "common.hpp"
#include "Scene.hpp"
#include "render/DeferredRenderer.hpp"
#include "render/Window.hpp"
#include "IResourceLoaderDelegate.hpp"

namespace donkey
{

class Game
{
  private:
    Scene scene_;
    render::Window window_;
    render::DeferredRenderer renderer_;
    SDL_GLContext  render_context_;

  private:
    void wait_render_thread_() const;

  public:
    Game(IResourceLoaderDelegate& resourceLoader);
    ~Game();
    void prepare_frame_packet();
    void update(Duration elapsed_time);
    void notify_exit();
};

}
