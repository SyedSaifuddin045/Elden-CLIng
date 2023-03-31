#include <ncurses.h>
#include <Window.h>
#include <Menu.h>

class UI{
  private:
  Window Main_Window,Actions_Window,Inventory_Window,Status_Window,Game_Window,Main_Menu_Window,Character_Select_Window;
  public:
  void GenerateUI(std::vector<std::string> Options,std::vector<std::string>INVENTORY);
  void GenerateWindow(Window &win,int height,int width,int start_y,int start_x);
  void GenerateMainMenu(std::vector<std::string> &Options);
  void GenerateCharaterSelect(std::vector<std::string> &Characters);
  void Clear();
};
