#include <Random.h>
#include <Game.h>
#include <UI.h>
#include <curses.h>
#include <string>
void UI::GeneratePlayGameUI(std::vector<std::string> Options,std::vector<std::string>INVENTORY)
{
  GenerateWindow(Main_Window,0,0,0,0);

  GenerateWindow(Actions_Window,8,COLS-2,LINES-9,1);

  Menu action_menu;
  action_menu.Create(Actions_Window.getWindow(),1,1,6,20,Options);
  Actions_Window.setMenu(action_menu);
  GenerateWindow(Inventory_Window,LINES-(Actions_Window.getHeight())-2 ,20,1,1);

  Menu inventory_menu;
  inventory_menu.Create(Inventory_Window.getWindow(), 3, 1, Inventory_Window.getHeight() -4, Inventory_Window.getWidth()-2, INVENTORY);
  Inventory_Window.setMenu(inventory_menu);

  std::string str = "INVENTORY";
  Inventory_Window.Print_Window_Title(str);

  GenerateWindow(Status_Window,9,COLS-(Inventory_Window.getWidth())-2,1,(Inventory_Window.getWidth() + 1));
  GenerateStatusWindow();

  GenerateWindow(Game_Window,LINES - (Status_Window.getHeight()+Actions_Window.getHeight()+2),(COLS-Inventory_Window.getWidth()-2),Status_Window.getHeight()+1,Inventory_Window.getWidth()+1);

  Game::game_row = Random::Random_Number(5, (Game_Window.getHeight() /2 )-2 )*2;
  Game::game_col = Random::Random_Number(5, (Game_Window.getWidth() /2) -2)*2;
  Generate_Grid(Game_Window.getWindow(), Game::game_row, Game::game_col);

  if(Game_Window.getWindow())
  {
    //curs_set(0);
    wgetch(Game_Window.getWindow());
  }

  inventory_menu.Destroy();
  action_menu.Destroy();
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

void UI::GenerateMainMenu(std::vector<std::string> &Options){
  GenerateWindow(Main_Menu_Window,Options.size()+2,12,(LINES-Options.size())/2,(COLS-Options[1].size())/2 );

  Menu main_menu;
  main_menu.Create(Main_Menu_Window.getWindow(), 1, 0, Main_Menu_Window.getHeight()-2, Main_Menu_Window.getWidth(), Options);
  Main_Menu_Window.setMenu(main_menu);
}

void UI::GenerateCharaterSelect(std::vector<std::string> &Characters)
{
  Main_Menu_Window.DestroyWindow();
  GenerateWindow(Character_Select_Window, Characters.size() + 2, Characters[1].size()+3, (LINES - Characters.size()) / 2, (COLS - Characters[1].size())/2);

  Menu Character_menu;
  Character_menu.Create(Character_Select_Window.getWindow(), 1, 0, Character_Select_Window.getHeight()-2, Character_Select_Window.getWidth(), Characters);
  Character_Select_Window.setMenu(Character_menu);
}

void UI::Clear(){
  for(auto win : { Main_Window,Actions_Window,Inventory_Window,Status_Window,Game_Window,Main_Menu_Window,Character_Select_Window})
  {
    //win.DestroyMenu();
    win.DestroyWindow();
  }
}

void UI::GenerateEndScreen()
{
  GenerateWindow(End_Screen,3,COLS-2,(LINES/2) - 1,1);
  std::string str = "YOU DIED !";
  End_Screen.Print_Window_Title(str);
}

void UI::Generate_Grid(WINDOW* window, int row, int col) {
  int col_begin = ((Game_Window.getWidth()/2) - (col /2));
  int row_begin = ((Game_Window.getHeight()/2) - (row/2));

  wattron(window, COLOR_PAIR(1));

  // Draw top and bottom borders
  mvwhline(window, row_begin, col_begin, ACS_HLINE, col);
  mvwhline(window, row_begin + row, col_begin, ACS_HLINE, col);

  // Draw left and right borders
  mvwvline(window, row_begin, col_begin, ACS_VLINE, row );
  mvwvline(window, row_begin, col_begin + col, ACS_VLINE, row);

  mvwaddch(window, row_begin, col_begin, ACS_ULCORNER);
  mvwaddch(window, row_begin + row, col_begin, ACS_LLCORNER);
  mvwaddch(window, row_begin, col_begin + col, ACS_URCORNER);
  mvwaddch(window, row_begin + row, col_begin + col, ACS_LRCORNER);

  wattroff(window, COLOR_PAIR(1));

  // Draw horizontal lines
  for (int i = 1; i < row; i += 2) {
    if(i == 1 || i == row -1) {
      wattron(window, COLOR_PAIR(1));
      mvwaddch(window, row_begin+i, col_begin, ACS_LTEE);
      mvwhline(window, row_begin + i, col_begin + 1, ACS_HLINE, col - 2);
      mvwaddch(window, row_begin+i, col_begin+col, ACS_RTEE);
      wattroff(window, COLOR_PAIR(1)); 
    }
    else {
      wattron(window, COLOR_PAIR(1));
      mvwaddch(window, row_begin+i, col_begin, ACS_LTEE);
      wattroff(window, COLOR_PAIR(1)); 
      mvwhline(window, row_begin + i, col_begin + 1, ACS_HLINE, col - 2);
      wattron(window, COLOR_PAIR(1));
      mvwaddch(window, row_begin+i, col_begin+col, ACS_RTEE);
      wattroff(window, COLOR_PAIR(1)); 
    }
  }

  // Draw vertical lines
  for (int i = 1; i < col; i += 2) {
    if(i == 1 || i == col -1) {
      wattron(window, COLOR_PAIR(1));
      mvwaddch(window, row_begin, col_begin+i, ACS_TTEE);
      mvwvline(window, row_begin + 1, col_begin + i, ACS_VLINE, row - 2);
      mvwaddch(window, row_begin+row, col_begin+i, ACS_BTEE);
      wattroff(window, COLOR_PAIR(1));
    }
    else {
      wattron(window, COLOR_PAIR(1));
      mvwaddch(window, row_begin, col_begin+i, ACS_TTEE);
      wattroff(window, COLOR_PAIR(1));
      mvwvline(window, row_begin + 1, col_begin + i, ACS_VLINE, row - 2);
      wattron(window, COLOR_PAIR(1));
      mvwaddch(window, row_begin+row, col_begin+i, ACS_BTEE);
      wattroff(window, COLOR_PAIR(1));
    }
  }

// Draw intersections
for (int i = 1; i < row; i += 2) {
  for (int j = 1; j < col; j += 2) {
    if(i == 1 || i == row-1 || j == 1 || j == col-1)
    {
      wattron(Game_Window.getWindow(), COLOR_PAIR(1));
      mvwaddch(Game_Window.getWindow(), row_begin + i, col_begin + j, ACS_PLUS);
      wattroff(Game_Window.getWindow(), COLOR_PAIR(1));
    }
    else{
      mvwaddch(Game_Window.getWindow(), row_begin + i, col_begin + j, ACS_PLUS);
    }
  }
}
  int obstacle_count = Random::Random_Number(row/2, col/2)/2;
  mvwprintw(window,0,0, "Obstacle Count : %d",obstacle_count);
      wattron(window,A_BOLD|COLOR_PAIR(3));
  for (int i = 0; i < obstacle_count; i += 1) {
    int x = Random::Random_Number(1, (row-1)/2)*2;
    int y = Random::Random_Number(1, (col-1)/2)*2; 
    mvwprintw(window, row_begin + x, col_begin + y, "0");
}
    wattroff(window, A_BOLD|COLOR_PAIR(3));
}

void UI::GenerateStatusWindow()
{
  wattron(Status_Window.getWindow(), COLOR_PAIR(4));
  int w_max_x = Status_Window.getWidth() , w_max_y = Status_Window.getHeight();
  int beg_x = 1 , beg_y = 1;
  mvwprintw(Status_Window.getWindow(),beg_x,beg_y,"PLAYER :");
  int rand_HP = Random::Random_Number(1, 10)*10;
  //int rand_HP = 100;
  std::string E = "HP:["+std::to_string(rand_HP)+"]";
  mvwprintw(Status_Window.getWindow(),beg_y+2,beg_x,"%s",E.c_str());
  int j=0;
  for(int i = rand_HP;i>0;i=i-10)
  {
    mvwaddch(Status_Window.getWindow(),beg_y+2,beg_x+E.size()+j, ACS_DIAMOND);
    j+=1;
  }
  //int rand_STAMINA = 100;
  int rand_STAMINA = Random::Random_Number(1, 10)*10;
  E = "STAMINA:["+std::to_string(rand_STAMINA)+"]";
  mvwprintw(Status_Window.getWindow(), beg_y+4, beg_x, "%s",E.c_str());
  j=1;
  for(int i = rand_STAMINA;i>0;i=i-10)
  {
    mvwaddch(Status_Window.getWindow(),beg_y+4,beg_x+E.size()+j, ACS_CKBOARD);
    j+=1;
  }
  int Player_Level = 100;
  E = "LEVEL:["+std::to_string(Player_Level)+"]";
  mvwprintw(Status_Window.getWindow(), beg_y+6, beg_x, "%s",E.c_str());
  E = "ENEMY :";
  mvwprintw(Status_Window.getWindow(),beg_x,w_max_x-E.size()-1,"%s", E.c_str());
  E = "HP :";
  mvwprintw(Status_Window.getWindow(),beg_x+2,w_max_x-E.size()-1,"%s", E.c_str());
  E = "STAMINA :";
  mvwprintw(Status_Window.getWindow(),beg_x+4,w_max_x-E.size()-1,"%s", E.c_str());
  E = "LEVEL : ";
  mvwprintw(Status_Window.getWindow(),beg_x+6,w_max_x-E.size()-1,"%s", E.c_str());
  wattroff(Status_Window.getWindow(), COLOR_PAIR(4));
  Status_Window.Refresh();
}
