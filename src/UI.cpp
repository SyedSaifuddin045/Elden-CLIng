#include "Enemy.h"
#include "Json.h"
#include "Location.h"
#include "Stats.h"
#include <Game.h>
#include <Random.h>
#include <UI.h>
#include <curses.h>
#include <menu.h>
#include <string>
#include <vector>
void UI::GeneratePlayGameUI(std::vector<std::string> Options,
                            std::vector<std::string> INVENTORY) {
  if (Play_Game_Generated)
    return;
  int action_menu_width = 12;
  GenerateWindow(Main_Window, 0, 0, 0, 0);

  GenerateWindow(Actions_Window, 8, COLS - 2, LINES - 9, 1);

  Menu action_menu;
  action_menu.Create(Actions_Window.getWindow(), 1, 1, 6, action_menu_width, Options);
  Actions_Window.setMenu(action_menu);
  GenerateWindow(Inventory_Window, LINES - (Actions_Window.getHeight()) - 2, 20,
                 1, 1);

  Menu inventory_menu;
  inventory_menu.Create(Inventory_Window.getWindow(), 3, 1,
                        Inventory_Window.getHeight() - 4,
                        Inventory_Window.getWidth()-1, INVENTORY);
  Inventory_Window.setMenu(inventory_menu);

  std::string str = "INVENTORY";
  Inventory_Window.Print_Window_Title(str);

  GenerateWindow(Status_Window, 11, COLS - (Inventory_Window.getWidth()) - 2, 1,
                 (Inventory_Window.getWidth() + 1));
  int enemy_index = Random::Random_Number(0, Game::Enemy_list.size() - 1);
  Location Zero_Location = {0,0};
  Stats Zero_Stats = Stats();
  Enemy enemy =  Enemy(Game::Enemy_list[enemy_index],Zero_Stats,Zero_Location,Game::player.getLevel()) ;
  Game::current_Enemy = enemy;

  GenerateWindow(
      Game_Window,
      LINES - (Status_Window.getHeight() + Actions_Window.getHeight() + 2),
      (COLS - Inventory_Window.getWidth() - 2), Status_Window.getHeight() + 1,
      Inventory_Window.getWidth() + 1);

  Game::game_row =
      Random::Random_Number(5, (Game_Window.getHeight() / 2) - 2) * 2;
  Game::game_col =
      Random::Random_Number(5, (Game_Window.getWidth() / 2) - 2) * 2;

  Generate_Grid(Game_Window.getWindow(), Game::game_row, Game::game_col);
  GenerateStatusWindow();
  Game_Window.Refresh();
  if (Actions_Window.getWindow()) {
    curs_set(1);
    while(Game::player.getStats().current_HP != 0)
    {
    std::string Selected = action_menu.Handle_Input();
    if (Selected == Game::ACTION_OPTIONS[4]) {
      inventory_menu.Refresh();
      inventory_menu.Handle_Input();
    }
    if (Selected == Game::ACTION_OPTIONS[3]) {
      MoveInGrid(2);
    }
    if(Selected == Game::ACTION_OPTIONS[0])
    {
      std::unordered_map<std::string,std::string> Attack_Options = {{"R - LIGHT","25"},{"R - HEAVY","35"},{"L - LIGHT","15"},{"L - HEAVY","35"},{"BACK","0"}};
      Menu attack_menu;
      attack_menu.Create(Actions_Window.getWindow(),1 , 1+action_menu_width, 7, 16, Attack_Options);
      attack_menu.Refresh();
      std::string attack_select = attack_menu.Handle_Input();
      attack_menu.Destroy();
      wattron(Actions_Window.getWindow(),COLOR_PAIR(1));
      Actions_Window.Box(0, 0);
      wattroff(Actions_Window.getWindow(),COLOR_PAIR(1));
      action_menu.Refresh();
      Actions_Window.Refresh();
    }
    }
    //wgetch(Game_Window.getWindow());
  }
  Play_Game_Generated = true;
  action_menu.Destroy();
}

