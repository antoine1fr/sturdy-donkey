#include <Game.hpp>
#include <GameManager.hpp>
#include "ResourceLoaderDelegate.hpp"

int main()
{
  donkey::GameManager game_manager;
  ResourceLoaderDelegate resource_loader;
  donkey::Game game(resource_loader);
  game_manager.run(game);
  return 0;
}
