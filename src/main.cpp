#include <GL/gl3w.h>
#include <SDL.h>
#include <iostream>
#include <string>

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
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
  SDL_GL_CreateContext(window);
  gl3wInit();
  return window;
}

int main()
{
  SDL_Window* window = setup_video();
  bool run = true;
  SDL_Event event;

  while (run)
  {
    while (SDL_PollEvent(&event))
    {
      if (event.type == SDL_QUIT)
        run = false;
    }

    glClearColor(0.0f, 1.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    SDL_GL_SwapWindow(window);
  }
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
