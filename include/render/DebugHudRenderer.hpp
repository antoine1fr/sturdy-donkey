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

#include "IResourceLoaderDelegate.hpp"
#include "render/Window.hpp"
#include "render/gl/Driver.hpp"
#include "render/CommandBucket.hpp"

namespace donkey {
namespace render {

class DebugHudRenderer
{
  public:
    DebugHudRenderer(
        Window* window,
        gl::Driver* driver,
        ResourceManager* resource_manager);
    ~DebugHudRenderer();

    void render(CommandBucket& render_commands);

  private:
    gl::Driver* driver_;
    ResourceManager* resource_manager_;
};

}
}
