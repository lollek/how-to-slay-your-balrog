#ifndef __MORIA_GRAPHICS_HH__
#define __MORIA_GRAPHICS_HH__

#include <curses.h>

class Graphics
{
  public:
    Graphics();
    Graphics(const Graphics&) = delete;
    void operator=(const Graphics&) = delete;

    ~Graphics();

  private:
    WINDOW *stdscr;
};

#endif //__MORIA_GRAPHICS_HH__
