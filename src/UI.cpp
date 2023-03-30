#include <UI.h>
#include <menu.h>
#include <string>
#include <vector>
void UI::GenerateUI(std::vector<std::string> Options,std::vector<std::string>INVENTORY)
{
  GenerateWindow(Main_Window,0,0,0,0);

  GenerateWindow(Actions_Window,8,COLS-2,LINES-9,1);
  Menu action_menu;
  action_menu.Create(Actions_Window.getWindow(),1,1,6,20,Options);
  Actions_Window.setMenu(action_menu);
  GenerateWindow(Inventory_Window,LINES-(Actions_Window.getHeight())-2 ,20,1,1);

  Menu inventory_menu;
  inventory_menu.Create(Inventory_Window.getWindow(), 2, 1, Inventory_Window.getHeight() -4, Inventory_Window.getWidth()-2, INVENTORY);
  Inventory_Window.setMenu(inventory_menu);

  std::string str = "INVENTORY";
  Inventory_Window.Print_Window_Title(str);

  GenerateWindow(Status_Window,9,COLS-(Inventory_Window.getWidth())-2,1,(Inventory_Window.getWidth() + 1));

  GenerateWindow(Game_Window,LINES - (Status_Window.getHeight()+Actions_Window.getHeight()+2),(COLS-Inventory_Window.getWidth()-2),Status_Window.getHeight()+1,Inventory_Window.getWidth()+1);

  wgetch(Game_Window.getWindow());

  inventory_menu.Destroy();
}

void UI::GenerateWindow(Window &win,int height,int width,int start_y,int start_x)
{
  if(height ==0 && width ==0 && start_x == 0 && start_y ==0)
    win = Window();
  else {
    win = Window(height,width,start_y,start_x);
  }
  wattron(win.getWindow(), COLOR_PAIR(1));
  win.Box(0, 0);
  wattroff(win.getWindow(), COLOR_PAIR(1));
  win.Refresh();
}

void UI::GenerateMainMenu(){

}
