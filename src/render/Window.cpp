#include <GL/gl3w.h>
#include "render/Window.hpp"

namespace render
{

Window::Window(const std::string& title, int width, int height):
  width_(width),
  height_(height)
{
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS,
    SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
  SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 1);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
    SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
  window_ = SDL_CreateWindow(title.c_str(), 0, 0, width,
    height, SDL_WINDOW_OPENGL);
  render_context_ = SDL_GL_CreateContext(window_);
  ancillary_context_ = SDL_GL_CreateContext(window_);
  gl3wInit();
}

Window::~Window()
{
  SDL_GL_DeleteContext(render_context_);
  SDL_GL_DeleteContext(ancillary_context_);
  SDL_DestroyWindow(window_);
}

SDL_GLContext& Window::get_render_context()
{
  return render_context_;
}

SDL_GLContext& Window::get_ancillary_context()
{
  return ancillary_context_;
}

int Window::get_width() const
{
  return width_;
}

int Window::get_height() const
{
  return height_;
}

void Window::make_current(SDL_GLContext context) const
{
  SDL_GL_MakeCurrent(window_, context);
}

void Window::swap()
{
  SDL_GL_SwapWindow(window_);
}

}