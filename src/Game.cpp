#include <sys/kdebug_signpost.h>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "common.hpp"
#include "render/ResourceManager.hpp"
#include "render/Material.hpp"
#include "Game.hpp"
#include "Buffer.hpp"
#include "BufferPool.hpp"

Game::Game(IResourceLoaderDelegate& resource_loader):
  window_("Pipelined rendering demo", 1280, 720),
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
  kdebug_signpost_start(0, 2, 0, 0, 0);
  Buffer& buffer = BufferPool::get_push_head();
  buffer.reset();
  void* ptr = buffer.allocate(sizeof(render::FramePacket<StackAllocator>));
  new (ptr) render::FramePacket<StackAllocator>(
      scene_.get_mesh_nodes(),
      scene_.get_camera_nodes());
  renderer_.frame_count++;
  BufferPool::next_push_head();
  renderer_.condition_variable.notify_one();
  kdebug_signpost_end(0, 2, 0, 0, 0);
}

void Game::notify_exit()
{
  renderer_.notify_exit();
}

void Game::update(Duration elapsed_time)
{
  kdebug_signpost_start(0, 0, 0, 0, 0);
  while (renderer_.frame_count - renderer_.render_frame_index > 1);
  kdebug_signpost_end(0, 0, 0, 0, 0);
  kdebug_signpost_start(0, 1, 0, 0, 0);
  const float rotation_speed = 50.0f;
  for (SceneNode& node: scene_.get_mesh_nodes())
  {
    if (node.pass_num == 0)
    {
      float angle = elapsed_time.count() * rotation_speed;
      node.angles.y += angle;
    }
  }
  kdebug_signpost_end(0, 1, 0, 0, 0);
}
