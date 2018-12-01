#pragma once

#include <thread>
#include <SDL.h>

#include "common.hpp"
#include "Scene.hpp"
#include "render/DeferredRenderer.hpp"
#include "render/Window.hpp"
#include "IResourceLoaderDelegate.hpp"

class Game
{
  private:
    Scene scene_;
    render::Window window_;
    render::DeferredRenderer renderer_;
    SDL_GLContext  render_context_;

  public:
    Game(IResourceLoaderDelegate& resourceLoader);
    ~Game();
    void prepare_frame_packet();
    void update(Duration elapsed_time);
    void notify_exit();
};
