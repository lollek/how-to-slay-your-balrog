#ifndef __MORIA_GAME_HH__
#define __MORIA_GAME_HH__

#include <string>

#include "Graphics.hh"

class Game
{
  public:
    Game(const std::string &savefile, unsigned seed);
    Game(const Game&) = delete;
    void operator=(const Game&) = delete;

    ~Game();

    /* This works as a main() function for the game */
    int run();

  private:
    int createCharacter();

    const std::string scorefile = "../files/scores";
    const std::string splashfile = "../files/news";
    const std::string savefile;
    const unsigned seed;

    Graphics *graphics;
};

#endif //__MORIA_GAME_HH__
