#include <iostream>

#include <getopt.h>

#include "Game.hh"

using namespace std;

int main(int argc, char *argv[])
{
  int seed;
  int c;

  while ((c = getopt(argc, argv, "NnSsW:w:")) != -1)
    switch(c)
    {
      case 'N': case 'n':
        /* New game */
        cout << "Received option: New game\n";
        break;

      case 'S': case 's':
        /* Display Score */
        cout << "Displaying score and exiting\n";
        break;

      case 'W': case 'w':
        cout << "Entering wizard mode\n";
        /* Wizard mode */
        seed = stoi(optarg);
        if (seed == 0)
          cerr << "Bad seed received, ignoring\n";
        break;

      default: 
        cerr << "Usage: moria [-nsw] [savefile]\n";
        return 1;
    }

  Game game;
  return game.run();
}
