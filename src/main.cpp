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
#include "Game.hpp"

int main()
{
  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
  IMG_Init(IMG_INIT_PNG);
  bool run = true;
  SDL_Event event;
  {
    Game game;
    auto last_time = Clock::now();

    while (run)
    {
      auto time = Clock::now();
      Duration elapsed_time = time - last_time;
      last_time = time;

      while (SDL_PollEvent(&event))
      {
        if (event.type == SDL_QUIT)
        {
          run = false;
          game.notify_exit();
        }
      }

      game.update(elapsed_time);
      game.prepare_frame_packet();
    }
  }

  SDL_Quit();
  return 0;
}
