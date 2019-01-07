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
#include "StackAllocator.hpp"

namespace donkey
{

Game::Game(IResourceLoaderDelegate& resource_loader):
  window_("Pipelined rendering demo", 1600, 900),
  renderer_(window_),
  render_context_(window_.get_render_context())
{
  window_.make_current(render_context_);
  resource_loader.load(window_, scene_, renderer_);
  renderer_.start_render_thread();
}

Game::~Game()
{
}

void Game::wait_render_thread_() const
{
  size_t simulated_frame_count = renderer_.get_simulated_frame_count();
  size_t rendered_frame_count;
  do
  {
    rendered_frame_count = renderer_.get_rendered_frame_count();
  } while (rendered_frame_count < simulated_frame_count);
}

template <typename T>
using StackAllocator_ = render::DeferredRenderer::StackAllocator<T>;
using StackFramePacket = render::DeferredRenderer::StackFramePacket;

void Game::prepare_frame_packet()
{
  signpost_start(0, 2, 0, 0, 0);
  size_t frame_packet_id = renderer_.get_simulated_frame_count_relaxed() % 2;
  if (StackFramePacket::frame_packets[frame_packet_id] != nullptr)
  {
    BufferPool::get_instance()->free_tag(Buffer::Tag::kFramePacket,
        frame_packet_id);
  }
  StackAllocator_<StackFramePacket> allocator(Buffer::Tag::kFramePacket,
      frame_packet_id);
  StackFramePacket* frame_packet = allocator.allocate(1);
  new (frame_packet) StackFramePacket(
    scene_.get_mesh_nodes(),
    scene_.get_camera_nodes(),
    scene_.get_directional_light_nodes(),
    allocator);
  StackFramePacket::frame_packets[frame_packet_id] = frame_packet;
  wait_render_thread_();
  renderer_.increment_simulated_frame_count();
  signpost_end(0, 2, 0, 0, 0);
}

void Game::notify_exit()
{
  renderer_.notify_exit();
}

void Game::update(Duration elapsed_time)
{
  signpost_start(0, 0, 0, 0, 0);
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
