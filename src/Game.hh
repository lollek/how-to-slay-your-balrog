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

    /* This works as a main() function for the game */
    int run();

  private:
    const std::string scorefile = "../files/scores";
    const std::string splashfile = "../files/news";
    Graphics graphics;

    /* Checks if we can read/write to scorefile */
    bool canOpenScorefile() const;
};

#endif //__MORIA_GAME_HH__
