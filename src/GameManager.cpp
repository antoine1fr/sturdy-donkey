#include <SDL.h>
#include <SDL_image.h>
#include <chrono>

#include "GameManager.hpp"

namespace donkey
{

GameManager::GameManager()
{
  assert(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) == 0);
  int img_flags = IMG_INIT_PNG;
  int bit_mask = IMG_Init(img_flags);
  assert((bit_mask & img_flags) == img_flags);
}

GameManager::~GameManager()
{
  SDL_Quit();
}

void GameManager::run(Game& game)
{
  bool run = true;
  SDL_Event event;
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

}
