#pragma once

#include <SDL.h>
#include <string>

namespace donkey
{

namespace render
{

class Window
{
  private:
    SDL_Window* window_;
    int width_;
    int height_;
    SDL_GLContext render_context_;
    SDL_GLContext ancillary_context_;

  public:
    Window(const std::string& title, int width, int height);
    ~Window();
    SDL_GLContext& get_render_context();
    SDL_GLContext& get_ancillary_context();
    void make_current(SDL_GLContext context) const;
    int get_width() const;
    int get_height() const;
    void swap();
};

}
}
