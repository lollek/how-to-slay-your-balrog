#include <clocale>
#include <iostream>
#include <fstream>

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

int Graphics::drawSplashScreen(std::string filename) const
{
  ifstream file(filename);
  if (!file)
    return 1;

  clear();

  char buf[81];
  int line_counter = 0;
  int status = 0;

  file.getline(buf, 81);
  while (file && line_counter < 25)
  {
    /* Escape bad chars */
    for (char c : buf)
      if (c == '\0' || c == '\t')
        c = ' ';
    status += mvaddstr(line_counter++, 0, buf);
    file.getline(buf, 81);
  }
  refresh();
  getch();

  return status;
}
