#ifndef __MORIA_GAME_HH__
#define __MORIA_GAME_HH__

#include <string>

#include "Graphics.hh"

class Game
{
  public:
    Game();
    Game(const Game&) = delete;

    ~Game();

    int run();

  private:
    const std::string scorefile = "../files/scores";
    Graphics graphics;

    bool canOpenScorefile() const;
};

#endif //__MORIA_GAME_HH__
