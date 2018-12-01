#include "common.hpp"
#include "Game.hpp"
#include "GameManager.hpp"

int main()
{
  GameManager game_manager;
  Game game;
  game_manager.run(game);
  return 0;
}
