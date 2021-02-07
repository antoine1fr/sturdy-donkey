/* Copyright (C) 2018 Antoine Luciani
 *
 * This file is part of Project Small Fawn.
 *
 * Project Small Fawn is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by the
 * Free Software Foundation, version 3.
 *
 * Project Small Fawn is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * Project Small Fawn. If not, see <https://www.gnu.org/licenses/>.
 */

#include <random>
#include <string>
#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include <render/Material.hpp>
#include <MeshLoader.hpp>
#include "ResourceLoaderDelegate.hpp"

void ResourceLoaderDelegate::load_render_resources(
    donkey::render::Window* window,
    donkey::render::ResourceManager* resource_manager,
    donkey::render::GpuResourceManager* gpu_resource_manager)
{
}

void ResourceLoaderDelegate::load_game_objects(donkey::Scene& scene)
{
  // create first pass' scene nodes
  scene.create_perspective_camera_node(
      0,
      45.0f,
      0.1f,
      1000.0f,
      glm::vec3(0.0f, 0.0f, 90.0f),
      glm::vec3(0.0f, 0.0f, 0.0f),
      glm::tvec2<int>(0, 0),
      glm::tvec2<GLsizei>(width_, height_));
}
