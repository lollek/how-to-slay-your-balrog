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

    int print(int x, int y, const std::string &line) const;
    int println(int x, int y, const std::string &line) const;
    int clear() const;
    int refresh() const;
    int clear_from(int line) const;
    void getStringInput(std::string &line, int max) const;
    int drawSplashScreen(const std::string &filename) const;

  private:
    WINDOW *stdscr;
};

#endif //__MORIA_GRAPHICS_HH__
