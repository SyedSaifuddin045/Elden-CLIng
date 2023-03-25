#include <ncurses.h>

class Window{
  private:
    WINDOW *window;
    int width,height,pos_x,pos_y;
  public:
    Window()
      :width(COLS),height(LINES),pos_x(0),pos_y(0)
    {
      window = newwin(height, width, pos_y, pos_x);
    }
    Window(int _Height,int _Width,int _PosY,int _PosX)
      :height(_Height),width(_Width),pos_x(_PosX),pos_y(_PosY)
    {
      window = newwin(height, width, pos_y, pos_x);
    }
    void Box(chtype Vertical,chtype Horizontal)
    {
      box(window,Vertical,Horizontal);
    }
    void Refresh()
    {
      wrefresh(window);
    }
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    int getPosX() const { return pos_x; }
    int getPosY() const { return pos_y; }
    WINDOW* getWindow() const { return window;}
};
