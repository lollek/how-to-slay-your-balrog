#include <cstring>

#include <fstream>
#include <iostream>

#include "Game.hh"

using namespace std;

Game::Game(const string &savefile, unsigned seed) :
  savefile(savefile),
  seed(seed),
  graphics(NULL)
{
  if (!this->canReadWriteFile(this->scorefile) ||
      !this->canReadWriteFile(this->savefile))
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

bool Game::canReadWriteFile(const string &filename) const
{
  ifstream file(filename, ios::app);
  if (file)
  {
    file.close();
    return true;
  }
  else
  {
    cerr << "Unable to open \"" << filename
         << "\": " << strerror(errno) << '\n';
    return false;
  }
}
