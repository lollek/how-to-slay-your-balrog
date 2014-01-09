#include <cstring>

#include <fstream>
#include <iostream>

#include "Game.hh"

using namespace std;

Game::Game() :
  graphics()
{
  if (!this->canOpenScorefile())
    exit(1);
  graphics.drawSplashScreen(this->splashfile);
}

Game::~Game() {}

int Game::run()
{
  return 0;
}

bool Game::canOpenScorefile() const
{
  ifstream scorefile(this->scorefile);
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
