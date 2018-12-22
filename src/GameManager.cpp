/* Copyright (C) 2018 Antoine Luciani
 *
 * This file is part of Sturdy Donkey.
 *
 * Sturdy Donkey is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, version 3.
 *
 * Sturdy Donkey is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * Sturdy Donkey. If not, see <https://www.gnu.org/licenses/>.
 */

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
