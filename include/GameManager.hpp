#pragma once

#include "Game.hpp"

namespace donkey
{

class GameManager
{
  public:
    GameManager();
    ~GameManager();
    void run(Game& game);
};

}
