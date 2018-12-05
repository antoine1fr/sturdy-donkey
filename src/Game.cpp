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
  render_context_(window_.get_ancillary_context())
{
  window_.make_current(render_context_);
  resource_loader.load(window_, scene_, renderer_);
  BufferPool::add_instance(10000000);
  BufferPool::add_instance(10000000);
}

Game::~Game()
{
}

void Game::prepare_frame_packet()
{
  Buffer& buffer = BufferPool::get_push_head();
  buffer.reset();
  void* ptr = buffer.allocate(sizeof(render::FramePacket));
  new (ptr) render::FramePacket(
      scene_.get_mesh_nodes(),
      scene_.get_camera_nodes());
  renderer_.frame_count++;
  BufferPool::next_push_head();
  renderer_.condition_variable.notify_one();
}

void Game::notify_exit()
{
  renderer_.notify_exit();
}

void Game::update(Duration elapsed_time)
{
  while (renderer_.frame_count - renderer_.render_frame_index > 1);
  const float rotation_speed = 50.0f;
  for (SceneNode& node: scene_.get_mesh_nodes())
  {
    if (node.pass_num == 0)
    {
      float angle = elapsed_time.count() * rotation_speed;
      node.angles.y += angle;
    }
  }
}
