#include <clocale>
#include <iostream>
#include <fstream>
#include <cstring>

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
  keypad(stdscr, true);
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

int Graphics::print(int x, int y, const string &filename) const
{
  return mvaddstr(y, x, filename.c_str());
}

int Graphics::clear() const
{
  return wclear(this->stdscr);
}

int Graphics::refresh() const
{
  return wrefresh(this->stdscr);
}

void Graphics::getStringInput(string &line, int max) const
{
  line.clear();
  for (int i = 0; i < max; ++i)
    line += ' ';

  int orig_x, orig_y;
  int i = 0;
  int c;

  getyx(this->stdscr, orig_y, orig_x);
  do
  {
    c = wgetch(this->stdscr);
    if (i < max && (
        ('a' <= c && c <= 'z') ||
        ('A' <= c && c <= 'Z') ||
        ('1' <= c && c <= '9') ))
    {
      line[i++] = c;
      move(orig_y, orig_x);
      addstr(line.c_str());
    }
    else if (c == KEY_BACKSPACE && i > 0)
    {
      line[--i] = ' ';
      move(orig_y, orig_x);
      addstr(line.c_str());
      move(orig_y, orig_x);
    }
  } while (c != KEY_ENTER && c != '\n');

  unsigned len = line.find_first_of(" ");
  if (len != string::npos)
    line = line.substr(0, len);

  move(orig_y, orig_x);
}

int Graphics::drawSplashScreen(const std::string &filename) const
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
    for (unsigned i = 0; buf[i] != '\0'; ++i)
      if (buf[i] == '\t')
        buf[i] = ' ';

    status += mvaddstr(line_counter++, 0, buf);
    file.getline(buf, 81);
  }
  strcpy(buf, "[Press any key to continue.]");
  status += mvaddstr(24, 40 -strlen(buf)/2, buf);
  refresh();
  getch();

  return status;
}
