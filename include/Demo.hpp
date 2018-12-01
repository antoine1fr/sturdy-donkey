#pragma once

#include <thread>
#include <SDL.h>

#include "common.hpp"
#include "Scene.hpp"
#include "render/DeferredRenderer.hpp"
#include "render/Window.hpp"

class Demo
{
  private:
    Scene scene_;
    render::Window window_;
    render::DeferredRenderer renderer_;
    SDL_GLContext  render_context_;

  private:
    void initialize_resources_(int width, int height);

  public:
    Demo();
    ~Demo();
    void prepare_frame_packet();
    void update(Duration elapsed_time);
    void notify_exit();
};