void UI::GenerateWindow(Window &win, int height, int width, int start_y,
                        int start_x) {
  if (height == 0 && width == 0 && start_x == 0 && start_y == 0)
    win = Window();
  else {
    win = Window(height, width, start_y, start_x);
  }
  wattron(win.getWindow(), COLOR_PAIR(1));
  win.Box(0, 0);
  wattroff(win.getWindow(), COLOR_PAIR(1));
  win.Refresh();
}

void UI::GenerateMainMenu(std::vector<std::string> &Options) {
  GenerateWindow(Main_Menu_Window, Options.size() + 2, 12,
                 (LINES - Options.size()) / 2, (COLS - Options[1].size()) / 2);

  Menu main_menu;
  main_menu.Create(Main_Menu_Window.getWindow(), 1, 0,
                   Main_Menu_Window.getHeight() - 2,
                   Main_Menu_Window.getWidth(), Options);
  Main_Menu_Window.setMenu(main_menu);
  std::string Selected = main_menu.Handle_Input();
  if (Selected == "Exit") {
    main_menu.Destroy();
    Main_Menu_Window.DestroyWindow();
    Game::ExitGame();
  }
  if (Selected == "Load") {
    main_menu.Destroy();
    Main_Menu_Window.DestroyWindow();
    Game::game_state = Game::Character_Select;
  }
  if (Selected == "New Game") {
    main_menu.Destroy();
    Main_Menu_Window.DestroyWindow();
    UI::EnterPlayerDetails();
    Stats newPlayerStat;
    newPlayerStat.max_HP = 75;
    newPlayerStat.current_HP = 75;
    Game::player.setStats(newPlayerStat);
    Game::game_state = Game::Play_Game;
  }
}

void UI::GenerateCharaterSelect(std::vector<std::string> &Characters) {
  if (Character_Select_generated)
    return;
  Characters.push_back("Exit");
  GenerateWindow(Character_Select_Window, Characters.size() + 2,
                 Characters[1].size() + 3, (LINES - Characters.size()) / 2,
                 (COLS - Characters[1].size()) / 2);
  mvprintw(0, 0, "Select file to load from :");
  refresh();
  Menu Character_menu;
  set_menu_mark(Character_menu.getMENU(), ">");
  curs_set(1);
  Character_menu.Create(Character_Select_Window.getWindow(), 1, 0,
                        Character_Select_Window.getHeight() - 2,
                        Character_Select_Window.getWidth(), Characters);
  Character_Select_Window.setMenu(Character_menu);
  Character_Select_generated = true;
  std::string Selected = Character_menu.Handle_Input();
  if (Selected == "Exit") {
    Game::ExitGame();
  } else {
    std::string file_name = Selected + ".json";
    Json json;
    json.Read(file_name);
    json.ToObject<Player>(Game::player);
  }
  Character_Select_Window.DestroyWindow();
  Character_menu.Destroy();
}

void UI::Clear() {
  for (auto win : {Main_Window, Actions_Window, Inventory_Window, Status_Window,
                   Game_Window, Main_Menu_Window, Character_Select_Window}) {
    // win.DestroyMenu();
    win.DestroyWindow();
  }
}

void UI::GenerateEndScreen() {
  if (End_Screen_generated)
    return;
  GenerateWindow(End_Screen, 3, COLS - 2, (LINES / 2) - 1, 1);
  std::string str = "YOU DIED !";
  End_Screen.Print_Window_Title(str);
  End_Screen_generated = true;
}

