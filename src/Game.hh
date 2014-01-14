#ifndef __MORIA_GAME_HH__
#define __MORIA_GAME_HH__

#include <string>

#include "Graphics.hh"
#include "Player.hh"

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
    void createCharacter();
    void CreateSub_selectRace();
    void CreateSub_selectSex();
    void CreateSub_generatePlayer();
    void CreateSub_printBackground() const;
    void CreateSub_printSocialStats() const;
    void CreateSub_printFightingStats() const;
    void CreateSub_selectJob();
    void CreateSub_addStartingMoney();

    const std::string scorefile = "../files/scores";
    const std::string splashfile = "../files/news";
    const std::string savefile;
    const unsigned seed;

    Graphics *graphics;
    Player *player;
};

#endif //__MORIA_GAME_HH__
