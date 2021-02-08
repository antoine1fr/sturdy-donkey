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

#pragma warning(push)
#pragma warning(disable : 26812 26819)
#include <SDL.h>
#include <SDL_image.h>
#pragma warning(pop)
#include <chrono>

#include "GameManager.hpp"

namespace donkey
{

GameManager::GameManager(IResourceLoaderDelegate& resource_loader):
  resource_loader_(resource_loader),
  simulated_frame_count_(0),
  rendered_frame_count_(0)
{
  assert(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) == 0);
  int img_flags = IMG_INIT_PNG;
  int bit_mask = IMG_Init(img_flags);
  assert((bit_mask & img_flags) == img_flags);
  window_ = new render::Window("Pipelined rendering demo", 1600, 900);
  driver_ = new render::gl::Driver;
  render::Window::Context render_context = window_->get_render_context();
  window_->make_current(render_context);
  resource_manager_ =
    new render::ResourceManager(driver_->get_resource_manager());
  renderer_ = new render::DeferredRenderer(window_, driver_,
      resource_manager_);
  resource_loader.load_render_resources(window_, resource_manager_,
                                        &(driver_->get_resource_manager()));
  window_->free_context();
  simulation_modules_.push_back(new Game(resource_loader_));
}

GameManager::~GameManager()
{
  for (auto simulation_module : simulation_modules_)
  {
    delete simulation_module;
  }
  delete renderer_;
  resource_manager_->cleanup();
  delete resource_manager_;
  delete driver_;
  delete window_;
  SDL_Quit();
}

size_t GameManager::wait_for_frame_packet_()
{
  size_t simulated_frame_count;
  size_t rendered_frame_count =
    rendered_frame_count_.load(std::memory_order_relaxed);
  do
  {
    std::this_thread::yield();
    simulated_frame_count = get_simulated_frame_count_();
  } while (rendered_frame_count == simulated_frame_count);
  return rendered_frame_count;
}

void GameManager::render_loop()
{
  render::Window::Context render_context = window_->get_render_context();
  window_->make_current(render_context);
  while (run_.load(std::memory_order_relaxed))
  {
    // Wait for simulation to produce a frame packet.
    size_t rendered_frame_count = wait_for_frame_packet_();
    size_t frame_packet_id = rendered_frame_count % 2;
    FramePacket* frame_packet =
      FramePacket::frame_packets[frame_packet_id];
    frame_packet->sort_mesh_nodes();
    render::CommandBucket render_commands;
    renderer_->render(frame_packet, render_commands);
    driver_->execute_commands(render_commands);
    window_->swap();
    increment_rendered_frame_count_();
  }
}

void GameManager::simulation_loop()
{
  SDL_Event event;
  auto last_time = Clock::now();
  Game game(resource_loader_);


  while (run_.load(std::memory_order_relaxed))
  {
    auto time = Clock::now();
    Duration elapsed_time = time - last_time;
    last_time = time;

    while (SDL_PollEvent(&event))
    {
      if (event.type == SDL_QUIT)
      {
        run_.store(false, std::memory_order_relaxed);
      }
    }

    update_simulation_(elapsed_time);
    prepare_frame_packet_(game);
  }
}

void GameManager::update_simulation_(Duration elapsed_time)
{
  for (auto simulation_module : simulation_modules_)
  {
    simulation_module->update(elapsed_time);
  }
}

void GameManager::wait_render_thread_() const
{
  size_t simulated_frame_count = get_simulated_frame_count_();
  size_t rendered_frame_count;
  bool run = run_.load(std::memory_order_relaxed);
  do
  {
    std::this_thread::yield();
    rendered_frame_count = get_rendered_frame_count_();
  } while (run && (rendered_frame_count < simulated_frame_count));
}

void GameManager::prepare_frame_packet_(Game& game)
{
  size_t simulated_frame_count = get_simulated_frame_count_relaxed_();
  size_t frame_packet_id = simulated_frame_count % 2;
  if (FramePacket::frame_packets[frame_packet_id] != nullptr)
  {
    BufferPool::get_instance()->free_tag(Buffer::Tag::kFramePacket,
        frame_packet_id);
  }
  StackAllocator<FramePacket> allocator(Buffer::Tag::kFramePacket,
      frame_packet_id);
  FramePacket* frame_packet = allocator.allocate(1);
  for (auto simulation_module : simulation_modules_)
  {
    simulation_module->prepare_frame_packet(frame_packet, allocator);
  }
  FramePacket::frame_packets[frame_packet_id] = frame_packet;
  wait_render_thread_();
  increment_simulated_frame_count_();
}

void GameManager::run()
{
  run_.store(true, std::memory_order_relaxed);
  std::thread thread([this]() { render_loop(); });
  simulation_loop();
  thread.join();
}

size_t GameManager::get_rendered_frame_count_() const
{
  return rendered_frame_count_.load(std::memory_order_acquire);
}

size_t GameManager::get_simulated_frame_count_() const
{
  return simulated_frame_count_.load(std::memory_order_acquire);
}

size_t GameManager::get_simulated_frame_count_relaxed_() const
{
  return simulated_frame_count_.load(std::memory_order_relaxed);
}

void GameManager::increment_rendered_frame_count_()
{
  rendered_frame_count_.fetch_add(1);
}

void GameManager::increment_simulated_frame_count_()
{
  simulated_frame_count_.fetch_add(1);
}

}
