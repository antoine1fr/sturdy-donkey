#pragma once

#include "Game.hpp"

class GameManager
{
  public:
    GameManager();
    ~GameManager();
    void run(Game& game);
};
