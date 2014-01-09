#ifndef __MORIA_GRAPHICS_HH__
#define __MORIA_GRAPHICS_HH__

#include <string>

#include <curses.h>

class Graphics
{
  public:
    Graphics();
    Graphics(const Graphics&) = delete;
    void operator=(const Graphics&) = delete;

    ~Graphics();

    int drawSplashScreen(std::string filename) const;

  private:
    WINDOW *stdscr;
};

#endif //__MORIA_GRAPHICS_HH__
