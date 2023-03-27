#include <UI.h>
#include <curses.h>
#include <menu.h>
#include <string>
#include <vector>

std::vector<std::string> INVENTORY = {
  "I 1",
  "I 2",
  "I 3",
  "I 4",
  "I 5",
  "I 6",
  "I 7",
  "I 8",
  "I 9",
  "I 10"
};

void UI::GenerateUI()
{
  GenerateWindow(Main_Window,0,0,0,0);

  GenerateWindow(Actions_Window,4,COLS-2,LINES-5,1);

  GenerateWindow(Inventory_Window,LINES-(Actions_Window.getHeight())-2 ,20,1,1);

  ITEM **items;
  MENU* menu;
  items = new ITEM*[INVENTORY.size()];

  for(int i = 0;i<INVENTORY.size();i++)
  {
    items[i] = new_item(INVENTORY[i].c_str(), "");
  }
  items[INVENTORY.size()] = new_item(NULL, NULL);

  menu = new_menu((ITEM**)items) ;
  set_menu_win(menu, Inventory_Window.getWindow());
  set_menu_sub(menu, derwin(Inventory_Window.getWindow(), 10, 10, 3, 1) );
  set_menu_format(menu, 10, 1);
  post_menu(menu);

  wrefresh(Inventory_Window.getWindow());
  //Inventory_Window.Refresh();
  std::string str = "INVETORY";
  Inventory_Window.Print_Window_Title(str);

  //unpost_menu(menu);
  GenerateWindow(Status_Window,9,COLS-(Inventory_Window.getWidth())-2,1,(Inventory_Window.getWidth() + 1));
  
  GenerateWindow(Game_Window,LINES - (Status_Window.getHeight()+Actions_Window.getHeight()+2),(COLS-Inventory_Window.getWidth()-2),Status_Window.getHeight()+1,Inventory_Window.getWidth()+1);

  wgetch(Game_Window.getWindow());
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
