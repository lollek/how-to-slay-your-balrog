#include <cstring>

#include <iostream>

#include "Game.hh"
#include "Tables.hh"

using namespace std;

Game::Game(const string &savefile, unsigned seed) :
  savefile(savefile),
  seed(seed),
  graphics(NULL),
  player(NULL)
{
  this->graphics = new Graphics();
  graphics->drawSplashScreen(this->splashfile);
  this->createCharacter();
}

Game::~Game()
{
  delete this->graphics;
}

int Game::run()
{
  return 0;
}

int Game::createCharacter()
{
  int status = 0;
  status += graphics->clear();
  status += graphics->print(1, 2, "Name        :");
  status += graphics->print(1, 3, "Race        :");
  status += graphics->print(1, 4, "Sex         :");
  status += graphics->print(1, 5, "Class       :");
  for (int i = 0; i < Tables::num_races; ++i)
  {
    const int x = 2 + (15 * (i % 5));
    const int y = 21 + (i / 5);
    string line = "a) " + Tables::races[i].name;
    line[0] += i;
    status += graphics->print(x, y, line);
  }
  status += graphics->print(2, 20, "Choose a race (? for Help): ");
  status += graphics->refresh();

  int race = -1;
  while (race == -1)
  {
    string race_str;
    graphics->getStringInput(race_str, 1);
    if (race_str.length() == 1 && 'a' <= race_str[0] && 
        race_str[0] <= ('a' + Tables::num_races -1))
      race = race_str[0] - 'a';
  }

  status += graphics->print(15, 3, Tables::races[race].name);
  graphics->refresh();

  return status;
}
