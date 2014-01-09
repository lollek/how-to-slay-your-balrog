#include <clocale>
#include <iostream>

#include "Graphics.hh"

using namespace std;

Graphics::Graphics() :
  stdscr(NULL)
{
  setlocale(LC_ALL, "");

  this->stdscr = initscr();
  if (this->stdscr == NULL)
  {
    cerr << "Error allocating curses screen (function=initscr)\n";
    exit(1);
  }

  int maxx, maxy;
  getmaxyx(this->stdscr, maxy, maxx);
  if (maxx < 80 || maxy < 24)
  {
    cerr << "Screen is too small for moria\n";
    exit(1);
  }

  noecho();
  cbreak();
  /* curs_set(0); // Cursor still exists in moria */
  /* start_color() //I want to add this
   *    init_pair(3, COLOR_YELLOW, COLOR_BLACK);
   *    init_pair(4, COLOR_BLUE, COLOR_BLACK);
   */

}

Graphics::~Graphics() 
{
  nocbreak();
  echo();
  endwin();
}


