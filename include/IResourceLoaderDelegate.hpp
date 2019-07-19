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

#include "render/Window.hpp"
#include "render/AResourceManager.hpp"
#include "render/ResourceManager.hpp"
#include "Scene.hpp"

namespace donkey
{

class IResourceLoaderDelegate
{
  public:
    virtual void load_game_objects(Scene& scene) = 0;
    virtual void load_render_resources(
        render::Window* window,
        render::ResourceManager* resource_manager,
        render::AResourceManager* gpu_resource_manager) = 0;
};

}
