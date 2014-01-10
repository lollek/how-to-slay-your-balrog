#include <cstring>

#include <fstream>
#include <iostream>

#include "Game.hh"

using namespace std;

Game::Game(string savefile, unsigned seed) :
  savefile(savefile),
  seed(seed),
  graphics(NULL)
{
  if (!this->canOpenScorefile())
    exit(1);
  this->graphics = new Graphics();
  graphics->drawSplashScreen(this->splashfile);
}

Game::~Game()
{
  delete this->graphics;
}

int Game::run()
{
  return 0;
}

bool Game::canOpenScorefile() const
{
  ifstream scorefile(this->scorefile, ios::app);
  if (scorefile)
  {
    scorefile.close();
    return true;
  }
  else
  {
    cerr 
      << "Unable to open score file \"" << this->scorefile
      << "\": " << strerror(errno) << '\n';
    return 1;
  }
}
