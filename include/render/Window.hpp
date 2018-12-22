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

#include <SDL.h>
#include <string>

namespace donkey
{

namespace render
{

class Window
{
  private:
    SDL_Window* window_;
    int width_;
    int height_;
    SDL_GLContext render_context_;
    SDL_GLContext ancillary_context_;

  public:
    Window(const std::string& title, int width, int height);
    ~Window();
    SDL_GLContext& get_render_context();
    SDL_GLContext& get_ancillary_context();
    void make_current(SDL_GLContext context) const;
    int get_width() const;
    int get_height() const;
    void swap();
};

}
}
