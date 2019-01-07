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

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "debug.hpp"
#include "common.hpp"
#include "render/AResourceManager.hpp"
#include "render/Material.hpp"
#include "Game.hpp"
#include "Buffer.hpp"
#include "BufferPool.hpp"

namespace donkey
{

Game::Game(IResourceLoaderDelegate& resource_loader):
  window_("Pipelined rendering demo", 1600, 900),
  renderer_(window_),
  render_context_(window_.get_render_context())
{
  window_.make_current(render_context_);
  resource_loader.load(window_, scene_, renderer_);
  BufferPool::add_instance(10000000);
  BufferPool::add_instance(10000000);
  renderer_.start_render_thread();
}

Game::~Game()
{
}

void Game::prepare_frame_packet()
{
  signpost_start(0, 2, 0, 0, 0);
  Buffer& buffer = BufferPool::get_push_head();
  buffer.reset();
  void* ptr = buffer.allocate(sizeof(render::FramePacket<StackAllocator>));
  new (ptr) render::FramePacket<StackAllocator>(
      scene_.get_mesh_nodes(),
      scene_.get_camera_nodes(),
      scene_.get_directional_light_nodes());
  renderer_.frame_count++;
  BufferPool::next_push_head();
  renderer_.condition_variable.notify_one();
  signpost_end(0, 2, 0, 0, 0);
}

void Game::notify_exit()
{
  renderer_.notify_exit();
}

void Game::update(Duration elapsed_time)
{
  signpost_start(0, 0, 0, 0, 0);
  while (renderer_.frame_count - renderer_.render_frame_index > 1);
  signpost_end(0, 0, 0, 0, 0);
  signpost_start(0, 1, 0, 0, 0);
  const float rotation_speed = 50.0f;
  for (SceneNode& node: scene_.get_mesh_nodes())
  {
    if (node.pass_num == 0)
    {
      float angle = elapsed_time.count() * rotation_speed;
      node.angles.y += angle;
    }
  }
  signpost_end(0, 1, 0, 0, 0);
}

}
