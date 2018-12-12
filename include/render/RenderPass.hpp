#pragma once

namespace donkey
{

namespace render
{

struct RenderPass
{
  uint32_t framebuffer_id;
  GLint clear_bits;
  glm::vec3 clear_color;
  bool depth_test;
  bool lighting; // loop through lights if true
};

}
}
