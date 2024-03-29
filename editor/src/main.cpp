/* Copyright (C) 2018 Antoine Luciani
 *
 * This file is part of Project Small Fawn.
 *
 * Project Small Fawn is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by the
 * Free Software Foundation, version 3.
 *
 * Project Small Fawn is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * Project Small Fawn. If not, see <https://www.gnu.org/licenses/>.
 */

#define SDL_MAIN_HANDLED

#if defined(MSVC)
# include <direct.h>
#else
# include <unistd.h>
#endif

#include <Game.hpp>
#include <GameManager.hpp>
#include <iostream>

#include "ResourceLoaderDelegate.hpp"

int main() {
#if defined(MSVC)
  const char* cwd = _getcwd(nullptr, 0);
#else
  const char* cwd = getcwd(nullptr, 0);
#endif
  std::cout << "CWD: '" << cwd << "'\n";
  delete cwd;

  ResourceLoaderDelegate resource_loader;
  donkey::GameManager game_manager(resource_loader);
  game_manager.run();
  return 0;
}
