#include <ncurses.h>
#include <Window.h>

class UI{
  private:
  Window Main_Window,Actions_Window,Inventory_Window,Status_Window,Game_Window;
  public:
  void GenerateUI();
  void GenerateWindow(Window &win,int height,int width,int start_y,int start_x);
};