void UI::Generate_Grid(WINDOW *window, int row, int col) {
  Game::col_beg = ((Game_Window.getWidth() / 2) - (col / 2));
  Game::row_beg = ((Game_Window.getHeight() / 2) - (row / 2));

  wattron(window, COLOR_PAIR(1));

  // Draw top and bottom borders
  mvwhline(window, Game::row_beg, Game::col_beg, ACS_HLINE, col);
  mvwhline(window, Game::row_beg + row, Game::col_beg, ACS_HLINE, col);

  // Draw left and right borders
  mvwvline(window, Game::row_beg, Game::col_beg, ACS_VLINE, row);
  mvwvline(window, Game::row_beg, Game::col_beg + col, ACS_VLINE, row);

  mvwaddch(window, Game::row_beg, Game::col_beg, ACS_ULCORNER);
  mvwaddch(window, Game::row_beg + row, Game::col_beg, ACS_LLCORNER);
  mvwaddch(window, Game::row_beg, Game::col_beg + col, ACS_URCORNER);
  mvwaddch(window, Game::row_beg + row, Game::col_beg + col, ACS_LRCORNER);

  wattroff(window, COLOR_PAIR(1));

  // Draw horizontal lines
  for (int i = 1; i < row; i += 2) {
    if (i == 1 || i == row - 1) {
      wattron(window, COLOR_PAIR(1));
      mvwaddch(window, Game::row_beg + i, Game::col_beg, ACS_LTEE);
      mvwhline(window, Game::row_beg + i, Game::col_beg + 1, ACS_HLINE,
               col - 2);
      mvwaddch(window, Game::row_beg + i, Game::col_beg + col, ACS_RTEE);
      wattroff(window, COLOR_PAIR(1));
    } else {
      wattron(window, COLOR_PAIR(1));
      mvwaddch(window, Game::row_beg + i, Game::col_beg, ACS_LTEE);
      wattroff(window, COLOR_PAIR(1));
      mvwhline(window, Game::row_beg + i, Game::col_beg + 1, ACS_HLINE,
               col - 2);
      wattron(window, COLOR_PAIR(1));
      mvwaddch(window, Game::row_beg + i, Game::col_beg + col, ACS_RTEE);
      wattroff(window, COLOR_PAIR(1));
    }
  }

  // Draw vertical lines
  for (int i = 1; i < col; i += 2) {
    if (i == 1 || i == col - 1) {
      wattron(window, COLOR_PAIR(1));
      mvwaddch(window, Game::row_beg, Game::col_beg + i, ACS_TTEE);
      mvwvline(window, Game::row_beg + 1, Game::col_beg + i, ACS_VLINE,
               row - 2);
      mvwaddch(window, Game::row_beg + row, Game::col_beg + i, ACS_BTEE);
      wattroff(window, COLOR_PAIR(1));
    } else {
      wattron(window, COLOR_PAIR(1));
      mvwaddch(window, Game::row_beg, Game::col_beg + i, ACS_TTEE);
      wattroff(window, COLOR_PAIR(1));
      mvwvline(window, Game::row_beg + 1, Game::col_beg + i, ACS_VLINE,
               row - 2);
      wattron(window, COLOR_PAIR(1));
      mvwaddch(window, Game::row_beg + row, Game::col_beg + i, ACS_BTEE);
      wattroff(window, COLOR_PAIR(1));
    }
  }

  // Draw intersections
  for (int i = 1; i < row; i += 2) {
    for (int j = 1; j < col; j += 2) {
      if (i == 1 || i == row - 1 || j == 1 || j == col - 1) {
        wattron(Game_Window.getWindow(), COLOR_PAIR(1));
        mvwaddch(Game_Window.getWindow(), Game::row_beg + i, Game::col_beg + j,
                 ACS_PLUS);
        wattroff(Game_Window.getWindow(), COLOR_PAIR(1));
      } else {
        mvwaddch(Game_Window.getWindow(), Game::row_beg + i, Game::col_beg + j,
                 ACS_PLUS);
      }
    }
  }
  // generate Obstacles
  int obstacle_count = Random::Random_Number(row / 2, col / 2) / 2;
  wattron(window, A_BOLD | COLOR_PAIR(3));
  for (int i = 0; i < obstacle_count; i += 1) {
    int x = Random::Random_Number(1, (col - 1) / 2) * 2;
    int y = Random::Random_Number(1, (row - 1) / 2) * 2;
    Location L;
    L.x = Game::col_beg + x;
    L.y = Game::row_beg + y;
    Game::Obstacle_Locations.push_back(L);
    mvwprintw(window, Game::row_beg + y, Game::col_beg + x, "0");
  }
  wattroff(window, A_BOLD | COLOR_PAIR(3));
  
  Game::player.setPlayerLocation(1,Random::Random_Number(1, (Game::game_row-1) / 2));
  if(! Game::isObstacleLocation(Game::player.getPlayerLocation()))
  {
    wattron(Game_Window.getWindow(), COLOR_PAIR(5));
    mvwprintw(window,Game::row_beg+(Game::player.getPlayerLocation().y * 2),Game::col_beg+Game::player.getPlayerLocation().x + 1,"P");
    wattroff(Game_Window.getWindow(), COLOR_PAIR(5));
  }

  //Game::current_Enemy.setEnemyLocation(Game::game_col - 2,Random::Random_Number(1, ((Game::game_row - 1) /2) ) * 2);
  Game::current_Enemy.setEnemyLocation((Game::game_col-2) / 2,Random::Random_Number(1, (Game::game_row-1) / 2));
  if(! Game::isObstacleLocation(Game::current_Enemy.getEnemyLocation()))
  {
    wattron(Game_Window.getWindow(), COLOR_PAIR(6));
    mvwprintw(window,Game::row_beg + (Game::current_Enemy.getEnemyLocation().y * 2), Game::col_beg + Game::current_Enemy.getEnemyLocation().x * 2, "E");
    wattroff(Game_Window.getWindow(), COLOR_PAIR(6));
  }
}

