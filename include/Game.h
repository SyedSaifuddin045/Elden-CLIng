#ifndef GAME_H
#define GAME_H
#include "Item.h"
#include <Initialize.h>
#include <UI.h>
#include <algorithm>
#include <cstring>
#include <curses.h>

class Enemy;
class Player;
struct Location;
class Game {
public:
  static std::vector<std::string> ACTION_OPTIONS;
  static Player player;
  static std::vector<Item> INVENTORY;
  static std::vector<std::string> MAIN_MENU_OPTIONS;
  static std::vector<std::string> CHARACTERS;
  static int game_row, game_col, row_beg, col_beg;
  static std::vector<std::string> Enemy_list;
  static Enemy current_Enemy;
  enum GameState {
    Game_Title,
    Main_Menu,
    Character_Select,
    Play_Game,
    End_Screen,
    End_Game
  };
  static GameState game_state;
  static UI ui;
  static const char *Ascii_Title;
  static std::vector<Location> Obstacle_Locations;
  static bool isObstacleLocation(Location& L);
  static void ExitGame()
  {
    Initialize::UnInitialize();
    std::exit(0);
  }
  static int Turn_Counter;
};
#endif // !GAME_H
