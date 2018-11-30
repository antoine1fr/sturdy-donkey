#include <GL/gl3w.h>
#include <SDL.h>
#include <SDL_image.h>
#include <chrono>
#include <fstream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <iostream>
#include <string>

#include "common.hpp"
#include "Demo.hpp"

SDL_Window* setup_video()
{
  SDL_Window* window = SDL_CreateWindow("Pipelined rendering demo", 0, 0, 800,
    720, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS,
    SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
    SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
  SDL_GL_CreateContext(window);
  gl3wInit();
  return window;
}

int main()
{
  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
  IMG_Init(IMG_INIT_PNG);
  SDL_Window* window = setup_video();
  bool run = true;
  SDL_Event event;
  int width, height;
  SDL_GL_GetDrawableSize(window, &width, &height);
  {
    Demo demo(width, height);
    auto last_time = Clock::now();

    while (run)
    {
      auto time = Clock::now();
      Duration elapsed_time = time - last_time;
      last_time = time;

      while (SDL_PollEvent(&event))
      {
        if (event.type == SDL_QUIT)
          run = false;
      }

      demo.update(elapsed_time);
      demo.render();
      SDL_GL_SwapWindow(window);
    }
  }

  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