void UI::GenerateStatusWindow() {
  wattron(Status_Window.getWindow(), COLOR_PAIR(4));
  int w_max_x = Status_Window.getWidth(), w_max_y = Status_Window.getHeight();
  int beg_x = 1, beg_y = 1;
  mvwprintw(Status_Window.getWindow(), beg_x, beg_y, "PLAYER : %s",
            Game::player.getName().c_str());

  Stats player_stats = Game::player.getStats();
  std::string E = "HP:[" + std::to_string(player_stats.current_HP) + "/" +
                  std::to_string(player_stats.max_HP) + "]";
  mvwprintw(Status_Window.getWindow(), beg_y + 2, beg_x, "%s", E.c_str());
  int j = 0;
  for (int i = player_stats.current_HP; i > 0; i = i - 10) {
    mvwaddch(Status_Window.getWindow(), beg_y + 2, beg_x + E.size() + j,
             ACS_DIAMOND);
    j += 1;
  }

  E = "STAMINA:[" + std::to_string(player_stats.current_Stamina) + "/" +
      std::to_string(player_stats.max_HP) + "]";
  mvwprintw(Status_Window.getWindow(), beg_y + 4, beg_x, "%s", E.c_str());
  j = 1;
  for (int i = player_stats.current_Stamina; i > 0; i = i - 10) {
    mvwaddch(Status_Window.getWindow(), beg_y + 4, beg_x + E.size() + j,
             ACS_CKBOARD);
    j += 1;
  }
  E = "LEVEL:[" + std::to_string(Game::player.getLevel()) + "]";
  mvwprintw(Status_Window.getWindow(), beg_y + 6, beg_x, "%s", E.c_str());
  E = "PLAYER LOCATION : x = "+std::to_string(Game::player.getPlayerLocation().x) + ", y = "+ std::to_string(Game::player.getPlayerLocation().y);
  mvwprintw(Status_Window.getWindow(), beg_y+8, beg_x, "%s",E.c_str());
  E = "ENEMY :" + Game::current_Enemy.getEnemyName();
  mvwprintw(Status_Window.getWindow(), beg_x, w_max_x - E.size() - 1, "%s",
            E.c_str());

  Stats Enemy_Stats = Game::current_Enemy.getEnemyStats();
  E = "HP :[" + std::to_string(Enemy_Stats.current_HP) +"/" + std::to_string(Enemy_Stats.max_HP) +"]";
  mvwprintw(Status_Window.getWindow(), beg_x + 2, w_max_x - E.size() - 1, "%s",
            E.c_str());
  E = "STAMINA : [" + std::to_string(Enemy_Stats.current_Stamina) +"/" +  std::to_string(Enemy_Stats.max_Stamina)+"]";
  mvwprintw(Status_Window.getWindow(), beg_x + 4, w_max_x - E.size() - 1, "%s",
            E.c_str());
  E = "LEVEL : " + std::to_string(Game::current_Enemy.getLevel());
  mvwprintw(Status_Window.getWindow(), beg_x + 6, w_max_x - E.size() - 1, "%s",
            E.c_str());
  E = "ENEMY LOCATION : x = "+std::to_string(Game::current_Enemy.getEnemyLocation().x) + ", y = "+ std::to_string(Game::current_Enemy.getEnemyLocation().y);
  mvwprintw(Status_Window.getWindow(), beg_y+8, w_max_x - E.size() - 1, "%s",E.c_str());
  wattroff(Status_Window.getWindow(), COLOR_PAIR(4));
  Status_Window.Refresh();
}
void UI::MoveInGrid(int increment) {
  int c;
  int cur_X = Game::col_beg + 2, cur_Y = Game::row_beg + 2;
  Location CurrentLocation;
  CurrentLocation.y = cur_Y;
  CurrentLocation.x = cur_X;
  wmove(Game_Window.getWindow(), CurrentLocation.y, CurrentLocation.x);
  while ((c = wgetch(Game_Window.getWindow())) != 10) {
    Location newLocation;
    switch (c) {
    case KEY_DOWN:
      newLocation = {CurrentLocation.x, CurrentLocation.y - increment};
      if (cur_Y + increment >= Game::row_beg + 2 &&
          cur_Y + increment <= Game::row_beg + Game::game_row - 2 &&
          Game::isObstacleLocation(newLocation) == false) {
        cur_Y += increment;
        CurrentLocation.y = cur_Y;
      }
      break;
    case KEY_UP:
      newLocation = {CurrentLocation.x, CurrentLocation.y - increment};
      if (cur_Y - increment >= Game::row_beg + 2 &&
          cur_Y <= Game::row_beg + Game::game_row - 2 &&
          Game::isObstacleLocation(newLocation) == false) {
        cur_Y -= increment;
        CurrentLocation.y = cur_Y;
      }
      break;
    case KEY_RIGHT:
      newLocation = {CurrentLocation.x + increment, CurrentLocation.y};
      if (cur_X + increment >= Game::col_beg + 2 &&
          cur_X + increment <= Game::col_beg + Game::game_col - 2 &&
          Game::isObstacleLocation(newLocation) == false) {
        cur_X += increment;
        CurrentLocation.x = cur_X;
      }
      break;
    case KEY_LEFT:
      newLocation = {CurrentLocation.x - increment, CurrentLocation.y};
      if (cur_X - increment >= Game::col_beg + 2 &&
          cur_X - increment <= Game::col_beg + Game::game_col - 2 &&
          Game::isObstacleLocation(newLocation) == false) {
        cur_X -= increment;
        CurrentLocation.x = cur_X;
      }
      break;
    }
    //mvwprintw(Game_Window.getWindow(), 0, 0,"Current Position = %d , %d and isObstacleLocation = %d",CurrentLocation.y, CurrentLocation.x,Game::isObstacleLocation(CurrentLocation));
    wmove(Game_Window.getWindow(), CurrentLocation.y, CurrentLocation.x);
    Game_Window.Refresh();
  }
}

void UI::EnterPlayerDetails() {
  std::string str = "Enter Your Name :";
  GenerateWindow(New_Game_Window, 5, COLS - 4,
                 (LINES - New_Game_Window.getHeight()) / 2, 2);
  New_Game_Window.Print_Window_Title(str);
  echo();
  curs_set(2);
  wattron(New_Game_Window.getWindow(), A_BLINK);
  New_Game_Window.Refresh();
  char name[50];
  mvwgetstr(New_Game_Window.getWindow(), 3, 1, name);
  noecho();
  Game::player.setName(std::string(name));
  //mvprintw(1, 0, "Player name : %s", Game::player.getName().c_str());
  wattroff(New_Game_Window.getWindow(), A_BLINK);
  refresh();
  //getch();
  New_Game_Window.DestroyWindow();
}
